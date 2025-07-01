setting.ui.profile.system =
{
    text = "系统参数",
    name = "system",
    index = 6,
    updateEvent = UpdateEvent.ChangeSystemParam,
    rowCount = 21,
    superRow = 9,
    administratorRow = 12,
    isMeaParaml = false,
    writePrivilege=  RoleType.Administrator,
    readPrivilege = RoleType.Administrator,
    {
        name = "faultProtect",
        text = "仪器保护",
        {
            name = "faultBlocking",
            text = "故障停机",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name = "reagentLackWarn",
            text = "更换试剂提醒",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name = "faultHandleEnable",
            text = "异常重测",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "adcDetect[1].enable",
            text = "漏液检测",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name = "adcDetect[2].enable",
            text = "干节点触发",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
    	name = "specialParameters",
        text = "特殊参数",
        {
            name = "modbusTableType",
            text = "Modbus表",
            type = DataType.Option,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            options =
            {
                "LS1.0",
                "LF88",
            },
        },
        {
            name = "CCEPMode",
            text = "CCEP认证协议",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "solidifyMeaParamFromUI",
            text = "界面固化测量参数",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "solidifyMeaParamFromModbus",
            text = "Modbus固化测量参数",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "displayMeaParamInGeneral",
            text = "开放测量参数",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "useQrencode",
            text = "二维码",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    {
        name = "screen",
        text = "显示屏",
        {
            name = "screenSaver.enable",
            text = "屏幕保护",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name = "screenSaver.darkTime",
            text = "自动暗屏时间",
            unit = "秒",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            currentValue = nil,
            checkValue = function(value)
                if setting.ui.profile.system.valuePattern(value) == true then
                    local num = tonumber(value)
                    if setting.ui.profile.system[3][3].currentValue ~= nil then
                        if num >= 60  and num < setting.ui.profile.system[3][3].currentValue then
                            setting.ui.profile.system[3][2].currentValue = num
                            return value
                        else
                            setting.ui.profile.system[3][2].currentValue = setting.ui.profile.system[3][3].currentValue - 1
                            return tostring(setting.ui.profile.system[3][2].currentValue)
                        end
                    else
                        if num >= 60  and num < config.system.screenSaver.offTime then
                            setting.ui.profile.system[3][2].currentValue = num
                            return value
                        else
                            setting.ui.profile.system[3][2].currentValue = config.system.screenSaver.darkTime
                            return tostring(config.system.screenSaver.darkTime)
                        end
                    end
                else
                    if setting.ui.profile.system[3][3].currentValue ~= nil then
                        setting.ui.profile.system[3][2].currentValue = setting.ui.profile.system[3][3].currentValue - 1
                    else
                        setting.ui.profile.system[3][2].currentValue = config.system.screenSaver.darkTime
                    end
                    return tostring(setting.ui.profile.system[3][2].currentValue)
                end
            end,
        },
        {
            name = "screenSaver.offTime",
            text = "自动关屏时间",
            unit = "秒",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            currentValue = nil,
            checkValue = function(value)
                if setting.ui.profile.system.valuePattern(value) == true then
                    local num = tonumber(value)
                    if setting.ui.profile.system[3][2].currentValue ~= nil then
                        if num >= 60  and num > setting.ui.profile.system[3][2].currentValue then
                            setting.ui.profile.system[3][3].currentValue = num
                            return value
                        else
                            setting.ui.profile.system[3][3].currentValue = setting.ui.profile.system[3][2].currentValue + 1
                            return tostring(setting.ui.profile.system[3][3].currentValue)
                        end
                    else
                        if num >= 60  and num > config.system.screenSaver.darkTime then
                            setting.ui.profile.system[3][3].currentValue = num
                            return value
                        else
                            setting.ui.profile.system[3][3].currentValue = config.system.screenSaver.offTime
                            return tostring(config.system.screenSaver.offTime)
                        end
                    end
                else
                    if setting.ui.profile.system[3][2].currentValue ~= nil then
                        setting.ui.profile.system[3][3].currentValue = setting.ui.profile.system[3][2].currentValue + 1
                    else
                        setting.ui.profile.system[3][3].currentValue = config.system.screenSaver.offTime
                    end
                    return tostring(setting.ui.profile.system[3][3].currentValue)
                end
            end,
        },
        {
            name = "screenSaver.loginKeepingTime",
            text = "登录保持时间",
            unit = "秒",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            checkValue = function(value)
                if setting.ui.profile.system.valuePattern(value) == true then
                    local num = tonumber(value)
                    if num < 60 then
                        return tostring(config.system.screenSaver.loginKeepingTime)
                    else
                        return value
                    end
                else
                    return tostring(config.system.screenSaver.loginKeepingTime)
                end
            end,
        },
    },
    {
        name = "debug",
        text = "调试",
        {
            name = "debugMode",
            text = "开发者模式",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "bridgeMode",
            text = "桥接模式",
            type = DataType.Bool,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "bridgeIP",
            text = "桥接IP",
            type = DataType.IP,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    {
        name = "wqimc",
        text = "水质云",
        {
            name = "serverMode",
            text = "云服务",
            type = DataType.Bool,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            isSolidifyFromUI = true,
        },
        {
            name = "serverIP",
            text = "服务器",
            type = DataType.IP,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            isSolidifyFromUI = true,
        },
        {
            name = "serverPort",
            text = "端口",
            type = DataType.Int,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
            isSolidifyFromUI = true,
            checkValue = function(value)
                if setting.ui.profile.system.valuePattern(value) == true then
                    local num = tonumber(value)
                    if num <= 0 or num > 65535 then
                        return tostring(config.system.serverPort)
                    else
                        return value
                    end
                else
                    return tostring(config.system.serverPort)
                end
            end,
        },
    },
    valuePattern = function(value)
        if type(value) == "string" then
            local patterm = "^%d?%d?%d?%d?%d?%d$"
            if not string.find(value, patterm) then
                return false
            else
                return true
            end
        else
            return false
        end
    end,
    defaultRestore = function(userType)
        local isRestart = false
        local logger = Log.Instance():GetLogger()
        logger:info("系统参数恢复默认参数")

        if userType == RoleType.Super then

            local defaultTable = ConfigLists.LoadSystemConfig(true)
            Helper.DefaultRestore(defaultTable, config.system)
            log:info(setting.ui.profile.system.text .. "恢复默认")

            isRestart = config.modifyRecord.system(true)
            ConfigLists.SaveSystemConfig()

        elseif userType == RoleType.Administrator then
            local defaultTable = ConfigLists.LoadSystemConfig(true)
            config.system.modbusTableType = defaultTable.modbusTableType
            config.system.screenSaver.enable = defaultTable.screenSaver.enable
            config.system.screenSaver.darkTime = defaultTable.screenSaver.darkTime
            config.system.screenSaver.offTime = defaultTable.screenSaver.offTime
            config.system.screenSaver.loginKeepingTime = defaultTable.screenSaver.loginKeepingTime
            config.system.serverMode = defaultTable.serverMode
	        config.system.serverIP = defaultTable.serverIP
            config.system.serverPort = defaultTable.serverPort
            config.system.faultBlocking = defaultTable.faultBlocking
            config.system.reagentLackWarn = defaultTable.reagentLackWarn
            config.system.adcDetect[1].enable = defaultTable.adcDetect[1].enable
            config.system.adcDetect[2].enable = defaultTable.adcDetect[2].enable
            log:info(setting.ui.profile.system.text .. "恢复默认")
	    
            isRestart = config.modifyRecord.system(true)
            ConfigLists.SaveSystemConfig()
        end
        App.SetScreenTime(config.system.screenSaver.darkTime, config.system.screenSaver.offTime, config.system.screenSaver.loginKeepingTime)
        App.SetSceenEnable(config.system.screenSaver.enable)

        return isRestart
    end,
    saveFile = function(isUpdate)
        local isRestart = false
        local logger = Log.Instance():GetLogger()
        logger:info("修改" .. setting.ui.profile.system.text)

        isRestart = config.modifyRecord.system(isUpdate)
        App.SetScreenTime(config.system.screenSaver.darkTime, config.system.screenSaver.offTime, config.system.screenSaver.loginKeepingTime)
        App.SetSceenEnable(config.system.screenSaver.enable)
        ConfigLists.SaveSystemConfig()

        return isRestart
    end,
}
return setting.ui.profile.system
