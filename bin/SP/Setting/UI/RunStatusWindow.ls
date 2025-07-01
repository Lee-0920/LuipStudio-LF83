setting.ui.runStatus =
{
    reportModeList =
    {
        "   运行 ",
        "   离线 ",
        "   维护 ",
        "   故障 ",
        "   校准 ",
    },
    targets =
    {
        {
            name = "抑制率",

            getData = function()
                local dateTime = status.measure.newResult.measure.dateTime
                local consistency = status.measure.newResult.measure.consistency

                return dateTime, consistency
            end,
            isAlarm = function()
                local consistency = status.measure.newResult.measure.consistency
                if consistency > config.interconnection.meaUpLimit then
                    return true
                elseif consistency < -config.interconnection.meaLowLimit then
                    return true
                else
                    return false
                end
            end,
	    
	    getResultType = function()
                local resultType  = status.measure.newResult.measure.resultType
                return resultType
            end,
        },
    },
	oneKeyMaintain = function()
            local flow = MeasureFlow:new({}, MeasureType.Sample)
            flow.name = setting.ui.operation.maintain[1].name
            flow.isOneKeyMaintain = true
            FlowList.AddFlow(flow)
	end,
    --unitChange = function(value, changeType) return ArguUnitChange(value, changeType) end,
}
return setting.ui.runStatus
