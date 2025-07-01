package.path = [[?;?.lua;?.ls;?;./SP/Controller/?.ls;./SP/Flow/?.ls;./SP/Common/?.ls;./SP/ExternalInterface/?.ls;]]
require("ControllerCreater")
require("FlowList")
require("Flow")
require("Measurer")
require("MeasureFlow")
require("CleanFlow")
require("LiquidOperateFlow")
require("MotorOperateFlow")
require("CollectSampleFlow")
require("HardwareTest")
require("CommunicationCheckFlow")
require("SensorCheckFlow")
require("MachineResetFlow")
require("LiquidPipeRecoverFlow")
require("TemperatureControlRecoverFlow")
require("MeasureModuleCheckFlow")
require("ModbusInterface")

package.path = [[?;?.lua;?.ls;?;./SP/Setting/?.ls;./SP/Setting/Modbus/?.ls;]]
if config.system.modbusTableType == 1 then
    require("LF88")
    modbusStr = "LF88"
else
    require("Labsun10")
    modbusStr = "LS1.0"
end

--环境温度监控
function EnvTempHandle(eTemp)
    if config.measureParam.temperatureMonitor == true then
        op:EevTempCheck(eTemp)
    end
end

--仪器初始化
function InitInstrument()
    log:debug("Init Instrument")
    if dc:GetConnectStatus() then
        --启动恒温
        op:StartThermostat(thermostats.bacteria, ThermostatMode.Refrigerate, config.measureParam.bacteriaSafetyTemp, 0, setting.temperature.coolingTimeout, false)

        local environmentTemp = dc:GetCurrEnvironmentTemp()
        log:debug("环境温度 temp = "..environmentTemp)

        --环境温度高时采用纯制冷模式
        if environmentTemp >=  config.measureParam.measureSafetyTemp then
            op:StartThermostat(thermostats.measure, ThermostatMode.Refrigerate, config.measureParam.measureSafetyTemp, 0, setting.temperature.coolingTimeout, false)
        else
            op:StartThermostat(thermostats.measure, ThermostatMode.Heater, config.measureParam.measureSafetyTemp, 0, setting.temperature.coolingTimeout, false)
        end

    end
end

--下位机复位
function DeviceResetHandle()
    log:debug("Device Reset Handle")
    if dc:GetConnectStatus() then
        --启动恒温
        op:StartThermostat(thermostats.bacteria, ThermostatMode.Refrigerate, config.measureParam.bacteriaSafetyTemp, 0, setting.temperature.coolingTimeout, false)

        local environmentTemp = dc:GetCurrEnvironmentTemp()
        log:debug("环境温度 temp = "..environmentTemp)

        --环境温度高时采用纯制冷模式
        if environmentTemp >=  config.measureParam.measureSafetyTemp then
            op:StartThermostat(thermostats.measure, ThermostatMode.Refrigerate, config.measureParam.measureSafetyTemp, 0, setting.temperature.coolingTimeout, false)
        else
            op:StartThermostat(thermostats.measure, ThermostatMode.Heater, config.measureParam.measureSafetyTemp, 0, setting.temperature.coolingTimeout, false)
        end
    end
end

function UpdateMeasureTimeStatus(oldTime)

	print("oldTime = " .. oldTime)

    local err,ret = pcall(function()
		local currTime = os.time()
		print("currTime = " .. currTime)

		status.measure.schedule.autoMeasure.dateTime = currTime - (oldTime - status.measure.schedule.autoMeasure.dateTime)
		status.measure.schedule.autoCheck.dateTime = currTime - (oldTime - status.measure.schedule.autoCheck.dateTime)
        status.measure.schedule.autoBlank.dateTime = currTime - (oldTime - status.measure.schedule.autoBlank.dateTime)
        status.measure.schedule.autoClean.dateTime = currTime - (oldTime - status.measure.schedule.autoClean.dateTime)

		ConfigLists.SaveMeasureStatus()

        return true
    end)

    if not err then      -- 出现异常
        if type(ret) == "userdata" then
            log:warn("UpdateMeasureTimeStatus() =>" .. ret:What())
        elseif type(ret) == "table" then
            log:warn("UpdateMeasureTimeStatus() =>" .. ret:What())
        elseif type(ret) == "string" then
            log:warn("UpdateMeasureTimeStatus() =>" .. ret)	--C++、Lua系统异常
        end
    end
end
