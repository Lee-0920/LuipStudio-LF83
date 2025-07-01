setting.ui.factoryTime =
{
    defaultRestore = function(userType)
        --[[
        测量排期, 测量参数,外联接口，上报模式,高级参数,锁屏参数，授权信息，
        设置IP模式,测量排期的状态
        --]]
        local isRestart = false

        local configTableInfo =
        {
            {
                fileName = "InterconnectionConfig.ls",
                tableName = "config.interconnection",
                configTable = config.interconnection,
                modifyRecord = function()
                    local isChanged = config.modifyRecord.interconnection(true)
                    return isChanged
                end
            },
            {
                fileName = "SchedulerConfig.ls",
                tableName = "config.scheduler",
                configTable = config.scheduler,
                modifyRecord = function()
                    config.modifyRecord.scheduler(true)
                    return false
                end
            },
        }

        for _,v in ipairs(configTableInfo) do

            local defaultTable = Serialization.LoadEncryptionFile(scriptsPath .. "/Config/Default/" .. v.fileName, v.tableName, false)
            Helper.DefaultRestore(defaultTable, v.configTable)
            if v.modifyRecord then
                if v.modifyRecord() == true then
                    isRestart = true
                end
            end
            Serialization.SaveEncryptionFile(v.configTable, scriptsPath .. "/Config/" .. v.fileName, v.tableName)

        end

        --恢复试剂配置
        local defaultTable = ConfigLists.LoadRemainConfig(true)
        Helper.DefaultRestore(defaultTable, config.remain)
        ConfigLists.SaveRemainConfig()
        --上传试剂配置
        WqimcManager.Instance():uploadReagentProfile()

        --恢复试剂记录
        local defaultTable = ConfigLists.LoadRemainStatus(true)
        Helper.DefaultRestore(defaultTable, status.remain)
        ConfigLists.SaveRemainStatus()
        --恢复试剂余量检查状态
        ReagentRemainManager.RecoverRemainCheckStatus()
        --上传试剂余量
        ReagentRemainManager.SaveRemainStatus()

        --恢复耗材配置
        local defaultTable = ConfigLists.LoadConsumableConfig(true)
        Helper.DefaultRestore(defaultTable, config.consumable)
        ConfigLists.SaveConsumableConfig()
        --上传耗材配置
        WqimcManager.Instance():uploadConsumableProfile()
        --恢复耗材检查状态
        MaterialLifeManager.RecoverMaterialLifeCheckStatus()

        --恢复测量状态
        local defaultTable = ConfigLists.LoadMeasureStatus(true)
        Helper.DefaultRestore(defaultTable.schedule, status.measure.schedule)
        ConfigLists.SaveMeasureStatus()


        --恢复测量参数,非工程师时不可恢复
        if userType == RoleType.Super then

            local defaultTable = ConfigLists.LoadMeasureParamConfig(true)
            Helper.DefaultRestore(defaultTable, config.measureParam)

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
            config.system.adcDetect[1].enable = defaultTable.adcDetect[1].enable
            config.system.adcDetect[2].enable = defaultTable.adcDetect[2].enable

            config.modifyRecord.measureParam(true)
            ConfigLists.SaveMeasureParamConfig()


        elseif userType == RoleType.Maintain then

            local defaultTable = ConfigLists.LoadMeasureParamConfig(true)
            config.measureParam.sampleReactTime = defaultTable.sampleReactTime
            config.measureParam.sampleVolume = defaultTable.sampleVolume
            config.measureParam.sampleBlankVolume = defaultTable.sampleBlankVolume
            config.measureParam.sampleSaltVolume = defaultTable.sampleSaltVolume
            config.measureParam.standardReactTime = defaultTable.standardReactTime
            config.measureParam.standardVolume = defaultTable.standardVolume
            config.measureParam.standardBlankVolume = defaultTable.standardBlankVolume
            config.measureParam.standardSaltVolume = defaultTable.standardSaltVolume
            config.measureParam.blankReactTime = defaultTable.blankReactTime
            config.measureParam.blankVolume = defaultTable.blankVolume
            config.measureParam.blankSaltVolume = defaultTable.blankSaltVolume
            config.measureParam.bacteriaVolume = defaultTable.bacteriaVolume
            config.measureParam.bacteriaSaltVolume = defaultTable.bacteriaSaltVolume
            config.measureParam.bacteriaBlankVolume = defaultTable.bacteriaBlankVolume

            config.modifyRecord.measureParam(true)
            ConfigLists.SaveMeasureParamConfig()
        end

        --恢复组件参数
        if userType == RoleType.Super then

            local defaultTable = ConfigLists.LoadComponentParamConfig(true)
            Helper.DefaultRestore(defaultTable, config.componentParam)

            config.modifyRecord.componentParam(true)
            ConfigLists.SaveComponentParamConfig()

        elseif userType == RoleType.Administrator then

            local defaultTable = ConfigLists.LoadComponentParamConfig(true)
            Helper.DefaultRestore(defaultTable, config.componentParam)

            config.modifyRecord.componentParam(true)
            ConfigLists.SaveComponentParamConfig()
        end

        --恢复系统参数
        if userType == RoleType.Super then

            local defaultTable = ConfigLists.LoadSystemConfig(true)
            Helper.DefaultRestore(defaultTable, config.system)

            if config.modifyRecord.system(true) == true then
                isRestart = true
            end
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

            if config.modifyRecord.system(true) == true then
                isRestart = true
            end
            ConfigLists.SaveSystemConfig()
        end
        App.SetScreenTime(config.system.screenSaver.darkTime, config.system.screenSaver.offTime, config.system.screenSaver.loginKeepingTime)
        App.SetSceenEnable(config.system.screenSaver.enable)

        return isRestart
    end,
    --[[
      * @brief 清除测量/定标/告警/日志/曲线/测量状态
      --]]
    clearData = function()
        local defaultTable = ConfigLists.LoadMeasureStatus(true)
        Helper.DefaultRestore(defaultTable, status.measure)

        ConfigLists.SaveMeasureStatus()
    end,
    --[[
  * @brief 清除故障状态标志
  --]]
    clearFault = function()
        status.measure.isDeviceFault = false
        ConfigLists.SaveMeasureStatus()

        local isFaultStatus = StatusManager.Instance():IsFaultStatus()
        if isFaultStatus == true then
            StatusManager.Instance():ResetDeviceStatus()
        end
    end,
}

return setting.ui.factoryTime
