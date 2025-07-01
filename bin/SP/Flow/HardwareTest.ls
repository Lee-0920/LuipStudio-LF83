--[[
 * @brief 硬件测试
--]]

HardwareTest =
{
    -- row 1
    {
        name = "SamplePump",
        open = function()
            local pump = pumps[setting.liquidType.sample.pump + 1]
            pump:Start(RollDirection.Drain, 1000, 0)
        end,
        close = function()
            local pump = pumps[setting.liquidType.sample.pump + 1]
            pump:Stop()
        end,
        status = false,
    },
    -- row 2
    {
        name = "WashPump",
        open = function()
            local pump = pumps[setting.liquidType.wash.pump + 1]
            pump:Start(RollDirection.Drain, 1000, 0)
        end,
        close = function()
            local pump = pumps[setting.liquidType.wash.pump + 1]
            pump:Stop()
        end,
        status = false,
    },
    -- row 3
    {
        name = "SaltPump",
        open = function()
            local pump = pumps[setting.liquidType.salt.pump + 1]
            pump:Start(RollDirection.Drain, 1000, 0)
        end,
        close = function()
            local pump = pumps[setting.liquidType.salt.pump + 1]
            pump:Stop()
        end,
        status = false,
    },
    -- row 4
    {
        name = "WastePump",
        open = function()
            local pump = pumps[setting.liquidType.waste.pump + 1]
            pump:Start(RollDirection.Drain, 1000, 0)
        end,
        close = function()
            local pump = pumps[setting.liquidType.waste.pump + 1]
            pump:Stop()
        end,
        status = false,
    },
    -- row 5
    {
        name = "WasteRoomPump",
        open = function()
            local pump = pumps[setting.liquidType.wasteRoom.pump + 1]
            pump:Start(RollDirection.Drain, 1000, 0)
        end,
        close = function()
            local pump = pumps[setting.liquidType.wasteRoom.pump + 1]
            pump:Stop()
        end,
        status = false,
    },
    -- row 6
    {
        name = "StirPump",
        open = function()
            local speed = config.measureParam.stirSpeed/1000
            local pump = pumps[setting.liquidType.stir.pump + 1]
            pump:Start(RollDirection.Drain, 1000, speed)
        end,
        close = function()
            local pump = pumps[setting.liquidType.stir.pump + 1]
            pump:Stop()
        end,
        status = false,
    },
    -- row 7
    {
        name = "BlankValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.blank.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.blank.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 8
    {
        name = "RefWashValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.refWash.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.refWash.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 9
    {
        name = "MeaWshValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.meaWash.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.meaWash.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 10
    {
        name = "RefWasteValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.refWaste.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.refWaste.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 11
    {
        name = "MeaWasteValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.meaWaste.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.meaWaste.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 12
    {
        name = "AirInValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.airIn.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.airIn.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 13
    {
        name = "AirOutValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.airOut.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.airOut.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 14
    {
        name = "WasteValve",
        open = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap | setting.liquidType.waste.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        close = function()
            local curmap = dc:GetISolenoidValve():GetValveMap():GetData()
            local map = ValveMap.new(curmap & ~setting.liquidType.waste.valve)
            dc:GetISolenoidValve():SetValveMap(map)
        end,
        status = false,
    },
    -- row 15
    {
        name = "MeasureCellHeating",
        open = function()
            if HardwareTest[16].status then
                return false
            end
            op:StopThermostat(thermostats.measure, false)
            local environmentTemp = dc:GetCurrEnvironmentTemp()
            op:StartThermostat(thermostats.measure,
                                ThermostatMode.Heater,
                                environmentTemp + 5,
                                setting.temperature.toleranceTemp,
                                setting.temperature.coolingTimeout,
                                false)
        end,
        close = function()
            op:StopThermostat(thermostats.measure, false)
        end,
        status = false,
    },
    -- row 16
    {
        name = "MeasureCellCooling",
        open = function()
            if HardwareTest[15].status then
                return false
            end
            --dc:GetITemperatureControl():SetFanOut(thermostats.measure.fanIndexList[1], 1)
            --dc:GetITemperatureControl():SetFanOut(thermostats.measure.fanIndexList[2], 1)
            dc:GetITemperatureControl():SetFanOut(thermostats.measure.refrigeratorIndex, 1)
        end,
        close = function()
            op:StopThermostat(thermostats.measure, false)
            dc:GetITemperatureControl():SetFanOut(thermostats.measure.refrigeratorIndex, 0)
        end,
        status = false,
    },
    -- row 17
    {
        name = "BacteriaStorageCooling",
        open = function()
            --dc:GetITemperatureControl():SetFanOut(thermostats.bacteria.fanIndexList[1], 1)
            --dc:GetITemperatureControl():SetFanOut(thermostats.bacteria.fanIndexList[2], 1)
            dc:GetITemperatureControl():SetFanOut(thermostats.bacteria.refrigeratorIndex, 1)
        end,
        close = function()
            op:StopThermostat(thermostats.bacteria, false)
            dc:GetITemperatureControl():SetFanOut(thermostats.bacteria.refrigeratorIndex, 0)
        end,
        status = false,
    },
    -- row 18
    {
        name = "CollectSampleRelay",
        open = function()
            WaterCollector.Instance():TurnOn()
        end,
        close = function()
            WaterCollector.Instance():TurnOff()
        end,
        status = false,
    },
    -- row 19
    {
        name = "Relay1",
        open = function()
            RelayControl.Instance():TurnOn(2)
        end,
        close = function()
            RelayControl.Instance():TurnOff(2)
        end,
        status = false,
    },
    -- row 20
    {
        name = "Relay2",
        open = function()
            RelayControl.Instance():TurnOn(3)
        end,
        close = function()
            RelayControl.Instance():TurnOff(3)
        end,
        status = false,
    },
    -- row 21
    {
        name = "SampleCurrent4Output",
        open = function()
            if HardwareTest[22].status or HardwareTest[23].status then
                return false
            end
            CurrentResultManager.Instance():OutputSampleCurrent(4)
            CurrentResultManager.Instance():OutputCheckCurrent(4)
        end,
        close = function()
            if HardwareTest[22].status or HardwareTest[23].status then
                return false
            end
            CurrentResultManager.Instance():OutputSample(status.measure.report.measure.consistency)
            CurrentResultManager.Instance():OutputCheck(status.measure.report.measure.consistency)
        end,
        status = false,
    },
    -- row 22
    {
        name = "SampleCurrent12Output",
        open = function()
            if HardwareTest[21].status or HardwareTest[23].status then
                return false
            end
            CurrentResultManager.Instance():OutputSampleCurrent(12)
            CurrentResultManager.Instance():OutputCheckCurrent(12)
        end,
        close = function()
            if HardwareTest[21].status or HardwareTest[23].status then
                return false
            end
            CurrentResultManager.Instance():OutputSample(status.measure.report.measure.consistency)
            CurrentResultManager.Instance():OutputCheck(status.measure.report.measure.consistency)
        end,
        status = false,
    },
    -- row 23
    {
        name = "SampleCurrent20Output",
        open = function()
            if HardwareTest[21].status or HardwareTest[22].status then
                return false
            end
            CurrentResultManager.Instance():OutputSampleCurrent(20)
            CurrentResultManager.Instance():OutputCheckCurrent(20)
        end,
        close = function()
            if HardwareTest[21].status or HardwareTest[22].status then
                return false
            end
            CurrentResultManager.Instance():OutputSample(status.measure.report.measure.consistency)
            CurrentResultManager.Instance():OutputCheck(status.measure.report.measure.consistency)
        end,
        status = false,
    },
    -- row 24
    {
        name = "SystemFan",
        open = function()
            dc:GetITemperatureControl():BoxFanSetOutput(1)
        end,
        close = function()
            dc:GetITemperatureControl():BoxFanSetOutput(0)
        end,
        status = false,
    },
    -- row 25
    {
        name = "PowerCtrl",
        open = function()
            op.IOpticalAcquire:PDPowerOn()
        end,
        close = function()
            op.IOpticalAcquire:PDPowerOff()
        end,
        status = false,
    },
}

function HardwareTest:execute(row, action)
--	print("HardwareTest try execute "..row)

    local ret
    local err,result = pcall
    (
        function()
            if action == true then
		--		print("HardwareTest try execute "..row.." open")
                ret = HardwareTest[row].open()
		--		print(ret)
            elseif action == false then
		--		print("HardwareTest try execute "..row.." close")
                ret = HardwareTest[row].close()
            end
        end
    )
    if not err then      -- 出现异常
        if type(result) == "userdata" then
            if result:GetType() == "CommandTimeoutException" then          --命令应答超时异常
                ExceptionHandler.MakeAlarm(result)
            else
                log:warn("HardwareTest:execute() =>" .. result:What())
            end
        elseif type(result) == "table" then
            log:warn("HardwareTest:execute() =>" .. result:What())								--其他定义类型异常
        elseif type(result) == "string" then
            log:warn("HardwareTest:execute() =>" .. result)	--C++、Lua系统异常
        end
--		print("Action fail")
        return false     --操作未成功
    else
        if ret == false then
--			print("operate conflict return")
            return false
        end

        if action == true then		-- 状态记录
            HardwareTest[row].status = true
        elseif action == false then
            HardwareTest[row].status = false
        end
--		print("Action success")
        return true      --操作成功
    end
end
