
Measurer =
{
    flow = nil,
    measureType = MeasureType.Sample,

    measureResult =
    {
        gainRef = 1,
        gainMea = 1,
        bacteriaTemp = 0,
        measureTemp = 0,
        environmentTemp = 0,

        darkReferenceAD =0,
        darkMeasureAD =0,
        initReferenceAD =0,
        initMeasureAD =0,
        startReferenceAD =0,
        startMeasureAD =0,
        finalReferenceAD =0,
        finalMeasureAD =0,
    },

    reactStartTime = 0,
    darkReferenceAD = {},
    darkMeasureAD = {},
    reactAD = {},

    procedure = {},

    steps =
    {
        --1 机械复位
        {
            action = setting.runAction.measure.machineReset,
            execute = function()
                local startTime = os.time()
                --电机驱动初始化
                op.IMotorControl:MotorDriverReinit()
                App.Sleep(2000)
                --机械臂复位
                op:ManipulatorReset()
                --移动至排液处
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                --注射器复位排空
                op:SyringeReset()
                --针管路加清洗水
                for i = 1,2 do
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                    op:SyringeAspiration(0.8)
                    op:SyringeReset()
                end
                op:CleanNeedlePipe(setting.clean.cleanVolume)

                --空白水检测
                op:BlankCheck()

                log:debug("机械复位时间 = " .. os.time() - startTime)
            end
        },
        --2 测量前清洗
        {
            action = setting.runAction.measure.cleanBeforeMeasure,
            execute = function()
				local startTime = os.time()

				op:AutoMove(setting.component.xPos.measureCell, 120)
                op:SyringeAspiration(0.6)
                op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 10)
                op:SyringeDischarge(0.6)
				op:CleanNeedlePipe(setting.clean.cleanVolume)				

				--针管路排干测量槽
				op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
				op:AirWasteNeedlePipe()

				op:AutoMove(setting.component.xPos.sampleMixer, 120)
                op:SyringeAspiration(0.6)
                op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 10)
                op:SyringeDischarge(0.6)
				op:CleanNeedlePipe(setting.clean.cleanVolume)

				--针管路排干样品混合槽
				op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
				op:AirWasteNeedlePipe()

				--针管路排干发光菌混合槽
				op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
				op:AirWasteNeedlePipe()
				op:AirWasteNeedlePipe()

                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:CleanNeedlePipe(setting.clean.cleanVolume * 3)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

				--开启测量槽排废液
				op:WasteRoomOutStart()
				for i = 1,config.measureParam.cleanBeforeMeasureTimes do
					--移动至测量槽底部
					op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 5)
					--针管路加清洗水
					op:CleanNeedlePipe(3)
					--等200ms
					App.Sleep(200)

					op:AutoMove(setting.component.xPos.measureCell, 120)
                    op:SyringeAspiration(0.6)
                    op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 10)
                    op:SyringeDischarge(0.6)
					op:CleanNeedlePipe(setting.clean.cleanVolume)

					--测量槽排空
					for i = 1,2 do
						op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 3)
						op:SyringeAspiration(1.0)
						op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
						op:SyringeReset()
						op:CleanNeedlePipe(setting.clean.cleanVolume)
					end
					op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom)
					op:SyringeAspiration(1.0)
					op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
					op:SyringeReset()
					op:CleanNeedlePipe(setting.clean.cleanVolume)

					--移动至样品混合槽底部
					op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 5)
					--针管路加清洗水
					op:CleanNeedlePipe(2.5)
					--等200ms
					App.Sleep(200)

					--移动至发光菌混合槽底部
					op:AutoMove(setting.component.xPos.bacteriaMixer, config.componentParam.bacteriaMixer.bottom - 5)
					--针管路加清洗水
					op:CleanNeedlePipe(3.5)
					--等200ms
					App.Sleep(200)

					--发光菌混合槽排空
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

                --空白水检测
                op:BlankCheck()

                --加渗透压调节液
                op:Pump(setting.liquidType.salt, config.measureParam.addSaltVolume, setting.liquid.addLiquidSpeed)
                --加空白水
                op:Pump(setting.liquidType.blank, config.measureParam.addBlankVolume, setting.liquid.addLiquidSpeed)
                --加水样
                if Measurer.measureType == MeasureType.Sample then  --测水样
                    log:debug("水样管路更新，体积 = "..config.measureParam.addSampleVolume)
                    op:Pump(setting.liquidType.sample, config.measureParam.addSampleVolume, setting.liquid.addLiquidSpeed)
                    --elseif Measurer.measureType == MeasureType.Blank then  --测空白
                end

                --持续采水关闭
                if config.interconnection.collectSampleMode == setting.mode.collectSample.toAddSampleEnd then
                    local waterCollector = WaterCollector.Instance()
                    waterCollector:TurnOff()
                    Helper.Result.RelayOutOperate(setting.mode.relayOut.collectInstruct, false)
                end

                --注水1秒
                --50uL空气隔离
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.top)
                op:SyringeAspiration(0.05)

                log:debug("测量前清洗时间 = " .. os.time() - startTime)
            end
        },
        --3 渗透压调节
        {
            action = setting.runAction.measure.infiltrateAdjust,
            execute = function()

                local startTime = os.time()
                local gstat = op.IMotorControl:MotorDriverCheck(setting.motor.driverSyrIndex)
                log:debug("渗透压调节 注射器电机驱动检测  stat = "..gstat)

			   --加空白水至发光菌混合槽
                if config.measureParam.bacteriaBlankVolume > 0 then
                    op:SyringeAddLiquid(setting.component.xPos.blankCell, setting.component.xPos.bacteriaMixer, config.measureParam.bacteriaBlankVolume/1000, 0.05)
                end
                --加渗透压调节液至发光菌混合槽
                if config.measureParam.bacteriaSaltVolume > 0 then
                    op:AutoMove(setting.component.xPos.saltCell, setting.component.zPos.top)
                    op:SyringeAspiration(0.05)
                    op:SyringeAddLiquid(setting.component.xPos.saltCell, setting.component.xPos.bacteriaMixer, config.measureParam.bacteriaSaltVolume/1000, 0.01)
                    --针清洗
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                end

                --发光菌混合槽混合---------------------
                op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.top)
                op:SyringeAspiration(0.05) --50uL空气隔离
                op:AirWasteNeedlePipe()
                op:SyringeAspiration(0.05) --100uL空气隔离
                op:AirWasteNeedlePipe()

                --吹气混匀
                for i = 1,2 do
					op:AutoMove(setting.component.xPos.bacteriaMixer, config.measureParam.airMixHeight)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
					op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
					op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡

					op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
					op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡

					op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
					op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)
					op:AirRecover(setting.liquidType.airWaste) --气压平衡

					op:AirWashNeedlePipe(config.measureParam.airMixDoubleTime)--吹气
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
				end

			    --开启发光菌搅拌
                op:StirOn()
                log:debug("开启发光菌搅拌")

                op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom) --触碰溶液

                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.cleanVolume)

				if not Measurer.flow:Wait(90) then
                    error(UserStopException:new())
                end

				--关闭搅拌
                op:StirOff()
                log:debug("关闭发光菌搅拌")

                log:debug("渗透压调节时间 = " .. os.time() - startTime)
            end
        },
        --4 发光菌混合
        {
            action = setting.runAction.measure.bacteriaMix,
            execute = function()
                local startTime = os.time()
                local gstat = op.IMotorControl:MotorDriverCheck(setting.motor.driverSyrIndex)
                log:debug("发光菌混合 注射器电机驱动检测  stat = "..gstat)
                --吸发光菌50uL至发光菌混合槽
                if config.measureParam.bacteriaVolume > 0 then
                    op:AutoMove(setting.component.xPos.bacteriaCell, setting.component.zPos.top)
                    op:SyringeAspiration(0.05)  --50uL空气隔离
                    op:AutoMove(setting.component.xPos.bacteriaCell, setting.component.zPos.bottom)
                    op:SyringeAspiration(config.measureParam.bacteriaVolume/1000 + 0.005) --额外多吸
                    op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
                    App.Sleep(200)
                    op:SyringeAccurateDischarge(config.measureParam.bacteriaVolume/1000)
                    --更新发光菌余量体积
                    ReagentRemainManager.ReduceReagentRemain( status.remain.bacteria, config.measureParam.bacteriaVolume/1000 + 0.005)
                end

                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

				--发光菌混合槽混合---------------------
                op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.top)
                op:SyringeAspiration(0.05)  --100uL隔离
                op:AirWasteNeedlePipe()
                op:SyringeAspiration(0.05)  --100uL隔离
                op:AirWasteNeedlePipe()
                for i = 1,config.measureParam.airMixLoopTimes do  --吸液吹气
                    --注射器混匀
                    op:AutoMove(setting.component.xPos.bacteriaMixer, config.measureParam.airMixHeight)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡
                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡
                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡

                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡
                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡

					op:AirWashNeedlePipe(config.measureParam.airMixDoubleTime)--吹气
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
					
                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)                                                                           
                    op:AirRecover(setting.liquidType.airWaste) --............                                                                                                           
                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)                                                                           
                    op:AirRecover(setting.liquidType.airWaste) --............

                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashRef)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡
                    op:AirWashNeedlePipe(config.measureParam.airMixSignleTime, setting.liquidType.airWashMea)
                    op:AirRecover(setting.liquidType.airWaste) --气压平衡

					op:AutoMove(setting.component.xPos.bacteriaMixer, config.measureParam.airMixHeight)
                    op:AirWashNeedlePipe(config.measureParam.airMixDoubleTime)--吹气
					op:AirRecover(setting.liquidType.airWaste) --气压平衡
                end
                --op:SyringeAspiration(0.05)  --50uL隔离
                --op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom) --触碰溶液

                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume * 3)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

                log:debug("吸发光菌至测量槽")

                --等待1min
                if not Measurer.flow:Wait(60) then
                    error(UserStopException:new())
                end

                --注水
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.top)
                op:SyringeAspiration(0.05)
                op:SyringeAddLiquid(setting.component.xPos.bacteriaMixer, setting.component.xPos.measureCell, config.measureParam.mixBacteriaVolume/1000, 0.03)

				op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.top)
				op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)

                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

				--水样混合槽排空
				op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 3)
				op:SyringeAspiration(1.0)
				op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
				op:SyringeReset()
				op:CleanNeedlePipe(setting.clean.cleanVolume)
				op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
				op:SyringeAspiration(1.0)
				op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
				op:SyringeReset()
				op:CleanNeedlePipe(setting.clean.cleanVolume)

				op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

                log:debug("发光菌混合时间 = " .. os.time() - startTime)
            end
        },
		--5 水样混合
        {
            action = setting.runAction.measure.sampleMix,
            execute = function()
                local startTime = os.time()

                local gstat = op.IMotorControl:MotorDriverCheck(setting.motor.driverSyrIndex)
                log:debug("水样混合 注射器电机驱动检测  stat = "..gstat)

                --制备待测样900uL至水样混合槽
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.top)
                op:SyringeAspiration(0.05)  --50uL隔离
                if Measurer.measureType == MeasureType.Sample then  --测水样
                    if config.measureParam.sampleBlankVolume > 0 then
                        op:SyringeAddLiquid(setting.component.xPos.blankCell, setting.component.xPos.sampleMixer, config.measureParam.sampleBlankVolume/1000, 0)
                    end
                    if config.measureParam.sampleVolume > 0 then
                        op:SyringeAddLiquid(setting.component.xPos.sampleCell, setting.component.xPos.sampleMixer, config.measureParam.sampleVolume/1000, 0.01)
                        --针清洗
                        op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                        op:SyringeReset()
                        op:CleanNeedlePipe(setting.clean.cleanVolume * 3)
                        op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                        op:CleanNeedlePipe(setting.clean.clearVolume)

						--针清洗
                        op:CleanNeedlePipe(setting.clean.cleanVolume)
                    end
                elseif Measurer.measureType == MeasureType.Standard then  --测质控
                    if config.measureParam.standardBlankVolume > 0 then
                        op:SyringeAddLiquid(setting.component.xPos.blankCell, setting.component.xPos.sampleMixer, config.measureParam.standardBlankVolume/1000, 0.05)
                    end
                    if config.measureParam.standardVolume > 0 then
                        op:SyringeAddLiquid(setting.component.xPos.standardCell, setting.component.xPos.sampleMixer, config.measureParam.standardVolume/1000, 0.01)
                        --针清洗
                        op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                        op:SyringeReset()
                        op:CleanNeedlePipe(setting.clean.cleanVolume * 3)
                        op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                        op:CleanNeedlePipe(setting.clean.clearVolume)

						--针清洗
                        op:CleanNeedlePipe(setting.clean.cleanVolume)
                    end
                elseif Measurer.measureType == MeasureType.Blank then  --测空白
                    if config.measureParam.blankVolume > 0 then
                        op:SyringeAddLiquid(setting.component.xPos.blankCell, setting.component.xPos.sampleMixer, config.measureParam.blankVolume/1000, 0.05)
                    end
                end

                local saltVolume = 100
                if Measurer.measureType == MeasureType.Sample then  --测水样
                    saltVolume = config.measureParam.sampleSaltVolume
                elseif Measurer.measureType == MeasureType.Standard then  --测质控
                    saltVolume = config.measureParam.standardSaltVolume
                elseif Measurer.measureType == MeasureType.Blank then  --测空白
                    saltVolume = config.measureParam.blankSaltVolume
                end

                --吸空气隔离
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.top)
                op:SyringeAspiration(0.05)
                --吸渗透压调节液至水样混合槽
                if saltVolume/1000 > 0 then
                    op:SyringeAddLiquid(setting.component.xPos.saltCell, setting.component.xPos.sampleMixer, saltVolume/1000, 0.01)
                end
                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

                --注射器混合
                op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.top)
                op:SyringeAspiration(0.1)
                op:AirWasteNeedlePipe() -- 抽空管路

                --冒泡混匀
                for i = 1,3 do
                    op:AutoMove(setting.component.xPos.sampleMixer, 120)
                    op:SyringeAspiration(0.6)
                    op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 5)
                    op:SyringeDischarge(0.6)
                end

                --针清洗
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

                --空白水检测
                op:BlankCheck()

                log:debug("水样混合时间 = " .. os.time() - startTime)
            end
        },
        --6 孵育
		{
            action = setting.runAction.measure.foster,
            execute = function()
                local startTime = os.time()

                log:debug("发光菌孵育开始")
                local incubationStartTime = os.time()

                --等待孵育
                local useTime = os.time() - incubationStartTime

				--开启测量模块电源
                --op.IOpticalAcquire:PDPowerOn()

                if useTime < config.measureParam.incubationTime and useTime >= 0 then
                    if not Measurer.flow:Wait(config.measureParam.incubationTime - useTime) then
                        error(UserStopException:new())
                    end
                end

				log:debug("发光菌孵育完成")

                log:debug("孵育时间 = " .. os.time() - startTime)
            end
        },
        --7 采集初始光强
        {
            action = setting.runAction.measure.readInitialAD,
            execute = function()
                local startTime = os.time()

                Measurer.measureResult.gainRef = op.IOpticalAcquire:GetStaticADControlParam(setting.signal.refIndex)
                Measurer.measureResult.gainMea = op.IOpticalAcquire:GetStaticADControlParam(setting.signal.meaIndex)
                log:debug("Derfault gainRef = "..Measurer.measureResult.gainRef.. " gainMea = "..Measurer.measureResult.gainMea)

				--开启测量模块电源
				op.IOpticalAcquire:PDPowerOn()
				log:debug("开启测量模块电源")

				if not Measurer.flow:Wait(setting.signal.readADTime) then
					error(UserStopException:new())
                end

                local initAD = op:AcquirerFilterAD(setting.signal.readADTime * 2)
                Measurer.measureResult.initReferenceAD = initAD:GetReference()
                Measurer.measureResult.initMeasureAD = initAD:GetMeasure()

                --PMT放大自适应调节
                if config.measureParam.pmtAuto == true or Measurer.measureType == MeasureType.Check then  --复苏菌品质检测
                    if Measurer.measureResult.initReferenceAD < config.measureParam.destLowLimit or Measurer.measureResult.initReferenceAD > config.measureParam.destHighLimit or
                    Measurer.measureResult.initMeasureAD < config.measureParam.destLowLimit or Measurer.measureResult.initMeasureAD > config.measureParam.destHighLimit then
                        log:debug("Invalid AD C0 = "..Measurer.measureResult.initReferenceAD.." S0 = "..Measurer.measureResult.initMeasureAD)

                        --PMT放大倍数调节
                        if (Measurer.measureResult.initReferenceAD < config.measureParam.destLowLimit and Measurer.measureResult.gainRef > 0) or
                            (Measurer.measureResult.initReferenceAD > config.measureParam.destHighLimit and Measurer.measureResult.gainRef < 1023) then
                            op:AutoStaticADControl(setting.signal.refIndex, config.measureParam.destTarget)
                            Measurer.flow:Wait(1)
                        end
                        if (Measurer.measureResult.initMeasureAD < config.measureParam.destLowLimit and Measurer.measureResult.gainMea > 0) or
                            (Measurer.measureResult.initMeasureAD > config.measureParam.destHighLimit and Measurer.measureResult.gainMea < 1023) then
                            op:AutoStaticADControl(setting.signal.meaIndex, config.measureParam.destTarget)
                            Measurer.flow:Wait(1)
                        end

                        Measurer.measureResult.gainRef = op.IOpticalAcquire:GetStaticADControlParam(setting.signal.refIndex)
                        Measurer.measureResult.gainMea = op.IOpticalAcquire:GetStaticADControlParam(setting.signal.meaIndex)
                        log:debug("Auto gainRef = "..Measurer.measureResult.gainRef.. " gainMea = "..Measurer.measureResult.gainMea)

                        initAD = op:AcquirerFilterAD(setting.signal.readADTime)
                        Measurer.measureResult.initReferenceAD= initAD:GetReference()
                        Measurer.measureResult.initMeasureAD = initAD:GetMeasure()
                    end
                end
                log:debug("初始光强AD C0 = "..Measurer.measureResult.initReferenceAD.." S0 = "..Measurer.measureResult.initMeasureAD)

				--关闭测量模块电源
				op.IOpticalAcquire:PDPowerOff()
				log:debug("关闭测量模块电源")

                log:debug("采集初始光强时间 = " .. os.time() - startTime)
            end
        },
        --8 菌样混合
        {
            action = setting.runAction.measure.mixBacteriaSample,
            execute = function()
                local startTime = os.time()
                local gstat = op.IMotorControl:MotorDriverCheck(setting.motor.driverSyrIndex)
                log:debug("菌样混合 注射器电机驱动检测  stat = "..gstat)
                --打开混样开关
                if config.measureParam.mixSwitch == true then
                    --隔离
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.top)
                    op:SyringeAspiration(0.05)  --50uL空气隔离
                    --吸混合水样至测量槽
                    op:SyringeAddLiquid(setting.component.xPos.sampleMixer, setting.component.xPos.measureCell, config.measureParam.mixSampleVolume/1000, 0.05)

                    --针清洗                                                                                                                                              
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)                                                                                 
                    op:SyringeReset()                                                                                        
                    op:CleanNeedlePipe(setting.clean.cleanVolume * 2)                                                             
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)                                                                         
                    op:CleanNeedlePipe(setting.clean.clearVolume)

                    --隔离
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.top)
                    op:SyringeAspiration(0.1)
                    op:AirWasteNeedlePipe() -- 抽空管路

                    if config.measureParam.mixBacteriaSampleMethod == 0 then
                        --冒泡混匀
                        for i = 1,config.measureParam.mixBacteriaSampleTimes do
                            op:AutoMove(setting.component.xPos.measureCell, 120)
                            op:SyringeAspiration(0.6)
                            op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 3)
                            op:SyringeDischarge(0.6)
                        end
                    else
                        --测量槽处注射器混合
                        for i = 1,config.measureParam.mixBacteriaSampleTimes do
                            op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 5)
                            op:SyringeAspiration(0.8)
                            op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 40)
                            op:SyringeDischarge(0.8)
                        end
                    end
					
					--针清洗
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                    op:CleanNeedlePipe(setting.clean.clearVolume)

                    --排空发光菌混合槽
                    op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
                    op:SyringeAspiration(0.2)

                    --排空水样混合槽
                    op:AutoMove(setting.component.xPos.sampleMixer, setting.component.zPos.bottom)
                    op:SyringeAspiration(0.2)

                    --针清洗
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                    op:CleanNeedlePipe(setting.clean.clearVolume)

                    --开启测量槽排废液
                    op:WasteRoomOutStart()
                    --水样混合槽注水清洗
                    op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 5)
                    op:CleanNeedlePipe(2.5)
                    --发光菌混合槽注水清洗
                    op:AutoMove(setting.component.xPos.bacteriaMixer, config.componentParam.bacteriaMixer.bottom - 5)
                    op:CleanNeedlePipe(3.5)

                    App.Sleep(1000)
                    --关闭测量槽排废液
                    op:WasteRoomOutStop()
                    --针外壁清洗
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
				else
					--水样混合槽排空
					op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 3)
					op:SyringeAspiration(1.0)
					op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
					op:SyringeReset()
					op:CleanNeedlePipe(setting.clean.cleanVolume)

                    --排空发光菌混合槽
                    op:AutoMove(setting.component.xPos.bacteriaMixer, setting.component.zPos.bottom)
                    op:SyringeAspiration(0.3)

                    --针清洗
                    op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                    op:SyringeReset()
                    op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                    op:CleanNeedlePipe(setting.clean.clearVolume)

                    --开启测量槽排废液
                    op:WasteRoomOutStart()
                    --水样混合槽注水清洗
                    op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 5)
                    op:CleanNeedlePipe(2.5)
                    --发光菌混合槽注水清洗
                    op:AutoMove(setting.component.xPos.bacteriaMixer, config.componentParam.bacteriaMixer.bottom - 5)
                    op:CleanNeedlePipe(3.5)

                    App.Sleep(1000)
                    --关闭测量槽排废液
                    op:WasteRoomOutStop()
                    --针外壁清洗
                    op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                    op:CleanNeedlePipe(setting.clean.cleanVolume)
                end

                log:debug("菌样混合时间 = " .. os.time() - startTime)
            end
        },
        --9 反应测量
        {
            action = setting.runAction.measure.reactMeasure,
            execute = function()
                local startTime = os.time()

                local reactTime = config.measureParam.sampleReactTime
                if Measurer.measureType == MeasureType.Sample then  --测水样
                    reactTime = config.measureParam.sampleReactTime
                elseif Measurer.measureType == MeasureType.Standard then  --测质控
                    reactTime = config.measureParam.standardReactTime
                elseif Measurer.measureType == MeasureType.Blank then  --测空白
                    reactTime = config.measureParam.blankReactTime
                end

                local T = math.floor(reactTime)
                local refAD = 0
                local meaAD = 0
                local mTempSum = 0
                local bTempSum = 0
                local eTempSum = 0
                local measureTemp = dc:GetThermostatSignalTemp(0)
                local bacteriaTemp = dc:GetThermostatSignalTemp(1)
                local environmentTemp = dc:GetEnvironmentSignalTemp()

				--op.IOpticalAcquire:PDPowerOn()
				--log:debug("开启测量模块电源")

                for i = 1,T do
					--等待采集间隔时间
                    if not Measurer.flow:Wait(math.abs(setting.signal.readADIntervalTime - setting.signal.readADTime*3)) then
                        error(UserStopException:new())
                    end

					--开启测量模块电源
					op.IOpticalAcquire:PDPowerOn()
					log:debug("开启测量模块电源")

					if not Measurer.flow:Wait(setting.signal.readADTime) then
                        error(UserStopException:new())
                    end

					if i == T then
						Measurer.reactAD[i] = op:AcquirerFilterAD(setting.signal.readADTime * 2)
					else
						Measurer.reactAD[i] = op:AcquirerFilterAD(setting.signal.readADTime)
					end

                    refAD = Measurer.reactAD[i]:GetReference()
                    meaAD = Measurer.reactAD[i]:GetMeasure()
                    mTempSum = mTempSum + dc:GetThermostatSignalTemp(0)
                    bTempSum = bTempSum + dc:GetThermostatSignalTemp(1)
                    eTempSum = eTempSum + dc:GetEnvironmentSignalTemp()
                    log:debug("采集反应过程光强AD: C["..i.."] = "..refAD.." S["..i.."] = "..meaAD)
                    if i == T then
                        break
                    end
 
					--关闭测量模块电源
					op.IOpticalAcquire:PDPowerOff()
					log:debug("关闭测量模块电源")
                end

                Measurer.measureResult.bacteriaTemp = bTempSum/T
                Measurer.measureResult.measureTemp = mTempSum/T
                Measurer.measureResult.environmentTemp = eTempSum/T
                log:debug("反应过程平均温度  测量仓温度 = "..Measurer.measureResult.measureTemp..
                        "℃ 发光菌温度 = "..Measurer.measureResult.bacteriaTemp..
                        "℃ 环境温度 = "..Measurer.measureResult.environmentTemp.."℃")

                Measurer.measureResult.startReferenceAD = Measurer.reactAD[1]:GetReference()
                Measurer.measureResult.startMeasureAD = Measurer.reactAD[1]:GetMeasure()
                Measurer.measureResult.finalReferenceAD = Measurer.reactAD[T]:GetReference()
                Measurer.measureResult.finalMeasureAD = Measurer.reactAD[T]:GetMeasure()
                log:debug("反应开始光强 C1 = "..Measurer.measureResult.startReferenceAD.." S1 = "..Measurer.measureResult.startMeasureAD)
                log:debug("反应结束光强 Ct = "..Measurer.measureResult.finalReferenceAD.." St = "..Measurer.measureResult.finalMeasureAD)

                --关闭测量模块电源
                op.IOpticalAcquire:PDPowerOff()
                log:debug("关闭测量模块电源")

                log:debug("反应测量时间 = " .. os.time() - startTime)
            end
        },
        --10 返回结果
        {
            action = nil,
            execute = function()
                if Measurer.measureType == MeasureType.Check and Measurer.flow.isQuickMeasure == false then  --复苏菌品质检测
                    if Measurer.flow.BacteriaCheckHandle then
                        Measurer.flow:BacteriaCheckHandle(Measurer.measureResult)
                    end
                else
                    if Measurer.flow.ResultHandle then
                        Measurer.flow:ResultHandle(Measurer.measureResult)
                    end
                end
            end
        },
        --11 测量后清洗
        {
            action = setting.runAction.measure.cleanAfterMeasure,
            execute = function()
                local startTime = os.time()

                --排空测量槽
                op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 3)
                op:SyringeAspiration(0.9)
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
                op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom)
                op:SyringeAspiration(0.9)
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume * 2)
				op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)
				
				--开启测量仓排废液
                op:WasteRoomOutStart()

				--测量槽注水清洗
				op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 5)
				op:CleanNeedlePipe(3)

				op:AutoMove(setting.component.xPos.sampleMixer, 120)
                op:SyringeAspiration(0.6)
                op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 10)
                op:SyringeDischarge(0.6)
				op:CleanNeedlePipe(setting.clean.cleanVolume)

				--排空样品混合槽
                op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 3)
                op:SyringeAspiration(0.9)
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume)
				op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom)
                op:SyringeAspiration(0.9)
                op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
                op:SyringeReset()
                op:CleanNeedlePipe(setting.clean.cleanVolume)
				op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
                op:CleanNeedlePipe(setting.clean.clearVolume)

				--水样混合槽注水清洗
				op:AutoMove(setting.component.xPos.sampleMixer, config.componentParam.sampleMixer.bottom - 5)
				op:CleanNeedlePipe(2.5)

				if Measurer.measureType == MeasureType.Sample or
				   Measurer.measureType == MeasureType.Standard then

					for i = 1,config.measureParam.cleanAfterMeasureTimes do
						op:AutoMove(setting.component.xPos.measureCell, 120)
						op:SyringeAspiration(0.6)
						op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 10)
						op:SyringeDischarge(0.6)
						op:CleanNeedlePipe(setting.clean.cleanVolume)
					end

					--针管路排干测量槽
					op:AutoMove(setting.component.xPos.measureCell, setting.component.zPos.bottom)
					op:AirWasteNeedlePipe()

					--针管清洗
					op:AutoMove(setting.component.xPos.wasteCell, setting.component.zPos.side)
					op:SyringeReset()
					op:CleanNeedlePipe(setting.clean.cleanVolume * 3)
					op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
					op:CleanNeedlePipe(setting.clean.clearVolume)

					--测量槽注水清洗
					op:AutoMove(setting.component.xPos.measureCell, config.componentParam.measureCell.bottom - 5)
					op:CleanNeedlePipe(2.5)
				end

                --等待排废液时间
                if not Measurer.flow:Wait(1) then
                    error(UserStopException:new())
                end

                --关闭测量仓排废液
                op:WasteRoomOutStop()
                --清洗槽底部
                op:AutoMove(setting.component.xPos.cleanCell, setting.component.zPos.bottom)
				op:CleanNeedlePipe(setting.clean.cleanVolume)

                log:debug("测量后清洗时间 = " .. os.time() - startTime)
            end
        },
    },
}

