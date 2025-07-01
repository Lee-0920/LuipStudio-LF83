--[[
 * @brief 测量模块检测流程。
--]]

MeasureModuleCheckFlow = Flow:new
{

}

function MeasureModuleCheckFlow:new(o)
        o = o or {}
        setmetatable(o, self)
        self.__index = self

		o.detectTime = os.time()
		o.operateResult = false
		o.refAD = 0
		o.meaAD = 0
	
        return o
end

function MeasureModuleCheckFlow:OnStart()
	local runStatus = Helper.Status.SetStatus(setting.runStatus.pmtCheck)
	StatusManager.Instance():SetStatus(runStatus)

	local runAction = Helper.Status.SetAction(setting.runAction.pmtCheck)
	StatusManager.Instance():SetAction(runAction)
end

function MeasureModuleCheckFlow:OnProcess()
	--开启测量模块电源
	op.IOpticalAcquire:PDPowerOn()
	log:debug("开启测量模块电源")
	--等待2秒
	if not self:Wait(2) then
		error(UserStopException:new())
	end

	--启动恒温控制
	local err,result = pcall(function() return op:AcquirerFilterAD(5) end)
	if not err then -- 出现异常
		if type(result) == "table" then        --Lua异常
			if getmetatable(result) == AcquirerADFailedException then
				ExceptionHandler.MakeAlarm(result)
			else
				error(result)
			end
		elseif type(result) == "userdata" then --C++异常
			if result:GetType() == "ExpectEventTimeoutException" then
				ExceptionHandler.MakeAlarm(result)
			else
				error(result)
			end
		else
			error(result)
		end
	else
		self.refAD = result:GetReference()
		self.meaAD = result:GetMeasure()
		self.operateResult = true
	end
end	

function MeasureModuleCheckFlow:OnStop()
	--关闭测量模块电源
	op.IOpticalAcquire:PDPowerOff()
	log:debug("关闭测量模块电源")

	if self.operateResult == true then
		self.result = "测量模块检测成功"
	else
		self.result = "测量模块检测失败"
	end

	log:info("参考AD="..tostring(self.refAD).." 样本AD="..tostring(self.meaAD))
	log:info("测量模块检测结束")
	log:debug("测量模块检测时间 = "..os.time() - self.detectTime )
end
