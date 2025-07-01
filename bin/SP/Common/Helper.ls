local _G = _ENV
local print = print
local Alarm = Alarm
local AlarmManager = AlarmManager
local RunStatus = RunStatus
local RunAction = RunAction
local type = type
local pairs = pairs
local Serialization = Serialization
local status = status
local config = config
local scriptsPath = scriptsPath
local ReportMode = ReportMode
local MeasureType = MeasureType
local setting = setting
local RelayControl = RelayControl
local RelayOut = RelayOut
local os = os
local CurrentResultManager = CurrentResultManager
local MeasureResultOutLimitException = MeasureResultOutLimitException
local ExceptionHandler = ExceptionHandler
local log = log

local P = {}
Helper = P
_ENV = P

MakeAlarm  = function (alarm, cause)
    return Alarm.new(alarm.type, alarm.name, cause, alarm.level)
end

DefaultRestore = function(defaultTable, destinationTable)

    if type(defaultTable) == "table" and type(destinationTable) == "table" then

        for k,v in pairs(defaultTable) do

            if type(v) == "table" then
                DefaultRestore(v, destinationTable[k])
            else
                destinationTable[k] = v
            end

        end
        return true
    else
        return false
    end
end

Status = {}

Status.SetStatus = function(statusTable)

    if type(statusTable.GetTime) == "function" then

        return RunStatus.new(statusTable.name, statusTable.text, statusTable:GetTime())

    else

        return RunStatus.new(statusTable.name, statusTable.text, 0)

    end
end

Status.SetAction = function(actionTable)

    if type(actionTable.GetTime) == "function" then

        return RunAction.new(actionTable.name, actionTable.text, actionTable.GetTime())

    else

        return RunAction.new(actionTable.name, actionTable.text, 0)

    end
end

Result = {}
Result.OnCommonMeasureResultAdded = function(name, resultType, dateTime, consistency, mode)
    Result.OnMeasureResultAdded(resultType, dateTime, consistency, mode, 0, 1, 1, 1, 1)
end

Result.OnMeasureResultAdded = function(resultType, dateTime, consistency, mode, absorbance, C0, S0, Ct, St)
    local currentResultManager = CurrentResultManager.Instance()

    status.measure.newResult.measure.dateTime = dateTime
    status.measure.newResult.measure.consistency = consistency
    status.measure.newResult.measure.resultType = resultType
    status.measure.newResult.measure.IR = consistency  --抑制率
    status.measure.newResult.measure.RLI = 100 - consistency  --相对发光度
    status.measure.newResult.measure.CF = absorbance    --相对稳定系数
    status.measure.newResult.measure.C0 = C0    --初始参考光强
    status.measure.newResult.measure.Ct = Ct    --结束参考光强
    status.measure.newResult.measure.S0 = S0    --初始测量光强
    status.measure.newResult.measure.St = St    --结束测量光强

    if mode == ReportMode.OnLine then
        if resultType == MeasureType.Standard then
            status.measure.newResult.measure.resultInfo = "N"        --正常
        else
            if consistency > config.interconnection.meaUpLimit then
                status.measure.newResult.measure.resultInfo = "T"        --超仪器上限
            elseif consistency < -config.interconnection.meaLowLimit then
                status.measure.newResult.measure.resultInfo = "L"        --超仪器下限
            else
                status.measure.newResult.measure.resultInfo = "N"        --正常
            end
        end
    elseif mode == ReportMode.OffLine then
        status.measure.newResult.measure.resultInfo = "B"    --仪器离线
    elseif mode == ReportMode.Maintain or mode == ReportMode.Calibrate then
        status.measure.newResult.measure.resultInfo = "M"    --维护数据
    elseif mode == ReportMode.Fault then
        status.measure.newResult.measure.resultInfo = "D"    --故障数据
    end

    if resultType == MeasureType.Sample then
        status.measure.schedule.autoMeasure.dateTime = dateTime

        if mode ~= ReportMode.OffLine then
            status.measure.report.measure.dateTime = dateTime
            status.measure.report.measure.consistency = consistency
            status.measure.report.measure.resultType = resultType
            status.measure.report.measure.measureTime = os.time() - dateTime
            status.measure.report.measure.absorbance = absorbance
            status.measure.report.measure.IR = consistency    --抑制率
            status.measure.report.measure.RLI = 100 - consistency    --相对发光度
            status.measure.report.measure.CF = absorbance    --相对稳定系数
            status.measure.report.measure.C0 = C0    --初始参考光强
            status.measure.report.measure.Ct = Ct    --结束参考光强
            status.measure.report.measure.S0 = S0    --初始测量光强
            status.measure.report.measure.St = St    --结束测量光强
	        status.measure.report.measure.deviceTypeNum = setting.instrument.deviceTypeNum.PHOT
            currentResultManager:OutputSample(consistency)
        end
    elseif resultType == MeasureType.Standard then
        status.measure.schedule.autoCheck.dateTime = dateTime

        if mode ~= ReportMode.OffLine then
            status.measure.report.check.dateTime = dateTime
            status.measure.report.check.consistency = consistency
            status.measure.report.check.IR = consistency   --抑制率
            status.measure.report.check.RLI = 100 - consistency   --相对发光度
            status.measure.report.check.CF = absorbance    --相对稳定系数
            status.measure.report.check.C0 = C0    --初始参考光强
            status.measure.report.check.Ct = Ct    --结束参考光强
            status.measure.report.check.S0 = S0    --初始测量光强
            status.measure.report.check.St = St    --结束测量光强
        end

    elseif resultType == MeasureType.Blank then
        status.measure.schedule.autoBlank.dateTime = dateTime

        if mode ~= ReportMode.OffLine then
            status.measure.report.blank.dateTime = dateTime
            status.measure.report.blank.consistency = consistency
            status.measure.report.blank.IR = consistency   --抑制率
            status.measure.report.blank.RLI = 100 - consistency   --相对发光度
            status.measure.report.blank.CF = absorbance    --相对稳定系数
            status.measure.report.blank.C0 = C0    --初始参考光强
            status.measure.report.blank.Ct = Ct    --结束参考光强
            status.measure.report.blank.S0 = S0    --初始测量光强
            status.measure.report.blank.St = St    --结束测量光强
        end
    end

    if config.interconnection.alarmValue == true then
        if (consistency > config.interconnection.meaUpLimit) and (resultType == MeasureType.Sample) then
            local alarm = MakeAlarm(setting.alarm.measureResultOutLimit, "")
            AlarmManager.Instance():AddAlarm(alarm)
            Result.RelayOutOperate(setting.mode.relayOut.upLimit, true)
        elseif (consistency < -config.interconnection.meaLowLimit) and (resultType == MeasureType.Sample) then
            local alarm = MakeAlarm(setting.alarm.measureResultOutLimit, "")
            AlarmManager.Instance():AddAlarm(alarm)
            Result.RelayOutOperate(setting.mode.relayOut.lowLimit, true)
        else
            Result.RelayOutOperate(setting.mode.relayOut.upLimit, false)
            Result.RelayOutOperate(setting.mode.relayOut.lowLimit, false)
        end
    end

