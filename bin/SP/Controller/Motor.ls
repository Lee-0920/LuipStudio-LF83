--[[
 * @brief 单个步进电机
 * @details 对步进电机的功能进行封装。
--]]

Motor =
{
    index = 0,
    isRunning = false,
    motorControlInterface =  0,
    localPos = 0,
}

function Motor:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.__metatable = "Motor"
    return o
end

function Motor:GetMotorParam()
    return self.motorControlInterface:GetMotorParam(self.index)
end

function Motor:SetMotorParam(param)
    return self.motorControlInterface:SetMotorParam(self.index, param)
end

function Motor:GetStatus()
    return self.motorControlInterface:GetMotorStatus(self.index)
end

function Motor:GetMaxSteps()
    return self.motorControlInterface:GetMotorMaxSteps(self.index)
end

function Motor:GetInitSteps()
    return self.motorControlInterface:GetMotorInitSteps(self.index)
end

function Motor:GetCurrentSteps()
    return self.motorControlInterface:GetMotorCurrentSteps(self.index)
end

function Motor:Move(steps, mode)
    self.isRunning = true;
    return self.motorControlInterface:MotorMove(self.index, steps, mode)
end

function Motor:Stop()
    self.isRunning = false;
    return self.motorControlInterface:MotorStop(self.index)
end

function Motor:Reset()
    self.isRunning = false;
    return self.motorControlInterface:MotorReset(self.index)
end

function Motor:ExpectResult(timeout)
    local  motorResult = MotorResult.new()
    motorResult:SetIndex(self.index)
    motorResult:SetResult(MotorOperateResult.Failed)

    motorResult = self.motorControlInterface:ExpectMotorResult(timeout)

    self.isRunning = false

    return motorResult
end