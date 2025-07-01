
setting.runAction =
{
    collectSample =
    {
        collect =
        {
            name = "Collect",
            text = "采集",
            GetTime = function()
                return config.interconnection.miningWaterTime
            end,
        },
        silent =
        {
            name = "Silent",
            text = "静置",
            GetTime = function()
                return config.interconnection.silentTime
            end,
        },
    },
    temperatureControl =
    {
        name = "TemperatureControl",
        text = "恒温",
        GetTime = function()   -- 消解器冷却时间，单位为秒
            return setting.temperature.coolingTimeout
        end,
    },
    thermostatsMeasureControl =
    {
        name = "ThermostatsMeasureControl",
        text = "测量仓恒温",
        GetTime = function()   -- 测量仓冷却时间，单位为秒
            return setting.temperature.coolingTimeout
        end,
    },
    thermostatsBacteriaControl =
    {
        name = "ThermostatsBacteriaControl",
        text = "菌室恒温",
        GetTime = function()   -- 消解器冷却时间，单位为秒
            return setting.temperature.coolingTimeout
        end,
    },
    cleanStopMeasure =
    {
        name = "CleanStopMeasure",
        text = "测量终止清洗",
        GetTime = function()
            return 280	-- 测量终止清洗时间，单位为秒
        end,
    },
    oneKeyPipeRenew =
    {
        name = "OneKeyPipeRenew",
        text = "一键管路填充",
        GetTime = function()    -- 一键管路填充时间，单位为秒
            return 40
        end,
    },
    measure =
    {
        machineReset =
        {
            name = "MachineReset",
            text = "仪器复位",
            GetTime = function()    -- 测量前排液时间，单位为秒
                return 80
            end,
        },
        cleanBeforeMeasure =
        {
            name = "CleanBeforeMeasure",
            text = "测量前清洗",
            GetTime = function()    -- 测量前清洗时间，单位为秒
                return  350 + 100*config.measureParam.cleanBeforeMeasureTimes
            end,
        },
        infiltrateAdjust =
        {
            name = "InfiltrateAdjust",
            text = "渗透压调节",
            GetTime = function()    -- 渗透压调节时间，单位为秒
                return 130
            end,
        },
        sampleMix =
        {
            name = "SampleMix",
            text = "水样混合",
            GetTime = function()  -- 水样混合时间，单位为秒
                return 165
            end,
        },
        bacteriaMix =
        {
            name = "BacteriaMix",
            text = "发光菌混合",
            GetTime = function()    -- 发光菌混合时间，单位为秒
                return 270
            end,
        },
        foster =
        {
            name = "Foster",
            text = "孵育",
            GetTime = function()
                local totalTime = config.measureParam.incubationTime
                return totalTime -- 孵育时间，单位为秒
            end,
        },
        readInitialAD =
        {
            name = "ReadInitialAD",
            text = "采集初始光强",
            GetTime = function()
                return 60			-- 采集初始光强时间，单位为秒
            end,
        },
        mixBacteriaSample =
        {
            name = "MixBacteriaSample",
            text = "菌样混合",
            GetTime = function()
                return 155			-- 菌样混合时间，单位为秒
            end,
        },
        reactMeasure =
        {
            name = "ReactMeasure",
            text = "反应测量",
            GetTime = function()
                local time = config.measureParam.sampleReactTime*60
                if Measurer.measureType == MeasureType.Sample then
                    time = config.measureParam.sampleReactTime*60
                elseif Measurer.measureType == MeasureType.Standard then
                    time = config.measureParam.standardReactTime*60
                elseif Measurer.measureType == MeasureType.Blank then
                    time = config.measureParam.blankReactTime*60
                end
                return 	time		-- 反应测量时间，单位为秒
            end,
        },
        cleanAfterMeasure =
        {
            name = "CleanAfterMeasure",
            text = "测量后清洗",
            GetTime = function()
                return 80 + 25*config.measureParam.cleanAfterMeasureTimes	-- 测量后清洗时间，单位为秒
            end,
        },
    },
    motorOperate =
    {
        moveToZero =
        {
            name = "MoveToZero",
            text = "移动至原点",
        },
        moveToMeasureCell =
        {
            name = "MoveToMeasureCell",
            text = "移动至测量槽",
        },
        moveToSampleMixer =
        {
            name = "MoveToSampleMixer",
            text = "移动至样品混合槽",
        },
        moveToBacteriaMixer =
        {
            name = "MoveToBacteriaMixer",
            text = "移动至发光菌混合槽",
        },
        moveToBacteriaCell =
        {
            name = "MoveToBacteriaCell",
            text = "移动至发光菌存储槽",
        },
        moveToBlankCell =
        {
            name = "MoveToBlankCell",
            text = "移动至空白水存储槽",
        },
        moveToSampleCell =
        {
            name = "MoveToSampleCell",
            text = "移动至水样存储槽",
        },
        moveToStandardCell =
        {
            name = "MoveToStandardCell",
            text = "移动至质控样存储槽",
        },
        moveToSaltCell =
        {
            name = "MoveToSaltCell",
            text = "移动至渗透压调节液槽",
        },
        moveToCleanCell =
        {
            name = "MoveToCleanCell",
            text = "移动至针清洗槽",
        },
        moveToWasteCell =
        {
            name = "MoveToWasteCell",
            text = "移动至针排液槽",
        },
    },
    cleanAll =
    {
        clean =
        {
            name = "Clean",
            text = "清洗",
            GetTime = function()
                return 450
            end,
        },
    },
    DCCommunication =
    {
        name = "DCCommunicationCheck",
        text = "驱动板检测",
    },

    suckFromSample =
    {
        name = "SuckFromSample",
        text = "填充水样",
    },
    suckFromBlank =
    {
        name = "SuckFromBlank",
        text = "填充空白水",
    },
    suckFromSalt =
    {
        name = "SuckFromSalt",
        text = "填充渗透压调节液",
    },
    suckFromWash =
    {
        name = "SuckFromWash",
        text = "针管路加清洗水",
    },
    airToNeedlePipe =
    {
        name = "AirToNeedlePipe",
        text = "针管路吹气",
    },
    drainToWastePool =
    {
        name = "DrainToWastePool",
        text = "针管路排进废液池",
    },
    drainToWasteOut =
    {
        name = "DrainToWasteOut",
        text = "排空废液池",
    },
    drainMeasureCellWaste =
    {
        name = "DrainMeasureCellWaste",
        text = "排空测量仓废液",
    },
    aspirationAirReagent =
    {
        name = "AspirationAirReagent",
        text = "注射器吸",
    },
    dischargeAirReagent =
    {
        name = "DischargeAirReagent",
        text = "注射器排",
    },
    syringeDrainMeasureRoom =
    {
        name = "SyringeDrainMeasureRoom",
        text = "注射器排测量仓溶液",
    },
    syringeDrainSampleMixer =
    {
        name = "SyringeDrainSampleMixer",
        text = "注射器排水样混合槽",
    },
    syringeDrainBacteriaMixer =
    {
        name = "SyringeDrainBacteriaMixer",
        text = "注射器排菌液槽溶液",
    },

    masterUpdate =
    {
        name = "Updater",
        text = "升级中",
    },
    erase =
    {
        name = "Erase",
        text = "擦除",
    },
    writeProgram =
    {
        name = "WriteProgram",
        text = "烧写",
    },
    hardwareTest =
    {
        samplePumpTest =
        {
            name = "SamplePumpTest",
            text = "水样泵测试",
        },
        washPumpTest =
        {
            name = "WashPumpTest",
            text = "清洗泵测试",
        },
        saltPumpTest =
        {
            name = "SaltPumpTest",
            text = "渗透压调节液泵测试",
        },
        wastePumpTest =
        {
            name = "WastePumpTest",
            text = "废液泵测试",
        },
        stirPumpTest =
        {
            name = "StirPumpTest",
            text = "搅拌电机测试",
        },
        blankValveTest =
        {
            name = "BlankValveTest",
            text = "空白水阀测试",
        },
        refWashValveTest =
        {
            name = "RefWashValveTest",
            text = "参考针清洗阀测试",
        },
        meaWshValveTest =
        {
            name = "MeaWshValveTest",
            text = "样本针清洗阀测试",
        },
        refWasteValveTest =
        {
            name = "RefWasteValveTest",
            text = "参考针气液阀测试",
        },
        meaWasteValveTest =
        {
            name = "MeaWasteValveTest",
            text = "样本针气液阀测试",
        },
        airInValveTest =
        {
            name = "AirInValveTest",
            text = "进气阀测试",
        },
        airOutValveTest =
        {
            name = "AirOutValveTest",
            text = "排气阀测试",
        },
        wasteValveTest =
        {
            name = "WasteValveTest",
            text = "废液阀测试",
        },
        measureCellHeatingTest =
        {
            name = "MeasureCellHeatingTest",
            text = "测量仓加热测试",
        },
        measureCellCoolingTest =
        {
            name = "MeasureCellCoolingTest",
            text = "测量仓制冷测试",
        },
        bacteriaStorageCoolingTest =
        {
            name = "BacteriaStorageCoolingTest",
            text = "菌室制冷测试",
        },
        measureCellWaterCoolerTest =
        {
            name = "MeasureCellWaterCoolerTest",
            text = "测量仓水冷泵测试",
        },
        measureCellFanTest =
        {
            name = "MeasureCellFanTest",
            text = "测量仓冷凝风扇测试",
        },
        bacteriaStorageWaterCoolerTest =
        {
            name = "BacteriaStorageWaterCoolerTest",
            text = "菌室水冷泵测试",
        },
        bacteriaStorageFanTest =
        {
            name = "BacteriaStorageFanTest",
            text = "菌室冷凝风扇测试",
        },
        collectSampleRelayTest =
        {
            name = "CollectSampleRelayTest",
            text = "采水继电器测试",
        },
        relay1Test =
        {
            name = "Relay1Test",
            text = "继电器1测试",
        },
        relay2Test =
        {
            name = "Relay2Test",
            text = "继电器2测试",
        },
        sampleCurrent4Output =
        {
            name = "SampleCurrent4Output",
            text = "样品4mA输出",
        },
        sampleCurrent12Output =
        {
            name = "SampleCurrent12Output",
            text = "样品12mA输出",
        },
        sampleCurrent20Output =
        {
            name = "SampleCurrent20Output",
            text = "样品20mA输出",
        },
        systemFanTest =
        {
            name = "SystemFanTest",
            text = "机箱冷却",
        },
    },
    pmtCheck =
    {
        name = "PMTCheck",
        text = "PMT检测",
        GetTime = function()
            return 8
        end,
    },
}
return setting.runAction
