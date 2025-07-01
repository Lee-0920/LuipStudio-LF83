setting.ui.profile.scheduler =
{
    name = "scheduler",
    text = "测量排期",
    index = 1,
    rowCount = 9,
    superRow = 0,
    administratorRow = 0,
    isMeaParaml = false,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    updateEvent = UpdateEvent.ChangeAutoMeasure,
    {
        name = "automeasurement",
        text = "水样测量",
        {
            name = "measure.mode",
            text = "测量模式",
            type = DataType.Option,
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
            options =
            {
                "触发测量",
                "周期测量",
                "整点测量",
                "连续测量",
            },
        },
        {
            name = "measure.interval",
            text = "间隔周期",
            type = DataType.Float,
            unit = "小时",
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.scheduler.tempPattern(value) == true then
                    return value
                else
                    return string.format("%.1f", config.scheduler.measure.interval)
                end
            end,
        },
        {
            name = "measure.timedPoint",
            text = "整点设置",
            type = DataType.IntArray,
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
        },
    },
    {
        name = "check",
        text = "质控测量",
        {
            name = "check.enable",
            text = "自动测量",
            type= DataType.Bool,
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
        },
        {
            name = "check.interval",
            text = "间隔周期",
            type = DataType.Float,
            unit = "小时",
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.scheduler.tempPattern(value) == true then
                    return value
                else
                    return string.format("%.1f", config.scheduler.check.interval)
                end
            end,
        },
    },
    {
        name = "blank",
        text = "空白水测量",
        {
            name = "blank.enable",
            text = "自动测量",
            type= DataType.Bool,
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
        },
        {
            name = "blank.interval",
            text = "间隔周期",
            type = DataType.Float,
            unit = "小时",
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.scheduler.tempPattern(value) == true then
                    return value
                else
                    return string.format("%.1f", config.scheduler.blank.interval)
                end
            end,
        },
    },
    {
        name = "clean",
        text = "仪器清洗",
        {
            name = "clean.enable",
            text = "自动清洗",
            type= DataType.Bool,
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
        },
        {
            name = "clean.interval",
            text = "间隔周期",
            type = DataType.Float,
            unit = "小时",
            writePrivilege=  RoleType.Maintain,
            readPrivilege = RoleType.Maintain,
            checkValue = function(value)
                if setting.ui.profile.scheduler.tempPattern(value) == true then
                    return value
                else
                    return string.format("%.1f", config.scheduler.clean.interval)
                end
            end,
        },
    },
    defaultRestore = function(userType)

        local defaultTable = ConfigLists.LoadSchedulerConfig(true)
        Helper.DefaultRestore(defaultTable, config.scheduler)
        local logger = Log.Instance():GetLogger()
        logger:info(setting.ui.profile.scheduler.text .. "恢复默认")
        config.modifyRecord.scheduler(true)
        ConfigLists.SaveSchedulerConfig()
    end,
    saveFile = function(isUpdate)
        local logger = Log.Instance():GetLogger()
        logger:info("修改".. setting.ui.profile.scheduler.text)
        config.modifyRecord.scheduler(isUpdate)
        ConfigLists.SaveSchedulerConfig()
    end,
    tempPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^%d?%d?%d%.%d$"
            local integerPatterm = "^%d?%d?%d$"
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

return setting.ui.profile.scheduler
