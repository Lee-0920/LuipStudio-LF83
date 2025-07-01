setting.ui.profile.componentParam =
{
    name = "componentParam",
    text = "组件参数",
    updateEvent = UpdateEvent.ChangeMeasureParam,
    index = 3,
    rowCount = 30,
    superRow = 0,
    administratorRow = 0,
    writePrivilege=  RoleType.Super,
    readPrivilege = RoleType.Super,
    isMeaParaml = true,
    {
        name = "measureCell",
        text = "测量槽",
        {
            name = "measureCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.measureCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.measureCell.pos)
                end
            end,
        },
        {
            name = "measureCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.measureCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.measureCell.side)
                end
            end,
        },
        {
            name = "measureCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.measureCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.measureCell.bottom)
                end
            end,
        },
    },
    {
        name = "sampleMixer",
        text = "待测样混合槽",
        {
            name = "sampleMixer.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.sampleMixer.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.sampleMixer.pos)
                end
            end,
        },
        {
            name = "sampleMixer.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.sampleMixer.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.sampleMixer.side)
                end
            end,
        },
        {
            name = "sampleMixer.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.sampleMixer.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.sampleMixer.bottom)
                end
            end,
        },
    },
    {
        name = "bacteriaMixer",
        text = "发光菌混合槽",
        {
            name = "bacteriaMixer.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.bacteriaMixer.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.bacteriaMixer.pos)
                end
            end,
        },
        {
            name = "bacteriaMixer.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.bacteriaMixer.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.bacteriaMixer.side)
                end
            end,
        },
        {
            name = "bacteriaMixer.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.bacteriaMixer.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.bacteriaMixer.bottom)
                end
            end,
        },
    },
    {
        name = "bacteriaCell",
        text = "发光菌存储槽",
        {
            name = "bacteriaCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.bacteriaCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.bacteriaCell.pos)
                end
            end,
        },
        {
            name = "bacteriaCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.bacteriaCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.bacteriaCell.side)
                end
            end,
        },
        {
            name = "bacteriaCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.bacteriaCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.bacteriaCell.bottom)
                end
            end,
        },
    },
    {
        name = "wasteCell",
        text = "排液槽",
        {
            name = "wasteCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.wasteCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.wasteCell.pos)
                end
            end,
        },
        {
            name = "wasteCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.wasteCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.wasteCell.side)
                end
            end,
        },
        {
            name = "wasteCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.wasteCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.wasteCell.bottom)
                end
            end,
        },
    },
    {
        name = "cleanCell",
        text = "清洗槽",
        {
            name = "cleanCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.cleanCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.cleanCell.pos)
                end
            end,
        },
        {
            name = "cleanCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.cleanCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.cleanCell.side)
                end
            end,
        },
        {
            name = "cleanCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.cleanCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.cleanCell.bottom)
                end
            end,
        },
    },
    {
        name = "blankCell",
        text = "空白水存储槽",
        {
            name = "blankCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.blankCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.blankCell.pos)
                end
            end,
        },
        {
            name = "blankCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.blankCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.blankCell.side)
                end
            end,
        },
        {
            name = "blankCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.blankCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.blankCell.bottom)
                end
            end,
        },
    },
    {
        name = "sampleCell",
        text = "水样存储槽",
        {
            name = "sampleCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.sampleCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.sampleCell.pos)
                end
            end,
        },
        {
            name = "sampleCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.sampleCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.sampleCell.side)
                end
            end,
        },
        {
            name = "sampleCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.sampleCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.sampleCell.bottom)
                end
            end,
        },
    },
    {
        name = "standardCell",
        text = "质控样存储槽",
        {
            name = "standardCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.standardCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.standardCell.pos)
                end
            end,
        },
        {
            name = "standardCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.standardCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.standardCell.side)
                end
            end,
        },
        {
            name = "standardCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.standardCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.standardCell.bottom)
                end
            end,
        },
    },
    {
        name = "saltCell",
        text = "渗透压调节液槽",
        {
            name = "saltCell.pos",
            text = "水平位置",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsX then
                        return string.format("%d", config.componentParam.saltCell.pos)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.saltCell.pos)
                end
            end,
        },
        {
            name = "saltCell.side",
            text = "边界深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.saltCell.side)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.saltCell.side)
                end
            end,
        },
        {
            name = "saltCell.bottom",
            text = "底部深度",
            unit = "步",
            type = DataType.Int,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
            checkValue = function(value)
                if setting.ui.profile.measureParam.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > setting.motor.maxStepsZ then
                        return string.format("%d", config.componentParam.saltCell.bottom)
                    else
                        return value
                    end
                else
                    return string.format("%d", config.componentParam.saltCell.bottom)
                end
            end,
        },
    },
    defaultRestore = function(userType)

        if userType == RoleType.Super then
            local defaultTable = ConfigLists.LoadComponentParamConfig(true)
            Helper.DefaultRestore(defaultTable, config.componentParam)

            log:info(setting.ui.profile.componentParam.text .. "恢复默认")
            config.modifyRecord.componentParam(true)
            ConfigLists.SaveComponentParamConfig()
        end

        return false  --无需重启仪器
    end,
    saveFile = function(isUpdate)
        log:info("修改" .. setting.ui.profile.componentParam.text)
        config.modifyRecord.componentParam(isUpdate)
        ConfigLists.SaveComponentParamConfig()

        return false  --无需重启仪器
    end,
    integerPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local integerPattern = "^%d?%d?%d?%d?%d?%d$"
            if string.find(value, integerPattern) then
                ret = true
            end
            return ret
        else
            return false
        end
    end,
}
return setting.ui.profile.componentParam
