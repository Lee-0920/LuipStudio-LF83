require("Version")

setting.unitVolume = 1

Target =
{
    PHOT = 1,
}

PDPower =
{
    ForeverOn = 0,
    MeasureOn = 1,
}

ResultMark =
{
    N = 0,      --正常/在线
    B = 1,      --离线
    M = 2,      --维护
    D = 3,      --故障
    C = 4,      --校准
    E = 5,      --超标
}

setting.unit =
{
    valid = true,
    {
        text = "%",
        multiple = 1,
    },
}

setting.instrument =
{
    name = "生物毒性水质在线自动监测仪",
    argus =
    {
        "生物毒性",
    },
    deviceTypeNum =           -- 设备参数类型号
    {
        PHOT = 0,
    },
}

setting.liquid = --液路相关参数
{
    meterLimit = --定量限值参设置--保留
    {
        sampleLimitVolume = 6, 			-- 水样定量的最大限值
        blankLimitVolume = 6,			-- 空白水定量的最大限值
        reagentLimitVolume = 5,			-- 试剂定量的最大限值
        digestionRoomLimitVolume = 3,		-- 消解室定量的最大限值
        pumpTimeoutFactor = 80000,		-- 泵操作结果等待超时时间系数,单位为s/mL
        meterTimeoutFactor = 25000,     -- 精确定量操作结果等待超时时间系数,单位为s/mL
        syringeTimeoutFactor = 20000    -- 注射器操作超时因子,单位为ms/mL
    },
    reagent1Volume = 0.3,			-- 试剂1体积，单位mL
    reagent2Volume = 0.3,			-- 试剂2体积，单位mL
    reagent3Volume = 0.3,			-- 试剂3体积，单位mL
    sampleMeterPoint = 2,			-- 水样精确定量点，1表示第一个定量点
    standardMeterPoint = 2,			-- 标准液精确定量点，1表示第一个定量点
    blankMeterPoint = 2,			-- 空白水精确定量点，1表示第一个定量点
    reagent1MeterPoint = 1,			-- 试剂1精确定量点，1表示第一个定量点
    reagent2MeterPoint = 1,			-- 试剂2精确定量点，1表示第一个定量点
    reagent3MeterPoint = 1,			-- 试剂3精确定量点，1表示第一个定量点
    digestionRoomMeterPoint = 2,		-- 消解混合液精确定量点，1表示第一个定量点
    samplePipeVolume = 4,			-- 水样管体积，单位mL
    standardPipeVolume = 4,			-- 标准液管体积，单位mL
    blankPipeVolume = 4,			-- 空白液液管体积，单位mL
    reagent1PipeVolume = 4,			-- 试剂1管体积，单位mL
    reagent2PipeVolume = 4,			-- 试剂2管体积，单位mL
    reagent3PipeVolume = 4,			-- 试剂3管体积，单位mL
    meterPipeVolume = 1,			-- 定量管体积，单位mL
    multipleValvePipeVolume = 0.5,	-- 多联阀体积，单位mL
    digestionRoomPipeVolume = 1.5,		    -- 消解室管路体积，单位mL
    wastePreAirVolume = 0.2,                         -- 真空泵抽空体积，单位mL
    wasteAirVolume = 5,                         -- 真空泵抽空体积，单位mL
    wastePipeVolume = 2,			            -- 定量废液管体积，单位mL
    pumpAirVolume = 4.5,			-- 冒泡的体积，单位mL
    pumpAirSpeed = 0.3,				-- 冒泡的速度，单位mL/S
    liquidSecludeVolume = 0.1,	-- 管道隔离的体积，单位mL
    liquidSecludeSpeed = 0.1,		-- 管道隔离的速度，单位mL/S
    meterPointMaxNum = 2,			            -- 最大定量点数目
    meterSpeed = 150,				-- 定量速度,单位步/s
    drainSpeed = 300,				-- 排液定量速度,单位步/s
    normalStirSpeed = 200,          --正常搅拌速度,单位步/s
    syringeSpeed = 200,             --注射器速度
    syringeMaxVolume = 1,           --注射器最大体积
    mixSampleVolume = 0.8,          --加混合水样体积,单位mL
    mixBacteriaVolume = 0.8,        --加发光菌悬浮液体积,单位mL
    dischargeErrorVolume = 0.01,     -- 注射器回程误差体积
    aspirationExtraVol = 0.08,        -- 额外吸液体积
    littleExtraVol = 0.01,        -- 少量额外吸液体积
    aspirationSpeed = 0.1,          -- 吸液速度,单位mL/s
    dischargeSpeed = 0.1,          -- 吸液速度,单位mL/s
    dcmotorSpeed = 100,
    meterFailedReTry = 3,			-- 定量失败重试次数
    meterOverflowReTry = 3,			-- 定量溢出重试次数
    meterUnfinishedReTry = 3,		-- 定量未达成目标重试次数
    meterAirBubbleReTry = 6,		-- 定量有气泡重试次数
    meterTimeoutReTry = 3,			-- 定量超时重试次数
    meterCleanVolume = 3,			-- 定量故障后，空白水清洗体积
    readInitADBlankVolume = 4,		        -- 读初始值时空白水体积，单位mL

    measureCellVolume = 5,
    cleanNeedleVolume = 5,          -- 冲洗针管路用水体积，单位mL
    addSampleVolume = 4,           -- 测量更新水样体积，单位mL
    addBlankVolume = 2,            -- 测量更新空白水体积，单位mL
    addSaltVolume = 2,              -- 测量更新渗透压调节液体积，单位mL
    addLiquidSpeed = 1,             -- 加液速度,单位mL/s[空气泵固定速度秒]
    bacteriaSaltVolume = 100,       -- 发光菌悬浮液渗透压调节液体积,单位uL
    bacteriaBlankVolume = 900,      -- 发光菌悬浮液空白水体积,单位uL
}

