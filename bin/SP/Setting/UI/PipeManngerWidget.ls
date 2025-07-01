setting.ui.operation.liquidOperator =
{
    name ="liquidOperator",
    text= "管道操作",
    rowCount = 13,
    superRow = 0,
    administratorRow = 0,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    useOption = false,
    -- row = 1
    {
        name = "SuckFromSample",
        text= "填充水样",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.sample, RollDirection.Suck, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[1].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 2
    {
        name = "SuckFromBlank",
        text= "填充空白水",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.blank, RollDirection.Suck, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[2].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 3
    {
        name = "SuckFromSalt",
        text= "填充渗透压调节液",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.salt, RollDirection.Suck, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[3].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 4
    {
        name = "SuckFromWash",
        text= "针管路加清洗水",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.wash, RollDirection.Suck, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[4].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 5
    {
        name = "AirToNeedlePipe",
        text= "针管路吹气",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airWash, RollDirection.Drain, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[5].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 6
    {
        name = "DrainToWastePool",
        text= "针管路排进废液池",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airWaste, RollDirection.Drain, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[6].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 7
    {
        name = "DrainToWasteOut",
        text= "排空废液池",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.wasteOut, RollDirection.Drain, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[7].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 8
    {
        name = "DrainToWasteRoom",
        text= "排空测量仓废液",
        data = 10,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.wasteRoom, RollDirection.Drain, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[8].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.pumpVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 50 then
                    return "10"
                else
                    return value
                end
            else
                return "10"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 9
    {
        name = "SyringeAspiration",
        text= "注射器吸",
        data = 1,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airReagent, RollDirection.Suck, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[9].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.syrVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 1 then
                    return "1"
                else
                    return value
                end
            else
                return "1"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 10
    {
        name = "SyringeDischarge",
        text= "注射器排(带回程误差)",
        data = 1,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airReagent, RollDirection.Drain, 0, volume)
            flow.name = setting.ui.operation.liquidOperator[10].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.syrVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 1 then
                    return "1"
                else
                    return value
                end
            else
                return "1"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 11
    {
        name = "SyringeDrainMeasureRoom",
        text= "注射器排测量仓溶液",
        data = 1,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airReagent, RollDirection.Suck, setting.component.xPos.measureCell, volume)
            flow.name = setting.ui.operation.liquidOperator[11].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.syrVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 1 then
                    return "1"
                else
                    return value
                end
            else
                return "1"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 12
    {
        name = "SyringeDrainSampleMixer",
        text= "注射器排水样混合槽",
        data = 1,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airReagent, RollDirection.Suck, setting.component.xPos.sampleMixer, volume)
            flow.name = setting.ui.operation.liquidOperator[12].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.syrVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 1 then
                    return "1"
                else
                    return value
                end
            else
                return "1"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 13
    {
        name = "SyringeDrainBacteriaMixer",
        text= "注射器排菌液槽溶液",
        data = 1,
        createFlow= function(mode, volume)
            local flow = LiquidOperateFlow:new({}, setting.liquidType.airReagent, RollDirection.Suck, setting.component.xPos.bacteriaMixer, volume)
            flow.name = setting.ui.operation.liquidOperator[13].name
            FlowList.AddFlow(flow)
        end,
        checkValue = function(mode, value)
            if setting.ui.operation.liquidOperator.syrVolumePattern(mode, value) == true then
                local num = tonumber(value)
                if num < 0 or num > 1 then
                    return "1"
                else
                    return value
                end
            else
                return "1"
            end
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    pumpVolumePattern = function(mode, value)
        local ret = false
        if type(value) == "string" then
            local decimalPatterm = "^%d?%d%.%d%d?%d?$"
            local integerPatterm = "^%d?%d$"
            if not string.find(value, decimalPatterm) then
                if string.find(value, integerPatterm) then
                    ret = true
                end
            else
                ret = true
            end
        end

        return ret
    end,
    syrVolumePattern = function(mode, value)
        local ret = false
        if type(value) == "string" then
            local decimalPatterm = "^%d%.%d%d?%d?$"
            local integerPatterm = "^%d$"
            if not string.find(value, decimalPatterm) then
                if string.find(value, integerPatterm) then
                    ret = true
                end
            else
                ret = true
            end
        end

        return ret
    end,
}
return setting.ui.operation.liquidOperator
