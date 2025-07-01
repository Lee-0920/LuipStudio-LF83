--[[
 * @brief 采集水样流程。
--]]


CollectSampleFlow = Flow:new
{
}

function CollectSampleFlow:new(o)
        o = o or {}
        setmetatable(o, self)
        self.__index = self
	
	o.detectTime = os.time()
	
        return o
end


function CollectSampleFlow:OnStart()
	local runStatus = Helper.Status.SetStatus(setting.runStatus.collectSample)
	StatusManager.Instance():SetStatus(runStatus)
end

function CollectSampleFlow:OnProcess()
		local waterCollector = WaterCollector.Instance()

		local runAction = Helper.Status.SetAction(setting.runAction.collectSample.collect)
		StatusManager.Instance():SetAction(runAction)

		waterCollector:TurnOn()
		Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, true)

		if not self:Wait(config.interconnection.miningWaterTime) then 	-- 采集等待
			waterCollector:TurnOff()
			Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)
			error(UserStopException:new())    --用户停止
		end

		local runAction = Helper.Status.SetAction(setting.runAction.collectSample.silent)
		StatusManager.Instance():SetAction(runAction)

		waterCollector:TurnOff()
		Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)

		if not self:Wait(config.interconnection.silentTime) then 	-- 静默等待
			error(UserStopException:new())    --用户停止
		end
end	

function CollectSampleFlow:OnStop()
	self.result = "采集水样结束"
	log:info("采集水样结束")
	log:debug("采集水样时间 = "..os.time() - self.detectTime )
end
