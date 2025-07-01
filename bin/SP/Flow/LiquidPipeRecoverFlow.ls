--[[
 * @brief 液路恢复流程。
--]]

LiquidPipeRecoverFlow = Flow:new
{
}

function LiquidPipeRecoverFlow:new(o)
        o = o or {}
        setmetatable(o, self)
        self.__index = self
	
		o.detectTime = os.time()
	
        return o
end


function LiquidPipeRecoverFlow:OnStart()
	local runStatus = Helper.Status.SetStatus(setting.runStatus.liquidPipeRecover)
	StatusManager.Instance():SetStatus(runStatus)
end

function LiquidPipeRecoverFlow:OnProcess()
	--电机驱动初始化
	op.IMotorControl:MotorDriverReinit()
	App.Sleep(2000)
	--机械臂复位，安全移动至排液处
	op:ManipulatorReset()
	op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)

	--注射器复位排空
	op:SyringeReset()

	--清洗针管路
	op:CleanNeedlePipe(setting.clean.cleanVolume)

	--移动至清洗槽底部
	op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)

	--清洗针管路
	op:CleanNeedlePipe(setting.clean.clearVolume)

end	

function LiquidPipeRecoverFlow:OnStop()
	self.result = "液路恢复结束"
	log:info("液路恢复结束")
	log:debug("液路恢复时间 = "..os.time() - self.detectTime )
end
