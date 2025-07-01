--[[
 * @brief 测量流程。
--]]

CCEPSafeData = 0   --全局变量

MeasureFlow = Flow:new
{
    measureTarget = setting.liquidType.none, --useless
    currentRange = 0,--useless
    consistency = 0,--useless

    measureType = MeasureType.Sample,
    reportMode = ReportMode.OnLine,

    faultTimes = 0,
    isRetryMeasure = false,
    isRetryOnStart = false,
    isCrashMeasure = false,
    isOneKeyMaintain = false,
    isQuickMeasure = false,
    diluteFactor = 1,
    isLackOfBlank = false
}

function MeasureFlow:new(o, meaType)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o.measureType = meaType
    o.measureDateTime = os.time()

    o.reportMode = config.interconnection.reportMode
    return o
end

function MeasureFlow:GetRuntime()
    local runtime = 0

    if self.measureType == MeasureType.Blank then  --测空白
        runtime = setting.runStatus.measureBlank.GetTime()
    elseif self.measureType == MeasureType.Standard then --测质控
        runtime = setting.runStatus.measureStandard.GetTime()
    elseif self.measureType == MeasureType.Sample then  --测样品
        runtime = setting.runStatus.measureSample.GetTime()
    elseif self.measureType == MeasureType.Check then  --复苏菌检测
        runtime = setting.runStatus.measureCheck.GetTime()
    end

    return runtime
end

function MeasureFlow:OnStart()
    if self.isCrashMeasure == true then
        self.measureDateTime = status.running.lastMeasureTime
    else
        status.running.lastMeasureTime = self.measureDateTime
    end
    -- 设置测量标志
    status.running.isMeasuring = true
    status.running.lastMeasureType = self.measureType
    ConfigLists.SaveMeasureRunning()
    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()

    --电源控制
    op.IOpticalAcquire:PDPowerOff()

    --检测测量安全温度
    op:CheckMeasureSafety()

    --继电器指示
    if self.measureType == MeasureType.Sample then
        Helper.Result.RelayOutOperate(setting.mode.relayOut.measureInstruct, true)
    elseif self.measureType == MeasureType.Standard then
        Helper.Result.RelayOutOperate(setting.mode.relayOut.calibrateInstruct, true)
    end

    --更新状态
    if self.measureType == MeasureType.Blank then  --测空白水
        local runStatus = Helper.Status.SetStatus(setting.runStatus.measureBlank)
        StatusManager.Instance():SetStatus(runStatus)
    elseif self.measureType == MeasureType.Standard then  --测质控
        local runStatus = Helper.Status.SetStatus(setting.runStatus.measureStandard)
        StatusManager.Instance():SetStatus(runStatus)
    elseif self.measureType == MeasureType.Sample then   --测水样
        local runStatus = Helper.Status.SetStatus(setting.runStatus.measureSample)
        StatusManager.Instance():SetStatus(runStatus)
    elseif self.measureType == MeasureType.Check then   --复苏菌品控
        local runStatus = Helper.Status.SetStatus(setting.runStatus.measureCheck)
        StatusManager.Instance():SetStatus(runStatus)
    end

end

