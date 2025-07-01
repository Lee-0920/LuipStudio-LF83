config = {}
config.modifyRecord =
{
    loadTableValue = function(valueName, valuetable)
        if type(valueName)=="string" then
            local t = valuetable or _G
            for k, v in string.gmatch (valueName, "([%w_]+)(.?)") do
                if v == "." or v == "[" or v == "]" then  -- 不是最后一项
                    local ret = tonumber(k)
                    if ret ~= nil then
                        t = t[ret]
                    else
                        t = t[k]
                    end
                else
                    return t[k]
                end
            end
            return t
        end
    end,
    compare = function(originalTable, uiprofileTable, isUpdate)
        local PRECISE = 0.000001
        local logger = Log.Instance():GetLogger()
        local configVector = ConfigVector.new()
        local isChanged = false

        for _,typeTable in ipairs(uiprofileTable) do
            if type(typeTable) == "table" then
                for _,item in ipairs(typeTable) do
                    if type(item) == "table" then
                        local changeText = uiprofileTable.text .. " " .. typeTable.text .. " " .. item.text .."更改 "
                        local changeName = "config." .. uiprofileTable.name .. "." .. item.name
                        local changeValue = config.modifyRecord.loadTableValue(changeName)
                        local originalValue = config.modifyRecord.loadTableValue(item.name, originalTable)
                        local unit = item.unit or ""
                        local valueStr = ""

                        if (item.type == DataType.Bool) then
                            local value = ""
                            if changeValue == true and originalValue == false then
                                logger:debug(changeText .. "关 -> 开")
                                valueStr = "true"
                                isChanged = true
                            elseif changeValue == false and originalValue == true then
                                logger:debug(changeText .. "开 -> 关")
                                valueStr = "false"
				                isChanged = true

                                if item.name ==  "temperatureMonitor" then
                                    if dc:GetConnectStatus() == true then
                                        op.ITemperatureControl:BoxFanSetOutput(0) --关机箱风扇
                                        log:debug("关闭机箱温度监控时,关闭机箱风扇.")
                                    end
                                end
                            end

                        elseif (item.type == DataType.Option) then
                            if originalValue ~=  changeValue then
                                local str = changeText .. item.options[originalValue + 1] .. unit .. " -> "
                                    .. item.options[changeValue + 1] .. unit
                                logger:debug(str)
                                valueStr = string.format("%d", changeValue)

                                if item.name ==  "unitIndex" then
                                    if setting.unit.valid == true then
                                        logger:debug("浓度单位更改 重启后生效")
                                        setting.unit.valid = false
                                    elseif setting.unit.valid == false then
                                        logger:debug("浓度单位更改 立即生效")
                                        setting.unit.valid = true
                                    end
                                end
				                isChanged = true
                            end

                        elseif (item.type == DataType.Int) then
                            if originalValue ~=  changeValue then
                                local str = changeText .. string.format("%d ", originalValue) .. unit .. " -> "
                                    .. string.format("%d", changeValue) .. unit
                                logger:debug(str)
                                valueStr = string.format("%d", changeValue)
                                isChanged = true
                            end

                        elseif (item.type == DataType.Float) then
                            if changeValue ~= nil and originalValue ~= nil then
                                if math.abs(originalValue - changeValue) > PRECISE then
                                    local str = changeText .. string.format("%f ", originalValue) .. unit .. " -> "
                                    .. string.format("%f ", changeValue) .. unit
                                    logger:debug(str)
                                    valueStr = string.format("%f", changeValue)
                                    isChanged = true
                                end
                            end

                        elseif (item.type == DataType.Double) then
                            if changeValue ~= nil and originalValue ~= nil then
                                if math.abs(originalValue - changeValue) > PRECISE then
                                    local str = changeText .. string.format("%f ", originalValue) .. unit .. " -> "
                                    .. string.format("%f ", changeValue) .. unit
                                    logger:debug(str)
                                    valueStr = string.format("%f", changeValue)
                                    isChanged = true
                                end
                            end

                        elseif (item.type == DataType.IP) then
                            if originalValue ~=  changeValue then
                                local str = changeText .. originalValue .. " -> "
                                    .. changeValue
                                logger:debug(str)
                                valueStr = changeValue
                                isChanged = true
                            end
                        elseif (item.type == DataType.IntArray) then
                            local isTimedEquation = true
                            for k,v in ipairs(originalValue) do
                                if v ~= changeValue[k] then
                                    isTimedEquation = false
                                    break
                                end
                            end

                            if isTimedEquation == false then
                                local newTimeStr = ""
                                local oldTimeStr = ""
                                for k,v in ipairs(originalValue) do
                                    if v then
                                        oldTimeStr = oldTimeStr .. tostring(k - 1) .. ","
                                    end
                                end
                                for k,v in ipairs(changeValue) do
                                    if v then
                                        newTimeStr = newTimeStr .. tostring(k - 1) .. ","
                                    end
                                end
                                local str = changeText .. oldTimeStr .. " -> " .. newTimeStr
                                logger:debug(str)
                                valueStr = newTimeStr
                                isChanged = true
                                if isUpdate == true then
                                    local configValue = Config.new()
                                    configValue:__set_profile(uiprofileTable.name)
                                    configValue:__set_config(item.name)
                                    configValue:__set_value(valueStr)
                                    configVector:Push(configValue)
                                end
                            end
                        end

                        if valueStr ~= "" and isUpdate == true then
                            local configValue = Config.new()
                            configValue:__set_profile(uiprofileTable.name)
                            configValue:__set_config(item.name)
                            configValue:__set_value(valueStr)
                            configVector:Push(configValue)
                        end
                    end
                end
            end
        end

        if configVector:Size() > 0  and isUpdate == true then
            WqimcManager.Instance():updateConfigs(configVector)
        end

        return isChanged
    end,
    scheduler = function(isUpdate)
        local uiprofileTable = setting.ui.profile.scheduler
        local originalTable = ConfigLists.LoadSchedulerConfig(false)
        return config.modifyRecord.compare(originalTable, uiprofileTable, isUpdate)
    end,

    measureParam = function(isUpdate)
        local uiprofileTable = setting.ui.profile.measureParam
        local originalTable = ConfigLists.LoadMeasureParamConfig(false)
        return config.modifyRecord.compare(originalTable, uiprofileTable, isUpdate)
    end,

    componentParam = function(isUpdate)
        local uiprofileTable = setting.ui.profile.componentParam
        local originalTable = ConfigLists.LoadComponentParamConfig(false)
        return config.modifyRecord.compare(originalTable, uiprofileTable, isUpdate)
    end,

    interconnection = function(isUpdate)
        local uiprofileTable = setting.ui.profile.interconnection
        local originalTable = ConfigLists.LoadInterconnectionConfig(false)
        return config.modifyRecord.compare(originalTable, uiprofileTable, isUpdate)
    end,

    system = function(isUpdate)
        local uiprofileTable = setting.ui.profile.system
        local originalTable = ConfigLists.LoadSystemConfig(false)
        return config.modifyRecord.compare(originalTable, uiprofileTable, isUpdate)
    end,
}
