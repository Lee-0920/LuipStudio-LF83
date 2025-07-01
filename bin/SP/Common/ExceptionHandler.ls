local _G = _ENV
local print = print
local Alarm = Alarm
local log = log
local Helper = Helper
local AlarmManager = AlarmManager
local setting = setting
local getmetatable = getmetatable
local PumpException = PumpException
local PumpFailedException = PumpFailedException
local PumpStoppedException = PumpStoppedException
local MotorException = MotorException
local MotorFailedException = MotorFailedException
local MotorStoppedException = MotorStoppedException
local RefCollideException = RefCollideException
local MeaCollideException = MeaCollideException
local MotorXMoveOutSensorFailException = MotorXMoveOutSensorFailException
local MotorZMoveOutSensorFailException = MotorZMoveOutSensorFailException
local MotorXMoveInSensorFailException = MotorXMoveInSensorFailException
local MotorZMoveInSensorFailException = MotorZMoveInSensorFailException
local SyringeMoveOutSensorFailException = SyringeMoveOutSensorFailException
local SyringeMoveInSensorFailException = SyringeMoveInSensorFailException
local DriverErrorException = DriverErrorException
local MeterException = MeterException
local MeterStoppedException = MeterStoppedException
local MeterFailedException = MeterFailedException
local MeterOverflowException = MeterOverflowException
local MeterUnfinishedException = MeterUnfinishedException
local MeterAirBubbleException = MeterAirBubbleException
local MeterExpectTimeoutException = MeterExpectTimeoutException

local ReagentRemainWarn = ReagentRemainWarn
local MaterialGoBadException = MaterialGoBadException

local ThermostatException = ThermostatException
local ThermostatFailedException = ThermostatFailedException
local ThermostatStoppedException = ThermostatStoppedException
local ThermostatTimeoutException = ThermostatTimeoutException

local UpdateException = UpdateException

local MeasureFlowException = MeasureFlowException
local UserStopException = UserStopException
local DeviceFaultException = DeviceFaultException
local DrainFromDigestionException = DrainFromDigestionException
local CalibratePumpException = CalibratePumpException
local MeasureResultOutLimitException = MeasureResultOutLimitException
local CalibrateResultWrongException = CalibrateResultWrongException
local MeasureResultWrongException = MeasureResultWrongException

local AcquirerADException = AcquirerADException
local AcquirerADFailedException = AcquirerADFailedException
local AcquirerADStoppedException = AcquirerADStoppedException

local StaticADControlException = StaticADControlException
local StaticADControlFailedException = StaticADControlFailedException
local StaticADControlStoppedException = StaticADControlStoppedException

local Exception = Exception
local tostring = tostring
local type = type
local ipairs =ipairs
local string = string

local P = {}
ExceptionHandler = P
_ENV = P

local GetLiquidName = function(name)
    local result = ""

    if name == "Blank" then
        result = "空白水"

    elseif name == "Standard" then
        result = "质控液"

    elseif name == "Sample" then
        result = "水样"

    elseif name == "Bacteria" then
        result = "发光菌"

    elseif name == "Waste" then
        result = "废液"

    elseif name == "Wash" then
        result = "清洗水"

    end

    return result
end