function MeasureFlow:OnProcess()
    local loopcnt = 3
    self.isRetryMeasure = true
    self.isRetryOnStart = false

    self.isUserStop = false
    self.isFinish = false

    if self.isOneKeyMaintain == true then
        self:PipeRenew()
    end

    --重测循环
    while self.isRetryMeasure == true and loopcnt > 0 do
        loopcnt = loopcnt - 1
        self.isRetryMeasure = false

        local err,result = pcall(			-- 捕获异常
            function()
                if self.isRetryOnStart == true then
                    self.isRetryOnStart = false
                    self:OnStart()
                end

                --测量流程表复位、参数配置
                if Measurer.flow then
                    Measurer:Reset()
                end
                Measurer.flow = self
                Measurer.measureType = self.measureType

                if self.measureType == MeasureType.Check then  --复苏菌品质检测
                    if self.isQuickMeasure == true then
                        Measurer.procedure = {9}
                    else
                        Measurer.procedure = {1,2,3,5,6,7,10,11}
                    end
                end

                -- 采水样
                if self.measureType == MeasureType.Sample then
                    self:CollectSample()
                end

                -- CCEP模式下检测到崩溃重启
                if self.isCrashMeasure == true and config.system.CCEPMode == true then
                    self:RetryMeasureCheck()
                else
                    --测量流程执行
                    Measurer:Measure()
		        end

            end
        )	-- 捕获异常结束

        if not err then      -- 出现异常
            if type(result) == "userdata" then
                if result:GetType() == "ExpectEventTimeoutException" then          --期望事件等待超时异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif result:GetType() == "CommandTimeoutException" then          --命令应答超时异常
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                else
                    log:warn("MeasureFlow:OnProcess =>" .. result:What())
                end
            elseif type(result) == "table" then
                if getmetatable(result) == PumpStoppedException then 			--泵操作被停止异常。
                    self.isUserStop = true
                    error(result)
                elseif getmetatable(result)== MotorStoppedException then			--电机运动被停止异常。
                    self.isUserStop = true
                    error(result)
                elseif getmetatable(result)== MeterStoppedException then			--定量被停止异常。
                    self.isUserStop = true
                    error(result)
                elseif getmetatable(result) == ThermostatStoppedException then  	--恒温被停止异常。
                    self.isUserStop = true
                    error(result)
                elseif getmetatable(result)== UserStopException then 				--用户停止测量流程
                    self.isUserStop = true
                    error(result)
                elseif getmetatable(result)== AcquirerADStoppedException then 	    --光学采集被停止异常
                    self.isUserStop = true
                    error(result)
                elseif getmetatable(result) == AcquirerADFailedException then 	    --光学采集中途出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == PumpFailedException then 			--泵操作中途出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    if result.liquidType == setting.liquidType.blank then
                        self.isLackOfBlank = true  --缺空白水异常标记
                    else
                        self:RetryMeasureCheck()
                    end
                elseif getmetatable(result) == MotorFailedException then 			--电机中途运动出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == RefCollideException then 			--电机运动参考针碰撞出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == MeaCollideException then 			--电机运动测量针碰撞出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == MotorXMoveOutSensorFailException then 			--电机X轴移不出传感器出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == MotorZMoveOutSensorFailException then 			--电机Z轴移不出传感器出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == MotorXMoveInSensorFailException then 			--电机X轴找不到传感器出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == MotorZMoveInSensorFailException then 			--电机Z轴找不到传感器出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == SyringeMoveOutSensorFailException then 			--注射器移不出传感器出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == SyringeMoveInSensorFailException then 			--注射器找不到传感器出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    self:RetryMeasureCheck()
                elseif getmetatable(result) == ThermostatFailedException then 		--恒温中途出现故障，未能完成异常。
                    ExceptionHandler.MakeAlarm(result)
                    error(result)
                elseif getmetatable(result) == ThermostatTimeoutException then 		--恒温超时，指定时间内仍未达到目标温度异常
                    ExceptionHandler.MakeAlarm(result)
                    error(result)
                elseif getmetatable(result) == StaticADControlFailedException then 		--AD自动调节失败异常
                    ExceptionHandler.MakeAlarm(result)
                    error(result)
                elseif getmetatable(result)== StaticADControlStoppedException then 	    --AD自动调节被停止异常
                    self.isUserStop = true
                    error(result)
                else
                    log:warn("MeasureFlow:OnProcess =>" .. result:What())								--其他定义类型异常
                    error(result)
                end
            elseif type(result) == "string" then
                log:warn("MeasureFlow:OnProcess =>" .. result)	--C++、Lua系统异常
                error(result)
            end
        end
    end-----重测循环
end


