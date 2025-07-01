--[[
 * @brief 机械臂移动模式
--]]
ManipulatorMoveMode =
{
    Auto = 0,
    Safe = 1,
}

--[[
 * @brief 各个驱动器操作业务。
 * @details 对液路器、温控器、信号采集器业务操作的功能进行封装。
--]]

Operator =
{
    ISolenoidValve = 0,
    IPeristalticPump = 0,
    ITemperatureControl = 0,
    IOpticalAcquire = 0,
    IMotorControl = 0,
    isThermostat = false,
    isAcquiring = false,
    isStaticADControl = false,
    SPEED = 1,
    PRECISE = 0.000001
}

function Operator:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.__metatable = "Operator"

    self.ISolenoidValve = dc:GetISolenoidValve()
    self.IPeristalticPump = dc:GetIPeristalticPump()
    self.ITemperatureControl = dc:GetITemperatureControl()
    self.IOpticalAcquire = dc:GetIOpticalAcquire()
    self.IMotorControl = dc:GetIMotorControl()

    return o
end

--[[
 * @brief 停止所有驱动控制器操作。
--]]
function Operator:Stop()
    -- 停止定量
    if self.isMetering then
        self.IOpticalMeter:StopMeter()
        self.isMetering= false
    end

   -- 停止泵
    for i,pump in pairs(pumps) do
        if pump.isRunning then
            pump:Stop()
        end
    end

	-- 停止信号采集
    if self.isAcquiring then
        self.IOpticalAcquire:StopAcquirer()
        self.isAcquiring= false
    end

    -- 停止AD调节
    if self.isStaticADControl then
        self.IOpticalAcquire:StopStaticADControl()
        self.isStaticADControl= false
    end

    -- 停止电机
    motorX:Stop()
    motorZ:Stop()

    -- 停止恒温控制 --只有温控恢复状态下才允许停止恒温器
    local runStatus = StatusManager.Instance():GetStatus()
    local statusName = runStatus:GetName()
    local runAction = StatusManager.Instance():GetAction()
    local actionName = runStatus:GetName()
    if statusName == "TemperatureControlRecover" and actionName == "ThermostatsMeasureControl" then
        if thermostats.measure.isRunning then
            thermostats.measure:Stop(false)  --不等待停止事件
        end
    elseif statusName == "TemperatureControlRecover" and actionName == "ThermostatsBacteriaControl" then
        if thermostats.bacteria.isRunning then
            thermostats.bacteria:Stop(false)  --不等待停止事件
        end
    end
end

