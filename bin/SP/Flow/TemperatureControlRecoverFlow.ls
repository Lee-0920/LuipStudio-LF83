--[[
 * @brief 温控恢复流程。
--]]

TemperatureControlRecoverFlow = Flow:new
{

}

function TemperatureControlRecoverFlow:new(o)
        o = o or {}
        setmetatable(o, self)
        self.__index = self

		o.thermostat = thermostats.measure
		o.detectTime = os.time()
		o.operateResult = false
	
        return o
end

function TemperatureControlRecoverFlow:OnStart()
	local runStatus = Helper.Status.SetStatus(setting.runStatus.temperatureControlRecover)
	StatusManager.Instance():SetStatus(runStatus)

	if self.thermostat == thermostats.measure then
		local runAction = Helper.Status.SetAction(setting.runAction.thermostatsMeasureControl)
		StatusManager.Instance():SetAction(runAction)
	elseif self.thermostat == thermostats.bacteria then
		local runAction = Helper.Status.SetAction(setting.runAction.thermostatsBacteriaControl)
		StatusManager.Instance():SetAction(runAction)
	end
end

function TemperatureControlRecoverFlow:OnProcess()
	local  measureTemp = thermostats.measure:GetTemperature()
	local  bacteriaTemp = thermostats.bacteria:GetTemperature()

	if self.thermostat == thermostats.measure then
		thermostats.measure:Stop(false)  --不等待停止事件
		log:debug("测量仓 温控恢复起始温度 = " .. measureTemp .. " ℃");
	elseif self.thermostat == thermostats.bacteria then
		thermostats.bacteria:Stop(false)  --不等待停止事件
		log:debug("发光菌存储槽 温控恢复起始温度 = " .. bacteriaTemp .. " ℃");
	end

	--等待两秒
	if not self:Wait(2) then
		error(UserStopException:new())
	end

	--启动恒温控制
	local err,result = pcall(function() return op:StartTemperatureControl(self.thermostat) end)
	if not err then -- 出现异常
		if type(result) == "table" then        --Lua异常
			if getmetatable(result) == ThermostatFailedException then
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == ThermostatTimeoutException then
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
		self.operateResult = true
	end

end	

function TemperatureControlRecoverFlow:OnStop()
	if self.thermostat == thermostats.measure then
		local  measureTemp = thermostats.measure:GetTemperature()
		log:debug("测量仓 温控恢复起始温度 = " .. measureTemp .. " ℃");
	elseif self.thermostat == thermostats.bacteria then
		local  bacteriaTemp = thermostats.bacteria:GetTemperature()
		log:debug("发光菌存储槽 温控恢复起始温度 = " .. bacteriaTemp .. " ℃");
	end

	if self.operateResult == true then
		self.result = "温控恢复成功"
	else
		self.result = "温控恢复失败"
	end
	log:info("温控恢复结束")
	log:debug("温控恢复时间 = "..os.time() - self.detectTime )
end
