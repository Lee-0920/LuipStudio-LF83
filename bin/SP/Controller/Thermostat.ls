--[[
 * @brief 单个恒温器。
 * @details 对单个恒温器的功能进行封装。
--]]

Thermostat =
{
    index = 0,
    isRunning = false,
    temperatureControlInterface =  0,
}

function Thermostat:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.__metatable = "Thermostat"
    return o
end

function Thermostat:GetFactor()
    return self.temperatureControlInterface:GetCalibrateFactor(self.index)
end

function Thermostat:SetFactor(factor)
    return  self.temperatureControlInterface:SetCalibrateFactor(self.index, factor)
end

function Thermostat:GetTemperature()
    return self.temperatureControlInterface:GetThermostatTemperature(self.index)
end

function Thermostat:GetParam()
    return self.temperatureControlInterface:GetThermostatParam(self.index)
end

function Thermostat:SetParam(param)
    return self.temperatureControlInterface:SetThermostatParam(self.index, param)
end

function Thermostat:GetStatus()
    return self.temperatureControlInterface:GetThermostatStatus(self.index)
end

function Thermostat:Start(mode, targetTemp, toleranceTemp, timeout, isExpect)
    --[[if self.fanIndexList ~= nil then
        for _,i in pairs(self.fanIndexList) do
            local status = self.temperatureControlInterface:SetFanOut(i, 1)
            if not status then
                log:debug("Thermostat Start: Open Fan "..i.." Failed!")
                return false
            end
        end
    end--]]

    local ret = self.temperatureControlInterface:StartThermostat(self.index, mode, targetTemp, toleranceTemp, timeout, isExpect)
    if ret == true then
        self.isRunning = true
    end
    return ret
end

function Thermostat:Stop(isExpect)
    local ret = self.temperatureControlInterface:StopThermostat(self.index, isExpect)
    if ret == true then
        --[[if self.fanIndexList ~= nil then
            for _,i in pairs(self.fanIndexList) do
                local status = self.temperatureControlInterface:SetFanOut(i, 0)
                if not status then
                    log:debug("Thermostat Stop: Close Fan "..i.." Failed!")
                    return false
                end
            end
        end--]]
        self.isRunning = false
    end
    return ret
end

function Thermostat:SetFanOutput(fanIndex, level)
    return self.temperatureControlInterface:ThermostatFanSetOutput(self.index, fanIndex, level)
end

function Thermostat:GetHeaterMaxDutyCycle()
    return self.temperatureControlInterface:GetHeaterMaxDutyCycle(self.index)
end

function Thermostat:SetHeaterMaxDutyCycle(maxDutyCycle)
    return self.temperatureControlInterface:SetHeaterMaxDutyCycle(self.index, maxDutyCycle)
end

function Thermostat:GetCurrentParam()
    return self.temperatureControlInterface:GetCurrentThermostatParam(self.index)
end

function Thermostat:SetCurrenParam(param)
    return self.temperatureControlInterface:SetCurrentThermostatParam(self.index, param)
end

function Thermostat:ExpectResult(timeout)
    local  thermostatResult = ThermostatResult.new()
    thermostatResult:SetIndex(0)
    thermostatResult:SetResult(0)
    thermostatResult:SetTemp(0)

    thermostatResult = self.temperatureControlInterface:ExpectThermostat(timeout)

    return thermostatResult
end
