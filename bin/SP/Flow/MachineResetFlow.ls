--[[
 * @brief 机械复位流程。
--]]

MachineResetFlow = Flow:new
{
}

function MachineResetFlow:new(o)
        o = o or {}
        setmetatable(o, self)
        self.__index = self
	
		o.detectTime = os.time()
	
        return o
end


function MachineResetFlow:OnStart()
	local runStatus = Helper.Status.SetStatus(setting.runStatus.machineReset)
	StatusManager.Instance():SetStatus(runStatus)
end

function MachineResetFlow:OnProcess()
	--电机驱动初始化
	op.IMotorControl:MotorDriverReinit()
	App.Sleep(2000)
	--机械臂复位，安全移动至排液处
	op:ManipulatorReset()
	op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)

	--注射器复位排空
	op:SyringeReset()

	--Z轴复位
	op:MotorZReset()

	--X轴复位
	op:MotorXReset()

end	

function MachineResetFlow:OnStop()
	self.result = "机械复位结束"
	log:info("机械复位结束")
	log:debug("机械复位时间 = "..os.time() - self.detectTime )
end
