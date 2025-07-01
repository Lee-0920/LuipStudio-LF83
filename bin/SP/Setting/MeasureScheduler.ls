setting.measureScheduler=
{
    -- 1  测量水样  --
    {
        name ="MeasureSample",
        text = "测量水样",
        
        --开启排期 
        isOpen = function()
            local open =false
            local timedMeasureMode = false

            local mode = config.scheduler.measure.mode
            
            if mode == MeasureMode.Periodic then                            -- 周期测量
                open =true
                timedMeasureMode = false
            elseif mode == MeasureMode.Timed then                       -- 整点测量
                open =true
                timedMeasureMode = true
            elseif mode == MeasureMode.Continous then                    -- 连续测量
                open =true
                timedMeasureMode = false
            elseif mode == MeasureMode.Trigger then                  -- 触发测量
                open =false
                timedMeasureMode = false
            end
            
            return open, timedMeasureMode
        end,
        
        --优先级
        getPriority= function()
            local priority = 1
            
            if config.scheduler.measure.mode == MeasureMode.Continous then
                priority = 20
            end
            
            return priority
        end,
        
        --排期周期
        getInterval = function()
            local interva = 0
            
            local mode = config.scheduler.measure.mode
            
            if mode == MeasureMode.Periodic then                            -- 周期测量
                interva =  config.scheduler.measure.interval
            elseif mode == MeasureMode.Timed then                       -- 整点测量
                    interva = 0
            elseif mode == MeasureMode.Continous then                    -- 连续测量
                local runTime = setting.runStatus.measureSample.GetTime()
                interva = runTime/3600
            end
            
            return interva
        end,
        
        --上次启动时间
        getLastTime = function()
            return status.measure.schedule.autoMeasure.dateTime
        end,
        
        --下次启动时间
        getNextTime = function()
            local isValid = false
            local nextTime = 0
            local timeList = {}
            
            local mode = config.scheduler.measure.mode
            
            if mode == MeasureMode.Periodic then                            -- 周期测量
                nextTime = status.measure.schedule.autoMeasure.dateTime + 3600 * config.scheduler.measure.interval
                isValid = true
            elseif mode == MeasureMode.Timed then                       -- 整点测量
                for i = 1,#config.scheduler.measure.timedPoint do
                    if config.scheduler.measure.timedPoint[i] == true then
                        table.insert(timeList, i - 1)
                    end
                end
                
                local curDateTime = os.date("*t", os.time()) 
                
                if #timeList > 0 then
                    local ret = false
                    for i = 1,#timeList do
                        if curDateTime.hour == timeList[i] and curDateTime.min == 0 and curDateTime.sec <= 5 then  --避免刚好这个函数调用时过了0秒
                            curDateTime.hour = timeList[i]  
                            curDateTime.min = 0 
                            curDateTime.sec = 0

                            ret = true
                            break
                        elseif curDateTime.hour < timeList[i]  then
                            curDateTime.hour = timeList[i]  
                            curDateTime.min = 0 
                            curDateTime.sec = 0

                            ret = true
                            break
                        end
                    end

                    if ret == false then
                        curDateTime.day = curDateTime.day  + 1  
                        curDateTime.hour = timeList[1]  
                        curDateTime.min = 0 
                        curDateTime.sec = 0
                    end
                
                    nextTime = os.time(curDateTime)
                    isValid = true
                end
            elseif mode == MeasureMode.Continous then -- 连续测量
                nextTime = os.time()
                isValid = true
            elseif mode == MeasureMode.Trigger then -- 触发测量
                nextTime = 0
                isValid = true
            end
            
            return isValid, nextTime
        end,
        
        --掉电重测
        isRetry = function()
            if status.running.isMeasuring == true and status.running.lastMeasureType == MeasureType.Sample then
                log:debug("MeasureSample Restart!")
                return true
            else
                return false
            end
        end,
        
        --预测下次启动时间
        calculateNextTime = function(startTime, runTime)
            local isValid = false
            local nextTime = 0
            local mode = config.scheduler.measure.mode
            
            if mode == MeasureMode.Periodic then                            -- 周期测量
                nextTime = startTime  + 3600 * config.scheduler.measure.interval
                isValid = true
            elseif mode == MeasureMode.Timed then                       -- 整点测量
                local timeList = {}

                for i = 1,#config.scheduler.measure.timedPoint do
                    if config.scheduler.measure.timedPoint[i] == true then
                        table.insert(timeList, i - 1)
                    end
                end
                                
                local dateTime = os.date("*t", startTime + runTime)
                
                if #timeList > 0 then
                    local ret = false
                    for i = 1,#timeList do
                        if dateTime.hour == timeList[i] and dateTime.min == 0 and dateTime.sec == 0 then
                            dateTime.hour = timeList[i]     
                            dateTime.min = 0    
                            dateTime.sec = 0

                            ret = true
                            break
                        elseif dateTime.hour < timeList[i]  then
                            dateTime.hour = timeList[i]     
                            dateTime.min = 0    
                            dateTime.sec = 0

                            ret = true
                            break
                        end
                    end

                    if ret == false then
                        dateTime.day = dateTime.day  + 1    
                        dateTime.hour = timeList[1]     
                        dateTime.min = 0    
                        dateTime.sec = 0
                    end

                    nextTime = os.time(dateTime)    
                    isValid = true
                end
            elseif mode == MeasureMode.Continous then                    -- 连续测量
                local runTime = setting.runStatus.measureSample.GetTime()
                nextTime = startTime  + runTime
                isValid = true
            end
            
            return isValid, nextTime
        end,
        
        --流程运行用时
        getRunTime = function()
            return setting.runStatus.measureSample.GetTime()
        end,
        
        --创建流程
        createFlow = function()
            local flow = MeasureFlow:new({}, MeasureType.Sample)
            flow.name  = setting.measureScheduler[1].name
            if status.running.isMeasuring == true then
                local osRunTime = App.GetOSRunTime()
                if osRunTime > 300 then
                    flow.isCrashMeasure = true --程序崩溃重启
                end
            end
            FlowList.AddFlow(flow)
        end,
    },
    -- 2 质控  --
    {
        name ="MeasureStandard ",
        text = "质控",
        
        --开启排期 
        isOpen = function()
            return config.scheduler.check.enable, false
        end,
        
        --优先级
        getPriority= function()
            return 2
        end,
        
        --排期周期
        getInterval = function()
            return config.scheduler.check.interval
        end,
        
        --上次启动时间
        getLastTime = function()
            return status.measure.schedule.autoCheck.dateTime
        end,
        
        --下次启动时间
        getNextTime = function()
            return true, status.measure.schedule.autoCheck.dateTime + 3600 * config.scheduler.check.interval
        end,
        
        --掉电重测
        isRetry = function()
            if status.running.isMeasuring == true and status.running.lastMeasureType == MeasureType.Standard then
                log:debug("MeasureStandard Restart!")
                return true
            else
                return false
            end
        end,
        
        --预测下次启动时间
        calculateNextTime = function(startTime, runTime)    
            local nextTime = startTime  + 3600 * config.scheduler.check.interval
            return true, nextTime
        end,
        
        --流程运行用时
        getRunTime = function()
            return setting.runStatus.measureStandard.GetTime()
        end,
        
        --创建流程
        createFlow = function()
            local flow = MeasureFlow:new({}, MeasureType.Standard)
            flow.name  = setting.measureScheduler[2].name
            FlowList.AddFlow(flow)
        end,
    },
    -- 3 空白水  --
    {
        name ="MeasureBlank",
        text = "空白水",

        --开启排期
        isOpen = function()
            return config.scheduler.blank.enable, false
        end,

        --优先级
        getPriority= function()
            return 3
        end,

        --排期周期
        getInterval = function()
            return config.scheduler.blank.interval
        end,

        --上次启动时间
        getLastTime = function()
            return status.measure.schedule.autoBlank.dateTime
        end,

        --下次启动时间
        getNextTime = function()
            return true, status.measure.schedule.autoBlank.dateTime + 3600 * config.scheduler.blank.interval
        end,

        --掉电重测
        isRetry = function()
            if status.running.isMeasuring == true and status.running.lastMeasureType == MeasureType.Blank then
                log:debug("MeasureBlank Restart!")
                return true
            else
                return false
            end
        end,

        --预测下次启动时间
        calculateNextTime = function(startTime, runTime)
            local nextTime = startTime  + 3600 * config.scheduler.blank.interval
            return true, nextTime
        end,

        --流程运行用时
        getRunTime = function()
            return setting.runStatus.measureBlank.GetTime()
        end,

        --创建流程
        createFlow = function()
            local flow = MeasureFlow:new({}, MeasureType.Blank)
            flow.name  = setting.measureScheduler[3].name
            FlowList.AddFlow(flow)
        end,
    },
    --4 清洗  --
    {
        name ="Clean ",
        text = "清洗",

        --开启排期
        isOpen = function()
            return config.scheduler.clean.enable, false
        end,

        --优先级
        getPriority= function()
            return 6
        end,

        --排期周期
        getInterval = function()
            return config.scheduler.clean.interval
        end,

        --上次启动时间
        getLastTime = function()
            return status.measure.schedule.autoClean.dateTime
        end,

        --下次启动时间
        getNextTime = function()
            return true, status.measure.schedule.autoClean.dateTime + 3600 * config.scheduler.clean.interval
        end,

        --掉电重测
        isRetry = function()
            return false
        end,

        --预测下次启动时间
        calculateNextTime = function(startTime, runTime)
            local nextTime = startTime  + 3600 * config.scheduler.clean.interval
            return true, nextTime
        end,

        --流程运行用时
        getRunTime = function()
            return setting.runStatus.cleanAll.GetTime()
        end,

        --创建流程
        createFlow= function()
            local flow = CleanFlow:new({})
            flow.name  = setting.measureScheduler[4].name
            FlowList.AddFlow(flow)
        end,
    },
}