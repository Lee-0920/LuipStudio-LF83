setting.ui.profile.hardwareParamIterms =
{
    name = "hardwareParamIterms",
    text = "硬件校准",
    writePrivilege=  RoleType.Administrator,
    readPrivilege = RoleType.Administrator,
    rowCount = 18,
    superRow = 10,
    administratorRow = 8,
    index = 10,
    -- 1
    {
        name = "XMotorParam",
        text = "X轴电机参数",
        motorParam = MotorParam.new(),
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                    return dc:GetIMotorControl():GetMotorParam(0)
                end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[1].motorParam:SetSpeed(0)
                    setting.ui.profile.hardwareParamIterms[1].motorParam:SetAcceleration(0)
                else
                    setting.ui.profile.hardwareParamIterms[1].motorParam = nil
                    setting.ui.profile.hardwareParamIterms[1].motorParam = result
                end
            else
                setting.ui.profile.hardwareParamIterms[1].motorParam:SetSpeed(0)
                setting.ui.profile.hardwareParamIterms[1].motorParam:SetAcceleration(0)
            end
        end,
        set = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                    return dc:GetIMotorControl():SetMotorParam(0, setting.ui.profile.hardwareParamIterms[1].motorParam)
                end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    local text = "设置" .. setting.ui.profile.hardwareParamIterms[1].text .. "系数失败\n"
                    return false, text
                else
                    return true, ""
                end
            else
                local text = "驱动板连接断开,\n设置" .. setting.ui.profile.hardwareParamIterms[1].text .. "系数失败\n"
                return false, text
            end
        end,
        {
            name = "MaxSpeed",
            text = "最大速度",
            refData = "motorParam.motorX.speed",
            unit = "步/秒",
            get = function()
                return setting.ui.profile.hardwareParamIterms[1].motorParam:GetSpeed()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.integerPattern(value) == true then
                    return value
                else
                    return string.format("%.0f",setting.ui.profile.hardwareParamIterms[1][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[1].motorParam:SetSpeed(value)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "Acceleration",
            text = "加速度",
            refData = "motorParam.motorX.acceleration",
            unit = "步/秒^2",
            get = function()
                return setting.ui.profile.hardwareParamIterms[1].motorParam:GetAcceleration()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.integerPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[1][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[1].motorParam:SetAcceleration(value)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    -- 2
    {
        name = "ZMotorParam",
        text = "Z轴电机参数",
        motorParam = MotorParam.new(),
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                    return dc:GetIMotorControl():GetMotorParam(1)
                end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[2].motorParam:SetSpeed(0)
                    setting.ui.profile.hardwareParamIterms[2].motorParam:SetAcceleration(0)
                else
                    setting.ui.profile.hardwareParamIterms[2].motorParam = nil
                    setting.ui.profile.hardwareParamIterms[2].motorParam = result
                end
            else
                setting.ui.profile.hardwareParamIterms[2].motorParam:SetSpeed(0)
                setting.ui.profile.hardwareParamIterms[2].motorParam:SetAcceleration(0)
            end
        end,
        set = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                    return dc:GetIMotorControl():SetMotorParam(1, setting.ui.profile.hardwareParamIterms[2].motorParam)
                end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    local text = "设置" .. setting.ui.profile.hardwareParamIterms[2].text .. "系数失败\n"
                    return false, text
                else
                    return true, ""
                end
            else
                local text = "驱动板连接断开,\n设置" .. setting.ui.profile.hardwareParamIterms[2].text .. "系数失败\n"
                return false, text
            end
        end,
        {
            name = "MaxSpeed",
            text = "最大速度",
            refData = "motorParam.motorZ.speed",
            unit = "步/秒",
            get = function()
                return setting.ui.profile.hardwareParamIterms[2].motorParam:GetSpeed()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.integerPattern(value) == true then
                    return value
                else
                    return string.format("%.0f",setting.ui.profile.hardwareParamIterms[2][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[2].motorParam:SetSpeed(value)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name = "Acceleration",
            text = "加速度",
            refData = "motorParam.motorZ.acceleration",
            unit = "步/秒^2",
            get = function()
                return setting.ui.profile.hardwareParamIterms[2].motorParam:GetAcceleration()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.integerPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[2][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[2].motorParam:SetAcceleration(value)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    -- 3
    {
        name = "ReactTempCalibrate",
        text = "测量仓温度校准",
        tempCalibrate = TempCalibrateFactor.new(),
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():GetCalibrateFactor(0)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetNegativeInput(0)
                    setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetReferenceVoltage(0)
                    setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetCalibrationVoltage(0)
                else
                    setting.ui.profile.hardwareParamIterms[3].tempCalibrate = nil
                    setting.ui.profile.hardwareParamIterms[3].tempCalibrate = result
                end
            else
                setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetNegativeInput(0)
                setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetReferenceVoltage(0)
                setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetCalibrationVoltage(0)
            end
        end,
        set = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():SetCalibrateFactor(0, setting.ui.profile.hardwareParamIterms[3].tempCalibrate)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    local text = "设置" .. setting.ui.profile.hardwareParamIterms[3].text .. "系数失败\n"
                    return false, text
                else
                    return true, ""
                end
            else
                local text = "驱动板连接断开,\n设置" .. setting.ui.profile.hardwareParamIterms[3].text .. "系数失败\n"
                return false, text
            end
        end,
        {--1
            name = "ReactNegativeInput",
            text = "负输入分压",
            refData = "tempCalibrate.react.negativeInput",
            unit = "V",
            get = function()
                return setting.ui.profile.hardwareParamIterms[3].tempCalibrate:GetNegativeInput()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.manyDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[3][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetNegativeInput(value)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {--2
            name = "ReactReferenceVoltage",
            text = "参考电压",
            refData = "tempCalibrate.react.referenceVoltage",
            unit = "V",
            get = function()
                return setting.ui.profile.hardwareParamIterms[3].tempCalibrate:GetReferenceVoltage()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.manyDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[3][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetReferenceVoltage(value)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {--3
            name = "ReactCalibrationVoltage",
            text = "校准电压",
            refData = "tempCalibrate.react.calibrationVoltage",
            unit = "V",
            get = function()
                return setting.ui.profile.hardwareParamIterms[3].tempCalibrate:GetCalibrationVoltage()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.manyDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[3][3].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[3].tempCalibrate:SetCalibrationVoltage(value)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    -- 4
    {
        name = "FungoidTempCalibrate",
        text = "菌室温度校准",
        tempCalibrate = TempCalibrateFactor.new(),
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():GetCalibrateFactor(1)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetNegativeInput(0)
                    setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetReferenceVoltage(0)
                    setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetCalibrationVoltage(0)
                else
                    setting.ui.profile.hardwareParamIterms[4].tempCalibrate = nil
                    setting.ui.profile.hardwareParamIterms[4].tempCalibrate = result
                end
            else
                setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetNegativeInput(0)
                setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetReferenceVoltage(0)
                setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetCalibrationVoltage(0)
            end
        end,
        set = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():SetCalibrateFactor(1, setting.ui.profile.hardwareParamIterms[4].tempCalibrate)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    local text = "设置" .. setting.ui.profile.hardwareParamIterms[4].text .. "系数失败\n"
                    return false, text
                else
                    return true, ""
                end
            else
                local text = "驱动板连接断开,\n设置" .. setting.ui.profile.hardwareParamIterms[4].text .. "系数失败\n"
                return false, text
            end
        end,
        {--1
            name = "FungoidNegativeInput",
            text = "负输入分压",
            refData = "tempCalibrate.fungoid.negativeInput",
            unit = "V",
            get = function()
                return setting.ui.profile.hardwareParamIterms[4].tempCalibrate:GetNegativeInput()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.manyDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[4][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetNegativeInput(value)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {--2
            name = "FungoidReferenceVoltage",
            text = "参考电压",
            refData = "tempCalibrate.fungoid.referenceVoltage",
            unit = "V",
            get = function()
                return setting.ui.profile.hardwareParamIterms[4].tempCalibrate:GetReferenceVoltage()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.manyDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[4][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetReferenceVoltage(value)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {--3
            name = "FungoidCalibrationVoltage",
            text = "校准电压",
            refData = "tempCalibrate.fungoid.calibrationVoltage",
            unit = "V",
            get = function()
                return setting.ui.profile.hardwareParamIterms[4].tempCalibrate:GetCalibrationVoltage()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.manyDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.4f",setting.ui.profile.hardwareParamIterms[4][3].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[4].tempCalibrate:SetCalibrationVoltage(value)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    -- 5
    {
        name = "ReactThermostatParam",
        text = "测量仓PID",
        thermostatParam = ThermostatParam.new(),
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():GetThermostatParam(0)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetProportion(0)
                    setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetIntegration(0)
                    setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetDifferential(0)
                else
                    setting.ui.profile.hardwareParamIterms[5].thermostatParam = result
                end
            else
                setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetProportion(0)
                setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetIntegration(0)
                setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetDifferential(0)
            end
        end,
        set = function()
            ConfigLists.SaveHardwareConfig()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():SetThermostatParam(0, setting.ui.profile.hardwareParamIterms[5].thermostatParam)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    local text = "设置" .. setting.ui.profile.hardwareParamIterms[5].text  .. "失败\n"
                    return false, text
                else
                    return true, ""
                end
            else
                local text = "驱动板连接断开,\n设置" .. setting.ui.profile.hardwareParamIterms[5].text  .. "失败\n"
                return false, text
            end
        end,
        synchronize = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                local PRECISE = 0.000001
                                                local getthermostatParam = dc:GetITemperatureControl():GetThermostatParam(0)
                                                if math.abs(getthermostatParam:GetProportion() - config.hardwareConfig.thermostatParam.react.proportion.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetIntegration() - config.hardwareConfig.thermostatParam.react.integration.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetDifferential() - config.hardwareConfig.thermostatParam.react.differential.real) > PRECISE then

                                                    local thermostatParam = ThermostatParam.new()
                                                    thermostatParam:SetProportion(config.hardwareConfig.thermostatParam.react.proportion.real)
                                                    thermostatParam:SetIntegration(config.hardwareConfig.thermostatParam.react.integration.real)
                                                    thermostatParam:SetDifferential(config.hardwareConfig.thermostatParam.react.differential.real)

                                                    dc:GetITemperatureControl():SetThermostatParam(0, thermostatParam)

                                                    getthermostatParam = dc:GetITemperatureControl():GetThermostatParam(0)
                                                    if math.abs(getthermostatParam:GetProportion() - config.hardwareConfig.thermostatParam.react.proportion.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetIntegration() - config.hardwareConfig.thermostatParam.react.integration.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetDifferential() - config.hardwareConfig.thermostatParam.react.differential.real) > PRECISE then
                                                        log:warn("同步" .. setting.ui.profile.hardwareParamIterms[5].text .. "失败。")
                                                    else
                                                        log:warn("同步" .. setting.ui.profile.hardwareParamIterms[5].text .. "成功。")
                                                    end
                                                end
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    log:warn("同步" .. setting.ui.profile.hardwareParamIterms[5].text .. "失败。")
                end
            end
        end,
        {--1
            name = "ReactProportion",
            text = "比例系数",
            refData = "thermostatParam.react.proportion.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[5].thermostatParam:GetProportion()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.threeDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.3f",setting.ui.profile.hardwareParamIterms[5][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetProportion(value)
                config.hardwareConfig.thermostatParam.react.proportion.real = value
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {--2
            name = "ReactIntegration",
            text = "积分系数",
            refData = "thermostatParam.react.integration.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[5].thermostatParam:GetIntegration()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.threeDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.3f",setting.ui.profile.hardwareParamIterms[5][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetIntegration(value)
                config.hardwareConfig.thermostatParam.react.integration.real = value
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {--3
            name = "ReactDifferential",
            text = "微分系数",
            refData = "thermostatParam.react.differential.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[5].thermostatParam:GetDifferential()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.threeDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.3f",setting.ui.profile.hardwareParamIterms[5][3].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[5].thermostatParam:SetDifferential(value)
                config.hardwareConfig.thermostatParam.react.differential.real = value
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    -- 6
    {
        name = "FungoidThermostatParam",
        text = "菌室PID",
        thermostatParam = ThermostatParam.new(),
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():GetThermostatParam(1)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetProportion(0)
                    setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetIntegration(0)
                    setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetDifferential(0)
                else
                    setting.ui.profile.hardwareParamIterms[6].thermostatParam = result
                end
            else
                setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetProportion(0)
                setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetIntegration(0)
                setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetDifferential(0)
            end
        end,
        set = function()
            ConfigLists.SaveHardwareConfig()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                return dc:GetITemperatureControl():SetThermostatParam(1, setting.ui.profile.hardwareParamIterms[6].thermostatParam)
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    local text = "设置" .. setting.ui.profile.hardwareParamIterms[6].text  .. "失败\n"
                    return false, text
                else
                    return true, ""
                end
            else
                local text = "驱动板连接断开,\n设置" .. setting.ui.profile.hardwareParamIterms[6].text  .. "失败\n"
                return false, text
            end
        end,
        synchronize = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                                                local PRECISE = 0.000001
                                                local getthermostatParam = dc:GetITemperatureControl():GetThermostatParam(1)
                                                if math.abs(getthermostatParam:GetProportion() - config.hardwareConfig.thermostatParam.fungoid.proportion.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetIntegration() - config.hardwareConfig.thermostatParam.fungoid.integration.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetDifferential() - config.hardwareConfig.thermostatParam.fungoid.differential.real) > PRECISE then

                                                    local thermostatParam = ThermostatParam.new()
                                                    thermostatParam:SetProportion(config.hardwareConfig.thermostatParam.fungoid.proportion.real)
                                                    thermostatParam:SetIntegration(config.hardwareConfig.thermostatParam.fungoid.integration.real)
                                                    thermostatParam:SetDifferential(config.hardwareConfig.thermostatParam.fungoid.differential.real)

                                                    dc:GetITemperatureControl():SetThermostatParam(1, thermostatParam)

                                                    getthermostatParam = dc:GetITemperatureControl():GetThermostatParam(1)
                                                    if math.abs(getthermostatParam:GetProportion() - config.hardwareConfig.thermostatParam.fungoid.proportion.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetIntegration() - config.hardwareConfig.thermostatParam.fungoid.integration.real) > PRECISE
                                                    or math.abs(getthermostatParam:GetDifferential() - config.hardwareConfig.thermostatParam.fungoid.differential.real) > PRECISE then
                                                        log:warn("同步" .. setting.ui.profile.hardwareParamIterms[6].text .. "失败。")
                                                    else
                                                        log:warn("同步" .. setting.ui.profile.hardwareParamIterms[6].text .. "成功。")
                                                    end
                                                end
                                            end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    log:warn("同步" .. setting.ui.profile.hardwareParamIterms[6].text .. "失败。")
                end
            end
        end,
        {--1
            name = "FungoidProportion",
            text = "比例系数",
            refData = "thermostatParam.fungoid.proportion.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[6].thermostatParam:GetProportion()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.threeDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.3f",setting.ui.profile.hardwareParamIterms[6][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetProportion(value)
                config.hardwareConfig.thermostatParam.fungoid.proportion.real = value
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {--2
            name = "FungoidIntegration",
            text = "积分系数",
            refData = "thermostatParam.fungoid.integration.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[6].thermostatParam:GetIntegration()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.threeDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.3f",setting.ui.profile.hardwareParamIterms[6][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetIntegration(value)
                config.hardwareConfig.thermostatParam.fungoid.integration.real = value
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {--3
            name = "FungoidDifferential",
            text = "微分系数",
            refData = "thermostatParam.fungoid.differential.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[6].thermostatParam:GetDifferential()
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.threeDecimalPattern(value) == true then
                    return value
                else
                    return string.format("%.3f",setting.ui.profile.hardwareParamIterms[6][3].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[6].thermostatParam:SetDifferential(value)
                config.hardwareConfig.thermostatParam.fungoid.differential.real = value
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    -- 7
    {
        name = "pmtVol",
        text = "PMT校准参数",
        reference = 0,
        measure = 0,
        get = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                    local ret = {}
                    ret[1] = dc:GetIOpticalAcquire():GetStaticADControlParam(0)
                    ret[2] = dc:GetIOpticalAcquire():GetStaticADControlParam(1)
                    return ret
                end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    setting.ui.profile.hardwareParamIterms[7].reference = 0
                    setting.ui.profile.hardwareParamIterms[7].measure = 0
                else
                    setting.ui.profile.hardwareParamIterms[7].reference = result[1]
                    setting.ui.profile.hardwareParamIterms[7].measure = result[2]
                end
            else
                setting.ui.profile.hardwareParamIterms[7].reference = 0
                setting.ui.profile.hardwareParamIterms[7].measure = 0
            end
        end,
        set = function()
            if dc:GetConnectStatus() then
                local status,result = pcall(function()
                    dc:GetIOpticalAcquire():SetStaticADControlParam(0, setting.ui.profile.hardwareParamIterms[7].reference)
                    dc:GetIOpticalAcquire():SetStaticADControlParam(1, setting.ui.profile.hardwareParamIterms[7].measure)
                end)
                if not status then
                    ExceptionHandler.MakeAlarm(result)
                    return false, "PMT校准参数校准失败\n"
                else
                    config.hardwareConfig.pmtVol.reference.real = setting.ui.profile.hardwareParamIterms[7].reference
                    config.hardwareConfig.pmtVol.measure.real = setting.ui.profile.hardwareParamIterms[7].measure
                    return true, ""
                end
            else
                return false, "驱动板连接断开,\nPMT校准参数校准失败\n"
            end
        end,
        {
            name = "reference",
            text = "参考通道",
            refData = "pmtVol.reference.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[7].reference
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 1023 then
                        return string.format("%d", setting.ui.profile.hardwareParamIterms[7][1].get())
                    else
                        return value
                    end
                else
                    return string.format("%d",setting.ui.profile.hardwareParamIterms[7][1].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[7].reference = value
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name = "measure",
            text = "样本通道",
            refData = "pmtVol.measure.set",
            get = function()
                return setting.ui.profile.hardwareParamIterms[7].measure
            end,
            checkValue = function(value)
                if setting.ui.profile.hardwareParamIterms.integerPattern(value) == true then
                    local num = tonumber(value)
                    if num < 0 or num > 1023 then
                        return string.format("%d", setting.ui.profile.hardwareParamIterms[7][2].get())
                    else
                        return value
                    end
                else
                    return string.format("%d",setting.ui.profile.hardwareParamIterms[7][2].get())
                end
            end,
            set = function(value)
                setting.ui.profile.hardwareParamIterms[7].measure = value
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    integerPattern = function(value)
        if type(value) == "string" then
            local patterm = "^%d?%d?%d?%d$"
            if not string.find(value, patterm) then
                return false
            else
                return true
            end
        else
            return false
        end
    end,
    manyDecimalPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^%d?%d?%d%.%d%d?%d?%d?%d?%d?%d?%d?$"
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
    twoDecimalPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^%d?%d?%d%.%d%d?$"
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
    threeDecimalPattern = function(value)
        if type(value) == "string" then
            local ret = false
            local decimalPatterm = "^%d?%d?%d%.%d%d?%d?$"
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
    synchronize = function()
        setting.ui.profile.hardwareParamIterms[5].synchronize()
        setting.ui.profile.hardwareParamIterms[6].synchronize()
    end,
}
