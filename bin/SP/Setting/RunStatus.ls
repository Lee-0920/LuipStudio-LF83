setting.runStatus =
{
    collectSample =
    {
        name = "CollectSample",
        text = "采集水样",
        GetTime = function()
            return config.interconnection.miningWaterTime + config.interconnection.silentTime
        end,
    },
    liquidPipeRecover =
    {
        name = "LiquidPipeRecover",
        text = "液路恢复",
        GetTime = function()
            return 10
        end,
    },
    temperatureControlRecover =
    {
        name = "TemperatureControlRecover",
        text = "温控恢复",
        GetTime = function()
            return setting.temperature.coolingTimeout
        end,
    },
    machineReset =
    {
        name = "MachineReset",
        text = "机械复位",
        GetTime = function()
            return 10
        end,
    },
    cleanAll =
    {
        name = "CleanAll",
        text = "仪器清洗",
        GetTime = function()
            return 450		-- 清洗所有管路流程总时间，单位为秒
        end,
    },
    measureBlank =
    {
        name = "MeasureBlank",
        text = "测量空白水",
        GetTime = function()
            local totalTime = setting.runAction.measure.machineReset.GetTime()
            + setting.runAction.measure.cleanBeforeMeasure.GetTime()
            + setting.runAction.measure.infiltrateAdjust.GetTime()
            + setting.runAction.measure.sampleMix.GetTime()
            + setting.runAction.measure.bacteriaMix.GetTime()
            + setting.runAction.measure.foster.GetTime()
            + setting.runAction.measure.readInitialAD.GetTime()
            + setting.runAction.measure.mixBacteriaSample.GetTime()
            + setting.runAction.measure.cleanAfterMeasure.GetTime()
            + config.measureParam.blankReactTime*60
            return totalTime
        end,
    },
    measureSample =
    {
        name = "MeasureSample",
        text = "测量水样",
        GetTime = function()
            local totalTime = setting.runAction.measure.machineReset.GetTime()
            + setting.runAction.measure.cleanBeforeMeasure.GetTime()
            + setting.runAction.measure.infiltrateAdjust.GetTime()
            + setting.runAction.measure.sampleMix.GetTime()
            + setting.runAction.measure.bacteriaMix.GetTime()
            + setting.runAction.measure.foster.GetTime()
            + setting.runAction.measure.readInitialAD.GetTime()
            + setting.runAction.measure.mixBacteriaSample.GetTime()
            + setting.runAction.measure.cleanAfterMeasure.GetTime()
            + config.measureParam.sampleReactTime*60
            return totalTime
        end,
    },
    measureStandard =
    {
        name = "MeasureStandard",
        text = "测量质控样",
        GetTime = function()
            local totalTime = setting.runAction.measure.machineReset.GetTime()
            + setting.runAction.measure.cleanBeforeMeasure.GetTime()
            + setting.runAction.measure.infiltrateAdjust.GetTime()
            + setting.runAction.measure.sampleMix.GetTime()
            + setting.runAction.measure.bacteriaMix.GetTime()
            + setting.runAction.measure.foster.GetTime()
            + setting.runAction.measure.readInitialAD.GetTime()
            + setting.runAction.measure.mixBacteriaSample.GetTime()
            + setting.runAction.measure.cleanAfterMeasure.GetTime()
            + config.measureParam.standardReactTime*60
            return totalTime
        end,
    },
    measureCheck =
    {
        name = "MeasureCheck",
        text = "PMT模块校准",
        GetTime = function()
            local totalTime = setting.runAction.measure.machineReset.GetTime()
            + setting.runAction.measure.cleanBeforeMeasure.GetTime()
            + setting.runAction.measure.infiltrateAdjust.GetTime()
            + setting.runAction.measure.bacteriaMix.GetTime()
            + setting.runAction.measure.foster.GetTime()
            + setting.runAction.measure.readInitialAD.GetTime()
            + setting.runAction.measure.cleanAfterMeasure.GetTime()
            return totalTime
        end,
    },
    pmtCheck =
    {
        name = "PMTCheck",
        text = "PMT模块检测",
        GetTime = function()
            return 8
        end,
    },
    communication =
    {
        name = "CommunicationCheck",
        text = "通信检测",
    },
    sensorCheck =
    {
        name = "SensorCheck",
        text = "状态查询",
    },
    liquidOperate =
    {
        name = "LiquidOperate",
        text = "管道操作",
    },
    motorOperate =
    {
        name = "MotorOperate",
        text = "电机操作",
    },
    hardwareTest =
    {
        name = "HardwareTest",
        text = "硬件测试",
    },
    DCUpdate =
    {
        name = "DCUpdate",
        text = "驱动板升级",
    },
    LCUpdate =
    {
        name = "LCUpdate",
        text = "液路板升级",
    },
    TCUpdate =
    {
        name = "TCUpdate",
        text = "温控板升级",
    },
    SCUpdate =
    {
        name = "SCUpdate",
        text = "信号板升级",
    },
    masterUpdate =
    {
        name = "MASTER",
        text = "主控板升级",
    },
}
return setting.runStatus