setting.motor =   --电机相关
{
    moveTimeOutFactor = 100,    --电机运动超时因子
    limitStepsX = 1060,         --X轴限制步数
    limitStepsZ = 400,         --Z轴限制步数
    maxStepsX = 1060,         --X轴限制步数
    maxStepsZ = 400,         --Z轴限制步数
    minStepsX = 0,         --X轴限制步数
    minStepsZ = 0,         --Z轴限制步数
    moveSpeedX = 100,            --X轴运动速度
    moveSpeedZ = 100,            --Z轴运动速度
    topZ = 0,                   --Z轴顶部
    safePosZ = 0,              --Z轴安全深度
    aboveSideOffset = -10,       --Z边界上方偏移
    belowSideOffset = 10,       --Z边界下方偏移
    aboveBottomOffset = -20,     --Z底部上方偏移
    driverXIndex = 0,               -- X电机驱动索引
    driverZIndex = 1,               -- Z电机驱动索引
    driverSyrIndex = 2,             -- 注射器电机驱动索引
    driverPumpIndex = 3,            -- 搅拌电机驱动索引
}

setting.signal =	--信号相关
{
    readADTime = 10,				-- 读AD值的时间,单位为秒
    readADIntervalTime = 60,            -- 读AD值间隔时间
    readDarkADTime = 0.5,			-- 读暗电流AD值的时间,单位为秒
    fullLimitV = 2.1,                -- 推荐电压下限
    maxLimitV = 1.5,                -- 推荐电压上限
    minLimitV = 1,                -- 推荐电压下限
    fullLimitAD = 14086800,             -- 饱和限制AD
    maxLimitAD = 10066329,          -- 推荐AD上限
    minLimitAD = 6708000,           -- 推荐AD下限
    gainLevelNum = 8,               -- 增益等级数量
    gain = {1, 3.2, 6.4, 8.5, 12.8, 18.5, 28.3, 44.5},      --各级放大倍数
    incubationTime = 0,           -- 孵育时间
    pmtAuto = false,                             -- PMT自适应调节
    destLowLimit = 3500000,         -- 调节下限
    destHighLimit = 4500000,        -- 调节上限
    destTarget = 4000000,           -- 调节目标
    gainHighLimit = 1023,           -- 调节系数上限(最低放大)
    gainLowLimit = 0,               -- 调节系数下限(最高放大)
    refIndex = 0,                   -- 参考通道索引
    meaIndex = 1,                   -- 测量通道索引
}

