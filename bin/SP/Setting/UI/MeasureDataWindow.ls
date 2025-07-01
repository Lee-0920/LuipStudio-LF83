setting.ui.measureDataWindow =
{
    measureData =
    {
        {
            name = "测量数据",
            resultDataname = setting.resultFileInfo.measureRecordFile[1].name,
        },
    },
    waveData =
    {
        {
            name = "测量波形",
            resultDataname = setting.resultFileInfo.measureRecordFile[1].name,
            profileTableName = "config.measureParam",
            rangeTableName = "setting.measure.range",
        },
    },
}
return setting.ui.measureDataWindow