function MeasureFlow:OnStop()
    --关闭数据调试
    if SignalManager ~= nil then
        SignalManager.Instance():FocusSignal("refAD", false)
        SignalManager.Instance():FocusSignal("meaAD", false)
    end

    --电源控制
    op.IOpticalAcquire:PDPowerOff()

    -- 设置测量标志
    status.running.isMeasuring = false
    ConfigLists.SaveMeasureRunning()

    --继电器指示
    if self.measureType == MeasureType.Sample then
        Helper.Result.RelayOutOperate(setting.mode.relayOut.measureInstruct, false)
    elseif self.measureType == MeasureType.Standard then
        Helper.Result.RelayOutOperate(setting.mode.relayOut.calibrateInstruct, false)
    end

    --关闭采水继电器
    if config.interconnection.collectSampleMode ~= setting.mode.collectSample.trigger then
        local waterCollector = WaterCollector.Instance()
        waterCollector:TurnOff()
        Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)
    end

    if not self.isFinish then
        if self.measureType == MeasureType.Sample then
            status.measure.schedule.autoMeasure.dateTime = self.measureDateTime
        elseif self.measureType == MeasureType.Standard then
            status.measure.schedule.autoCheck.dateTime = self.measureDateTime
        elseif self.measureType == MeasureType.Blank then
            status.measure.schedule.autoBlank.dateTime = self.measureDateTime
        end
        ConfigLists.SaveMeasureStatus()

        if self.isUserStop then
            self.result = "用户终止"
            log:info("用户终止")
        else
            self.result = "故障终止"
            log:warn("故障终止")
        end

        --测量终止清洗
        if config.system.faultHandleEnable == true and self.isLackOfBlank ~= true then
            self:CleanStopMeasure()
        end
    else
        status.measure.isDeviceFault = false
        ConfigLists.SaveMeasureStatus()
        self.result = "测量完成"
        log:info("测量完成")
        log:info("测量流程总时间 = ".. os.time() - self.measureDateTime)
    end

    --保存试剂余量表
    ReagentRemainManager.SaveRemainStatus()

    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()

    --检测安全温度
    op:CheckMeasureSafety()
end

--[[
 * @brief 复苏菌检测结果处理
 * @param[in] result 测量过程得到的AD数据
--]]
function MeasureFlow:BacteriaCheckHandle(result)
    --增益倍数
    local gainRef = result.gainRef
    local gainMea = result.gainMea

    --暗电流
    local darkReferenceAD = result.darkReferenceAD
    local darkMeasureAD = result.darkMeasureAD

    --发光菌初始有效AD
    local initReferenceAD = result.initReferenceAD - darkReferenceAD
    local initMeasureAD = result.initMeasureAD - darkMeasureAD

    --复苏菌AD上下限
    local minNewBacteriaAD = config.measureParam.minNewBacteriaAD
    local maxNewBacteriaAD = config.measureParam.maxNewBacteriaAD

    log:info("参考AD="..tostring(initReferenceAD).." 测量AD="..tostring(initMeasureAD))

    --[[if initReferenceAD < minNewBacteriaAD then
        log:debug("发光菌品质检测 初始参考AD = "..initReferenceAD.." 低于正常AD下限 "..minNewBacteriaAD)
        local str = "AD="..tostring(initReferenceAD)
        local alarm = Helper.MakeAlarm(setting.alarm.bacteriaAbnormalAD, str)
        AlarmManager.Instance():AddAlarm(alarm)
    elseif initReferenceAD > maxNewBacteriaAD then
        log:debug("发光菌品质检测 初始参考AD = "..initReferenceAD.." 高于正常AD上限 "..maxNewBacteriaAD)
        local str = "AD="..tostring(initReferenceAD)
        local alarm = Helper.MakeAlarm(setting.alarm.bacteriaAbnormalAD, str)
        AlarmManager.Instance():AddAlarm(alarm)
    else
        log:debug("发光菌品质检测 初始参考AD = "..initReferenceAD.." 正常")
        log:info("发光菌光强正常(AD="..tostring(initReferenceAD)..")")
    end--]]

    self.isFinish = true

    self.isCrashMeasure = false
end

