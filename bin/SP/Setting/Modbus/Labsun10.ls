setting.externalInterface = {}

setting.externalInterface.modbus =
{
    inputRegAddr = 0,
    inputRegNum = 256,
    holdRegAddr = 256,
    holdRegNum = 256,
    registers =
    {
        [0] =   --测量结果(抑制率)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 0 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.measure.IR)
           end,

            write = function()
            end,
        },
        [2] =   --质控结果(抑制率)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 2 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.check.IR)
           end,

            write = function()
            end,
        },
        [4] =     -- 空白结果(抑制率)
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 4 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.blank.IR)
            end,

            write = function()
            end,
        },
        [6] =     -- 测量时长
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 6 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Input, offsetAddress, status.measure.report.measure.measureTime)
            end,

            write = function()
            end,
        },
        [7] =     --测量时间
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 7 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Input, offsetAddress, status.measure.report.measure.dateTime)
            end,

            write = function()
            end,
        },
        [10] =     --质控时间
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 10 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Input, offsetAddress, status.measure.report.check.dateTime)
            end,

            write = function()
            end,
        },
        [13] =     --空白时间
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 13 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Input, offsetAddress, status.measure.report.blank.dateTime)
            end,

            write = function()
            end,
        },
        [16] =     --   系统状态
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 16 - setting.externalInterface.modbus.inputRegAddr

                local statusManager = StatusManager.Instance()
                local runStatus = statusManager:GetStatus()
                local name = runStatus:GetName()

                local modbusManager = ModbusManager.Instance()
                if setting.modbusCoder.statusID[name] ~= nil then
                    modbusManager:SetShort(RegisterType.Input, offsetAddress, setting.modbusCoder.statusID[name].ID)
                else
                    modbusManager:SetShort(RegisterType.Input, offsetAddress, 0)
                end
            end,

            write = function()
            end,
        },
        [17] =     --  当前动作
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 17 - setting.externalInterface.modbus.inputRegAddr

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
        [18] =     --   当前温度
        {
            number = 4,	-- Register number

            read = function()
                local offsetAddress = 18 - setting.externalInterface.modbus.inputRegAddr

                local measureTemp = SignalManager.Instance():GetSignal("tTemp")
                local bacteriaTemp = SignalManager.Instance():GetSignal("temp1")

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, measureTemp)
                modbusManager:SetFloat(RegisterType.Input, offsetAddress + 2, bacteriaTemp)
            end,

            write = function()
            end,
        },
        [22] =     --  报警时间
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 22 - setting.externalInterface.modbus.inputRegAddr

                local AlarmManager = AlarmManager.Instance()
                local dateTime = AlarmManager:GetCurrentAlarm():GetTime()

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Input, offsetAddress, dateTime )
            end,

            write = function()
            end,
        },
        [25] =   --报警代码
        {
            number = 1,		-- Register number

            read = function()
                local offsetAddress = 25 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                local AlarmManager = AlarmManager.Instance()
                local alarm = AlarmManager:GetCurrentAlarm()
                local name = alarm:GetName()
                local cause = alarm:GetCause()

                local alarmID = 0
                local modbusManager = ModbusManager.Instance()

                if name == "电机运动故障" then
                    alarmID = 1
                elseif name == "参考针碰撞" then
                    alarmID = 2
                elseif name == "样本针碰撞" then
                    alarmID = 3
                elseif name == "X轴移不出传感器" then
                    alarmID = 4
                elseif name == "Z轴移不出传感器" then
                    alarmID = 5
                elseif name == "X轴找不到传感器" then
                    alarmID = 6
                elseif name == "Z轴找不到传感器" then
                    alarmID = 7
                elseif name == "光学信号采集故障" then
                    alarmID = 8
                elseif name == "温控故障" then
                    alarmID = 9
                elseif name == "仪器运行故障" then
                    alarmID = 10
                elseif name == "测量仓温度超警戒范围" then
                    alarmID = 11
                elseif name == "菌室温度超警戒范围" then
                    alarmID = 12
                elseif name == "通信异常" then
                    alarmID = 13
                elseif name == "升级失败" then
                    alarmID = 14
                elseif name == "测量结果异常" then
                    alarmID = 15
                elseif name == "水样超标" then
                    alarmID = 16
                elseif name == "请更换试剂" then
                    if cause == "发光菌" then
                        alarmID = 17
                    elseif cause == "渗透压调节液" then
                        alarmID = 18
                    elseif cause == "质控液" then
                        alarmID = 19
                    elseif cause == "空白水" then
                        alarmID = 20
                    elseif cause == "废液桶" then
                        alarmID = 21
                    end
                elseif name == "请更换耗材" then
                    if cause == "软管泵头" then
                        alarmID = 22
                    elseif cause == "PTFE硬管" then
                        alarmID = 23
                    end
                elseif name == "发光菌光强偏低" then
                    alarmID = 24
                elseif name == "发光菌光强偏高" then
                    alarmID = 25
                elseif name == "发光菌光强异常" then
                    alarmID = 26
                elseif name == "注射器移不出传感器" then
                    alarmID = 27
                elseif name == "注射器找不到传感器" then
                    alarmID = 28
                elseif name == "抽取失败" then
                    if cause == "空白水" then
                        alarmID = 29
                    else
                        alarmID = 30
                    end
                else
                    alarmID = 0
                end

                modbusManager:SetShort(RegisterType.Input, offsetAddress, alarmID)
            end,

            write = function()
            end,
        },
        [26] =   --测量结果(稳定系数CF)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 26 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.measure.CF)
            end,

            write = function()
            end,
        },
        [28] =   --质控结果(稳定系数CF)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 28 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.check.CF)
            end,

            write = function()
            end,
        },
        [30] =   --空白结果(稳定系数CF)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 30 - setting.externalInterface.modbus.inputRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, status.measure.report.blank.CF)
            end,

            write = function()
            end,
        },
        [32] =     --
        {
            number = 4,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [36] =     --  产品名称
        {
            number = 32,	-- Register number

            read = function()
                local offsetAddress = 36 - setting.externalInterface.modbus.inputRegAddr
                local regSize = setting.externalInterface.modbus.registers[36].number

                local modbusManager = ModbusManager.Instance()
                local str = setting.instrument.name

                modbusManager:SetString(RegisterType.Input, offsetAddress, str, regSize)
            end,

            write = function()
            end,
        },
        [68] =     --  产品型号
        {
            number = 32,	-- Register number

            read = function()
                local offsetAddress = 68 - setting.externalInterface.modbus.inputRegAddr
                local regSize = setting.externalInterface.modbus.registers[68].number

                local modbusManager = ModbusManager.Instance()
                local str = config.info.instrument["model"]

                modbusManager:SetString(RegisterType.Input, offsetAddress, str, regSize)
            end,

            write = function()
            end,
        },
        [100] =     --  生产厂商
        {
            number = 32,	-- Register number

            read = function()
                local offsetAddress = 100 - setting.externalInterface.modbus.inputRegAddr
                local regSize = setting.externalInterface.modbus.registers[100].number

                local modbusManager = ModbusManager.Instance()
                local str = config.info.instrument["manuFacturer"]

                modbusManager:SetString(RegisterType.Input, offsetAddress, str, regSize)
            end,

            write = function()
            end,
        },
        [132] =     -- 生产序列号
        {
            number = 16,	-- Register number

            read = function()
                local offsetAddress = 132 - setting.externalInterface.modbus.inputRegAddr
                local regSize = setting.externalInterface.modbus.registers[132].number

                local modbusManager = ModbusManager.Instance()
                local str = config.info.instrument["sn"]

                modbusManager:SetString(RegisterType.Input, offsetAddress, str, regSize)
            end,

            write = function()
            end,
        },
        [148] =     --   液路板固件版本
        {
            number = 2,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [150] =     --   温控板固件版本
        {
            number = 2,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [152] =     --   信号板固件版本
        {
            number = 2,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [154] =     --  主控板固件版本
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 154 - setting.externalInterface.modbus.inputRegAddr

                local ver = Version.new(setting.version.software)

                local major = ver:GetMajor()
                local minor = ver:GetMinor()
                local build  = ver:GetBuild()
                local revision  = ver:GetRevision()
                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Input, offsetAddress, major * 256 + minor)
                modbusManager:SetShort(RegisterType.Input, offsetAddress + 1, revision * 256 + build)

                ver = nil
            end,

            write = function()
            end,
        },
        [156] =     --   驱动板固件版本
        {
            number = 2,	-- Register number

            read = function()
                if dc ~= nil then
                    local offsetAddress = 156 - setting.externalInterface.modbus.inputRegAddr

                    local ver = 0
                    local major = 0
                    local minor = 0
                    local build  = 0
                    local revision  = 0
                    local err,result = pcall(function()
                        if dc:GetConnectStatus() == true then
                            ver = dc:GetIDeviceInfo():GetSoftwareVersion()
                        end
                    end)

                    if not err then      -- 出现异常
                        if type(result) == "userdata" then
                            if result:GetType() == "ExpectEventTimeoutException" then          --期望事件等待超时异常。
                                ExceptionHandler.MakeAlarm(result)
                            elseif result:GetType() == "CommandTimeoutException" then          --命令应答超时异常
                                ExceptionHandler.MakeAlarm(result)
                            else
                                log:warn(result:What())
                            end
                        elseif type(result) == "table" then
                            log:warn(result:What())								--其他定义类型异常
                        elseif type(result) == "string" then
                            log:warn(result)	--C++、Lua系统异常
                        end
                    else
                        if ver ~= 0 then
                            major = ver:GetMajor()
                            minor = ver:GetMinor()
                            build  = ver:GetBuild()
                            revision  = ver:GetRevision()
                        end
                        local modbusManager = ModbusManager.Instance()
                        modbusManager:SetShort(RegisterType.Input, offsetAddress, major * 256 + minor)
                        modbusManager:SetShort(RegisterType.Input, offsetAddress + 1, revision * 256 + build)
                    end
                end
            end,

            write = function()
            end,
        },
        [158] =     --预留
        {
            number = 2,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [160] =   --水样测量结果(参考初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 160 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.C0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [162] =   --水样测量结果(样本初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 162 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.S0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [164] =   --水样测量结果(参考结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 164 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.Ct)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [166] =   --水样测量结果(样本结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 166 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.measure.St)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [168] =   --水样测量结果(发光菌稳定系数)
        {
            number = 2,		-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [170] =   --质控测量结果(参考初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 170 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.C0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [172] =   --质控测量结果(样本初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 172 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.S0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [174] =   --质控测量结果(参考结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 174 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.Ct)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [176] =   --质控测量结果(样本结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 176 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.check.St)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [178] =
        {
            number = 2,		-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [180] =   --空白测量结果(参考初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 180 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.blank.C0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [182] =   --空白测量结果(样本初始发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 182 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.blank.S0)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [184] =   --空白测量结果(参考结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 184 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.blank.Ct)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [186] =   --空白测量结果(样本结束发光值)
        {
            number = 2,		-- Register number

            read = function()
                local offsetAddress = 186 - setting.externalInterface.modbus.inputRegAddr

                local outputValue = math.floor(status.measure.report.blank.St)

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, outputValue)
            end,

            write = function()
            end,
        },
        [188] = --预留
        {
            number = 2,		-- Register number

            read = function()
            end,

            write = function()
            end,
        },
        [190] =     --   环境温度
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 190 - setting.externalInterface.modbus.inputRegAddr

                local value = SignalManager.Instance():GetSignal("eTemp")

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Input, offsetAddress, value)
            end,

            write = function()
            end,
        },
        [192] =     --   参考AD
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 192 - setting.externalInterface.modbus.inputRegAddr

                local value = math.floor(SignalManager.Instance():GetSignal("refAD"))

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, value)
            end,

            write = function()
            end,
        },
        [194] =     --   样本AD
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 194 - setting.externalInterface.modbus.inputRegAddr

                local value = math.floor(SignalManager.Instance():GetSignal("meaAD"))

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetInt(RegisterType.Input, offsetAddress, value)
            end,

            write = function()
            end,
        },
        [196] = --预留
        {
            number = 60,		-- Register number

            read = function()
            end,

            write = function()
            end,
        },

        [256] =    --  报警上限(抑制报警限-绝对值)
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 256 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Hold, offsetAddress, config.interconnection.meaUpLimit)
            end,

            write = function()
                local offsetAddress = 256 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetFloat(RegisterType.Hold, offsetAddress)

                    if inputValue >= config.interconnection.meaLowLimit then
                        config.interconnection.meaUpLimit = inputValue
                    end

                    config.modifyRecord.interconnection(true)
                    ConfigLists.SaveInterconnectionConfig()

                    local updateWidgetManager = UpdateWidgetManager.Instance()
                    updateWidgetManager:Update(UpdateEvent.ChangeInterconnectionParam, "Modbus")
                end
            end,
        },
        [258] =    --  报警下限(促进报警限-绝对值)
        {
            number = 2,	-- Register number

            read = function()
                local offsetAddress = 258 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetFloat(RegisterType.Hold, offsetAddress, config.interconnection.meaLowLimit)
            end,

            write = function()
                local offsetAddress = 258 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetFloat(RegisterType.Hold, offsetAddress)

                    if inputValue <= config.interconnection.meaUpLimit then
                        config.interconnection.meaLowLimit = inputValue
                    end

                    config.modifyRecord.interconnection(true)
                    ConfigLists.SaveInterconnectionConfig()

                    local updateWidgetManager = UpdateWidgetManager.Instance()
                    updateWidgetManager:Update(UpdateEvent.ChangeInterconnectionParam, "Modbus")
                end
            end,
        },
        [260] =    -- 测量前清洗次数
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 260 - setting.externalInterface.modbus.holdRegAddr

                local outputValue = config.measureParam.cleanBeforeMeasureTimes

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Hold, offsetAddress, outputValue)
            end,

            write = function()
                local offsetAddress = 260 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                    if inputValue >= 1 and inputValue <= 10 then
                        config.measureParam.cleanBeforeMeasureTimes = inputValue
                    end
                end
                config.modifyRecord.measureParam(true)
                ConfigLists.SaveMeasureParamConfig()

                local updateWidgetManager = UpdateWidgetManager.Instance()
                updateWidgetManager:Update(UpdateEvent.ModbusChangeParam, "Modbus")
            end,
        },
        [261] =    -- 测量后清洗次数
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 261 - setting.externalInterface.modbus.holdRegAddr

                local outputValue = config.measureParam.cleanAfterMeasureTimes

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Hold, offsetAddress, outputValue)
            end,

            write = function()
                local offsetAddress = 261 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                    if inputValue >= 1 and inputValue <= 10 then
                        config.measureParam.cleanAfterMeasureTimes = inputValue
                    end
                end
                config.modifyRecord.measureParam(true)
                ConfigLists.SaveMeasureParamConfig()

                local updateWidgetManager = UpdateWidgetManager.Instance()
                updateWidgetManager:Update(UpdateEvent.ModbusChangeParam, "Modbus")
            end,
        },
        [262] =    -- 样本反应时间（分钟）
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 262 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Hold, offsetAddress, config.measureParam.sampleReactTime)
            end,

            write = function()
                local offsetAddress = 262 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                    if inputValue >= 5 and inputValue <= 60 then
                        config.measureParam.sampleReactTime = inputValue
                    end
                end
                config.modifyRecord.measureParam(true)
                ConfigLists.SaveMeasureParamConfig()

                local updateWidgetManager = UpdateWidgetManager.Instance()
                updateWidgetManager:Update(UpdateEvent.ModbusChangeParam, "Modbus")
            end,
        },
        [263] =    -- 质控反应时间（分钟）
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 263 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Hold, offsetAddress, config.measureParam.standardReactTime)
            end,

            write = function()
                local offsetAddress = 263 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                    if inputValue >= 5 and inputValue <= 60 then
                        config.measureParam.standardReactTime = inputValue
                    end
                end
                config.modifyRecord.measureParam(true)
                ConfigLists.SaveMeasureParamConfig()

                local updateWidgetManager = UpdateWidgetManager.Instance()
                updateWidgetManager:Update(UpdateEvent.ModbusChangeParam, "Modbus")
            end,
        },
        [264] =    -- 空白反应时间（分钟）
        {
            number = 1,	-- Register number

            read = function()
                local offsetAddress = 264 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetShort(RegisterType.Hold, offsetAddress, config.measureParam.blankReactTime)
            end,

            write = function()
                local offsetAddress = 264 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    local inputValue = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                    if inputValue >= 5 and inputValue <= 60 then
                        config.measureParam.blankReactTime = inputValue
                    end
                end
                config.modifyRecord.measureParam(true)
                ConfigLists.SaveMeasureParamConfig()

                local updateWidgetManager = UpdateWidgetManager.Instance()
                updateWidgetManager:Update(UpdateEvent.ModbusChangeParam, "Modbus")
            end,
        },

        [265] =     --预留
        {
            number = 7,	-- Register number

            read = function()
            end,

            write = function()
            end,
        },

        [272] =    --  测量控制
        {
            number = 1,	-- Register number
            read = function()
            end,

            write = function()
                local offsetAddress = 272 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                local optcode = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                local flowManager = FlowManager.Instance()

                if flowManager:IsAuthorize() == true then
                    local maintain

                    if optcode == 0 then  --停止
                        flowManager:StopFlow()

                        if config.scheduler.measure.mode == MeasureMode.Continous then
                            config.scheduler.measure.mode = MeasureMode.Trigger
                            config.modifyRecord.scheduler(true)
                            ConfigLists.SaveSchedulerConfig()

                            local updateWidgetManager = UpdateWidgetManager.Instance()
                            updateWidgetManager:Update(UpdateEvent.ChangeMeaModeOnHome, "Modbus")
                        end
                    elseif optcode == 1 then  --测量水样
                        maintain = setting.ui.operation.maintain[1]
                    elseif optcode == 2 then  --测质控样
                        maintain = setting.ui.operation.maintain[2]
                    elseif optcode == 3 then  --测空白水
                        maintain = setting.ui.operation.maintain[3]
                    elseif optcode == 4 then
                        maintain = setting.ui.operation.maintain[4]
                    elseif optcode == 5 then
                        maintain = setting.ui.operation.maintain[5]
                    elseif optcode == 6 then
                        maintain = setting.ui.operation.maintain[6]
                    elseif optcode == 7 then
                        maintain = setting.ui.operation.maintain[7]
                    elseif optcode == 8 then
                        maintain = setting.ui.operation.maintain[8]
                    elseif optcode == 9 then
                        maintain = setting.ui.operation.maintain[9]
                    elseif optcode == 10 then
                        maintain = setting.ui.operation.maintain[10]
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
        [273] =    --  外部采水继电器控制
        {
            number = 1,	-- Register number
            read = function()
            end,

            write = function()
                local offsetAddress = 273 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                local optcode = modbusManager:GetShort(RegisterType.Hold, offsetAddress)

                if config.interconnection.collectSampleMode == setting.mode.collectSample.trigger then
                    local waterCollector = WaterCollector.Instance()
                    if optcode == 0 then
                        log:debug("远程控制采水继电器 关")
                        waterCollector:TurnOff()
                        Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)
                    elseif optcode == 1 then
                        log:debug("远程控制采水继电器 开")
                        waterCollector:TurnOn()
                        Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, true)
                    end
                end
            end
        },
        [274] =     -- 系统时间
        {
            number = 3,	-- Register number

            read = function()
                local offsetAddress = 274 - setting.externalInterface.modbus.holdRegAddr

                local modbusManager = ModbusManager.Instance()
                modbusManager:SetHexTime(RegisterType.Hold, offsetAddress, os.time() )
            end,

            write = function()
                local offsetAddress = 274 - setting.externalInterface.modbus.holdRegAddr

                if not config.system.solidifyMeaParamFromModbus then
                    local modbusManager = ModbusManager.Instance()
                    modbusManager:SetHexSysTime(RegisterType.Hold, offsetAddress)
                end
            end,
        },
    }
}
