config.measureParam =
{                                                       -- 测量参数设置
    sampleReactTime = 30,                               --样本反应时间
    sampleVolume = 900,                                 --样本体积
    sampleBlankVolume = 0,                         --样本测试蒸馏水体积
    sampleSaltVolume = 100,                             --样本渗透压调节液体积

    standardReactTime = 30,                             --质控反应时间
    standardVolume = 100,                               --质控液体积
    standardBlankVolume = 800,                       --质控混合蒸馏水体积
    standardSaltVolume = 100,                             --样本渗透压调节液体积

    blankReactTime = 30,                                 -- 测空白水反应时间
    blankVolume = 900,                                 -- 空白水体积
    blankSaltVolume = 100,                             --样本渗透压调节液体积

    bacteriaVolume = 50,                                --发光菌体积
    bacteriaSaltVolume = 100,                           --渗透压调节液体积
    bacteriaBlankVolume = 900,                          --菌液空白水体积
    incubationTime = 0,                                 --孵育时间
    cleanBeforeMeasureTimes = 1,                        --测量前清洗次数
    cleanAfterMeasureTimes = 1,                         --测量后清洗次数

    addSaltVolume = 2,                                --渗透压调解液更新体积
    addBlankVolume = 2,                                --空白水更新体积
    addSampleVolume = 15,                                --水样更新体积
    mixSwitch = true,                                   --菌样混合开关
    mixBacteriaSampleMethod = 0,                               --菌样混合方式
    mixBacteriaSampleTimes = 3,                         --菌样混合次数
    mixBacteriaVolume = 800,                        --测量加菌液混合体积
    mixSampleVolume = 800,                          --测量加样本混合体积
    maxLimitLevel = 7,                              --最大限制档位
    PDPowerCtrl = false,                            --测量模块电源控制

    minNewBacteriaAD = 3000000,                  --最低复苏菌AD
    maxNewBacteriaAD = 4500000,                    --最高复苏菌AD

    pmtAuto = false,                             -- PMT自适应调节
    destLowLimit = 1800000,                     -- PMT调节上限
    destHighLimit = 2600000,                    -- PMT调节下限
    destTarget = 2200000,                       -- PMT调节目标

    airMixHeight = 165,                         -- 发光菌混合吹气高度
    airMixSignleTime = 0.5,                      -- 发光菌混合单针吹气时间
    airMixDoubleTime = 1,                        -- 发光菌混合双针吹气时间
    airMixLoopTimes = 3,                        -- 发光菌混合循环次数

    stirSpeed = 600,                                    --发光菌混合槽搅拌速度
    measureSafetyTemp = 15,                             --测量仓安全温度
    bacteriaSafetyTemp = 4,                             --菌室安全温度
    openTemperatureControl = true,                      --开启温控警告
    temperatureMonitor = false,                          --温度监控
    adDebug = false,
    currentRangeIndex = 0,                              --量程 防止报错
}