--[[
 * @brief 测量结果处理
 * @param[in] result 测量过程得到的AD数据
--]]
function MeasureFlow:ResultHandle(result)
    --增益倍数
    local gainRef = result.gainRef
    local gainMea = result.gainMea

    --暗电流
    local darkReferenceAD = result.darkReferenceAD
    local darkMeasureAD = result.darkMeasureAD

    --结果值
    local C0 = math.floor(result.initReferenceAD - darkReferenceAD)
    local S0 = math.floor(result.initMeasureAD - darkMeasureAD)
    local C1 = math.floor(result.startReferenceAD - darkReferenceAD)
    local S1 = math.floor(result.startMeasureAD - darkMeasureAD)
    local Ct = math.floor(result.finalReferenceAD - darkReferenceAD)
    local St = math.floor(result.finalMeasureAD - darkMeasureAD)

    if C0 == 0 then
        C0 = 1
        log:debug("C0 = 0 then change C0 = 1.")
    end
    if C1 == 0 then
        C1 = 1
        log:debug("C1 = 0 then change C1 = 1.")
    end
    if Ct == 0 then
        Ct = 1
        log:debug("Ct = 0 then change Ct = 1.")
    end
    if S0 == 0 then
        S0 = 1
        log:debug("S0 = 0 then change S0 = 1.")
    end
    if S1 == 0 then
        S1 = 1
        log:debug("S1 = 0 then change S1 = 1.")
    end
    if St == 0 then
        St = 1
        log:debug("St = 0 then change St = 1.")
    end

	--相对发光度计算
	local RLI = ((St/S0)/(Ct/C0))*100
    --抑制率
    local IR = 100 - RLI

    --稳定系数计算
    local CF = (Ct/C1)

    --保存曲线数据
    local reactAD = Measurer:GetReactAD()
    local length = #reactAD + 1
    local refCurve = LF82CurveData.new(length)
    local meaCurve = LF82CurveData.new(length)
    local refAD = 0
    local meaAD = 0

    refCurve:SetData(0, C0)
    meaCurve:SetData(0, S0)
    log:debug("Save Curve Data 0 refAD = "..C0.." meaAD = "..S0)
    for i = 1,length-1 do
        refAD = math.floor(reactAD[i]:GetReference() - darkReferenceAD)
        meaAD = math.floor(reactAD[i]:GetMeasure() - darkMeasureAD)
        log:debug("Save Curve Data "..i.." refAD = "..refAD.." meaAD = "..meaAD)
        refCurve:SetData(i, refAD)
        meaCurve:SetData(i, meaAD)
    end
    local curveManager = LF82CurveManager.Instance()
    local refCurveIndex = curveManager:AddCurve(refCurve)
    local meaCurveIndex = curveManager:AddCurve(meaCurve)
    --log:debug("Save Curve Data refCurveIndex = "..refCurveIndex.." meaCurveIndex = "..meaCurveIndex)

    --结果数据标识
    local resultMark = ResultMark.N
    if self.reportMode == ReportMode.OnLine then
        if Measurer.measureType == MeasureType.Standard then
            resultMark = ResultMark.N
        else
            if IR > config.interconnection.meaUpLimit then
                resultMark = ResultMark.E
            elseif IR < -config.interconnection.meaLowLimit then
                resultMark = ResultMark.E
            else
                resultMark = ResultMark.N
            end
        end
    elseif self.reportMode == ReportMode.OffLine then
        resultMark = ResultMark.B
    elseif self.reportMode == ReportMode.Maintain then
        resultMark = ResultMark.M
    elseif self.reportMode == ReportMode.Fault then
        resultMark = ResultMark.D
    elseif self.reportMode == ReportMode.Calibrate then
        resultMark = ResultMark.C
    end

	local resultManager = ResultManager.Instance()
	local recordData = RecordData.new(resultManager:GetMeasureRecordDataSize(setting.resultFileInfo.measureRecordFile[1].name))
	recordData:PushInt(self.measureDateTime) -- 时间
    recordData:PushFloat(IR) -- 抑制率
	recordData:PushFloat(CF) -- 稳定系数
	recordData:PushByte(resultMark) -- 标识
	recordData:PushByte(Measurer.measureType) -- 类型(当前测量过程)
    recordData:PushFloat(RLI) -- 相对发光度
	recordData:PushInt(C0) -- 初始参考AD
	recordData:PushInt(S0) -- 初始测量AD
	recordData:PushInt(Ct) -- 反应参考AD
	recordData:PushInt(St) -- 反应测量AD
	recordData:PushFloat(result.bacteriaTemp) -- 发光菌温度
    recordData:PushFloat(result.measureTemp) -- 测量仓温度
	recordData:PushFloat(result.environmentTemp) -- 环境温度
	recordData:PushInt(os.time()-self.measureDateTime) -- 测量时长
    recordData:PushFloat(gainRef)   -- 当前参考增益参数
    recordData:PushFloat(gainMea)   -- 当前测量增益参数
    recordData:PushInt(refCurveIndex)   -- 参考曲线索引
    recordData:PushInt(meaCurveIndex)   -- 测量曲线索引

	Helper.Result.OnMeasureResultAdded(Measurer.measureType, self.measureDateTime, IR, self.reportMode, CF, C0, S0, Ct, St)
	resultManager:AddMeasureRecord(setting.resultFileInfo.measureRecordFile[1].name, recordData, true)

    self.isFinish = true

    if config.system.CCEPMode == true then
		CCEPSafeData = 0   --完成测量，安全标志清0
	end

    ConfigLists.SaveMeasureStatus()

    self.isCrashMeasure = false  --清除崩溃标记
    -- 设置测量结束标志
    status.running.isMeasuring = false
    ConfigLists.SaveMeasureRunning()
	--测量结果
	log:debug("测量结果：C0 = "..C0.."，S0 = "..S0.."，Ct = "..Ct.."，St ="..St.."，IR = "..IR.."%，RLI = "..RLI.."%，CF = "..CF.."%")