MakeAlarm  = function(exception)

    local str = ""
    local alarm
    if type(exception) == "userdata" then -- C++向LUA抛出的异常为自定义数据(通信异常)
        local errortype = exception:GetType()
        if errortype == "CommandTimeoutException" or
            errortype == "ExpectEventTimeoutException" then
            local a1 = exception:GetAddrA1()
            local a2 = exception:GetAddrA2()
            local a3 = exception:GetAddrA3()
            local a4 = exception:GetAddrA4()

            for k,v in ipairs(setting.plugins.controllers) do
                if a1 == v.address[1] and
                a2 == v .address[2] and
                a3 == v .address[3] and
                a4 == v .address[4] then

                    if v.name == "LF82DriveControllerPlugin" then
                        str = str .. "驱动板"
                    end
                    break
                end
            end

            if errortype == "CommandTimeoutException" then
                str = str .. "-命令"
            else
                str = str .. "-事件"
            end

            str = str .. "-[" .. string.format("0x%04X", exception:GetCode()) .. "]超时"

            alarm = Helper.MakeAlarm(setting.alarm.communication, str)
            AlarmManager.Instance():AddAlarm(alarm)
        end

    elseif type(exception) == "table" then -- LUA定义的异常为表

        if getmetatable(exception) == PumpException or
           getmetatable(getmetatable(exception)) == PumpException then

            str = GetLiquidName(exception.liquidType.name)
            str = str .. "-泵"
            if exception.dir == 0 then
                str = str .. "抽"
            else
                str = str .. "排"
            end

            if getmetatable(exception) == PumpStoppedException then
                alarm = Helper.MakeAlarm(setting.alarm.pumpStop, str)
            else
                alarm = Helper.MakeAlarm(setting.alarm.pumpFailed, str)
            end

            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == MotorException or
        getmetatable(getmetatable(exception)) == MotorException then


            if getmetatable(exception) == MotorStoppedException then
                alarm = Helper.MakeAlarm(setting.alarm.motorStop, "")
            elseif getmetatable(exception) == RefCollideException then
                alarm = Helper.MakeAlarm(setting.alarm.refCollide, "")
            elseif getmetatable(exception) == MeaCollideException then
                alarm = Helper.MakeAlarm(setting.alarm.meaCollide, "")
            elseif getmetatable(exception) == MotorXMoveOutSensorFailException then
                alarm = Helper.MakeAlarm(setting.alarm.motorXMoveOutSensorFail, "")
            elseif getmetatable(exception) == MotorZMoveOutSensorFailException then
                alarm = Helper.MakeAlarm(setting.alarm.motorZMoveOutSensorFail, "")
            elseif getmetatable(exception) == MotorXMoveInSensorFailException then
                alarm = Helper.MakeAlarm(setting.alarm.motorXMoveInSensorFail, "")
            elseif getmetatable(exception) == MotorZMoveInSensorFailException then
                alarm = Helper.MakeAlarm(setting.alarm.motorZMoveInSensorFail, "")
            elseif getmetatable(exception) == SyringeMoveOutSensorFailException then
                alarm = Helper.MakeAlarm(setting.alarm.syringeMoveOutSensorFail, "")
            elseif getmetatable(exception) == SyringeMoveInSensorFailException then
                alarm = Helper.MakeAlarm(setting.alarm.syringeMoveInSensorFail, "")
            elseif getmetatable(exception) == DriverErrorException then
                alarm = Helper.MakeAlarm(setting.alarm.motorDriverError, "")
            else
                alarm = Helper.MakeAlarm(setting.alarm.motorFailed, "")
            end

            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == AcquirerADException or
           getmetatable(getmetatable(exception)) == AcquirerADException then

            if getmetatable(exception) == AcquirerADFailedException then
                alarm = Helper.MakeAlarm(setting.alarm.acquirerADFailed, "")
            else
                alarm = Helper.MakeAlarm(setting.alarm.acquirerADStop, "")
            end
            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == StaticADControlException or
        getmetatable(getmetatable(exception)) == StaticADControlException then

            if getmetatable(exception) == StaticADControlFailedException then
                alarm = Helper.MakeAlarm(setting.alarm.staticADControlFailed, "")
            else
                alarm = Helper.MakeAlarm(setting.alarm.staticADControlStopped, "")
            end
            AlarmManager.Instance():AddAlarm(alarm)
            
        elseif getmetatable(exception) == MeterException or
           getmetatable(getmetatable(exception)) == MeterException then

            str = GetLiquidName(exception.liquidType.name)
            str = str .. "-定量"

            if getmetatable(exception) == MeterStoppedException then
                alarm = Helper.MakeAlarm(setting.alarm.meterStop, str)
            else
                if getmetatable(exception) == MeterFailedException then
                    str = str .. "-失败"
                elseif getmetatable(exception) == MeterOverflowException then
                    str = str .. "-溢出"
                elseif getmetatable(exception) == MeterUnfinishedException then
                    str = str .. "-抽取不上"
                elseif getmetatable(exception) == MeterAirBubbleException then
                    str = str .. "-出现气泡"
                elseif getmetatable(exception) == MeterExpectTimeoutException then
                    str = str .. "-事件超时"
                end
                alarm = Helper.MakeAlarm(setting.alarm.meterFailed, str)
            end

            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == ReagentRemainWarn then
            str = GetLiquidName(exception.liquidType.name)
            alarm = Helper.MakeAlarm(setting.alarm.reagentRemain, str)
            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == MaterialGoBadException then
            str = exception.materialType.text
            alarm = Helper.MakeAlarm(setting.alarm.materialGoBad, str)
            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == ThermostatException or
           getmetatable(getmetatable(exception)) == ThermostatException then

            if exception.mode  == 0 then
                str = "自动恒温"
            elseif exception.mode  == 1 then
                str = "加热"
            elseif exception.mode  == 2 then
                str = "冷却"
            elseif exception.mode  == 3 then
                str = "自然冷却"
            end

            if getmetatable(exception) == ThermostatStoppedException then
                str = str .. "-目标" .. tostring(exception.targetTemp) .. "度"
                alarm = Helper.MakeAlarm(setting.alarm.thermostatStop, str)
            else
                str = "-" .. str
                if getmetatable(exception) == ThermostatFailedException then
                    str = str .. "-失败，传感器异常"
                elseif getmetatable(exception) == ThermostatTimeoutException then
                    str = str .. "-超时"
                end
                str = str .. "-目标" .. tostring(exception.targetTemp) .. "度"
                alarm = Helper.MakeAlarm(setting.alarm.thermostatFault, str)
            end

            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == UpdateException  then
            str = exception.updateType.name
            alarm = Helper.MakeAlarm(setting.alarm.updateFault, str)
            AlarmManager.Instance():AddAlarm(alarm)

        elseif getmetatable(exception) == MeasureFlowException or
           getmetatable(getmetatable(exception)) == MeasureFlowException then

            if getmetatable(exception) == UserStopException then
                alarm = Helper.MakeAlarm(setting.alarm.userStop, "")
                AlarmManager.Instance():AddAlarm(alarm)

            elseif getmetatable(exception) == DeviceFaultException then
                alarm = Helper.MakeAlarm(setting.alarm.deviceRunFault, "")
                AlarmManager.Instance():AddAlarm(alarm)

            elseif getmetatable(exception) == DrainFromDigestionException then
                alarm = Helper.MakeAlarm(setting.alarm.drainFromDigestionFault, "")
                AlarmManager.Instance():AddAlarm(alarm)

            elseif getmetatable(exception) == CalibratePumpException then
                alarm = Helper.MakeAlarm(setting.alarm.calibratePumpFailed, "")
                AlarmManager.Instance():AddAlarm(alarm)

            elseif getmetatable(exception) == MeasureResultOutLimitException then
                alarm = Helper.MakeAlarm(setting.alarm.measureResultOutLimit, "")
                AlarmManager.Instance():AddAlarm(alarm)

            elseif getmetatable(exception) == CalibrateResultWrongException then
                alarm = Helper.MakeAlarm(setting.alarm.calibrateResultWrong, "")
                AlarmManager.Instance():AddAlarm(alarm)

            elseif getmetatable(exception) == MeasureResultWrongException then
                alarm = Helper.MakeAlarm(setting.alarm.measureResultWrong, "")
                AlarmManager.Instance():AddAlarm(alarm)
            end

        elseif getmetatable(exception) == Exception then
            alarm = Helper.MakeAlarm(setting.alarm.exception, "")
            AlarmManager.Instance():AddAlarm(alarm)
        end
    end
end


return ExceptionHandler
