LiquidOperateFlow = Flow:new
{
    target = setting.liquidType.blank,
    dir = RollDirection.Suck,
    type = 0,
    volume = 1,
}

function LiquidOperateFlow:new(o, target, dir, type, vol)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o.target = target
    o.dir = dir
    o.type = type
    o.volume = vol

    return o
end

function LiquidOperateFlow:GetRuntime()
    return 0
end

function LiquidOperateFlow:OnStart()
    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()
    --检测安全温度
    op:CheckMeasureSafety()

end

function LiquidOperateFlow:OnProcess()

    self.isUserStop = false
    self.isFinish = false

    self.dateTime = os.time()

    local runStatus = Helper.Status.SetStatus(setting.runStatus.liquidOperate)
    StatusManager.Instance():SetStatus(runStatus)

    if self.target == setting.liquidType.blank then
        if self.dir == RollDirection.Suck then
            local runAction = Helper.Status.SetAction(setting.runAction.suckFromBlank)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.sample then
        if self.dir == RollDirection.Suck then
            local runAction = Helper.Status.SetAction(setting.runAction.suckFromSample)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.salt then
        if self.dir == RollDirection.Suck then
            local runAction = Helper.Status.SetAction(setting.runAction.suckFromSalt)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.wash then
        if self.dir == RollDirection.Suck then
            local runAction = Helper.Status.SetAction(setting.runAction.suckFromWash)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.airWash then
        if self.dir == RollDirection.Drain then
            local runAction = Helper.Status.SetAction(setting.runAction.airToNeedlePipe)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.airWaste then
        if self.dir == RollDirection.Drain then
            local runAction = Helper.Status.SetAction(setting.runAction.drainToWastePool)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.wasteOut then
        if self.dir == RollDirection.Drain then
            local runAction = Helper.Status.SetAction(setting.runAction.drainToWasteOut)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.wasteRoom then
        if self.dir == RollDirection.Drain then
            local runAction = Helper.Status.SetAction(setting.runAction.drainMeasureCellWaste)
            StatusManager.Instance():SetAction(runAction)
        end

    elseif self.target == setting.liquidType.airReagent then
        if self.type == setting.component.xPos.measureCell then
            local runAction = Helper.Status.SetAction(setting.runAction.syringeDrainMeasureRoom)
            StatusManager.Instance():SetAction(runAction)
        elseif self.type == setting.component.xPos.sampleMixer then
            local runAction = Helper.Status.SetAction(setting.runAction.syringeDrainSampleMixer)
            StatusManager.Instance():SetAction(runAction)
        elseif self.type == setting.component.xPos.bacteriaMixer then
            local runAction = Helper.Status.SetAction(setting.runAction.syringeDrainBacteriaMixer)
            StatusManager.Instance():SetAction(runAction)
        else
            if self.dir == RollDirection.Suck then
                local runAction = Helper.Status.SetAction(setting.runAction.aspirationAirReagent)
                StatusManager.Instance():SetAction(runAction)
            elseif self.dir == RollDirection.Drain then
                local runAction = Helper.Status.SetAction(setting.runAction.dischargeAirReagent)
                StatusManager.Instance():SetAction(runAction)
            end
        end
        --[[if self.dir == RollDirection.Suck then
            local runAction = Helper.Status.SetAction(setting.runAction.aspirationAirReagent)
            StatusManager.Instance():SetAction(runAction)
        elseif self.dir == RollDirection.Drain then
            local runAction = Helper.Status.SetAction(setting.runAction.dischargeAirReagent)
            StatusManager.Instance():SetAction(runAction)
        end--]]
    end

    local err,result = pcall
    (
        function()
            local speed = 1
            if self.target == setting.liquidType.airReagent then
                if self.type == setting.component.xPos.measureCell then
                    op:ManipulatorReset()
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
                    op:SyringeAspiration(self.volume)
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                elseif self.type == setting.component.xPos.sampleMixer then
                    op:ManipulatorReset()
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
                    op:SyringeAspiration(self.volume)
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                elseif self.type == setting.component.xPos.bacteriaMixer then
                    op:ManipulatorReset()
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
                    op:SyringeAspiration(self.volume)
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                else
                    if self.dir == RollDirection.Suck then
                        op:SyringeAspiration(self.volume)
                    elseif self.dir == RollDirection.Drain then
                        op:SyringeAccurateDischarge(self.volume)
                    end
                end
            else
                if self.dir == RollDirection.Suck then
                    op:Pump(self.target, self.volume, speed)
                elseif self.dir == RollDirection.Drain then
                    op:Drain(self.target, self.volume, speed)
                end
            end

        end
    )
    if not err then      -- 出现异常
        if type(result) == "table" then
            if getmetatable(result) == PumpStoppedException then 			--泵操作被停止异常。
                self.isUserStop = true
                error(result)
            elseif getmetatable(result)== MeterStoppedException then			--定量被停止异常。
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

function LiquidOperateFlow:OnStop()

    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()
    --检测安全温度
    op:CheckMeasureSafety()
    --保存试剂余量表
    ReagentRemainManager.SaveRemainStatus()

    if not self.isFinish then
        if self.isUserStop then
            self.result = "用户终止"
            log:info("用户终止")
        else
            self.result = "故障终止"
            log:warn("故障终止")
        end
    else
        self.result = "管路操作结束"
        log:info("管路操作结束")
        local str = "管路操作流程总时间 = " .. tostring(os.time() - self.dateTime)
        log:debug(str)
    end
end