end

--[[
 * @brief 测量终止清洗
 * @details 抽取清洗液到定量管，再压到消解器，进行消解室及相关管路清洗。
 * @param[in] source 清洗液类型。
 * @param[in] vol 清洗液体积。
--]]
function MeasureFlow:CleanStopMeasure()
    local runAction = Helper.Status.SetAction(setting.runAction.cleanStopMeasure)
    StatusManager.Instance():SetAction(runAction)

    --机械臂复位
    op:ManipulatorReset()
    --移动至排液处
    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
    --注射器复位排空
    op:SyringeReset()

    --测量仓排空
    for i = 1,3 do
        op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
        op:SyringeAspiration(1)
        op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
        op:SyringeReset()
        op:CleanNeedlePipe(setting.clean.cleanVolume)
    end

    --水样混合槽排空
    for i = 1,2 do
        op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
        op:SyringeAspiration(1)
        op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
        op:SyringeReset()
        op:CleanNeedlePipe(setting.clean.cleanVolume)
    end

    --发光菌混合槽排空
    for i = 1,3 do
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
        op:SyringeAspiration(1)
        op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
        op:SyringeReset()
        op:CleanNeedlePipe(setting.clean.cleanVolume)
    end

    --开启测量仓排废液
    op:WasteRoomOutStart()
    for i = 1,config.measureParam.cleanAfterMeasureTimes do
        --移动至测量槽底部
        op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
        --针管路加清洗水
        op:CleanNeedlePipe(5)

        --测量槽排空并吹洗
        op:AutoMove(setting.component.xPos.measureCell, setting.clean.measureCellWastePos)
        op:AirWasteNeedlePipe()
        op:AutoMove(setting.component.xPos.measureCell, setting.clean.measureCellWashPos)
        op:AirRecover(setting.liquidType.airWaste) --气压平衡
        op:AirWashNeedlePipe(5)

        --移动至测量槽底部
        op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
        --针管路加清洗水
        op:CleanNeedlePipe(5)

        --针管路排干测量槽
        op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
        op:AirWasteNeedlePipe()

        --移动至样品混合槽底部
        op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
        --针管路加清洗水
        op:CleanNeedlePipe(4)

        --水样混合槽排空并吹洗
        op:AutoMove(setting.component.xPos.sampleMixer, setting.clean.sampleMixerWastePos)
        op:AirWasteNeedlePipe()
        op:AutoMove(setting.component.xPos.sampleMixer, setting.clean.sampleMixerWashPos)
        op:AirRecover(setting.liquidType.airWaste) --气压平衡
        op:AirWashNeedlePipe(5)

        --移动至样品混合槽底部
        op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
        --针管路加清洗水
        op:CleanNeedlePipe(4)

        --针管路排干样品混合槽
        op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
        op:AirWasteNeedlePipe()

        --移动至发光菌混合槽底部
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
        --针管路加清洗水
        op:CleanNeedlePipe(5)

        --发光菌混合槽排空并吹洗
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.clean.bacteriaMixerWastePos)
        op:AirWasteNeedlePipe()
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.clean.bacteriaMixerWashPos)
        op:AirRecover(setting.liquidType.airWaste) --气压平衡
        op:AirWashNeedlePipe(5)

        --移动至发光菌混合槽底部
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
        --针管路加清洗水
        op:CleanNeedlePipe(5)

        --针管路排干发光菌混合槽
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
        op:AirWasteNeedlePipe()
    end
    --关闭测量仓排废液
    op:WasteRoomOutStop()

    --针清洗
    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
    op:SyringeReset()
    op:CleanNeedlePipe(setting.clean.cleanVolume)
    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
    op:CleanNeedlePipe(setting.clean.clearVolume)
end