end

Result.RelayOutOperate = function(operate,  status)
    local relayOne = config.interconnection.relayOne
    local relayTwo = config.interconnection.relayTwo
    local relayControl = RelayControl.Instance()

    if  operate == setting.mode.relayOut.lowLimit then
        if relayOne == setting.mode.relayOut.lowLimit then
            if status == true then
                relayControl:TurnOn(2)
            else
                relayControl:TurnOff(2)
            end
        end

        if relayTwo == setting.mode.relayOut.lowLimit then
            if status == true then
                relayControl:TurnOn(3)
            else
                relayControl:TurnOff(3)
            end
        end
    elseif operate == setting.mode.relayOut.upLimit then
        if relayOne == setting.mode.relayOut.upLimit then
            if status == true then
                relayControl:TurnOn(2)
            else
                relayControl:TurnOff(2)
            end
        end

        if relayTwo == setting.mode.relayOut.upLimit then
            if status == true then
                relayControl:TurnOn(3)
            else
                relayControl:TurnOff(3)
            end
        end
    elseif operate == setting.mode.relayOut.measureInstruct then
        if relayOne == setting.mode.relayOut.measureInstruct then
            if status == true then
                relayControl:TurnOn(2)
            else
                relayControl:TurnOff(2)
            end
        end

        if relayTwo == setting.mode.relayOut.measureInstruct then
            if status == true then
                relayControl:TurnOn(3)
            else
                relayControl:TurnOff(3)
            end
        end
    elseif operate == setting.mode.relayOut.calibrateInstruct then
        if relayOne == setting.mode.relayOut.calibrateInstruct then
            if status == true then
                relayControl:TurnOn(2)
            else
                relayControl:TurnOff(2)
            end
        end

        if relayTwo == setting.mode.relayOut.calibrateInstruct then
            if status == true then
                relayControl:TurnOn(3)
            else
                relayControl:TurnOff(3)
            end
        end
    elseif operate == setting.mode.relayOut.cleanInstruct then
        if relayOne == setting.mode.relayOut.cleanInstruct then
            if status == true then
                relayControl:TurnOn(2)
            else
                relayControl:TurnOff(2)
            end
        end

        if relayTwo == setting.mode.relayOut.cleanInstruct then
            if status == true then
                relayControl:TurnOn(3)
            else
                relayControl:TurnOff(3)
            end
        end

    elseif operate == setting.mode.relayOut.collectInstruct then
        if relayOne == setting.mode.relayOut.collectInstruct then
            if status == true then
                relayControl:TurnOn(2)
            else
                relayControl:TurnOff(2)
            end
        end

        if relayTwo == setting.mode.relayOut.collectInstruct then
            if status == true then
                relayControl:TurnOn(3)
            else
                relayControl:TurnOff(3)
            end
        end
    end
end

return Helper
