setting.ui.operation = {}
setting.ui.operation.maintain =
{
    name ="maintain",
    text= "维护",
    rowCount = 12,
    superRow = 0,
    administratorRow = 0,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    {
        name ="MeasureSample",
        text= "测量水样",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.measureSample.GetTime()
        end,
        createFlow= function()
            local flow = MeasureFlow:new({}, MeasureType.Sample)
            flow.name = setting.ui.operation.maintain[1].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="MeasureStandard",
        text= "测量质控样",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.measureStandard.GetTime()
        end,
        createFlow= function()
            local flow = MeasureFlow:new({}, MeasureType.Standard)
            flow.name  = setting.ui.operation.maintain[2].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="MeasureBlank",
        text= "测量空白水",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.measureBlank.GetTime()
        end,
        createFlow= function()
            local flow = MeasureFlow:new({}, MeasureType.Blank)
            flow.name  = setting.ui.operation.maintain[3].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="MacineReset",
        text= "机械复位",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.measureBlank.GetTime()
        end,
        createFlow= function()
            local flow = MachineResetFlow:new({})
            flow.name  = setting.ui.operation.maintain[4].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="LiquidPipeRecover",
        text= "液路恢复",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.measureBlank.GetTime()
        end,
        createFlow= function()
            local flow = LiquidPipeRecoverFlow:new({})
            flow.name  = setting.ui.operation.maintain[5].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="ThermostatsMeasureControl",
        text= "测量仓温控恢复",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.temperatureControlRecover.GetTime()
        end,
        createFlow= function()
            local flow = TemperatureControlRecoverFlow:new({})
            flow.thermostat = thermostats.measure
            flow.name  = setting.ui.operation.maintain[6].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="ThermostatsBacteriaControl",
        text= "菌室温控恢复",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.temperatureControlRecover.GetTime()
        end,
        createFlow= function()
            local flow = TemperatureControlRecoverFlow:new({})
            flow.thermostat = thermostats.bacteria
            flow.name  = setting.ui.operation.maintain[7].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="MachineClean",
        text= "仪器清洗",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.cleanAll.GetTime()
        end,
        createFlow= function()
            local flow = CleanFlow:new({})
            flow.name  = setting.ui.operation.maintain[8].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="CollectSample",
        text= "采集水样",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.collectSample.GetTime()
        end,
        createFlow= function()
            local flow = CollectSampleFlow:new({})
            flow.name  = setting.ui.operation.maintain[9].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="MeasureCheck",
        text= "PMT模块校准",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.measureCheck.GetTime()
        end,
        createFlow= function()
            local flow = MeasureFlow:new({}, MeasureType.Check)
            flow.name  = setting.ui.operation.maintain[10].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="PMTCheck",
        text= "PMT模块检测",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.pmtCheck.GetTime()
        end,
        createFlow= function()
            local flow = MeasureModuleCheckFlow:new({})
            flow.name  = setting.ui.operation.maintain[11].name
            FlowList.AddFlow(flow)
        end,
    },
    {
        name ="QuickMeasure",
        text= "快速测试",
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
        getRunTime = function()
            return setting.runStatus.pmtCheck.GetTime()
        end,
        createFlow= function()
            local flow = MeasureFlow:new({}, MeasureType.Check)
            flow.name  = setting.ui.operation.maintain[12].name
            flow.isQuickMeasure = true
            FlowList.AddFlow(flow)
        end,
    },
}
return setting.ui.operation.maintain
