status.running =
{
    isMeasuring = false, -- 是否正常测量，用于测量过程中断电或重启后重新测量
    lastMeasureTime = 0,
    lastMeasureType = _G.MeasureType.Sample,
}