function Measurer:Measure()
    if #self.procedure ~= 0  then
        print("--------------Execute configuration procedure--------------")
        for i, index in pairs(self.procedure) do
            print("index = ".. index)
            local step = self.steps[index]
            if step.action then
                local runAction = Helper.Status.SetAction(step.action)
                StatusManager.Instance():SetAction(runAction)
            end

            step.execute()
        end
    else
        print("--------------Execute default procedure--------------")
        for i, step in pairs(self.steps) do
            print("i = ".. i)
            if step.action then
                local runAction = Helper.Status.SetAction(step.action)
                StatusManager.Instance():SetAction(runAction)
            end

            step.execute()
        end
    end

    return Measurer.measureResult
end

function Measurer:Reset()
    Measurer.flow = nil
    Measurer.measureType = MeasureType.Sample
    Measurer.procedure = {}
    Measurer.reactAD = {}
    Measurer.darkReferenceAD = {}
    Measurer.darkMeasureAD = {}
    Measurer.reactStartTime = 0

    for k, v in pairs(Measurer.measureResult) do
        v = 0
    end

    for k, v in pairs(Measurer.reactAD) do
        v = 0
    end
end

function Measurer:GetMeasureResult()
    local dstTable = {}
    for k, v in pairs(Measurer.measureResult) do
        dstTable[k] = v
    end
    return dstTable
end

function Measurer:GetReactAD()
    local dstTable = {}
    for k, v in pairs(Measurer.reactAD) do
        dstTable[k] = v
    end
    return dstTable
end

