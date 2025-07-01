setting.ui.diagnosis.sensorCheck =
{
    name ="sensorCheck",
    text= "状态查询",
    rowCount = 6,
    writePrivilege=  RoleType.Administrator,
    readPrivilege = RoleType.Administrator,
    superRow = 0,
    administratorRow = 6,
    {
        text = "驱动板",
        name = "DriveController ",
        {
            name ="SyringeInitSensorCheck",
            text= "注射器初位传感器",
            createFlow = function()
                local flow = SensorCheckFlow:new({})
                flow.name  = setting.ui.diagnosis.sensorCheck[1][1].name
                FlowList.AddFlow(flow)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="MotorXInitSensorCheck",
            text= "X轴初位传感器",
            createFlow = function()
				local flow = SensorCheckFlow:new({})
				flow.name  = setting.ui.diagnosis.sensorCheck[1][2].name
				FlowList.AddFlow(flow)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="MotorZInitSensorCheck",
            text= "Z轴初位传感器",
            createFlow = function()
                local flow = SensorCheckFlow:new({})
                flow.name  = setting.ui.diagnosis.sensorCheck[1][3].name
                FlowList.AddFlow(flow)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="RefCollisionSensorCheck",
            text= "参考针碰撞传感器",
            createFlow = function()
                local flow = SensorCheckFlow:new({})
                flow.name  = setting.ui.diagnosis.sensorCheck[1][4].name
                FlowList.AddFlow(flow)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="MeaCollisionSensorCheck",
            text= "测量针碰撞传感器",
            createFlow = function()
                local flow = SensorCheckFlow:new({})
                flow.name  = setting.ui.diagnosis.sensorCheck[1][5].name
                FlowList.AddFlow(flow)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="BlankSensorCheck",
            text= "空白水检测传感器",
            createFlow = function()
                local flow = SensorCheckFlow:new({})
                flow.name  = setting.ui.diagnosis.sensorCheck[1][6].name
                FlowList.AddFlow(flow)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
}
return setting.ui.diagnosis.sensorCheck
