setting.ui.operation.motorOperator =
{
    name ="motorOperator",
    text= "电机操作",
    rowCount = 11,
    superRow = 0,
    administratorRow = 0,
    writePrivilege=  RoleType.Maintain,
    readPrivilege = RoleType.Maintain,
    -- row = 1
    {
        name = "MoveToZero",
        text= "移动至原点",
        data = 0,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.zero, step)
            flow.name = setting.ui.operation.motorOperator[1].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 2
    {
        name = "MoveToMeasureCell",
        text= "移动至测量槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.measureCell, step)
            flow.name = setting.ui.operation.motorOperator[2].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 3
    {
        name = "MoveToSampleMixer",
        text= "移动至样品混合槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.sampleMixer, step)
            flow.name = setting.ui.operation.motorOperator[3].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 4
    {
        name = "MoveToBacteriaMixer",
        text= "移动至发光菌混合槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.bacteriaMixer, step)
            flow.name = setting.ui.operation.motorOperator[4].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 5
    {
        name = "MoveToBacteriaCell",
        text= "移动至发光菌存储槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.bacteriaCell, step)
            flow.name = setting.ui.operation.motorOperator[5].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 6
    {
        name = "MoveToWasteCell",
        text= "移动至针排液槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.wasteCell, step)
            flow.name = setting.ui.operation.motorOperator[6].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 7
    {
        name = "MoveToCleanCell",
        text= "移动至针清洗槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.cleanCell, step)
            flow.name = setting.ui.operation.motorOperator[7].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 8
    {
        name = "MoveToBlankCell",
        text= "移动至空白水存储槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.blankCell, step)
            flow.name = setting.ui.operation.motorOperator[8].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 9
    {
        name = "MoveToSampleCell",
        text= "移动至水样存储槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.sampleCell, step)
            flow.name = setting.ui.operation.motorOperator[9].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 10
    {
        name = "MoveToStandardCell",
        text= "移动至质控样存储槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.standardCell, step)
            flow.name = setting.ui.operation.motorOperator[10].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    -- row = 11
    {
        name = "MoveToSaltCell",
        text= "移动至渗透压调节液槽",
        data = 200,
        createFlow= function(step)
            local flow = MotorOperateFlow:new({}, setting.component.xPos.saltCell, step)
            flow.name = setting.ui.operation.motorOperator[11].name
            FlowList.AddFlow(flow)
        end,
        writePrivilege=  RoleType.Maintain,
        readPrivilege = RoleType.Maintain,
    },
    checkvalue = function(value)
        local ret = false
        if type(value) == "string" then
            local integerPatterm = "^[-+]?%d?%d?%d$"
            if string.find(value, integerPatterm) then
                local num = tonumber(value)
                if num >= 0 and num <= setting.motor.maxStepsZ then
                    ret = true
                end
            end
        end

        if ret == true then
            return value
        else
            return "0"
        end
    end,
}
return setting.ui.operation.motorOperator
