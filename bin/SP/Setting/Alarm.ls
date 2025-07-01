setting.alarm =
{
    pumpFailed =--
    {
        level = "Error",
        type = "仪器故障",
        name = "抽取失败",
    },
    pumpStop =--
    {
        level = "Warning", --只输出日志
        type = "仪器故障",
        name = "抽取被停止",
    },
    motorFailed =--
    {
        level = "Error",
        type = "仪器故障",
        name = "电机运动故障",
    },
    refCollide =--
    {
        level = "Error",
        type = "仪器故障",
        name = "参考针碰撞",
    },
    meaCollide =--
    {
        level = "Error",
        type = "仪器故障",
        name = "样本针碰撞",
    },
    motorXMoveOutSensorFail =--
    {
        level = "Error",
        type = "仪器故障",
        name = "X轴移不出传感器",
    },
    motorZMoveOutSensorFail =--
    {
        level = "Error",
        type = "仪器故障",
        name = "Z轴移不出传感器",
    },
    motorXMoveInSensorFail =--
    {
        level = "Error",
        type = "仪器故障",
        name = "X轴找不到传感器",
    },
    motorZMoveInSensorFail =--
    {
        level = "Error",
        type = "仪器故障",
        name = "Z轴找不到传感器",
    },
    syringeMoveOutSensorFail =--
    {
        level = "Error",
        type = "仪器故障",
        name = "注射器移不出传感器",
    },
    syringeMoveInSensorFail =--
    {
        level = "Error",
        type = "仪器故障",
        name = "注射器找不到传感器",
    },
    motorDriverError =--
    {
        level = "Error",
        type = "仪器故障",
        name = "电机驱动错误",
    },
    motorStop =--
    {
        level = "Warning", --只输出日志
        type = "仪器故障",
        name = "电机运动停止",
    },
    meterFailed =--
    {
        level = "Error",
        type = "仪器故障",
        name = "抽取失败",
    },
    acquirerADStop =--
    {
        level = "Warning",
        type = "仪器故障",
        name = "光学采集被停止",
    },
    acquirerADFailed =--
    {
        level = "Error",
        type = "仪器故障",
        name = "光学信号采集故障",
    },
    meterStop =--
    {
        level = "Warning",
        type = "仪器故障",
        name = "抽取被停止",
    },
    reagentRemain =--
    {
        level = "Error",
        type = "维护提示",
        name = "请更换试剂",
    },
    materialGoBad =--
    {
        level = "Error",
        type = "维护提示",
        name = "请更换耗材",
    },
    thermostatStop =--
    {
        level = "Warning",
        type = "仪器故障",
        name = "恒温被停止",
    },
    thermostatFault =--
    {
        level = "Error",
        type = "仪器故障",
        name = "温控故障",
    },
    measureTempOutLimit =--
    {
        level = "Error",
        type = "仪器故障",
        name = "测量仓温度超警戒范围",
    },
    bacteriaTempOutLimit =--
    {
        level = "Error",
        type = "仪器故障",
        name = "菌室温度超警戒范围",
    },
    dncpStackInit =--
    {
        level = "Info", --只输出日志，并且是warn类型的日志
        type = "通信异常",
        name = "DncpStack失败",
    },
    communication =--
    {
        level = "Error",
        type = "通信异常",
        name = "通信异常",
    },
    updateFault =--
    {
        level = "Error",
        type = "升级异常",
        name = "升级失败",
    },
    userStop =--
    {
        level = "Warning",
        type = "用户停止",
        name = "用户停止",
    },
    deviceRunFault =--
    {
        level = "Error",
        type = "仪器故障",
        name = "仪器运行故障",
    },
    drainFromDigestionFault =--
    {
        level = "Error",
        type = "仪器故障",
        name = "排消解液异常",
    },
    calibratePumpFailed =--
    {
        level = "Error",
        type = "仪器故障",
        name = "蠕动泵校准失败",
    },
    measureResultOutLimit =--
    {
        level = "Error",
        type = "测量异常",
        name = "水样超标",
    },
    measureResultWrong =
    {
        level = "Error",
        type = "测量异常",
        name = "测量结果异常",
    },
    calibrateResultWrong =
    {
        level = "Error",
        type = "测量异常",
        name = "定标结果错误",
    },
    bacteriaADBelowLimit =
    {
        level = "Error",
        type = "测量异常",
        name = "发光菌光强偏低",
    },
    bacteriaADAboveLimit =
    {
        level = "Error",
        type = "测量异常",
        name = "发光菌光强偏高",
    },
    bacteriaAbnormalAD =
    {
        level = "Error",
        type = "测量异常",
        name = "发光菌光强异常",
    },
    exception =--
    {
        level = "Info",
        type = "系统异常",
        name = "系统异常",
    },
    std_exception =--
    {
        level = "Info",
        type = "系统异常",
        name = "std异常",
    },
}

return setting.alarm
