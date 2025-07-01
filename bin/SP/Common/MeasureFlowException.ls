

UpdateType =
{
    DC =
    {
        name = "驱动板",
    },
    MC =
    {
        name = "主控板",
    },
}

--[[
@brief 测量流程异常。
--]]
MeasureFlowException = Exception:new
{
    message = "MeasureFlowException",
}

--[[
@brief 测量结果错误异常。
--]]
MeasureResultWrongException = MeasureFlowException:new
{
    message = "MeasureResultWrongException",
}

--[[
@brief 量程错误异常。
--]]
MeasureRangeWrongException = MeasureFlowException:new
{
    message = "MeasureRangeWrongException",
}

--[[
@brief 定标结果错误异常。
--]]
CalibrateResultWrongException = MeasureFlowException:new
{
    message = "CalibrateResultWrongException",
}

--[[
@brief 快速量程错误异常。
--]]
FastMeasureRangeWrongException = MeasureFlowException:new
{
    message = "FastMeasureRangeWrongException",
    consistency = 0,
    currange = 0,
}

--[[
@brief 仪器故障异常。
--]]
DeviceFaultException = MeasureFlowException:new
{
    message = "DeviceFaultException",
}

--[[
@brief 用户停止异常。
--]]
UserStopException = MeasureFlowException:new
{
    message = "UserStopException",
}

--[[
@brief 升级异常。
--]]
UpdateException = MeasureFlowException:new
{
    message = "UpdateException",
    updateType = UpdateType.DC,
}

--[[
@brief 排消解液异常。
--]]
DrainFromDigestionException = MeasureFlowException:new
{
    message = "DrainFromDigestionException",
}

--[[
@brief 泵校准异常。
--]]
CalibratePumpException = MeasureFlowException:new
{
    message = "CalibratePumpException",
}

--[[
@brief 水样超标异常。
--]]
MeasureResultOutLimitException = MeasureFlowException:new
{
    message = "MeasureResultOutLimitException",
}


