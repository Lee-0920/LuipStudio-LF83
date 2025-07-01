--[[
 * @brief 各个业务流程的基类。
 * @details 各个业务流程的公共部分。
--]]

Flow =
{
	isStarted = false,
	isUserStop = false,
	isFinish = false,
	isDeviceFault = false,
	isFatalError = false,

	name = "",
	result = "",
}

function Flow:new(o)
        o = o or {}
        setmetatable(o, self)
        self.__index = self

        return o
end

function Flow:Stop()
	self.isStarted = false
end

function Flow:Wait(seconds)
	local cnt = math.floor(seconds * 1000/200)

	while true do
		if  self.isStarted then
			App.Sleep(200)
		else
		    return false
		end

		cnt = cnt -1
		if cnt <= 0 then
			return true
		end
	end
end

function Flow:GetRuntime()
	return 0
end

function Flow:OnStart()
	print("Flow OnStart")
end

function Flow:OnProcess()
	print("Flow OnProcess")
end

function Flow:OnStop()
	print("Flow OnStop")

end

function Flow:Run()
	print("Flow Run")

	self.isStarted = true

	local err,result = pcall(function() self:OnStart() self:OnProcess() end)

	self:ExceptionHandle(err, result)

	err,result = pcall(function() self:OnStop() end)

	self:ExceptionHandle(err, result)

	self.isStarted = false

	if Measurer.flow then
		Measurer:Reset()
	end

	--检查是否发生仪器运行故障
	if self.isDeviceFault == true and config.system.faultBlocking == true then
		status.measure.isDeviceFault = true
		ConfigLists.SaveMeasureStatus()
	end

	--仪器状态显示复位
	StatusManager.Instance():ResetDeviceStatus()
	--清除告警
	if AlarmManager.Instance().ClearAlarm ~= nil then
		AlarmManager.Instance():ClearAlarm()
	end

	return self.name, self.result
end

function Flow:ExceptionHandle(err, result)
	if not err then      -- 出现异常
		if type(result) == "userdata" then
			if result:GetType() == "ExpectEventTimeoutException" then          --期望事件等待超时异常。
				ExceptionHandler.MakeAlarm(result)
			elseif result:GetType() == "CommandTimeoutException" then          --命令应答超时异常
				ExceptionHandler.MakeAlarm(result)
			else
				log:warn(result:What())
			end
		elseif type(result) == "table" then
			if getmetatable(result) == PumpStoppedException then 			--泵操作被停止异常。
				ExceptionHandler.MakeAlarm(result)
				self.isUserStop = true
			elseif getmetatable(result) == PumpFailedException then 			--泵操作中途出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MotorFailedException then 			--电机中途运动出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == RefCollideException then 			--电机运动参考针碰撞出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MeaCollideException then 			--电机运动测量针碰撞出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MotorXMoveOutSensorFailException then 			--电机X轴移不出传感器出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MotorZMoveOutSensorFailException then 			--电机Z轴移不出传感器出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MotorXMoveInSensorFailException then 			--电机X轴找不到传感器出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MotorZMoveInSensorFailException then 			--电机Z轴找不到传感器出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == SyringeMoveOutSensorFailException then 		--注射器移不出传感器出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == SyringeMoveInSensorFailException then 			--注射器找不到传感器出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result)== MotorStoppedException then			--电机运动被停止异常。
				ExceptionHandler.MakeAlarm(result)
				self.isUserStop = true
            elseif getmetatable(result)== AcquirerADStoppedException then 	    --光学采集被停止异常
                ExceptionHandler.MakeAlarm(result)
                self.isUserStop = true
            elseif getmetatable(result) == AcquirerADFailedException then 	    --光学采集中途出现故障，未能完成异常。
                ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == StaticADControlFailedException then 		--AD自动调节失败异常
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result)== StaticADControlStoppedException then 	    --AD自动调节被停止异常
				ExceptionHandler.MakeAlarm(result)
				self.isUserStop = true
            elseif getmetatable(result)== MeterStoppedException then			--定量被停止异常。
				ExceptionHandler.MakeAlarm(result)
				self.isUserStop = true
			elseif getmetatable(result) == MeterFailedException then 			--定量中途出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MeterOverflowException then 		--定量溢出异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MeterUnfinishedException then 		--定量目标未达成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result)== MeterAirBubbleException then			--定量气泡异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == MeterExpectTimeoutException then 	--定量超时异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == ThermostatStoppedException then  	--恒温被停止异常。
				ExceptionHandler.MakeAlarm(result)
				self.isUserStop = true
			elseif getmetatable(result) == ThermostatFailedException then 		--恒温中途出现故障，未能完成异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == ThermostatTimeoutException then 		--恒温超时，指定时间内仍未达到目标温度异常
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == DeviceFaultException then			--仪器异常停机异常。
				self.isDeviceFault = true
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == DrainFromDigestionException then 	 	--排消解液异常
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result) == UpdateException then 				--升级异常。
				ExceptionHandler.MakeAlarm(result)
			elseif getmetatable(result)== UserStopException then 				--用户停止测量流程
				ExceptionHandler.MakeAlarm(result)
				self.isUserStop = true
			elseif getmetatable(result)== CalibrateResultWrongException then 	--精准定标结果错误异常
				ExceptionHandler.MakeAlarm(result)
			else
				log:warn(result:What())								--其他定义类型异常
			end
		elseif type(result) == "string" then
			log:warn(result)										--C++、Lua系统异常
		else

		end
	end
end
