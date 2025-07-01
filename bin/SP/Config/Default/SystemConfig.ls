config.system =
{
    screenSaver =
    {
        enable = true,              -- 是否开屏保
        darkTime = 300,	            -- 暗屏时间
        offTime = 600,	            -- 关屏时间
        loginKeepingTime = 600,		-- 用户退出时间
    },
    adcDetect =
    {
        --漏液检测
        {
            enable = false,
        },
        --测量触发
        {
            enable = false,
        },
    },
    faultBlocking = false,   --故障停机
    reagentLackWarn = false,    -- 缺试剂告警
    faultHandleEnable = true,       -- 异常处理
    debugMode= false,			 -- 开发调试模式
    bridgeMode= false,			 -- 桥接模式
    bridgeIP= "192.168.2.1",	 -- 桥接IP
    modbusTableType = 0,              -- Modbus表类型
    CCEPMode = false,                                   -- CCEP认证模式
    solidifyMeaParamFromUI = false,                     -- 不可通过UI修改测量参数
    solidifyMeaParamFromModbus = false,                 -- 不可通过Modbus修改测量参数
    displayMeaParamInGeneral = false,                   -- 普通用户显示测量参数
    unitIndex = 0,                                      --单位索引
    useQrencode = false,                                 -- 二维码
    serverIP = "ins.watertestcloud.com",
    serverPort = 8800,
    serverMode = true,
    targetMap =
    {
        PHOT =
        {
            enable = true,
            num = _G.Target.PHOT,
        },
    },
    hj212Type = _G.HJ212Type.None,              --HJ212协议输出类型
    hj212DataType = _G.HJ212DataType.HourData,  --HJ212协议输出数据类型
    hj212Interval = 10,                         --间隔时间,单位:分钟
    st = "21",
    pw = "123456",
    mn = "A20800000_TCP30",
}
