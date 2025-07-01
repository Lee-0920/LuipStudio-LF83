setting.modbusCoder = {}

setting.modbusCoder.statusID=
{
    Idle =
    {
        ID = 0,
        lf88ID = 1,
    },
    MeasureSample =
    {
        ID = 1,
        lf88ID = 4,
    },
    MeasureStandard =
    {
        ID = 2,
        lf88ID = 4,
    },
    MeasureBlank =
    {
        ID = 3,
        lf88ID = 4,
    },
    MachineReset =
    {
        ID = 4,
        lf88ID = 5,
    },
    LiquidPipeRecover =
    {
        ID = 5,
        lf88ID = 5,
    },
    TemperatureControlRecover =
    {
        ID = 6,
        lf88ID = 5,
    },
    CleanAll =
    {
        ID = 7,
        lf88ID = 5,
    },
    CollectSample =
    {
        ID = 8,
        lf88ID = 2,
    },
    LiquidOperate =
    {
        ID = 9,
        lf88ID = 0,
    },
    MotorOperate =
    {
        ID = 10,
        lf88ID = 5,
    },
    SensorCheck =
    {
        ID = 11,
        lf88ID = 5,
    },
    CommunicationCheck =
    {
        ID = 12,
        lf88ID = 5,
    },
    HardwareTest =
    {
        ID = 13,
        lf88ID = 5,
    },
    MASTER =
    {
        ID = 14,
        lf88ID = 5,
    },
    DCUpdate =
    {
        ID = 15,
        lf88ID = 5,
    },
    MeasureCheck =
    {
        ID = 16,
        lf88ID = 5,
    },
    PMTCheck =
    {
        ID = 17,
        lf88ID = 5,
    },
    Fault =
    {
        ID = 255,
        lf88ID = 7,
    },
}

setting.modbusCoder.actionID= {
    Idle =
    {
        ID = 0,
    },
    MachineReset =
    {
        ID = 1,
    },
    CleanBeforeMeasure =
    {
        ID = 2,
    },
    BacteriaMix =
    {
        ID = 3,
    },
    SampleMix =
    {
        ID = 4,
    },
    Foster =
    {
        ID = 5,
    },
    ReadInitialAD =
    {
        ID = 6,
    },
    MixBacteriaSample =
    {
        ID = 7,
    },
    ReactMeasure =
    {
        ID = 8,
    },
    CleanAfterMeasure =
    {
        ID = 9,
    },
    CleanStopMeasure =
    {
        ID = 10,
    },
    TemperatureControl =
    {
        ID = 11,
    },
    Collect =
    {
        ID = 12,
    },
    Silent =
    {
        ID = 13,
    },
    MoveToZero =
    {
        ID = 14,
    },
    MoveToMeasureCell =
    {
        ID = 15,
    },
    MoveToSampleMixer =
    {
        ID = 16,
    },
    MoveToBacteriaMixer =
    {
        ID = 17,
    },
    MoveToBacteriaCell =
    {
        ID = 18,
    },
    MoveToBlankCell =
    {
        ID = 19,
    },
    MoveToSampleCell =
    {
        ID = 20,
    },
    MoveToStandardCell =
    {
        ID = 21,
    },
    MoveToSaltCell =
    {
        ID = 22,
    },
    MoveToCleanCell =
    {
        ID = 23,
    },
    MoveToWasteCell =
    {
        ID = 24,
    },
    SuckFromSample =
    {
        ID = 25,
    },
    SuckFromBlank =
    {
        ID = 26,
    },
    SuckFromSalt =
    {
        ID = 27,
    },
    SuckFromWash =
    {
        ID = 28,
    },
    AirToNeedlePipe =
    {
        ID = 29,
    },
    DrainToWastePool =
    {
        ID = 30,
    },
    DrainToWasteOut =
    {
        ID = 31,
    },
    AspirationAirReagent =
    {
        ID = 32,
    },
    DischargeAirReagent =
    {
        ID = 33,
    },
    Updater =
    {
        ID = 34,
    },
    Erase =
    {
        ID = 35,
    },
    WriteProgram =
    {
        ID = 36,
    },
    SamplePumpTest =
    {
        ID = 37,
    },
    WashPumpTest =
    {
        ID = 38,
    },
    SaltPumpTest =
    {
        ID = 39,
    },
    WastePumpTest =
    {
        ID = 40,
    },
    StirPumpTest =
    {
        ID = 41,
    },
    BlankValveTest =
    {
        ID = 42,
    },
    RefWashValveTest =
    {
        ID = 43,
    },
    MeaWshValveTest =
    {
        ID = 44,
    },
    RefWasteValveTest =
    {
        ID = 45,
    },
    MeaWasteValveTest =
    {
        ID = 46,
    },
    AirInValveTest =
    {
        ID = 47,
    },
    AirOutValveTest =
    {
        ID = 48,
    },
    WasteValveTest =
    {
        ID = 49,
    },
    MeasureCellHeatingTest =
    {
        ID = 50,
    },
    MeasureCellCoolingTest =
    {
        ID = 51,
    },
    BacteriaStorageCoolingTest =
    {
        ID = 52,
    },
    MeasureCellWaterCoolerTest =
    {
        ID = 53,
    },
    MeasureCellFanTest =
    {
        ID = 54,
    },
    BacteriaStorageWaterCoolerTest =
    {
        ID = 55,
    },
    BacteriaStorageFanTest =
    {
        ID = 56,
    },
    CollectSampleRelayTest =
    {
        ID = 57,
    },
    Relay1Test =
    {
        ID = 58,
    },
    Relay2Test =
    {
        ID = 59,
    },
    SampleCurrent4Output =
    {
        ID = 60,
    },
    SampleCurrent12Output =
    {
        ID = 61,
    },
    SampleCurrent20Output =
    {
        ID = 62,
    },
    CheckCurrent4Output =
    {
        ID = 63,
    },
    CheckCurrent12Output =
    {
        ID = 64,
    },
    CheckCurrent20Output =
    {
        ID = 65,
    },
    SystemFanTest =
    {
        ID = 66,
    },
    Clean =
    {
        ID = 67,
    },
    InfiltrateAdjust =
    {
        ID = 68,
    },
    DrainMeasureCellWaste =
    {
        ID = 69,
    },
    ThermostatsMeasureControl =
    {
        ID = 70,
    },
    ThermostatsBacteriaControl =
    {
        ID = 71,
    },
    OneKeyPipeRenew =
    {
        ID = 72,
    },
    PMTCheck =
    {
        ID = 73,
    },
}

setting.modbusCoder.detectResultID=
{
    idle = 0,
    detecting = 1,
    passed = 2,
    fail = 3,
    stop = 4,
}



return setting.modbusCoder
