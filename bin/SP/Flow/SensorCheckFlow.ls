--[[
 * @brief 传感器状态检测流程。
--]]

SensorCheckFlow = Flow:new
{

}

function SensorCheckFlow:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o.name = ""
    o.result = ""

    return o
end

function SensorCheckFlow:OnStart()
    local runStatus = Helper.Status.SetStatus(setting.runStatus.sensorCheck)
    StatusManager.Instance():SetStatus(runStatus)

    local flowManager = FlowManager.Instance()
    flowManager:UpdateFlowMessage(self.name, "测试中")
end

function SensorCheckFlow:OnProcess()

    local err, ret = pcall(function()
                                    if self.name == "SyringeInitSensorCheck" then
                                        return dc:GetIMotorControl():GetSensorStatus(0)
                                    elseif self.name == "MotorXInitSensorCheck" then
                                        return dc:GetIMotorControl():GetSensorStatus(1)
                                    elseif self.name == "MotorZInitSensorCheck" then
                                        return dc:GetIMotorControl():GetSensorStatus(2)
                                    elseif self.name == "RefCollisionSensorCheck" then
                                        return dc:GetIMotorControl():GetSensorStatus(3)
                                    elseif self.name == "MeaCollisionSensorCheck" then
                                        return dc:GetIMotorControl():GetSensorStatus(4)
                                    elseif self.name == "BlankSensorCheck" then
                                        return dc:GetIMotorControl():GetSensorStatus(5)
                                    end
                                end)

    if not err then      -- 出现异常
        if type(ret) == "userdata" then
            if ret:GetType() == "ExpectEventTimeoutException" then          --期望事件等待超时异常。
                ExceptionHandler.MakeAlarm(ret)
            elseif ret:GetType() == "CommandTimeoutException" then          --命令应答超时异常
                ExceptionHandler.MakeAlarm(ret)
            else
                log:warn("SensorCheckFlow.OnProcess()=>" .. ret:What())
            end
        elseif type(ret) == "table" then
            log:warn("SensorCheckFlow.OnProcess()=>" .. ret:What())
        elseif type(ret) == "string" then
            log:warn("SensorCheckFlow.OnProcess()=>" .. ret)	--C++、Lua系统异常
        end

        self.result = "未知"
    else
        if ret == true then
            if self.name == "RefCollisionSensorCheck" or self.name == "MeaCollisionSensorCheck" then
                self.result = "遮挡"
            else
                self.result = "未遮挡"
            end
        else
            if self.name == "RefCollisionSensorCheck" or self.name == "MeaCollisionSensorCheck" then
                self.result = "未遮挡"
            else
                self.result = "遮挡"
            end
        end
    end
end


function SensorCheckFlow:OnStop()



end