--[[
 * @brief 机械臂运动
 * @param[in] positionX 水平位置
 * @param[in] positionZ 垂直位置
--]]
function Operator:AutoMove(positionX, positionZ)
    local posX = nil
    local posZ = nil
    local nameX = "unknow"
    local nameZ = "unknow"

    if type(positionX) == "table" then
        posX = config.componentParam[positionX.name].pos
        nameX = positionX.name
    elseif type(positionX) == "number" then
        posX = positionX
        nameX = tostring(positionX)
    end

    if type(positionZ) == "table" then
        if type(config.componentParam[positionX.name]) == "table" then
            posZ = config.componentParam[positionX.name][positionZ.name]
            nameZ = positionZ.name
        else
            posZ = config.componentParam["zero"][positionZ.name]
            nameZ = positionZ.name
        end
    elseif type(positionZ) == "number" then
        posZ = positionZ
        nameZ = tostring(positionZ)
    end

    if posX == nil or posZ == nil then
        log:warn("机械臂运动目标位置未知")
        error(MotorMoveParamException:new())
    end

    log:debug("{AutoMove} posX = "..nameX.." posZ = "..nameZ)

    local failedCnt = 0
    local failedRetry = 3

    while true do
        local err,result = pcall(function()
            op:ManipulatorAutoMove(posX, posZ)

            failedCnt = 0
        end)

        if not err then      -- 出现异常
            if type(result) == "userdata" then
                if result:GetType() == "ExpectEventTimeoutException" then          --事件超时异常。
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        op:ManipulatorReset() --机械臂复位
                        error (MotorFailedException:new())
                    end
                else
                    error(result)
                end
            elseif type(result) == "table" then
                if getmetatable(result) == RefCollideException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (RefCollideException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                elseif getmetatable(result) == MeaCollideException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (MeaCollideException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                elseif getmetatable(result) == MotorXMoveOutSensorFailException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (MotorXMoveOutSensorFailException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                elseif getmetatable(result) == MotorZMoveOutSensorFailException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (MotorZMoveOutSensorFailException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                elseif getmetatable(result) == MotorXMoveInSensorFailException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (MotorXMoveInSensorFailException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                elseif getmetatable(result) == MotorZMoveInSensorFailException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (MotorZMoveInSensorFailException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                elseif getmetatable(result) == MotorFailedException then
                    failedCnt = failedCnt +1
                    log:warn(result:What());
                    if  failedCnt >= failedRetry then
                        error (MotorFailedException:new())
                    else
                        op:ManipulatorReset() --机械臂复位
                    end
                else
                    error(result)
                end
            else
                error(result)
            end
        else    --函数调用正常
            break
        end
    end

end

--[[
 * @brief 机械臂复位
--]]
function Operator:ManipulatorReset()
    --Z轴复位
    self:MotorZReset()
    --X轴复位
    self:MotorXReset()
end

--[[
 * @brief 机械臂安全运动
 * @param[in] posX 水平位置坐标
 * @param[in] posZ 垂直位置坐标
--]]
function Operator:ManipulatorSafeMove(posX, posZ)
    --log:debug("{Manipulator Safe Move} posX = "..posX.." posZ = "..posZ)

    if posX > setting.motor.maxStepsX or posX < setting.motor.minStepsX then
        log:debug("X轴运动距离超出范围")
        return false
    end
    if posZ > setting.motor.maxStepsZ or posZ < setting.motor.minStepsZ then
        log:debug("Z轴运动距离超出范围")
        return false
    end

    --Z轴复位
    self:MotorZReset()

    --X轴复位
    self:MotorXReset()

    --X轴移动(绝对模式)
    self:MotorXMove(posX, MotorMoveMode.Absolute)

    --Z轴移动(绝对模式)
    self:MotorZMove(posZ, MotorMoveMode.Absolute)
end


--[[
 * @brief 机械臂自动运动
 * @param[in] posX 水平位置坐标
 * @param[in] posZ 垂直位置坐标
--]]
function Operator:ManipulatorAutoMove(posX, posZ)
    local currentX = motorX:GetCurrentSteps()
    local currentZ = motorZ:GetCurrentSteps()

    if posX > setting.motor.maxStepsX or posX < setting.motor.minStepsX then
        log:debug("X轴运动距离超出范围")
        return false
    end
    if posZ > setting.motor.maxStepsZ or posZ < setting.motor.minStepsZ then
        log:debug("Z轴运动距离超出范围")
        return false
    end

    log:debug("{Manipulator Auto Move} localPosX = "..motorX.localPos.." currentX = "..currentX..
                " localPosZ = "..motorZ.localPos.." currentZ = "..currentZ.." posX = "..posX.." posZ = "..posZ)

    --Z轴位置不确定或X轴需移动且Z轴太低时复位
    if math.abs(motorZ.localPos - currentZ) > 0.0001 or
    ((math.abs(motorX.localPos - posX) > 0.0001 or math.abs(currentX - posX) > 0.0001)  and currentZ > 0) then
        self:MotorZReset()
    end

    --X轴位置不确定时X轴复位
    if math.abs(motorX.localPos - currentX) > 0.0001 then
        --X轴复位
        self:MotorXReset()
        --X轴移动(绝对模式)
        self:MotorXMove(posX, MotorMoveMode.Absolute)
    else
        --X轴移动(相对模式)
        self:MotorXMove(posX - currentX, MotorMoveMode.Relative)
    end

    currentZ = motorZ:GetCurrentSteps()
    --Z轴位置不确定时Z轴复位
    if math.abs(motorZ.localPos - currentZ) > 0.0001 then
        self:MotorZReset()
        --Z轴移动(绝对模式)
        self:MotorZMove(posZ, MotorMoveMode.Absolute)
    else
        --Z轴移动(相对模式)
        self:MotorZMove(posZ - currentZ, MotorMoveMode.Relative)
    end
end


--[[
 * @brief 电机X轴水平运动
 * @param[in] pos 水平位置
 * @param[in] mode 运动模式
--]]
function Operator:MotorXMove(pos, mode)
    local ret =false
    local timeout = 90000

    --log:debug("{MotorX Move} pos = ".. pos .. " mode = " .. mode)

    -- X轴电机运动
    local err,result = pcall(function() return motorX:Move(pos, mode) end)
    if not err then      -- 出现异常
        motorX:Stop()
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待电机运动结果事件
    err,result = pcall(function() return motorX:ExpectResult(timeout) end)
    if not err then -- 出现异常
        motorX:Stop()
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
            --更新电机当前位置
            if mode == MotorMoveMode.Absolute or mode == MotorMoveMode.Safe then
                motorX.localPos = pos
            else
                motorX.localPos = motorX.localPos + pos
            end
            if motorX.localPos >= setting.motor.maxStepsX then
                motorX.localPos = setting.motor.maxStepsX
            elseif motorX.localPos <= setting.motor.minStepsX then
                motorX.localPos = setting.motor.minStepsX
            end
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_0 then
            error (RefCollideException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_1 then
            error (MeaCollideException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_X then
            error (MotorXMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_Z then
            error (MotorZMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_X then
            error (MotorXMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_Z then
            error (MotorZMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 电机Z轴垂直运动
 * @param[in] pos 垂直位置
 * @param[in] mode 运动模式
--]]
function Operator:MotorZMove(pos, mode)
    local ret =false
    local timeout = 90000

    --log:debug("{MotorZ Move} pos = ".. pos .. " mode = " .. mode)

    -- Z轴电机运动
    local err,result = pcall(function() return motorZ:Move(pos, mode) end)
    if not err then      -- 出现异常
        motorZ:Stop()
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待电机运动结果事件
    err,result = pcall(function() return motorZ:ExpectResult(timeout) end)
    if not err then -- 出现异常
        motorZ:Stop()
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
            --更新电机当前位置
            if mode == MotorMoveMode.Absolute or mode == MotorMoveMode.Safe then
                motorZ.localPos = pos
            else
                motorZ.localPos = motorZ.localPos + pos
            end
            if motorZ.localPos >= setting.motor.maxStepsZ then
                motorZ.localPos = setting.motor.maxStepsZ
            elseif motorZ.localPos <= setting.motor.minStepsZ then
                motorZ.localPos = setting.motor.minStepsZ
            end
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_0 then
            error (RefCollideException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_1 then
            error (MeaCollideException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_X then
            error (MotorXMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_Z then
            error (MotorZMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_X then
            error (MotorXMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_Z then
            error (MotorZMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 电机X轴复位
--]]
function Operator:MotorXReset()
    local ret =false
    local timeout = 90000

    log:debug("{MotorX Reset}")

    -- X轴电机运动
    local err,result = pcall(function() return motorX:Reset() end)
    if not err then      -- 出现异常
        motorX:Stop()
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待电机运动结果事件
    err,result = pcall(function() return motorX:ExpectResult(timeout) end)
    if not err then -- 出现异常
        motorX:Stop()
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
            --更新电机当前位置
            motorX.localPos = 0
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_0 then
            error (RefCollideException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_1 then
            error (MeaCollideException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_X then
            error (MotorXMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_Z then
            error (MotorZMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_X then
            error (MotorXMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_Z then
            error (MotorZMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 电机Z轴复位
--]]
function Operator:MotorZReset()
    local ret =false
    local timeout = 90000

    log:debug("{MotorZ Reset}")

    -- Z轴电机运动
    local err,result = pcall(function() return motorZ:Reset() end)
    if not err then      -- 出现异常
        motorZ:Stop()
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待电机运动结果事件
    err,result = pcall(function() return motorZ:ExpectResult(timeout) end)
    if not err then -- 出现异常
        motorZ:Stop()
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
            --更新电机当前位置
            motorZ.localPos = 0
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_0 then
            error (RefCollideException:new())
        elseif result:GetResult() == MotorOperateResult.Collsion_1 then
            error (MeaCollideException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_X then
            error (MotorXMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_Z then
            error (MotorZMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_X then
            error (MotorXMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_Z then
            error (MotorZMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 注射器吸液体操作。(带回程:先排空复位再吸)
 * @param[in] volume 溶液体积。
--]]
function Operator:SyringeSafeAspiration(volume)
    local speed = setting.liquid.aspirationSpeed
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.syringeTimeoutFactor)   -- 获取操作结果事件超时时间
    local pump = pumps[1]  --  1号泵为注射器

    if volume > setting.liquid.syringeMaxVolume then
        volume = setting.liquid.syringeMaxVolume
        log:debug("Syringe Aspiration volume more than max volume ,then change volume to  ".. setting.liquid.syringeMaxVolume)
    elseif volume <= 0 then
        log:debug("Invalid volume.")
        return
    end
    log:debug("{Syringe Safe Aspiration} volume = ".. volume .. " speed = " .. speed)

    --注射器先复位
    self:SyringeReset()

    --注射器进行吸操作
    local err,result = pcall(function() return pump:Start(RollDirection.Suck, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                                                      -- 停止泵
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_SYR then
            error (SyringeMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_SYR then
            error (SyringeMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 注射器吸液体操作。(不带回程:直接吸)
 * @param[in] volume 溶液体积。
--]]
function Operator:SyringeAspiration(volume)
    local speed = setting.liquid.aspirationSpeed
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.syringeTimeoutFactor)   -- 获取操作结果事件超时时间
    local pump = pumps[1]  --  1号泵为注射器

    if volume > setting.liquid.syringeMaxVolume then
        volume = setting.liquid.syringeMaxVolume
        log:debug("Syringe Aspiration volume more than max volume ,then change volume to  ".. setting.liquid.syringeMaxVolume)
    elseif volume <= 0 then
        log:debug("Invalid volume.")
        return
    end
    log:debug("{Syringe Aspiration} volume = ".. volume .. " speed = " .. speed)

    --注射器进行吸操作
    local err,result = pcall(function() return pump:Start(RollDirection.Suck, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                                                      -- 停止泵
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_SYR then
            error (SyringeMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_SYR then
            error (SyringeMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 注射器排液体操作。
 * @param[in] volume 溶液体积。
--]]
function Operator:SyringeDischarge(volume)
    local speed = setting.liquid.dischargeSpeed
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.syringeTimeoutFactor)   -- 获取操作结果事件超时时间
    local pump = pumps[1]  --  1号泵为注射器

    if volume > setting.liquid.syringeMaxVolume then
        volume = setting.liquid.syringeMaxVolume
        log:debug("Syringe Discharge volume more than max volume ,then change volume to  ".. setting.liquid.syringeMaxVolume)
    elseif volume <= 0 then
        log:debug("Invalid volume.")
        return
    end
    log:debug("{Syringe Discharge} volume = ".. volume .. " speed = " .. speed)

    --注射器进行排操作
    local err,result = pcall(function() return pump:Start(RollDirection.Drain, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                                                      -- 停止泵
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_SYR then
            error (SyringeMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_SYR then
            error (SyringeMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 注射器排液体操作(消除回程误差)。
 * @param[in] volume 溶液体积。
--]]
function Operator:SyringeAccurateDischarge(volume)
    local speed = setting.liquid.dischargeSpeed
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.syringeTimeoutFactor)   -- 获取操作结果事件超时时间
    local pump = pumps[1]  --  1号泵为注射器

    if volume > setting.liquid.syringeMaxVolume then
        volume = setting.liquid.syringeMaxVolume
        log:debug("Syringe Accurate Discharge volume more than max volume ,then change volume to  ".. setting.liquid.syringeMaxVolume)
    elseif volume <= 0 then
        log:debug("Invalid volume.")
        return false
    end
    log:debug("Syringe Accurate Discharge volume = ".. volume .. " speed = " .. speed)

    local realVol = volume + setting.liquid.dischargeErrorVolume
    if realVol > 1 then
        realVol = 1
    end

    log:debug("realVol = ".. volume)

    --注射器进行排操作
    local err,result = pcall(function() return pump:Start(RollDirection.Drain, realVol, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    log:debug("wait event timeout = "..timeout)

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        log:debug("syr expect error")
        pump:Stop()                                                                      -- 停止泵
        error(result)
    else    --函数调用正常
        log:debug("syr expect result ="..result:GetResult())
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_SYR then
            error (SyringeMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_SYR then
            error (SyringeMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end
end

--[[
 * @brief 注射器复位操作。
 * @param[in] volume 溶液体积。
 * @param[in] speed 泵速，0为默认速度。
--]]
function Operator:SyringeReset()
    local speed = setting.liquid.dischargeSpeed
    local ret =false
    local volume = 1
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.syringeTimeoutFactor)   -- 获取操作结果事件超时时间
    local pump = pumps[1]  --  1号泵为注射器

    log:debug("{Syringe Reset}")

    --注射器进行排操作
    local err,result = pcall(function() return pump:Start(RollDirection.Drain, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                                                      -- 停止泵
        error(result)
    else    --函数调用正常
        if result:GetResult()  == MotorOperateResult.Finished then
            ret = true
        elseif result:GetResult() == MotorOperateResult.Failed then
            error (MotorFailedException:new())
        elseif result:GetResult()  == MotorOperateResult.Stopped then
            error (MotorStoppedException:new())
        elseif result:GetResult() == MotorOperateResult.MoveOutSensorFail_SYR then
            error (SyringeMoveOutSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.MoveInSensorFail_SYR then
            error (SyringeMoveInSensorFailException:new())
        elseif result:GetResult() == MotorOperateResult.DriverError then
            error (DriverErrorException:new())
        else
            error (MotorFailedException:new())
        end
    end

end

--[[
 * @brief 注射器吸源液体到目标点
 * @param[in] src 源地址。
 * @param[in] dst 目的地址。
 * @param[in] volume 溶液体积。
  * @param[in] extraVol 过量体积。
--]]
function Operator:SyringeAddLiquid(src, dst, volume, extraVol)
    
	local bottomSrc = nil
    local bottomDst = nil

	if extraVol == nil then
        extraVol = 0
    end

    if type(src) == "table" then
        bottomSrc = config.componentParam[src.name].bottom - 1
    end

    if type(dst) == "table" then
        bottomDst = config.componentParam[dst.name].bottom - 3
    end

    if volume > 0 and volume <= 1 then
        --移动到源地址
        self:AutoMove(src, bottomSrc)

        --消除回程误差多吸液体
        local suckVol = volume + extraVol

        --[[local extraVol = setting.liquid.aspirationExtraVol
        if volume <= 0.1 then
            extraVol = setting.liquid.aspirationExtraVol/2
        end--]]

        --吸液
        self:SyringeAspiration(suckVol)

        --移动到目的地址
        self:AutoMove(dst, bottomDst)
        --排液
        self:SyringeAccurateDischarge(volume)
    else
        log:debug("Syringe add liquid volume invalid.")
    end
end

--[[
 * @brief 清洗针管路
 * @details 抽取清洗水冲洗参考/测量针管路。
 * @param[in] volume 清洗水体积。
--]]
function Operator:CleanNeedlePipe(volume)
    if volume == nil then
        volume = 1
    end

    local source = setting.liquidType.wash
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.pumpTimeoutFactor)   -- 获取操作结果事件超时时间
    local map = ValveMap.new(source.valve)
    local pump = pumps[source.pump + 1]  --  +1 当泵号为0时，从pumps[1]取泵对象
    local speed = 1  --mL/s

    log:debug("{Clean Needle Pipe } volume = "..volume)

    -- 打开相关液路的阀门
    local err,result = pcall(function() return self.ISolenoidValve:SetValveMap(map) end)
    if not err then -- 出现异常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                   --关闭所有阀门
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    --延迟200ms
    App.Sleep(200)

    --打开蠕动泵进行吸操作
    err,result = pcall(function() return pump:Start(RollDirection.Suck, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                             -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                          -- 停止泵
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                  --关闭所有阀门
        error(result)
    else    --函数调用正常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)               --关闭所有阀门
        if result:GetResult() == PumpOperateResult.Failed then
            error (PumpFailedException:new())
        elseif result:GetResult()  == PumpOperateResult.Stopped then
            error (PumpStoppedException:new())
        elseif result:GetResult()  == PumpOperateResult.Finished then
            ret = true;
        end
    end

    map = nil
    --更新溶液余量
    ReagentRemainManager.ReduceReagentRemain( status.remain.blank, volume)
    return ret
end

--[[
 * @brief 针管路充气
 * @details 通过针管路吹气。
--]]
function Operator:AirWashNeedlePipe(volume, src)
    if volume == nil then
        volume = 1
    end

    local source = setting.liquidType.airWash
    if  src == setting.liquidType.airWashRef or src == setting.liquidType.airWashMea then
        source = src
    end

    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.pumpTimeoutFactor)   -- 获取操作结果事件超时时间
    local map = ValveMap.new(source.valve)
    local pump = pumps[source.pump + 1]  --  +1 当泵号为0时，从pumps[1]取泵对象
    local speed = 1  --mL/s

    log:debug("{Air Wash Needle Pipe} " .. source.name)

    -- 打开相关液路的阀门
    local err,result = pcall(function() return self.ISolenoidValve:SetValveMap(map) end)
    if not err then -- 出现异常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                   --关闭所有阀门
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    --延迟200ms
    App.Sleep(200)

    --打开空气泵进行吸操作
    err,result = pcall(function() return pump:Start(RollDirection.Suck, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                             -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                          -- 停止泵
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                  --关闭所有阀门
        error(result)
    else    --函数调用正常
        if result:GetResult() == PumpOperateResult.Failed then
            error (PumpFailedException:new())
        elseif result:GetResult()  == PumpOperateResult.Stopped then
            error (PumpStoppedException:new())
        elseif result:GetResult()  == PumpOperateResult.Finished then
            ret = true;
        end
    end

    --等待1秒管路气压恢复
    App.Sleep(1000)

    --关闭所有阀门
    map:SetData(0)
    self.ISolenoidValve:SetValveMap(map)
    map = nil

    return ret
end

--[[
 * @brief 气压平衡
 * @details 开启目标液路阀一段时间后关闭。
 * @param[in] src 液路
--]]
function Operator:AirRecover(src)
    local source = src
    if src == nil then
        source = setting.liquidType.airWaste
    end
    local map = ValveMap.new(source.valve)

    -- 打开相关液路的阀门
    local err,result = pcall(function() return self.ISolenoidValve:SetValveMap(map) end)
    if not err then -- 出现异常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                   --关闭所有阀门
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    --等待1秒管路气压恢复
    App.Sleep(1000)

    --关闭所有阀门
    map:SetData(0)
    self.ISolenoidValve:SetValveMap(map)
end

--[[
 * @brief 针管路抽废液
 * @details 空气泵通过针管路抽废液。
 * @param src 针管路类型
 * @param vol 体积
--]]
function Operator:AirWasteNeedlePipe(src, vol)
    local source = setting.liquidType.airWaste
    if src == setting.liquidType.airWasteRef or src == setting.liquidType.airWasteMea then
        source = src
    end
    local ret =false
    local volume = setting.liquid.wasteAirVolume
    if vol ~= nil and tonumber(vol) > 0 then
        volume = vol
    end
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.pumpTimeoutFactor)   -- 获取操作结果事件超时时间
    local map = ValveMap.new(source.valve)
    local pump = pumps[source.pump + 1]  --  +1 当泵号为0时，从pumps[1]取泵对象
    local speed = 1  --mL/s

    log:debug("{Air Waste Needle Pipe} " .. source.name)

    -- 打开排气阀抽真空
    local airMap = ValveMap.new(setting.liquidType.airOut.valve)
    local err,result = pcall(function() return self.ISolenoidValve:SetValveMap(airMap) end)
    if not err then -- 出现异常
        airMap:SetData(0)
        self.ISolenoidValve:SetValveMap(airMap)                   --关闭所有阀门
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    --先开泵 形成负压
    local err,result = pcall(function() return pump:Start(RollDirection.Suck, 0.5, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                             -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                          -- 停止泵
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                  --关闭所有阀门
        error(result)
    else    --函数调用正常
        if result:GetResult() == PumpOperateResult.Failed then
            error (PumpFailedException:new())
        elseif result:GetResult()  == PumpOperateResult.Stopped then
            error (PumpStoppedException:new())
        elseif result:GetResult()  == PumpOperateResult.Finished then
            ret = true;
        end
    end

    -- 打开相关液路的阀门
    local err,result = pcall(function() return self.ISolenoidValve:SetValveMap(map) end)
    if not err then -- 出现异常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                   --关闭所有阀门
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    --延迟200ms
    App.Sleep(200)

    --打开空气泵进行吸操作
    err,result = pcall(function() return pump:Start(RollDirection.Suck, volume, speed) end)
    if not err then      -- 出现异常
        pump:Stop()                                             -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)
    if not err then -- 出现异常
        pump:Stop()                                          -- 停止泵
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                  --关闭所有阀门
        error(result)
    else    --函数调用正常
        if result:GetResult() == PumpOperateResult.Failed then
            error (PumpFailedException:new())
        elseif result:GetResult()  == PumpOperateResult.Stopped then
            error (PumpStoppedException:new())
        elseif result:GetResult()  == PumpOperateResult.Finished then
            ret = true;
        end
    end

    --等待1秒管路气压恢复
    App.Sleep(1000)

    --关闭所有阀门
    map:SetData(0)
    self.ISolenoidValve:SetValveMap(map)

    --排空废液池
    self:DrainToWaste(0)

    --关闭所有阀门
    map:SetData(0)
    self.ISolenoidValve:SetValveMap(map)
    map = nil

    return ret
end

--[[
 * @brief 空白水检查
--]]
function Operator:BlankCheck()
    local errorTimes = 0
    if self.IMotorControl:GetSensorStatus(5) == true then  --IDLE未遮挡
        --加空白水
        op:Pump(setting.liquidType.blank, 5, setting.liquid.addLiquidSpeed)
        if self.IMotorControl:GetSensorStatus(5) == true then --IDLE未遮挡
            log:debug("缺少空白水")
            error (PumpFailedException:new{liquidType = setting.liquidType.blank, dir = RollDirection.Suck,})
        end
    else
        --log:debug("空白水充足")
    end
end

--[[
 * @brief 泵抽取液体操作。
 * @param[in] source 管道类型。
 * @param[in] volume 溶液体积。
  * @param[in] speed 泵速，0为默认速度。
--]]
function Operator:Pump(source, volume, speed)
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.pumpTimeoutFactor)   -- 获取操作结果事件超时时间
    local map = ValveMap.new(source.valve)
    local pump = pumps[source.pump + 1]  --  +1 当泵号为0时，从pumps[1]取泵对象

    log:debug("{Pump} source = " .. source.name .. ", volume = " .. volume .. ", speed = " .. speed)

    -- 打开相关液路的阀门
    err,result = pcall(function() return self.ISolenoidValve:SetValveMap(map) end)

    if not err then -- 出现异常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                                     --关闭所有阀门
        pump:Stop()                                                                  -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            pump:Stop()
            return false
        end
    end

    --打开蠕动泵进行吸操作
    local err,result = pcall(function() return pump:Start(RollDirection.Suck, volume, speed) end)

    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)

    if not err then -- 出现异常
        pump:Stop()                                                                      -- 停止泵
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                                     --关闭所有阀门
        error(result)
    else    --函数调用正常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                                     --关闭所有阀门

        if result:GetResult() == PumpOperateResult.Failed then
            error (PumpFailedException:new{liquidType = source, dir = RollDirection.Suck,})
        elseif result:GetResult()  == PumpOperateResult.Stopped then
            error (PumpStoppedException:new{liquidType = source, dir = RollDirection.Suck,})
        elseif result:GetResult()  == PumpOperateResult.Finished then
            ret = true;
        end
    end

    map = nil
    --更新溶液余量
    if source == setting.liquidType.wash or source == setting.liquidType.blank then
        ReagentRemainManager.ReduceReagentRemain( status.remain.blank, volume)
    elseif source == setting.liquidType.salt then
        ReagentRemainManager.ReduceReagentRemain( status.remain.salt, volume)
    elseif source == setting.liquidType.sample then
        ReagentRemainManager.ReduceReagentRemain( status.remain.waste, volume)
    end
    return ret
end

--[[
 * @brief 排液操作。
 * @param[in] dest 管道类型。
 * @param[in] volume 溶液体积。
  * @param[in] speed 泵速，0为默认速度。
--]]
function Operator:Drain(dest, volume, speed)
    local ret =false
    local timeout = 1000 + math.floor(volume * setting.liquid.meterLimit.pumpTimeoutFactor)  -- 获取操作结果事件超时时间
    local map = ValveMap.new(dest.valve)
    local pump = pumps[dest.pump + 1]  --  +1 当泵号为0时，从pumps[1]取泵对象

    log:debug("{Drain} dest = " .. dest.name .. ", volume = " .. volume .. ", speed = " .. speed)

    -- 打开相关液路的阀门
    local err,result = pcall(function() return self.ISolenoidValve:SetValveMap(map) end)

    if not err then -- 出现异常
        error(result)
    else    --函数调用正常
         if not result then
            return false
        end
    end

    --打开蠕动泵进行吸操作
    err,result = pcall(function() return pump:Start(RollDirection.Drain, volume, speed) end)

    if not err then      -- 出现异常
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                                     --关闭所有阀门
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    -- 等待泵操作结果事件
    err,result = pcall(function() return pump:ExpectResult(timeout) end)

    if not err then -- 出现异常
        pump:Stop()                                                                      -- 停止泵
        App.Sleep(1000);                                                        --等待1秒气压恢复
        map:SetData(0)
        self.ISolenoidValve:SetValveMap(map)                                     --关闭所有阀门
        error(result)
    else    --函数调用正常

        if result:GetResult() == PumpOperateResult.Failed then
            error (PumpFailedException:new{liquidType = dest, dir = RollDirection.Drain,})
        elseif result:GetResult()  == PumpOperateResult.Stopped then
            error (PumpStoppedException:new{liquidType = dest, dir = RollDirection.Drain,})
        elseif result:GetResult()  == PumpOperateResult.Finished then
            ret = true;
        end
    end

    --等待1秒气压恢复
    App.Sleep(1000);

    --关闭所有阀门
    map:SetData(0)
    self.ISolenoidValve:SetValveMap(map)

    map = nil
    return ret
end

--[[
 * @brief 开启发光菌存储槽控速搅拌
--]]
function Operator:SafeStirOn(number)
    local pump = pumps[setting.liquidType.stir.pump + 1]  --  获取搅拌泵对象
    local factor = self.IPeristalticPump:GetPumpFactor(setting.liquidType.stir.pump)
    local volume = 10000
    local speed = number*factor
    local ret =false
    --log:debug("stir on")
    --打开蠕动泵进行吸操作
    local err,result = pcall(function() return pump:Start(RollDirection.Drain, volume, speed) end)

    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    return ret
end

--[[
 * @brief 开启发光菌存储槽搅拌
--]]
function Operator:StirOn()
    local pump = pumps[setting.liquidType.stir.pump + 1]  --  获取搅拌泵对象
    local volume = 10000
    local speed = config.measureParam.stirSpeed/1000
    local ret =false
    --log:debug("stir on")
    --打开蠕动泵进行吸操作
    local err,result = pcall(function() return pump:Start(RollDirection.Drain, volume, speed) end)

    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        end
    end

    return ret
end

--[[
 * @brief 关闭发光菌存储槽搅拌
--]]
function Operator:StirOff()
    local pump = pumps[setting.liquidType.stir.pump + 1]  --  获取搅拌泵对象
    local timeout = 3000;
    --log:debug("stir off")
    if pump:GetStatus() == PumpStatus.Busy then
        pump:Stop()
        -- 等待泵操作结果事件
        local err,result = pcall(function() return pump:ExpectResult(timeout) end)
        if not err then      -- 出现异常
            error(result)
        end
    else
        pump:Stop()
    end
end

--[[
 * @brief 空气混合
 * @details 向消解器空气，使得消解器溶液充分反应。
 * @param[in] xPos 空气混合处水平位置
 * @param[in] zPosAir 空气混合处深度
 * @param[in] zPosLiquid 空气混合处深度
 * @param[in] volume 空气体积
 * @param[in] times 混合次数
--]]
function Operator:AirMix(xPos, zPosAir, zPosLiquid, volume, times)
    --移动至目标位置顶部
    op:ManipulatorSafeMove(xPos, setting.motor.topZ)

    --抽空针管路
    op:AirWasteNeedlePipe()
    --吸空气
    --op:SyringeAspiration(0.05)
    --抽空针管路
    op:AirWasteNeedlePipe()

    --移动至目标深度
    op:ManipulatorAutoMove(xPos, zPosLiquid)

    --混合
    for i = 1,times do
        op:SyringeAspiration(volume)
        op:ManipulatorAutoMove(xPos, zPosAir)
        --op:AirWashNeedlePipe(1)
        op:ManipulatorAutoMove(xPos, zPosLiquid)
        op:SyringeDischarge(volume)
    end

    --移动至目标位置顶部
    op:ManipulatorSafeMove(xPos, setting.motor.topZ)
end

--[[
 * @brief 泵操作从定量管排液到废液桶。
 * @param[in] vol 溶液体积。
--]]
function Operator:DrainToWaste(vol)
    if vol == nil then
        vol = 0
    end
    self:Drain(setting.liquidType.wasteOut,
        vol + setting.liquid.wastePipeVolume,
		self.SPEED)
end

--[[
 * @brief 测量仓排废液(定量)
 * @param vol 体积
--]]
function Operator:DrainToWasteRoom(vol)
    if vol == nil then
        vol = 5
    end
    self:Drain(setting.liquidType.wasteRoom, vol, self.SPEED)
end

--[[
 * @brief 开启测量仓排废液
--]]
function Operator:WasteRoomOutStart()
    local pump = pumps[setting.liquidType.wasteRoom.pump + 1]  --  获取搅拌泵对象
    local volume = 10000
    local speed = 1
    local ret =false
    --log:debug("stir on")
    --打开蠕动泵进行吸操作
    local err,result = pcall(function() return pump:Start(RollDirection.Drain, volume, speed) end)

    if not err then      -- 出现异常
        pump:Stop()                                                            -- 停止泵
        error(result)
    else    --函数调用正常
        if not result then
            return false
        else
            log:debug("开启测量仓排废液")
            ret = true
        end
    end

    return ret
end

--[[
 * @brief 关闭测量仓排废液
--]]
function Operator:WasteRoomOutStop()
    local pump = pumps[setting.liquidType.wasteRoom.pump + 1]  --  获取量仓排废液对象
    local timeout = 3000;
    --log:debug("stir off")
    if pump:GetStatus() == PumpStatus.Busy then
        pump:Stop()
        -- 等待泵操作结果事件
        local err,result = pcall(function() return pump:ExpectResult(timeout) end)
        if not err then      -- 出现异常
            error(result)
        else
            log:debug("关闭测量仓排废液")
        end
    else
        pump:Stop()
    end
end

--[[
 * @brief 开启指定恒温器恒温控制
 * @details 恒温至目标温度
 * @param[in] thermostat 恒温器,
 * @param[in] mode 恒温模式,
 * @param[in] targetTemp 目标温度,
 * @param[in] tolerance 容差温度,
 * @param[in] timeout 超时时间,
--]]
function Operator:StartThermostat(rThermostat, rMode, rTargetTemp, rTolerance, rTimeout, rIsExpect)
    --恒温过程
    local process = function(thermostat, mode, targetTemp, tolerance, timeout, isExpect)
        local temp = 0

        if isExpect == true then
            dc:ClearThermostatRemainEvent() --清除温控剩余事件池中温控事件
        end

        local ret = thermostat:Start(mode, targetTemp, tolerance, timeout, isExpect)
        if ret == true then

            log:debug("{StartThermostat} index = " .. thermostat.index .. ", mode = " .. mode .. ", targetTemp = " .. targetTemp .. ", tolerance = " .. tolerance ..", timeout = " .. timeout)

            if isExpect == true then
                local result = thermostat:ExpectResult(timeout * 1000 + 1000);

                log:debug("{thermostatExpectResult} index = " .. result:GetIndex() .. "，result = " .. result:GetResult() .. "，temp = " .. result:GetTemp())

                if result:GetResult() == ThermostatOperateResult.Failed then
                    thermostat.isRunning = false
                    error(ThermostatFailedException:new({mode = mode, targetTemp = targetTemp, toleranceTemp = tolerance, timeout = timeout}))
                elseif result:GetResult() == ThermostatOperateResult.Stopped then
                    thermostat.isRunning = false
                    error(ThermostatStoppedException:new({mode = mode, targetTemp = targetTemp, toleranceTemp = tolerance, timeout = timeout}))
                elseif result:GetResult() == ThermostatOperateResult.Timeout then
                    error(ThermostatTimeoutException:new({mode = mode, targetTemp = targetTemp, toleranceTemp = tolerance, timeout = timeout}))
                elseif result:GetResult() == ThermostatOperateResult.Reached then
                    thermostat.isRunning = false
                    temp = result:GetTemp() --返回温度
                end
            end
        else
            log:debug("开启恒温器" .. thermostat.index .. "失败！")
        end

        return temp
    end

    --异常捕获，处理
    local thermostatTemp = 0

    local err,result = pcall(function() return process(rThermostat, rMode, rTargetTemp, rTolerance, rTimeout, rIsExpect) end)
    if not err then -- 出现异常
        if type(result) == "table" then        --Lua异常
            if getmetatable(result) == ThermostatFailedException then
                ExceptionHandler.MakeAlarm(result)
            elseif getmetatable(result) == ThermostatTimeoutException then
                ExceptionHandler.MakeAlarm(result)
            else
                error(result)
            end
        elseif type(result) == "userdata" then --C++异常
            if result:GetType() == "ExpectEventTimeoutException" then
                ExceptionHandler.MakeAlarm(result)
            else
                error(result)
            end
        else
            error(result)
        end
    else  -- 正常
        thermostatTemp = result
    end

    return thermostatTemp
end

--[[
 * @brief 停止指定恒温器控制.
--]]
function Operator:StopThermostat(thermostat, isExpect)
    local temp = 0

    if isExpect == true then
        dc:ClearThermostatRemainEvent() --清除温控剩余事件池中温控事件
    end

    local ret = thermostat:Stop(isExpect)

    if ret == true then
        log:debug("{StopThermostat " .. thermostat.index .. "}")

        if isExpect == true then
            local result = thermostat:ExpectResult(2000)
            temp  = result:GetTemp()

            log:debug("{thermostatExpectResult} index = " .. result:GetIndex() .. "，result = " .. result:GetResult() .. "，temp = " .. result:GetTemp())
        end

        thermostat.isRunning = false
    end

    return temp
end

--[[
 * @brief 开启恒温器恒温控制
 * @details 恒温至设定目标温度
 * @param Index 恒温器索引
--]]
function Operator:StartTemperatureControl(index)
    local startTime = os.time()
    local measureSafetyTemp = config.measureParam.measureSafetyTemp
    local bacteriaSafetyTemp = config.measureParam.bacteriaSafetyTemp
    local operateResult = false

    dc:ClearThermostatRemainEvent() --清除温控剩余事件池中温控事件

    local environmentTemp = dc:GetCurrEnvironmentTemp()
    log:debug("环境温度 temp = "..environmentTemp)

    local ret = nil
    if index == thermostats.measure then
        if environmentTemp >=  config.measureParam.measureSafetyTemp then
            ret = thermostats.measure:Start(ThermostatMode.Refrigerate, measureSafetyTemp, setting.temperature.toleranceTemp, setting.temperature.coolingTimeout, true)
        else
            ret = thermostats.measure:Start(ThermostatMode.Heater, measureSafetyTemp, setting.temperature.toleranceTemp, setting.temperature.coolingTimeout, true)
        end
        if ret then
            local err,result = pcall(function()
                return self.ITemperatureControl:ExpectThermostat(setting.temperature.coolingTimeout * 1000 + 1000)
            end)
            if result:GetResult() == ThermostatOperateResult.Failed then
                error(ThermostatFailedException:new())
            elseif result:GetResult() == ThermostatOperateResult.Stopped then
                error(ThermostatStoppedException:new())
            elseif result:GetResult() == ThermostatOperateResult.Timeout then
                error(ThermostatTimeoutException:new())
            elseif result:GetResult() == ThermostatOperateResult.Reached then
                operateResult = true
            else
                log:debug("StartTemperatureControl() ==> Unknow Result..")
            end
        else
            log:debug("开启测量仓恒温器失败！")
            error(ThermostatFailedException:new())
        end
    elseif index == thermostats.bacteria then
        ret = thermostats.bacteria:Start(ThermostatMode.Refrigerate, bacteriaSafetyTemp, setting.temperature.toleranceTemp, setting.temperature.coolingTimeout, true)
        if ret then
            local err,result = pcall(function()
                return self.ITemperatureControl:ExpectThermostat(setting.temperature.coolingTimeout * 1000 + 1000)
            end)
            if result:GetResult() == ThermostatOperateResult.Failed then
                error(ThermostatFailedException:new())
            elseif result:GetResult() == ThermostatOperateResult.Stopped then
                error(ThermostatStoppedException:new())
            elseif result:GetResult() == ThermostatOperateResult.Timeout then
                error(ThermostatTimeoutException:new())
            elseif result:GetResult() == ThermostatOperateResult.Reached then
                operateResult = true
            else
                log:debug("StartTemperatureControl() ==> Unknow Result..")
            end
        else
            log:debug("开启菌室恒温器失败！")
            error(ThermostatFailedException:new())
        end
    else
        log:debug("StartTemperatureControl() ==> Invalid Thermostats..")
    end

    if operateResult == true then
        return true
    else
        error(ThermostatFailedException:new())
    end
end

--[[
 * @brief 检查测量仓温度安全
 * @details 当测量仓温度高于安全温度时，启动降温操作.
--]]
function Operator:CheckMeasureSafety()
    local measureSafe = true
    local bacteriaSafe = true
    local measureSafetyTemp = config.measureParam.measureSafetyTemp
    local bacteriaSafetyTemp = config.measureParam.bacteriaSafetyTemp
    local startTime = os.time()

    local  measureTemp = thermostats.measure:GetTemperature()
    local  bacteriaTemp = thermostats.bacteria:GetTemperature()
    log:debug("安全温度检查 测量仓温度 = " .. measureTemp .. " ℃".." 发光菌存储槽温度 = " .. bacteriaTemp .. " ℃");

    if math.abs(measureTemp - measureSafetyTemp) > setting.temperature.measureWarnLimit then
        measureSafe = false
    end
    if math.abs(bacteriaTemp - bacteriaSafetyTemp) > setting.temperature.bacteriaWarnLimit then
        bacteriaSafe = false
    end

    if true == config.measureParam.openTemperatureControl then
        if not measureSafe  then
            local alarm = Helper.MakeAlarm(setting.alarm.measureTempOutLimit, "")
            AlarmManager.Instance():AddAlarm(alarm)
        end

        if not bacteriaSafe  then
            local alarm = Helper.MakeAlarm(setting.alarm.bacteriaTempOutLimit, "")
            AlarmManager.Instance():AddAlarm(alarm)
        end
    end
end

--[[
 * @brief 采集AD
 * @details 采集光学测量AD信号
 * @param[in] acquireTime 采集时间
--]]
function Operator:StartAcquirerAD(acquireTime)
	local AD = OpticalAD.new()
    local temp = 0

	local timeout = acquireTime * 1000 + 2000

	self.isAcquiring = true

	if self.IOpticalAcquire:StartAcquirer(acquireTime) == true then
		local err,result = pcall(function() return self.IOpticalAcquire:ExpectADAcquirer(timeout) end)
		if not err then -- 出现异常
			self.IOpticalAcquire:StopAcquirer() --停止采集
			self.isAcquiring = false
			error(result) --向上层抛异常
		else
            temp = result
            self.isAcquiring = false
            if result.GetResult ~= nil then
                if result:GetResult() == OpticalAcquireResult.Failed then
                    error (AcquirerADFailedException:new())
                elseif result:GetResult()  == OpticalAcquireResult.Stopped then
                    error (AcquirerADStoppedException:new())
                elseif result:GetResult()  == OpticalAcquireResult.Finished then
                    temp = result:GetAD()
                end
            end
            local refAD = temp:GetReference()
            local meaAD = temp:GetMeasure()
            AD:SetReference(refAD)
            AD:SetMeasure(meaAD)
		end
	end

    --print("Get AD mea = "..AD:GetMeasure().." ref = "..AD:GetReference())

	return AD
end

--[[
 * @brief PMT放大调节
 * @param[in] index 调节索引
 * @param[in] target 目标AD值
--]]
function Operator:AutoStaticADControl(index, target)
    local ret = false
    local timeout = 120 * 1000

    local ret = self.IOpticalAcquire:StartStaticADControl(index,target)

    if ret == true then
        self.isStaticADControl = true
        log:debug("Auto static AD control started index = "..index.." target = "..target)

        local result = self.IOpticalAcquire:ExpectStaticADControlResult(timeout)

        if result == StaticADControlResult.Unfinished then
            if self.isStaticADControl == true then
                self.isStaticADControl = false
                error (StaticADControlFailedException:new())
            elseif self.isStaticADControl == false then  --被停止
                error (StaticADControlStoppedException:new())
            end
            return false
        elseif result == StaticADControlResult.Finished then
            self.isStaticADControl = false
            log:debug("Auto static AD control index = "..index.." target = "..target.." finished !")
            return true
        end
    else
        log:debug("Auto static AD control index = "..index.." target = "..target.." start fail !")
        return false
    end
end

--[[
 * @brief 环境温度监控
 * @param[in] eTemp 环境温度
--]]
function Operator:EevTempCheck(eTemp)
    local temp = eTemp

    local err,ret = pcall(function()
        if temp - setting.temperature.insideEnvironmentTemp > 0.1 then
            self.ITemperatureControl:BoxFanSetOutput(1) --打开箱风扇
            --log:debug("打开机箱风扇")
        elseif setting.temperature.insideEnvironmentTemp - temp > 2 then
            self.ITemperatureControl:BoxFanSetOutput(0) --关机箱风扇
            --log:debug("关闭机箱风扇")
        end

        return true
    end)
    if not err then      -- 出现异常
        if type(ret) == "userdata" then
            log:warn("Operator:EevTempCheck() =>" .. ret:What())
        elseif type(ret) == "table" then
            log:warn("Operator:EevTempCheck() =>" .. ret:What())
        elseif type(ret) == "string" then
            log:warn("Operator:EevTempCheck() =>" .. ret)	--C++、Lua系统异常
        end
    end

end

--[[
 * @brief 采集处理AD
 * @details 采集AD信号数据并滤波
 * @param[in] acquireTime 采集时间
--]]
function Operator:AcquirerFilterAD(acquireTime)
    local AD = {}
    local refAD = {}
    local meaAD = {}
    local filterAD = OpticalAD.new()

    for i = 1,acquireTime do
        AD[i] = self:StartAcquirerAD(1)  --采集1秒数据
        refAD[i] = AD[i]:GetReference()
        meaAD[i] = AD[i]:GetMeasure()
        log:debug("Get refAD["..i.."] = "..refAD[i].." meaAD["..i.."] = "..meaAD[i])
    end

    local filterHigh = math.floor(acquireTime * 0.2)
    local filterLow = math.floor(acquireTime * 0.2)

    local filterADPair = self:FilterDataPair(refAD, meaAD, filterHigh, filterLow)
    local filterRefAD = math.floor(filterADPair[1])
    local filterMeaAD = math.floor(filterADPair[2])

    filterAD:SetReference(filterRefAD)
    filterAD:SetMeasure(filterMeaAD)

    return filterAD
end

--[[
 * @brief 数据滤波
 * @details 去掉部分大值和部分小值后平均
 * @param[in] inputTable1 数据表1
 * @param[in] inputTable2 数据表2
 * @param[in] filterHigh 去掉大值数据个数
 * @param[in] filterLow 去掉小值数据个数
--]]
function Operator:FilterDataPair(inputTable1, inputTable2, filterHigh, filterLow)
    local sumData = {0,0}
    local avgData = {0,0}
    local dataTable = {}
    local valueTable1 = {}
    local valueTable2 = {}
    for i = 1,#inputTable1,1 do
		if inputTable1[i] == 0 or inputTable2[i] == 0 then
            dataTable[i] = 0
        else
            dataTable[i] = inputTable1[i]/inputTable2[i]
        end
        valueTable1[dataTable[i]] = inputTable1[i]
        valueTable2[dataTable[i]] = inputTable2[i]
    end

    --排序
    table.sort(dataTable)

    local catTable = {}
    if #dataTable > filterHigh + filterLow then
        table.move(dataTable, filterLow + 1, #dataTable - filterHigh, 1, catTable)
    else
        log:debug("滤波参数错误")
        return 0
    end

    --取平均值
    for i = 1,#catTable do
        sumData[1] = sumData[1] + valueTable1[catTable[i]]
        sumData[2] = sumData[2] + valueTable2[catTable[i]]
    end
    avgData[1] = sumData[1] / #catTable
    avgData[2] = sumData[2] / #catTable
    log:debug("a[1] = "..avgData[1].." a[2] = "..avgData[2])

    return avgData;
end

--[[
 * @brief 数据滤波
 * @details 去掉部分大值和部分小值后平均
 * @param[in] dataTable 数据表
 * @param[in] filterHigh 去掉大值数据个数
 * @param[in] filterLow 去掉小值数据个数
--]]
function Operator:FilterData(inputTable, filterHigh, filterLow)
    local sumData = 0
    local avgData = 0

    local dataTable = {}
    table.move(inputTable, 1, #inputTable, 1, dataTable)

    --排序
    table.sort(dataTable)

    local catTable = {}
    if #dataTable > filterHigh + filterLow then
        table.move(dataTable, filterLow + 1, #dataTable - filterHigh, 1, catTable)
    else
        log:debug("滤波参数错误")
        return 0
    end

    --取平均值
    for i = 1,#catTable do
        sumData = sumData + catTable[i]
    end
    avgData = sumData / #catTable

    return avgData;
end

--[[
 * @brief 冒泡排序
 * @details 对表内数据进行排序
 * @param[in] dataTable 数据表
--]]
function Operator:BubbleSort(dataTable)
    local length = #dataTable

    local i = 0
    local j = 0
    local temp = 0

    for i = 1,length,1 do
        for j = length,i+1,-1 do
            if dataTable[j] < dataTable[j-1] then
                temp = dataTable[j-1]
                dataTable[j-1] = dataTable[j]
                dataTable[j] = temp
            end
        end
    end

    return dataTable
end

