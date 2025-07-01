setting.ui.operation.hardwareTest =
{
    name ="hardwareTest",
    text= "硬件测试",
    rowCount = 25,
    writePrivilege=  RoleType.Administrator,
    readPrivilege = RoleType.Administrator,
    superRow = 3,
    administratorRow = 0,
    {
        name = "PumpGroup",
        text = "泵组",
        {
            name ="SamplePump",
            text= "水样泵",
            createFlow= function(action)
                return HardwareTest:execute(1, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="WashPump",
            text= "空白水泵",
            createFlow= function(action)
                return HardwareTest:execute(2, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="SaltPump",
            text= "渗透压调节液泵",
            createFlow= function(action)
                return HardwareTest:execute(3, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="WastePump",
            text= "真空废液泵",
            createFlow= function(action)
                return HardwareTest:execute(4, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="WasteRoomPump",
            text= "测量仓废液泵",
            createFlow= function(action)
                return HardwareTest:execute(5, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="StirPump",
            text= "搅拌电机",
            createFlow= function(action)
                return HardwareTest:execute(6, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
        name = "ValveGroup",
        text = "阀组",
        {
            name ="BlankValve",
            text= "空白水阀",
            createFlow= function(action)
                return HardwareTest:execute(7, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="RefWashValve",
            text= "参考针清洗阀",
            createFlow= function(action)
                return HardwareTest:execute(8, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="MeaWshValve",
            text= "样本针清洗阀",
            createFlow= function(action)
                return HardwareTest:execute(9, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="RefWasteValve",
            text= "参考针气液阀",
            createFlow= function(action)
                return HardwareTest:execute(10, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="MeaWasteValve",
            text= "样本针气液阀",
            createFlow= function(action)
                return HardwareTest:execute(11, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="AirInValve",
            text= "进气阀",
            createFlow= function(action)
                return HardwareTest:execute(12, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="AirOutValve",
            text= "排气阀",
            createFlow= function(action)
                return HardwareTest:execute(13, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="WasteValve",
            text= "废液阀",
            createFlow= function(action)
                return HardwareTest:execute(14, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
        name = "MeasureCellTemperatureControl",
        text = "测量仓温控",
        {
            name ="MeasureCellHeating",
            text= "测量仓加热",
            createFlow= function(action)
                return HardwareTest:execute(15, action)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
        {
            name ="MeasureCellCooling",
            text= "测量仓制冷",
            createFlow= function(action)
                return HardwareTest:execute(16, action)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    {
        name = "BacteriaStorageTemperatureControl",
        text = "菌室温控",
        {
            name ="BacteriaStorageCooling",
            text= "菌室制冷",
            createFlow= function(action)
                return HardwareTest:execute(17, action)
            end,
            writePrivilege=  RoleType.Super,
            readPrivilege = RoleType.Super,
        },
    },
    {
        name = "MiningRelay",
        text = "采水继电器",
        {
            name ="CollectSampleRelay",
            text= "采水继电器",
            createFlow= function(action)
                return HardwareTest:execute(18, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="Relay1",
            text= "继电器1",
            createFlow= function(action)
                return HardwareTest:execute(19, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="Relay2",
            text= "继电器2",
            createFlow= function(action)
                return HardwareTest:execute(20, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
        name = "Sample4-20mA",
        text = "4-20mA",
        {
            name ="SampleCurrent4Output",
            text= "4mA输出",
            createFlow= function(action)
                return HardwareTest:execute(21, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="SampleCurrent12Output",
            text= "12mA输出",
            createFlow= function(action)
                return HardwareTest:execute(22, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
        {
            name ="SampleCurrent20Output",
            text= "20mA输出",
            createFlow= function(action)
                return HardwareTest:execute(23, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
        name = "Fan",
        text = "风扇",
        {
            name ="BoxFan",
            text= "机箱风扇",
            createFlow= function(action)
                return HardwareTest:execute(24, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
    {
        name = "Power",
        text = "电源",
        {
            name ="PowerCtrl",
            text= "测量模块",
            createFlow= function(action)
                return HardwareTest:execute(25, action)
            end,
            writePrivilege=  RoleType.Administrator,
            readPrivilege = RoleType.Administrator,
        },
    },
}
return setting.ui.operation.hardwareTest