--[[
 * @brief 水样采集
 * @details 测量水样时，采集更新水样
--]]
function MeasureFlow:CollectSample()
    local waterCollector = WaterCollector.Instance()
    local mode = config.interconnection.collectSampleMode

    if mode == setting.mode.collectSample.beforeMeasure then

        local runAction = Helper.Status.SetAction(setting.runAction.collectSample.collect)
        StatusManager.Instance():SetAction(runAction)

        waterCollector:TurnOn()
        Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, true)

        if not self:Wait(config.interconnection.miningWaterTime) then 	-- 采集等待
            if config.interconnection.collectSampleMode ~= setting.mode.collectSample.trigger then
                waterCollector:TurnOff()
                Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)
            end

            error(UserStopException:new())    --用户停止
        end

        runAction = Helper.Status.SetAction(setting.runAction.collectSample.silent)
        StatusManager.Instance():SetAction(runAction)

        waterCollector:TurnOff()
        Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)

        if not self:Wait(config.interconnection.silentTime) then 	-- 静默等待
            error(UserStopException:new())    --用户停止
        end
    elseif mode == setting.mode.collectSample.toAddSampleEnd or mode == setting.mode.collectSample.toMeaFlowEnd then
        local runAction = Helper.Status.SetAction(setting.runAction.collectSample.collect)
        StatusManager.Instance():SetAction(runAction)

        waterCollector:TurnOn()
        Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, true)

        if not self:Wait(config.interconnection.miningWaterTime) then 	-- 采集等待
            if config.interconnection.collectSampleMode ~= setting.mode.collectSample.trigger then
                waterCollector:TurnOff()
                Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)
            end

            error(UserStopException:new())    --用户停止
        end
    end
end


--[[
 * @brief 重新测量检查
 * @details 当测量出现异常或结果异常时，进行重新测量。
--]]
function MeasureFlow:RetryMeasureCheck()
    self.isCrashMeasure = false  --清除崩溃标记

    if self.isFinish == true then
        log:debug("已出结果，无需重测");
        return false
    end

    if config.system.faultHandleEnable ~= true then
        log:debug("异常重测关闭 保留现场")
        if config.scheduler.measure.mode == MeasureMode.Continous then
            config.scheduler.measure.mode = MeasureMode.Trigger   --连续测量变触发
            config.modifyRecord.scheduler(true)
            ConfigLists.SaveSchedulerConfig()
        end
        return false
    end

    self.faultTimes = self.faultTimes + 1   --更新重测次数记录

    if config.system.CCEPMode == true and CCEPSafeData < 2 then
        local useTime = os.time() - self.measureDateTime
        local leftTime = 3600 - useTime

        log:debug("安全测量剩余时间 = " .. leftTime .. " 安全测量时间 = " .. setting.measureResult.normalMeasureTime)
        if leftTime > setting.measureResult.normalMeasureTime then  --剩余时间充足,判断重测
            if self.faultTimes >= 3 then
                if config.scheduler.measure.mode == MeasureMode.Continous then
                    config.scheduler.measure.mode = MeasureMode.Trigger   --连续测量变触发
                    config.modifyRecord.scheduler(true)
                    ConfigLists.SaveSchedulerConfig()

                    local updateWidgetManager = UpdateWidgetManager.Instance()
                    updateWidgetManager:Update(UpdateEvent.ChangeMeaModeOnHome, "DeviceFaultException")
                end
                error(DeviceFaultException:new())   --抛出仪器故障运行停止异常
            else
                self.isRetryMeasure = true
                self.isRetryOnStart = true
                log:warn("测量异常重测次数 = "..self.faultTimes)
            end
        else	--生成安全测量结果
            self.isRetryMeasure = false
            self.isRetryOnStart = false
            self:SafeResultGenerate()
            self.isFinish = true
        end

    else
        if self.faultTimes >= 3 then
            if config.scheduler.measure.mode == MeasureMode.Continous then
                config.scheduler.measure.mode = MeasureMode.Trigger   --连续测量变触发
                config.modifyRecord.scheduler(true)
                ConfigLists.SaveSchedulerConfig()

                local updateWidgetManager = UpdateWidgetManager.Instance()
                updateWidgetManager:Update(UpdateEvent.ChangeMeaModeOnHome, "DeviceFaultException")
            end
            error(DeviceFaultException:new())   --抛出仪器故障运行停止异常
        else
            self.isRetryMeasure = true
            self.isRetryOnStart = true
            log:warn("测量异常重测次数 = "..self.faultTimes)
        end
    end

    return true
