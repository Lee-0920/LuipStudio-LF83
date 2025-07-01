status.measure =
{
    schedule =                      -- 自动测量
    {
        autoMeasure =               -- 自动测量
        {
            dateTime = _G.os.time(),   -- 测量日期
        },
        autoCalibrate =             -- 自动定标
        {
            dateTime = _G.os.time(),   -- 定标日期
        },
        autoClean =                 -- 自动清洗
        {
            dateTime = _G.os.time(),   -- 清洗日期
        },
        autoCheck =                 -- 自动质控测量
        {
            dateTime = _G.os.time(),   -- 质控日期
        },
        autoBlank =                 -- 自动空白水测量
        {
            dateTime = _G.os.time(),   -- 空白水日期
        },
    },

    report =
    {
        measure =                   -- 测量结果上报
        {
            dateTime = 0,           -- 测量日期
            consistency = 0,        --浓度(兼容平台)
            resultType = 0,        --  测量类型
            measureTime = 0,        -- 测量时长
            absorbance = 0,         -- 吸光度(兼容平台)
            deviceTypeNum = 0,      -- 设备参数类型号
            oldTypeNum = 0,         -- 旧设备参数类型号
            IR = 0,                 -- 抑制率
            RLI = 0,                -- 相对发光度
            CF = 1,                 -- 相对发光系数
            C0 = 1,                 -- 参考初始发光值
            Ct = 1,                 -- 参考结束发光值
            S0 = 1,                 -- 样本初始发光值
            St = 1,                 -- 样本结束发光值
            resultInfo = "N",       -- 数据标识
        },
        check =
        {
            dateTime = 0,           -- 核查日期
            consistency = 0,        --4-20输出浓度(兼容平台)
            IR = 0,                 -- 抑制率
            RLI = 0,                -- 相对发光度
            CF = 1,                 -- 相对发光系数
            C0 = 1,                 -- 参考初始发光值
            Ct = 1,                 -- 参考结束发光值
            S0 = 1,                 -- 质控初始发光值
            St = 1,                 -- 质控结束发光值
            resultInfo = "N",       -- 数据标识
        },
        blank =
        {
            dateTime = 0,           -- 空白核查日期
            consistency = 0,        --4-20输出浓度(兼容平台)
            IR = 0,                 -- 抑制率
            RLI = 0,                -- 相对发光度
            CF = 1,                 -- 相对发光系数
            C0 = 1,                 -- 参考初始发光值
            Ct = 1,                 -- 参考结束发光值
            S0 = 1,                 -- 质控初始发光值
            St = 1,                 -- 质控结束发光值
            resultInfo = "N",       -- 数据标识
        },
    },

    newResult =
    {
        measure =                   -- 新测量结果
        {
            dateTime = 0,           -- 测量日期
            consistency = 0,        -- 测量浓度(兼容平台)
            IR = 0,                 -- 抑制率
            RLI = 0,                -- 相对发光度
            CF = 1,                 -- 相对发光系数
            C0 = 1,                 -- 参考初始发光值
            Ct = 1,                 -- 参考结束发光值
            S0 = 1,                 -- 质控初始发光值
            St = 1,                 -- 质控结束发光值
            resultType = 0,        --  测量类型
            resultInfo = "N",       -- 数据标识
        },
    },

    isDeviceFault = false,
    isFatalError = false,
}
