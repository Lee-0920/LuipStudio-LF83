setting.ui.profile.interconnection =
{
    text = "外联接口",
    name = "interconnection",
    updateEvent = UpdateEvent.ChangeInterconnectionParam,
    index = 5,
    rowCount = 17,
    superRow = 0,
    administratorRow = 17,
    writePrivilege=  RoleType.Administrator,
    readPrivilege = RoleType.Administrator,
    isMeaParaml = false,
    -- group 1
    {
        name = "report",
        text = "上报",
        {
            name = "reportMode",
            text = "模式",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "运行",
                "离线",
                "维护",
                "故障",
                "校准",
            },
        },
    },
    -- group 2
    {
        name = "alarm",
        text = "超标报警",
        {
            name = "alarmValue",
            text = "报警",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name = "meaUpLimit",
            text = "抑制报警限(+)",
            unit = "%",
            type = DataType.Float,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            currentValue = nil,
            checkValue = function(value)
                if setting.ui.profile.interconnection.limitPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%.3f", config.interconnection.meaUpLimit)
                    else
                        return value
                    end
                else
                    return string.format("%.3f", config.interconnection.meaUpLimit)
                end
            end,
        },
        {
            name = "meaLowLimit",
            text = "促进报警限(-)",
            unit = "%",
            type = DataType.Float,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            currentValue = nil,
            checkValue = function(value)
                if setting.ui.profile.interconnection.limitPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 100 then
                        return string.format("%.3f", config.interconnection.meaLowLimit)
                    else
                        return value
                    end
                else
                    return string.format("%.3f", config.interconnection.meaLowLimit)
                end
            end,
        },
    },
    -- group 3
    {
        name = "MODBUS",
        text = "MODBUS",
        {
            name = "connectAddress",
            text = "通信地址",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
            if setting.ui.profile.interconnection.addressPattern(value) == true then
                    local num = tonumber(value)
                    if num <= 0 then
                        return string.format("%d", config.interconnection.connectAddress)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.interconnection.connectAddress)
                end
            end,
        },
    },
    -- group 4
    {
        name = "RS485",
        text = "RS485",
        {
            name = "RS485BaudRate",
            text = "传输速率",
            unit = "bps",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "9600",
                "19200",
                "38400",
                "115200",
                "230400",
            },
        },
        {
            name = "RS485Parity",
            text = "校验位",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "无",
                "奇校验",
                "偶校验",
            },
        },
    },
    -- group 5
    {
        text = "RS232",
        name = "RS232",
        {
            name = "RS232BaudRate",
            text = "传输速率",
            unit = "bps",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "9600",
                "19200",
                "38400",
                "115200",
                "230400",
            },
        },
        {
            name = "RS232Parity",
            text = "校验位",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "无",
                "奇校验",
                "偶校验",
            },
        },
    },
    -- group 6
    {
        name = "sample4-20mA",
        text = "水样4-20mA",
        {
            name = "sampleLowLimit",
            text = "发光度下限",
            unit = "%",
            type = DataType.Float,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            currentValue = nil,
            checkValue = function(value)
                if setting.ui.profile.interconnection.positiveDecimalPattern(value) == true then
                    local num = tonumber(value)
                    if setting.ui.profile.interconnection[6][2].currentValue ~= nil then
                        if num <= setting.ui.profile.interconnection[6][2].currentValue and num >= 0 then
                            setting.ui.profile.interconnection[6][1].currentValue = num
                            return value
                        else
                            setting.ui.profile.interconnection[6][1].currentValue = setting.ui.profile.interconnection[6][2].currentValue
                            return string.format("%.3f", setting.ui.profile.interconnection[6][1].currentValue)
                        end
                    else
                        if num <= config.interconnection.sampleUpLimit and num >= 0 then
                            setting.ui.profile.interconnection[6][1].currentValue = num
                            return value
                        else
                            setting.ui.profile.interconnection[6][1].currentValue = config.interconnection.sampleLowLimit
                            return string.format("%.3f", config.interconnection.sampleLowLimit)
                        end
                    end
                else
                    if setting.ui.profile.interconnection[6][2].currentValue~= nil then
                        setting.ui.profile.interconnection[6][1].currentValue = setting.ui.profile.interconnection[6][2].currentValue
                    else
                        setting.ui.profile.interconnection[6][1].currentValue = config.interconnection.sampleLowLimit
                    end
                    return string.format("%.3f", setting.ui.profile.interconnection[6][1].currentValue)
                end
            end,
        },
        {
            name = "sampleUpLimit",
            text = "发光度上限",
            unit = "%",
            type = DataType.Float,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            currentValue = nil,
            checkValue = function(value)
                if setting.ui.profile.interconnection.positiveDecimalPattern(value) == true then
                    local num = tonumber(value)
                    if setting.ui.profile.interconnection[6][1].currentValue ~= nil then
                        if num >= setting.ui.profile.interconnection[6][1].currentValue then
                            setting.ui.profile.interconnection[6][2].currentValue = num
                            return value
                        else
                            setting.ui.profile.interconnection[6][2].currentValue = setting.ui.profile.interconnection[6][1].currentValue
                            return string.format("%.3f", setting.ui.profile.interconnection[6][2].currentValue)
                        end
                    else
                        if num >= config.interconnection.sampleLowLimit then
                            setting.ui.profile.interconnection[6][2].currentValue = num
                            return value
                        else
                            setting.ui.profile.interconnection[6][2].currentValue = config.interconnection.sampleUpLimit
                            return string.format("%.3f", config.interconnection.sampleUpLimit)
                        end
                    end
                else
                    if setting.ui.profile.interconnection[6][1].currentValue~= nil then
                        setting.ui.profile.interconnection[6][2].currentValue = setting.ui.profile.interconnection[6][1].currentValue
                    else
                        setting.ui.profile.interconnection[6][2].currentValue = config.interconnection.sampleUpLimit
                    end
                    return string.format("%.3f", setting.ui.profile.interconnection[6][2].currentValue)
                end
            end,
        },
    },
    -- group 7
    {
        name = "watersampling",
        text = "采水样",
        {
            name = "collectSampleMode",
            text = "采水模式",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "外部触发",
                "测量前采水",
                "采至加样完",
                "采至测量完",
            },
        },
        {
            name = "miningWaterTime",
            text = "测量前采水时间",
            unit = "秒",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.interconnection.timePattern(value) == true then
                    return value
                else
                    return string.format("%d", config.interconnection.miningWaterTime)
                end
            end,
        },
        {
            name = "silentTime",
            text = "测量前静默时间",
            unit = "秒",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.interconnection.timePattern(value) == true then
                    return value
                else
                    return string.format("%d", config.interconnection.silentTime)
                end
            end,
        },
    },
    -- group 8
    {
        name = "multifunctionalrelay",
        text = "多功能继电器",
        {
            name = "relayOne",
            text = "继电器1",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "上限警报",
                "下限警报",
                "测样指示",
                "质控指示",
                "清洗指示",
                "采水指示",
            },
        },
        {
            name = "relayTwo",
            text = "继电器2",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "抑制警报",
                "促进警报",
                "测样指示",
                "质控指示",
                "清洗指示",
                "采水指示",
            },
        },
    },
    -- group 9
    {
        name = "networksettings",
        text = "网络设置",
        {
            name = "settingIPMode",
            text = "设置IP模式",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "DHCP",
                "静态",
            },
        },
    },
    defaultRestore = function()
        local isRestart = false
        local defaultTable = ConfigLists.LoadInterconnectionConfig(true)

        Helper.DefaultRestore(defaultTable, config.interconnection)
        log:info(setting.ui.profile.interconnection.text .. "恢复默认")
        isRestart = config.modifyRecord.interconnection(true)
        ConfigLists.SaveInterconnectionConfig()

        return isRestart
    end,
    saveFile = function(isUpdate)
        local isRestart = false
        local logger = Log.Instance():GetLogger()
        logger:info("修改" .. setting.ui.profile.interconnection.text)
        isRestart = config.modifyRecord.interconnection(isUpdate)
        ConfigLists.SaveInterconnectionConfig()


        return isRestart
    end,
    limitPattern = function(value) -- 小数
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^[-+]?%d?%d?%d?%d%.%d%d?$"
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
    addressPattern = function(value) -- 匹配范围1-247
        if type(value) == "string" then
            local ret = false
            local patterm1 = "^[1]?%d?%d$"
            local patterm2 = "^[2][0-3]%d$"
            local patterm3 = "^[2][4][0-7]$"
            if not string.find(value, patterm1) then
                if not string.find(value, patterm2) then
                    if string.find(value, patterm3) then
                        ret = true
                    end
                else
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
    positiveDecimalPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^%d?%d?%d?%d%.%d%d?%d?$"
            local integerPatterm = "^%d?%d?%d?%d$"
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
}

return setting.ui.profile.interconnection
