setting.ui.profile = {}
setting.ui.profile.measureParam =
{
    name = "measureParam",
    text = "测量参数",
    updateEvent = UpdateEvent.ChangeMeasureParam,
    index = 2,
    rowCount = 37,
    superRow = 25,
    administratorRow = 7,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    isMeaParaml = true,
    {
        name = "SampleMeasure",
        text = "水样测量",
        {
            name = "sampleReactTime",
            text = "反应时间",
            type = DataType.Int,
            unit = "分钟",
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 5 or num > 300 then
                        return string.format("%d", config.measureParam.sampleReactTime)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.sampleReactTime)
                end
            end,
        },
        {
            name = "sampleVolume",
            text = "水样体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 900 then
                        return string.format("%d", config.measureParam.sampleVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.sampleVolume)
                end
            end,
        },
        {
            name = "sampleBlankVolume",
            text = "空白水体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 900 then
                        return string.format("%d", config.measureParam.sampleBlankVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.sampleBlankVolume)
                end
            end,
        },
    },
    {
        name = "StandardMeasure",
        text = "质控测量",
        {
            name = "standardReactTime",
            text = "反应时间",
            type = DataType.Int,
            unit = "分钟",
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 5 or num > 300 then
                        return string.format("%d", config.measureParam.standardReactTime)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.standardReactTime)
                end
            end,
        },
        {
            name = "standardVolume",
            text = "质控样体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 900 then
                        return string.format("%d", config.measureParam.standardVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.standardVolume)
                end
            end,
        },
        {
            name = "standardBlankVolume",
            text = "空白水体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 900 then
                        return string.format("%d", config.measureParam.standardBlankVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.standardBlankVolume)
                end
            end,
        },
    },
    {
        name = "BlankMeasure",
        text = "空白水测量",
        {
            name = "blankReactTime",
            text = "反应时间",
            type = DataType.Int,
            unit = "分钟",
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 5 or num > 300 then
                        return string.format("%d", config.measureParam.blankReactTime)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.blankReactTime)
                end
            end,
        },
    },
    {
        name = "Common",
        text = "公共",
        {
            name = "cleanBeforeMeasureTimes",
            text = "测量前清洗",
            type = DataType.Int,
            unit = "次数",
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 1 or num > 10 then
                        return string.format("%d", config.measureParam.cleanBeforeMeasureTimes)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.cleanBeforeMeasureTimes)
                end
            end,
        },
        {
            name = "cleanAfterMeasureTimes",
            text = "测量后清洗",
            type = DataType.Int,
            unit = "次数",
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 1 or num > 10 then
                        return string.format("%d", config.measureParam.cleanAfterMeasureTimes)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.cleanAfterMeasureTimes)
                end
            end,
        },
        {
            name = "addSaltVolume",
            text = "调节液更新体积",
            unit = "mL",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%d", config.measureParam.addSaltVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.addSaltVolume)
                end
            end,
        },
        {
            name = "addBlankVolume",
            text = "空白水更新体积",
            unit = "mL",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%d", config.measureParam.addBlankVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.addBlankVolume)
                end
            end,
        },
	    {
            name = "addSampleVolume",
            text = "水样更新体积",
            unit = "mL",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%d", config.measureParam.addSampleVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.addSampleVolume)
                end
            end,
        },
        {
            name = "temperatureMonitor",
            text = "机箱温度监控",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
        name = "PMTAdjust",
        text = "PMT模块校准",
        {
            name = "pmtAuto",
            text = "自动调节",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "destTarget",
            text = "调节目标",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 20000000 then
                        return string.format("%d", config.measureParam.destTarget)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.destTarget)
                end
            end,
        },
        {
            name = "destLowLimit",
            text = "调节下限",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 20000000 then
                        return string.format("%d", config.measureParam.destLowLimit)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.destLowLimit)
                end
            end,
        },
        {
            name = "destHighLimit",
            text = "调节上限",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num <= 0 or num > 20000000 then
                        return string.format("%d", config.measureParam.destHighLimit)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.destHighLimit)
                end
            end,
        },
    },
    {
        name = "AirMix",
        text = "发光菌混合",
        {
            name = "airMixHeight",
            text = "吹气高度",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 300 then
                        return string.format("%d", config.measureParam.airMixHeight)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.airMixHeight)
                end
            end,
        },
        {
            name = "airMixSignleTime",
            text = "单针吹气时间",
            unit = "秒",
            type = DataType.Float,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.arguPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 5 then
                        return string.format("%.2f", config.measureParam.airMixSignleTime)
                    else
                        return value
                    end
                else
                    return string.format("%.2f", config.measureParam.airMixSignleTime)
                end
            end,
        },
        {
            name = "airMixDoubleTime",
            text = "双针吹气时间",
            unit = "秒",
            type = DataType.Float,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.arguPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 5 then
                        return string.format("%.2f", config.measureParam.airMixDoubleTime)
                    else
                        return value
                    end
                else
                    return string.format("%.2f", config.measureParam.airMixDoubleTime)
                end
            end,
        },
        {
            name = "airMixLoopTimes",
            text = "循环次数",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 1 or num > 10 then
                        return string.format("%d", config.measureParam.airMixLoopTimes)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.airMixLoopTimes)
                end
            end,
        },
    },
    {
        name = "Debug",
        text = "调试",
        {
            name = "stirSpeed",
            text = "搅拌速度",
            unit = "步/秒",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 20 or num > 800 then
                        return string.format("%d", config.measureParam.stirSpeed)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.stirSpeed)
                end
            end,
        },
        {
            name = "bacteriaSaltVolume",
            text = "公共渗透压体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 500 then
                        return string.format("%d", config.measureParam.bacteriaSaltVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.bacteriaSaltVolume)
                end
            end,
        },
        {
            name = "bacteriaBlankVolume",
            text = "菌液体空白水体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 900 then
                        return string.format("%d", config.measureParam.bacteriaBlankVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.bacteriaBlankVolume)
                end
            end,
        },
        {
            name = "bacteriaVolume",
            text = "发光菌体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%d", config.measureParam.bacteriaVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.bacteriaVolume)
                end
            end,
        },
        {
            name = "sampleSaltVolume",
            text = "水样渗透压体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 500 then
                        return string.format("%d", config.measureParam.sampleSaltVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.sampleSaltVolume)
                end
            end,
        },
        {
            name = "standardSaltVolume",
            text = "质控渗透压体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 500 then
                        return string.format("%d", config.measureParam.standardSaltVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.standardSaltVolume)
                end
            end,
        },
        {
            name = "blankSaltVolume",
            text = "空白渗透压体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 500 then
                        return string.format("%d", config.measureParam.blankSaltVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.blankSaltVolume)
                end
            end,
        },
        {
            name = "incubationTime",
            text = "孵育时间",
            type = DataType.Int,
            unit = "秒",
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 300 then
                        return string.format("%d", config.measureParam.incubationTime)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.incubationTime)
                end
            end,
        },
        {
            name = "mixSwitch",
            text = "混样开关",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "mixBacteriaSampleMethod",
            text = "菌样混合方式",
            type = DataType.Option,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            options =
            {
                "冒泡混匀",
                "注射器混匀",
            },
        },
        {
            name = "mixBacteriaSampleTimes",
            text = "菌样混合",
            type = DataType.Int,
            unit = "次数",
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%d", config.measureParam.mixBacteriaSampleTimes)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.mixBacteriaSampleTimes)
                end
            end,
        },
        {
            name = "mixBacteriaVolume",
            text = "发光菌混合液体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 800 then
                        return string.format("%d", config.measureParam.mixBacteriaVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.mixBacteriaVolume)
                end
            end,
        },
        {
            name = "mixSampleVolume",
            text = "样本混合液体积",
            unit = "uL",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 800 then
                        return string.format("%d", config.measureParam.mixSampleVolume)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.measureParam.mixSampleVolume)
                end
            end,
        },
        {
            name = "measureSafetyTemp",
            text = "测量仓安全温度",
            type = DataType.Float,
            unit = "℃",
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.tempPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 50 then
                        return string.format("%.1f", config.measureParam.measureSafetyTemp)
                    else
                        return value
                    end
                else
                    return string.format("%.1f", config.measureParam.measureSafetyTemp)
                end
            end,
        },
        {
            name = "bacteriaSafetyTemp",
            text = "菌室安全温度",
            type = DataType.Float,
            unit = "℃",
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.tempPattern(value) == true then
                    local num = tonumber(value)
                    if num < -10 or num > 20 then
                        return string.format("%.1f", config.measureParam.bacteriaSafetyTemp)
                    else
                        return value
                    end
                else
                    return string.format("%.1f", config.measureParam.bacteriaSafetyTemp)
                end
            end,
        },
        {
            name = "openTemperatureControl",
            text = "温控异常告警",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    defaultRestore = function(userType)

        if userType == RoleType.Super then

            local defaultTable = ConfigLists.LoadMeasureParamConfig(true)
            Helper.DefaultRestore(defaultTable, config.measureParam)
            log:info(setting.ui.profile.measureParam.text .. "恢复默认")
            config.modifyRecord.measureParam(true)
            ConfigLists.SaveMeasureParamConfig()

        elseif userType == RoleType.Administrator then

            local defaultTable = ConfigLists.LoadMeasureParamConfig(true)
            config.measureParam.sampleReactTime = defaultTable.sampleReactTime
            config.measureParam.sampleVolume = defaultTable.sampleVolume
            config.measureParam.sampleBlankVolume = defaultTable.sampleBlankVolume
            config.measureParam.standardReactTime = defaultTable.standardReactTime
            config.measureParam.blankReactTime = defaultTable.blankReactTime
            config.measureParam.cleanBeforeMeasureTimes = defaultTable.cleanBeforeMeasureTimes
            config.measureParam.cleanAfterMeasureTimes = defaultTable.cleanAfterMeasureTimes
            config.measureParam.temperatureMonitor = defaultTable.temperatureMonitor
	        config.measureParam.addSaltVolume = defaultTable.addSaltVolume
            config.measureParam.addBlankVolume = defaultTable.addBlankVolume
            config.measureParam.addSampleVolume = defaultTable.addSampleVolume
            config.measureParam.openTemperatureControl = defaultTable.openTemperatureControl

            log:info(setting.ui.profile.measureParam.text .. "恢复默认")
            config.modifyRecord.measureParam(true)
            ConfigLists.SaveMeasureParamConfig()
        end

        return false  --无需重启仪器
    end,
    saveFile = function(isUpdate)
        log:info("修改" .. setting.ui.profile.measureParam.text)
        config.modifyRecord.measureParam(isUpdate)
        ConfigLists.SaveMeasureParamConfig()
	    return false  --无需重启仪器
    end,
    integerPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local integerPattern = "^%d?%d?%d?%d?%d?%d?%d?%d$"
            if string.find(value, integerPattern) then
                ret = true
            end
            return ret
        else
            return false
        end
    end,
    tempPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^[-+]?[0-1]?%d?%d%.%d$"
            local integerPatterm = "^[-+]?[0-1]?%d?%d$"
            if not string.find(value, decimalPatterm) then
                if string.find(value, integerPatterm) then
                    ret = true
                end
            else
                ret = true
            end
            return ret
        else
            return false
        end
    end,
    timePattern = function(value)
        if type(value) == "string" then
            local patterm = "^%d?%d?%d?%d$"
            if not string.find(value, patterm) then
                return false
            else
                return true
            end
            return ret
        else
            return false
        end
    end,
    arguPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^[-+]?%d?%d?%d?%d%.%d%d?%d?%d?%d?%d?$"
            local integerPatterm = "^[-+]?%d?%d?%d?%d$"
            if not string.find(value, decimalPatterm) then
                if string.find(value, integerPatterm) then
                    ret = true
                end
            else
                ret = true
            end
            return ret
        else
            return false
        end
    end,
    twoDecimalPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^%d?%d%.%d%d?$"
            local integerPatterm = "^%d?%d$"
            if not string.find(value, decimalPatterm) then
                if string.find(value, integerPatterm) then
                    ret = true
                end
            else
                ret = true
            end
            return ret
        else
            return false
        end
    end,
    meterPointMultipleCheck = function(value)

        local pointNum = config.hardwareConfig.meterPoint.num
        local maxPoint = config.hardwareConfig.meterPoint.point[pointNum].setVolume
        local minPoint = config.hardwareConfig.meterPoint.point[1].setVolume
        local PRECISE = 0.000001
        local operateVol = 0

        local vol = tonumber(value)

        if not vol then
            return false, string.format("%.2f", 0)
        end


        local volList = {}

        for i = 1, pointNum do
            volList[i] = config.hardwareConfig.meterPoint.point[i].setVolume
        end

        if vol <= PRECISE then -- 输入体积为0

            operateVol = 0

        elseif volList[1] - vol >= -PRECISE then -- 输入体积小于或等于低定量点

            operateVol = minPoint

        elseif volList[1] - vol < -PRECISE  and
            volList[pointNum] - vol > PRECISE then -- 输入体积大于低定量点，小于高定量点

            pointNum = pointNum + 1
            volList[pointNum] = vol

            local temp = 0
            for i = 1, pointNum - 1 do
                for j = pointNum, i + 1, -1 do
                    if volList[j] < volList[j - 1] then
                        temp = volList[j - 1]
                        volList[j - 1] = volList[j]
                        volList[j] = temp
                    end
                end
            end

            local index = 1
            for i = 1, pointNum do
                if (-PRECISE <= volList[i] - vol) and
                    (volList[i] - vol <= PRECISE) then
                    break;
                end
                index = index + 1
            end

            local pPoint = volList[index - 1]
            local nPoint = volList[index + 1]
            local pD = vol - pPoint

            if pD < (nPoint - pPoint) / 2 then
                operateVol = pPoint
            else
                operateVol = nPoint
            end
        else -- 输入体积大于或等于高定量点

            local isMatch = false

            for i = 1, pointNum do
                local MP = volList[i]
                local fcount = vol / MP
                local count = math.floor(fcount + PRECISE)
                local residue = vol - count * MP

                if (-PRECISE <= residue) and (residue <= PRECISE) then
                    operateVol = vol
                    isMatch = true
                    break
                end
            end

            if isMatch == false then
                return false, string.format("%.2f", 0)
                --operateVol = maxPoint
            end

        end

        return true, string.format("%.2f", operateVol)
    end,
}
return setting.ui.profile.measureParam
