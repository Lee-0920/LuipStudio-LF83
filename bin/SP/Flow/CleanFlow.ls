--[[
 * @brief 清洗流程。
--]]
CleanFlow = Flow:new
{
}

function CleanFlow:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o.cleanDateTime = os.time()

    return o
end

function CleanFlow:GetRuntime()
    local runtime = setting.runStatus.cleanAll.GetTime()
    return runtime
end

function CleanFlow:OnStart()
    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()
    --检测安全温度
    op:CheckMeasureSafety()
    --继电器指示
    Helper.Result.RelayOutOperate(setting.mode.relayOut.cleanInstruct, true)

    --设置运行状态
    local runStatus = Helper.Status.SetStatus(setting.runStatus.cleanAll)
    StatusManager.Instance():SetStatus(runStatus)
end

function CleanFlow:OnProcess()
    self.isUserStop = false
    self.isFinish = false

    --清洗流程执行
    local err,result = pcall
    (
        function()
                return self:CleanAll()
        end
    )
    if not err then      -- 出现异常
        if type(result) == "table" then
            if getmetatable(result) == PumpStoppedException then 			--泵操作被停止异常。
                self.isUserStop = true
                error(result)
            elseif getmetatable(result)== MotorStoppedException then			--电机运动被停止异常。
                self.isUserStop = true
                error(result)
            elseif getmetatable(result) == ThermostatStoppedException then  	--恒温被停止异常。
                self.isUserStop = true
                error(result)
            elseif getmetatable(result)== UserStopException then 				--用户停止测量流程
                self.isUserStop = true
                error(result)
	        else
	    	    error(result)
            end
        else
            error(result)
        end
    end

    self.isFinish = true
end

function CleanFlow:OnStop()
    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()
    --检测安全温度
    op:CheckMeasureSafety()
    --继电器指示
    Helper.Result.RelayOutOperate(setting.mode.relayOut.cleanInstruct, false)

    status.measure.schedule.autoClean.dateTime = self.cleanDateTime
    ConfigLists.SaveMeasureStatus()

    if not self.isFinish then
        if self.isUserStop then
            self.result = "用户终止"
            log:info("用户终止")
        else
            self.result = "故障终止"
            log:warn("故障终止")
        end
    else
        self.result = "清洗完成"
        log:info("清洗完成")
        log:info("清洗流程总时间 = "..os.time()-self.cleanDateTime)
    end

    --保存试剂余量表
    ReagentRemainManager.SaveRemainStatus()
end

--[[
 * @brief 清洗所有管路
--]]
function CleanFlow:CleanAll()
    local runAction = Helper.Status.SetAction(setting.runAction.cleanAll.clean)
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

    --开启测量槽排废液
    op:WasteRoomOutStart()
    for i = 1,config.measureParam.cleanBeforeMeasureTimes do
        for j = 1,1 do
            --移动至测量槽底部
            op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
            --针管路加清洗水
            op:CleanNeedlePipe(5)

            --测量槽排空并吹洗
            op:AutoMove(setting.component.xPos.measureCell, setting.clean.measureCellWastePos)
            op:AirWasteNeedlePipe(setting.liquidType.airWaste, 4)
            op:AutoMove(setting.component.xPos.measureCell, setting.clean.measureCellWashPos)
            op:AirRecover(setting.liquidType.airWaste) --气压平衡
            op:AirWashNeedlePipe(5)
        end

        --移动至测量槽底部
		op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 3)
        --针管路加清洗水
        op:CleanNeedlePipe(3)
        --等200ms
        App.Sleep(200)

        --针管路排干测量槽
        op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
        op:AirWasteNeedlePipe()

        for j = 1,1 do
            --移动至样品混合槽底部
            op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 3)
            --针管路加清洗水
            op:CleanNeedlePipe(2.5)

            --水样混合槽排空并吹洗
            op:AutoMove(setting.component.xPos.sampleMixer, setting.clean.sampleMixerWastePos)
            op:AirWasteNeedlePipe(setting.liquidType.airWaste, 4)
            op:AutoMove(setting.component.xPos.sampleMixer, setting.clean.sampleMixerWashPos)
            op:AirRecover(setting.liquidType.airWaste) --气压平衡
            op:AirWashNeedlePipe(5)
        end

        --移动至样品混合槽底部
        op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 3)
        --针管路加清洗水
        op:CleanNeedlePipe(2.5)
        --等200ms
        App.Sleep(200)

        --针管路排干样品混合槽
        op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
        op:AirWasteNeedlePipe()

        for j = 1,1 do
            --移动至发光菌混合槽底部
            op:AutoMove(setting.component.xPos.bacteriaMixer, config.componentParam.bacteriaMixer.bottom - 3)
            --针管路加清洗水
            op:CleanNeedlePipe(3.5)

            --发光菌混合槽排空并吹洗
            op:AutoMove(setting.component.xPos.bacteriaMixer, setting.clean.bacteriaMixerWastePos)
            op:AirWasteNeedlePipe(setting.liquidType.airWaste, 4)
            op:AutoMove(setting.component.xPos.bacteriaMixer, setting.clean.bacteriaMixerWashPos)
            op:AirRecover(setting.liquidType.airWaste) --气压平衡
            op:AirWashNeedlePipe(5)
        end

        --移动至发光菌混合槽底部
        op:AutoMove(setting.component.xPos.bacteriaMixer, config.componentParam.bacteriaMixer.bottom - 3)
        --针管路加清洗水
        op:CleanNeedlePipe(3.5)
        --等200ms
        App.Sleep(200)

        --针管路排干发光菌混合槽
        op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
        op:AirWasteNeedlePipe()
        op:AirWasteNeedlePipe()
    end
    App.Sleep(1000)
    --关闭测量仓排废液
    op:WasteRoomOutStop()

    --针清洗
    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
    op:SyringeReset()
    op:CleanNeedlePipe(setting.clean.cleanVolume * 3)
    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
    op:CleanNeedlePipe(setting.clean.clearVolume)

    self.isFinish = true

    return true
end