setting.temperature  =	--温度相关
{
    measureSafetyTemp = 15,         -- 测量仓安全温度
    bacteriaSafetyTemp = 5,         -- 细菌存储槽安全温度
    safeTempRange = 1,              --
    coolingTimeout = 900,      --测量仓安全检查降温时间
    measureCheckCoolingTime = 300,      --测量仓安全检查降温时间
    readInitADTemp = 55,			-- 读初始值的温度,单位为℃
    digestTempTimeout = 900,		-- 加热到消解温度的超时时间,单位为秒
    reactTempTimeout = 900,			-- 加热到反应温度的超时时间,单位为秒
    naturalCoolingTime = 900,		            -- 加热后再自然冷却时间,单位为秒
    digestToReactCoolingTime = 900, -- 消解到反应冷却时间,单位为秒
    naturalCoolingTempValue = 1,	        -- 消解冷却温度到反应温度的自然冷却温度差,单位为℃
    preheating = 30,				-- 加浓硫酸前预热温度,单位为℃
    digestCoolingTemp = 60,         -- 消解冷却温度
    cleanDigestTemp = 90,			-- 清洗消解器的温度,单位为℃
    digestSafetyTemp = 45,			-- 消解室安全温度,单位为℃
    insideEnvironmentTemp = 30,		        -- 机箱内部环境温度,单位为℃
    toleranceTemp = 0,				-- 容差温度，与目标温度的差值在该参数范围内即认为到达目标温度
    smartHeatDetectTemp = 10,		        -- 加热模块智能诊断上升温度,单位为℃
    smartHeatDetectTime = 300,		        -- 加热模块智能诊断加热时间,单位为秒
    measureWarnLimit = 2,           -- 测量仓温控告警限
    bacteriaWarnLimit = 1,          -- 菌室温控告警限
}

setting.measureResult =
{
    decimalNum = 1,					-- 测量结果小数位
    useNum = 3,						-- 测量结果有效位数
    measureLowLimit = 0.02,	-- 测定下限
    zeroReviseValue = 0.001,		-- 零修正值
    faultValue = 0.002,				-- 错误值,作为错误标识
    normalMeasureTime = 3200,		-- 正常测量时间,单位为秒
    resultWaveRange = 2,			-- 结果波动范围(%)
}

setting.common =
{
    zeroCalibrateWaveRange = 2,		-- 精准定标-零点波动范围
    standardCalibrateWaveRange = 20,-- 精准定标-标点波动范围
    dataCopyDir = "/mnt/udisk/LF83-PHOT_",     -- 数据复制路径
    --dataCopyDir = "E:/LF83-PHOT_",
}

setting.measure = {} --测量参数设置
setting.measure.range  = --量程  --波形界面需要
{
    rangeNum = 1,					-- 量程的数量
    {
        viewRange = 200,			            -- 用于显示的量程值
        waveUpLimit = 100,                      -- 波形曲线上限
        waveLowLimit = -100,                    -- 波形曲线下限
    },
}

setting.clean =	--清洗参数设置
{
    cleanVolume = 0.5,   --针内壁清洗
    cleanTimes = 1,
    clearVolume = 1.5,    --针外壁清洗
    clearTimes = 2,
    bacteriaMixerWastePos = 225,
    bacteriaMixerWashPos = 180,
    sampleMixerWastePos = 240,
    sampleMixerWashPos = 200,
    measureCellWastePos = 265,
    measureCellWashPos = 225,
}


package.path = [[?;?.lua;?.ls;?;./SP/Setting/?.ls;./SP/Setting/ResultManager/?.ls;./SP/Setting/UI/?.ls;./SP/Setting/Modbus/?.ls;]]

require("MeasureFileFormat")
require("ResultFileInfo")
require("Alarm")
require("RunStatus")
require("RunAction")
require("ModeDefine")
require("MeasureData")
require("WarnManagerWidget")
require("SignalIterms")
require("MeasureParamItems")
require("ComponentParamWidget")
require("AutoMeasureWidget")
require("LiquidType")
require("Component")
require("MaterialType")
require("InterconnectionWidget")
require("ReagentManagerWidget")
require("UseResourceWidget")
require("HardwareParamIterms")
require("DeviceInformation")
require("FactoryTime")
require("WaveData")
require("MeasureScheduler")
require("RemoteMaintainWindow")
require("MaintenanceWidget")
require("PipeManngerWidget")
require("MotorManngerWidget")
require("CommunicationTestWidget")
require("SensorCheckWidget")
require("HardwareTestWidget")
require("UpdateWidget")
require("RunStatusWindow")
require("SystemConfigWidget")
require("ModbusCoder")
require("MeasureDataWindow")
require("ResultDetail")
require("ADCDetect")

return setting
