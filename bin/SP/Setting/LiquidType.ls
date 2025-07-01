setting.liquidType = {}

setting.liquidType.map =
{
    valve1 = 1,
    valve2 = 2,
    valve3 = 4,
    valve4 = 8,
    valve5 = 16,
    valve6 = 32,
    valve7 = 64,
    valve8 = 128,
    valve9 = 256,
    valve10= 512,
    valve11= 1024,
    valve12= 2048,
    valve13= 4096,
    valve14= 8192,
    valve15= 16384,
}

--无任何阀
setting.liquidType.none =
{
    name = "None",
    pump = 0,
    valve = 0,
}

--搅拌
setting.liquidType.stir =
{
    name = "Stir",
    pump = 1,
    valve = 0,
}

--发光菌(仅用于记录试剂)
setting.liquidType.bacteria =
{
    name = "Bacteria",
    pump = 0,
    valve = 0,
}

--水样(泵抽)
setting.liquidType.sample =
{
    name = "Sample",
    pump = 3,
    valve = 0,
}

--质控样
setting.liquidType.standard =
{
    name = "Standard",
    pump = 3,
    valve = 0,
}

--参考水/空白水(空白水阀+泵抽)
setting.liquidType.blank =
{
    name = "Blank",
    pump = 5,
    valve = setting.liquidType.map.valve9,
}

--渗透压调节液(泵抽)
setting.liquidType.salt =
{
    name = "Salt",
    pump = 2,
    valve = 0,
}

--清洗水(样本针清洗+参考针清洗)(泵抽)
setting.liquidType.wash =
{
    name = "Wash",
    pump = 5,
    valve = setting.liquidType.map.valve5 + setting.liquidType.map.valve6,
}

--样本针清洗水
setting.liquidType.meaWash =
{
    name = "MeaWash",
    pump = 5,
    valve = setting.liquidType.map.valve5,
}

--参考针清洗水
setting.liquidType.refWash =
{
    name = "RefWash",
    pump = 5,
    valve = setting.liquidType.map.valve6,
}

--样本针气液管路
setting.liquidType.meaWaste =
{
    name = "MeaWaste",
    pump = 4,
    valve = setting.liquidType.map.valve1,
}

--参考针气液管路
setting.liquidType.refWaste =
{
    name = "RefWaste",
    pump = 4,
    valve = setting.liquidType.map.valve2,
}

--样本针针管路充气(样本针气液+进气)
setting.liquidType.airWashMea =
{
    name = "AirWashMea",
    pump = 4,
    valve = setting.liquidType.map.valve1 + setting.liquidType.map.valve4,
}

--参考针针管路充气(参考针气液+进气)
setting.liquidType.airWashRef =
{
    name = "AirWashRef",
    pump = 4,
    valve = setting.liquidType.map.valve2 + setting.liquidType.map.valve4,
}

--针管路充气(参考针气液+样本针气液+进气)
setting.liquidType.airWash =
{
    name = "AirWash",
    pump = 4,
    valve = setting.liquidType.map.valve1 + setting.liquidType.map.valve2 + setting.liquidType.map.valve4,
}

--针管路抽废液(参考针气液+样本针气液+排气)
setting.liquidType.airWaste =
{
    name = "WastePool",
    pump = 4,
    valve = setting.liquidType.map.valve1 + setting.liquidType.map.valve2 + setting.liquidType.map.valve3,
}

--参考针管路抽废液(参考针气液+排气)
setting.liquidType.airWasteRef =
{
    name = "RefWastePool",
    pump = 4,
    valve = setting.liquidType.map.valve2 + setting.liquidType.map.valve3,
}

--样本针管路抽废液(样本针气液+排气)
setting.liquidType.airWasteMea =
{
    name = "MeaWastePool",
    pump = 4,
    valve = setting.liquidType.map.valve1 + setting.liquidType.map.valve3,
}

--废液(废液阀+进气)
setting.liquidType.wasteOut =
{
    name = "WasteOut",
    pump = 4,
    valve = setting.liquidType.map.valve7 + setting.liquidType.map.valve4,
}

--废液排气(抽气)管路
setting.liquidType.airOut =
{
    name = "AirOut",
    pump = 4,
    valve = setting.liquidType.map.valve3,
}

--废液进气(打气)管路
setting.liquidType.airIn =
{
    name = "AirIn",
    pump = 4,
    valve = setting.liquidType.map.valve4,
}

--废液阀
setting.liquidType.waste =
{
    name = "Waste",
    pump = 4,
    valve = setting.liquidType.map.valve7,
}

--注射器空气
setting.liquidType.airReagent =
{
    name = "AirReagent",
    pump = 0,
    valve = 0,
}

--测量仓废液阀
setting.liquidType.wasteRoom =
{
    name = "WasteRoom",
    pump = 6,
    valve = 0,
}





return setting.liquidType
