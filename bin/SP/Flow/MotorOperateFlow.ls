MotorOperateFlow = Flow:new
{
    dir = setting.component.xPos.zero,
    step = 0,
}

function MotorOperateFlow:new(o, dst, step)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o.dst = dst
    o.step = step

    return o
end

function MotorOperateFlow:GetRuntime()
    return 0
end

function MotorOperateFlow:OnStart()
    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()
    --检测安全温度
    op:CheckMeasureSafety()

end

function MotorOperateFlow:OnProcess()

    self.isUserStop = false
    self.isFinish = false

    self.dateTime = os.time()

    local runStatus = Helper.Status.SetStatus(setting.runStatus.motorOperate)
    StatusManager.Instance():SetStatus(runStatus)

    if self.dst == setting.component.xPos.zero then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToZero)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.measureCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToMeasureCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.sampleMixer then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToSampleMixer)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.bacteriaMixer then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToBacteriaMixer)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.bacteriaCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToBacteriaCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.blankCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToBlankCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.sampleCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToSampleCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.standardCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToStandardCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.saltCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToSaltCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.cleanCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToCleanCell)
        StatusManager.Instance():SetAction(runAction)
    elseif self.dst == setting.component.xPos.wasteCell then
        local runAction = Helper.Status.SetAction(setting.runAction.motorOperate.moveToWasteCell)
        StatusManager.Instance():SetAction(runAction)
    end


    local err,result = pcall
    (
        function()
            if self.dst == setting.component.xPos.zero and self.step == 0 then
                op:ManipulatorReset()
            else
                op:AutoMove(self.dst, self.step)
            end
        end
    )
    if not err then      -- 出现异常
        if type(result) == "table" then
            if getmetatable(result) == PumpStoppedException then 			--泵操作被停止异常。
                self.isUserStop = true
                error(result)
            elseif getmetatable(result)== MotorStoppedException then			--定量被停止异常。
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

function MotorOperateFlow:OnStop()

    -- 初始化下位机
    dc:GetIDeviceStatus():Initialize()
    --检测安全温度
    op:CheckMeasureSafety()

    if not self.isFinish then
        if self.isUserStop then
            self.result = "用户终止"
            log:info("用户终止")
        else
            self.result = "故障终止"
            log:warn("故障终止")
        end
    else
        self.result = "电机操作结束"
        log:info("电机操作结束")
        local str = "电机操作流程总时间 = " .. tostring(os.time() - self.dateTime)
        log:debug(str)
    end
end
