setting.externalInterface = {}

setting.externalInterface.modbus =
{
    inputRegAddr = 0,
    inputRegNum = 256,
    holdRegAddr = 0,
    holdRegNum = 256,
    registers =
    {
        [0] =     --水样测量时间/测量反控
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 0 - setting.externalInterface.modbus.inputRegAddr

                local temp = os.date("*t", status.measure.report.measure.dateTime)
                temp.year = temp.year - 30
                if temp.year < 1970 then
                    temp.year = 1970
                end
                local LF88Time = os.time(temp) --转换成LF88格式的时间

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Input, offsetAddress, LF88Time)
            end,

            write = function()
                local offsetAddress = 0 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                local optcode = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                local flowManager = FlowManager.Instance()

                if flowManager:IsAuthorize() == true then
                    local maintain

                    if optcode == 0 then  --测量水样
                        maintain = setting.ui.operation.maintain[1]
                    elseif optcode == 1 then  --测质控样
                        maintain = setting.ui.operation.maintain[2]
                    elseif optcode == 2 then  --测空白水
                        maintain = setting.ui.operation.maintain[3]
                    elseif optcode == 3 then
                        flowManager:StopFlow()

                        if config.scheduler.measure.mode == MeasureMode.Continous then
                            config.scheduler.measure.mode = MeasureMode.Trigger
                            config.modifyRecord.scheduler(true)
                            ConfigLists.SaveSchedulerConfig()

                            local updateWidgetManager = UpdateWidgetManager.Instance()
                            updateWidgetManager:Update(UpdateEvent.ChangeMeaModeOnHome, "Modbus")
                        end
                    else
                        maintain = nil
                    end

                    if maintain and flowManager:IsFlowEnable() == true then
                        maintain.createFlow()
                        flowManager:StartFlow()
                    end
                end
            end,
        },
        [3] =   --水样测量结果(参考初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 3 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.C0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [5] =   --水样测量结果(样本初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 5 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.S0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [7] =   --水样测量结果(参考结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 7 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.Ct)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [9] =   --水样测量结果(样本结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 9 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.St)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [11] =   --水样测量结果(发光菌稳定系数)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 11 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.measure.CF)
            end,

            write = function()
            end,
        },
        [13] =   --水样测量结果(相对发光度)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 13 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.measure.RLI/100)
            end,

            write = function()
            end,
        },
        [15] =     --质控测量时间
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 15 - setting.externalInterface.modbus.inputRegAddr

                local temp = os.date("*t", status.measure.report.check.dateTime)
                temp.year = temp.year - 30
                if temp.year < 1970 then
                    temp.year = 1970
                end
                local LF88Time = os.time(temp) --转换成LF88格式的时间

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Input, offsetAddress, LF88Time)
            end,

            write = function()
            end,
        },
        [18] =   --质控测量结果(参考初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 18 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.C0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [20] =   --质控测量结果(质控初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 20 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.S0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [22] =   --质控测量结果(参考结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 22 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.Ct)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [24] =   --质控测量结果(质控结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 24 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.St)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [26] =   --质控测量结果(发光菌稳定系数)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 26 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.check.CF)
            end,

            write = function()
            end,
        },
        [28] =   --质控测量结果(相对发光度)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 28 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.check.RLI/100)
            end,

            write = function()
            end,
        },
        [30] =     --   系统状态
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 30 - setting.externalInterface.modbus.inputRegAddr

                local statusManager = StatusManager.Instance()
                local runStatus = statusManager:GetStatus()
                local name = runStatus:GetName()

                local modbusManager = ModbusManager.Instance()
                if setting.modbusCoder.statusID[name] ~= nil then
                    modbusManager:SetShort(RegisterType.Input, offsetAddress, setting.modbusCoder.statusID[name].lf88ID)
                else
                    modbusManager:SetShort(RegisterType.Input, offsetAddress, 0)
                end
            end,

            write = function()
            end,
        },
        [31] =     --  当前动作
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 31 - setting.externalInterface.modbus.inputRegAddr

                local statusManager = StatusManager.Instance()
                local action = statusManager:GetAction()
                local name = action:GetName()

                local modbusManager = ModbusManager.Instance()

                if setting.modbusCoder.actionID[name] ~= nil then
                    modbusManager:SetShort(RegisterType.Input, offsetAddress, setting.modbusCoder.actionID[name].ID)
                else
                    modbusManager:SetShort(RegisterType.Input, offsetAddress, 0)
                end

            end,

            write = function()
            end,
        },
        [32] =     --   发光菌温度
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 32 - setting.externalInterface.modbus.inputRegAddr

                local  bacteriaTemp = dc:GetThermostatSignalTemp(1)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, bacteriaTemp)
            end,

            write = function()
            end,
        },
        [34] =     --   测量仓温度
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 34 - setting.externalInterface.modbus.inputRegAddr

                local  measureTemp = dc:GetThermostatSignalTemp(0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, measureTemp)
            end,

            write = function()
            end,
        },
        [36] =     --   环境温度
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 36 - setting.externalInterface.modbus.inputRegAddr

                local  environmentTemp = dc:GetEnvironmentSignalTemp()

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, environmentTemp)
            end,

            write = function()
            end,
        },
        [38] =     --   发光菌余量
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 38 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.remain.bacteria)
            end,

            write = function()
            end,
        },
        [40] =     --   质控液余量
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 40 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.remain.standard)
            end,

            write = function()
            end,
        },
        [42] =     --   渗透压调节液余量
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 42 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.remain.salt)
            end,

            write = function()
            end,
        },
        [44] =   -- 故障码
        {
            number = 1,		-- Register number

            read = function()
                local offsetAddress = 44 - setting.externalInterface.modbus.inputRegAddr

                local AlarmManager = AlarmManager.Instance()
                local alarm = AlarmManager:GetCurrentAlarm()
                local name = alarm:GetName()
                local cause = alarm:GetCause()

                local alarmType = 0
                local alarmData = 0

                if name == "电机运动故障" then
                    alarmType = 0x4
                    alarmData = 0x2
                elseif name == "参考针碰撞" then
                    alarmType = 0x3
                    alarmData = 0x4
                elseif name == "样本针碰撞" then
                    alarmType = 0x3
                    alarmData = 0x4
                elseif name == "注射器移不出传感器" then
                    alarmType = 0x1
                    alarmData = 0x1
                elseif name == "注射器找不到传感器" then
                    alarmType = 0x1
                    alarmData = 0x0
                elseif name == "X轴移不出传感器" then
                    alarmType = 0x2
                    alarmData = 0x1
                elseif name == "Z轴移不出传感器" then
                    alarmType = 0x3
                    alarmData = 0x1
                elseif name == "X轴找不到传感器" then
                    alarmType = 0x2
                    alarmData = 0x0
                elseif name == "Z轴找不到传感器" then
                    alarmType = 0x3
                    alarmData = 0x0
                elseif name == "光学信号采集故障" then
                    alarmType = 0x4
                    alarmData = 0x2
                elseif name == "温控故障" then
                    alarmType = 0x8
                elseif name == "仪器运行故障" then
                    alarmType = 0x4
                    alarmData = 0x2
                elseif name == "测量仓温度超警戒范围" then
                    alarmType = 0x8
                elseif name == "菌室温度超警戒范围" then
                    alarmType = 0x8
                elseif name == "通信异常" then
                    alarmType = 0x4
                    alarmData = 0x2
                elseif name == "升级失败" then
                    alarmType = 0x4
                    alarmData = 0x2
                end

                --故障码+细节
                --local outputValue = (alarmData << 8) + alarmType
                local outputValue = (alarmType << 8) + alarmData

                --log:debug("outputValue = "..outputValue.." type = "..alarmType.." data = "..alarmData)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [45] =     --   45~256预留
        {
            number = 211,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },
    }
}