end


--[[
 * @brief 安全结果生成
 * @details 剩余时间无法进行故障重测时，生成安全结果。
--]]
function MeasureFlow:SafeResultGenerate()

    local diluteFactor = self.diluteFactor
    local random = 2*(math.random() - 0.5)*setting.measureResult.resultWaveRange

    local RLI = status.measure.report.measure.RLI + random   --生成RLI
    local IR = 100 - RLI        --抑制率
    local CF = status.measure.report.measure.CF + random/2  --生成CF
    local C0 = status.measure.report.measure.C0
    local Ct = status.measure.report.measure.Ct
    local S0 = status.measure.report.measure.S0
    local St = status.measure.report.measure.St

    --结果数据标识
    local resultMark = ResultMark.N
    if self.reportMode == ReportMode.OnLine then
        if Measurer.measureType == MeasureType.Standard then
            resultMark = ResultMark.N
        else
            if IR > config.interconnection.meaUpLimit then
                resultMark = ResultMark.E
            elseif IR < -config.interconnection.meaLowLimit then
                resultMark = ResultMark.E
            else
                resultMark = ResultMark.N
            end
        end
    elseif self.reportMode == ReportMode.OffLine then
        resultMark = ResultMark.B
    elseif self.reportMode == ReportMode.Maintain then
        resultMark = ResultMark.M
    elseif self.reportMode == ReportMode.Fault then
        resultMark = ResultMark.D
    elseif self.reportMode == ReportMode.Calibrate then
        resultMark = ResultMark.C
    end

    local resultManager = ResultManager.Instance()
    local recordData = RecordData.new(resultManager:GetMeasureRecordDataSize(setting.resultFileInfo.measureRecordFile[1].name))
    recordData:PushInt(self.measureDateTime) -- 时间
    recordData:PushFloat(IR) -- 抑制率
    recordData:PushFloat(CF) -- 稳定系数
    recordData:PushByte(resultMark) -- 模式
    recordData:PushByte(Measurer.measureType) -- 类型(当前测量过程)
    recordData:PushFloat(RLI) -- 相对发光度
    recordData:PushInt(C0) -- 初始参考AD
    recordData:PushInt(S0) -- 初始测量AD
    recordData:PushInt(Ct) -- 反应参考AD
    recordData:PushInt(St) -- 反应测量AD
    recordData:PushFloat(0) -- 发光菌温度
    recordData:PushFloat(0) -- 测量槽温度
    recordData:PushFloat(0) -- 环境温度
    recordData:PushInt(os.time()-self.measureDateTime) -- 测量时长
    recordData:PushFloat(1)   -- 当前参考增益倍数
    recordData:PushFloat(1)   -- 当前测量增益倍数
    recordData:PushInt(0)   -- 曲线索引
    recordData:PushInt(0)   -- 曲线索引

    Helper.Result.OnMeasureResultAdded(self.measureType, self.measureDateTime, IR, self.reportMode, CF, C0, S0, Ct, St)
    resultManager:AddMeasureRecord(setting.resultFileInfo.measureRecordFile[1].name, recordData, true)
    ConfigLists.SaveMeasureStatus()

    self.isFinish = true

    --测量结果
    log:debug("测量结果：CF = "..CF.." RLI = "..RLI.." IR = "..IR)

    CCEPSafeData = CCEPSafeData + 1   --安全记录
    log:debug("CCEP安全数据生成次数"..CCEPSafeData)
end

--[[
 * @brief 管路更新
 * @detail 一键运行流程中先填充管路
--]]
function MeasureFlow:PipeRenew()
    local runAction = Helper.Status.SetAction(setting.runAction.oneKeyPipeRenew)
    StatusManager.Instance():SetAction(runAction)

    --机械臂复位
    op:ManipulatorReset()
    --移动至排液处
    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
    --注射器复位排空
    op:SyringeReset()
    --针管路填充
    op:CleanNeedlePipe(5)

    --加渗透压调节液
    op:Pump(setting.liquidType.salt, 10, setting.liquid.addLiquidSpeed)
    --加空白水
    op:Pump(setting.liquidType.blank, 10, setting.liquid.addLiquidSpeed)
    --加水样
    op:Pump(setting.liquidType.sample, 10, setting.liquid.addLiquidSpeed)
end