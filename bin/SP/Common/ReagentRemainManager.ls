local _G = _ENV
local setting = setting
local status = status
local config = config
local Serialization = Serialization
local scriptsPath = scriptsPath
local ReagentRemainWarn = ReagentRemainWarn
local ExceptionHandler = ExceptionHandler
local ConfigLists = ConfigLists
local print = print
local os = os
local pcall = pcall
local type = type
local log = log
local ReagentRemainVector = ReagentRemainVector
local ReagentRemain = ReagentRemain
local WqimcManager = WqimcManager
local P = {}
ReagentRemainManager = P
_ENV = P

local isBacteriaEnough = true    -- 发光菌充足
local isSaltEnough = true        -- 渗透压调节液充足
local isStandardEnough = true    -- 质控液充足
local isBlankEnough = true       -- 空白水充足
local isWasteEnough = true       -- 废液充足

local isBacteriaBad = false
local isSaltBad = false
local isStandardBad = false
local isBlankBad = false
local isWasteBad = false

local REMAIN = 0.1
local PRECISE = 0.000001

function ChangeReagent(reagent, vol)

    if reagent == setting.liquidType.bacteria then
        status.remain.bacteria = vol
        ConfigLists.SaveRemainStatus()
        isBacteriaEnough = true
        isBlankBad = false

    elseif reagent == setting.liquidType.salt then
        status.remain.salt = vol
        ConfigLists.SaveRemainStatus()
        isSaltEnough = true
        isSaltBad = false

    elseif reagent == setting.liquidType.standard then
        status.remain.standard = vol
        ConfigLists.SaveRemainStatus()
        isStandardEnough = true
        isStandardBad = false

    elseif reagent == setting.liquidType.blank then
        status.remain.blank = vol
        ConfigLists.SaveRemainStatus()
        isBlankEnough = true
        isBlankBad = false

    elseif reagent == setting.liquidType.waste then
        status.remain.waste = vol
        ConfigLists.SaveRemainStatus()
        isWasteEnough = true
        isWasteBad = false

    end
end

local function Check(reagent)

    local currentTime = os.time()

    if reagent == setting.liquidType.blank then
        local temp = os.date("*t", config.remain.blank.lastTime)
        temp.month = temp.month + config.remain.blank.cycle

        local reagentGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then  --出现异常
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("ReagentRemainManager ==> Check(reagent) Error.")
            end
        else
            reagentGoBadTime = result
            if reagentGoBadTime - currentTime < 0 then
                local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.blank,})
                ExceptionHandler.MakeAlarm(exception)
                isBlankBad = true
            end
        end

    elseif reagent == setting.liquidType.standard then
        local temp = os.date("*t", config.remain.standard.lastTime)
        temp.month = temp.month + config.remain.standard.cycle

        local reagentGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then  --出现异常
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("ReagentRemainManager ==> Check(reagent) Error.")
            end
        else
            reagentGoBadTime = result
            if reagentGoBadTime - currentTime < 0 then
                local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.standard,})
                ExceptionHandler.MakeAlarm(exception)
                isStandardBad = true
            end
        end

    elseif reagent == setting.liquidType.bacteria then
        local temp = os.date("*t", config.remain.bacteria.lastTime)
        temp.month = temp.month + config.remain.bacteria.cycle

        local reagentGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then  --出现异常
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("ReagentRemainManager ==> Check(reagent) Error.")
            end
        else
            reagentGoBadTime = result
            if reagentGoBadTime - currentTime < 0 then
                local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.bacteria,})
                ExceptionHandler.MakeAlarm(exception)
                isBacteriaBad = true
            end
        end

    elseif reagent == setting.liquidType.salt then
        local temp = os.date("*t", config.remain.salt.lastTime)
        temp.month = temp.month + config.remain.salt.cycle

        local reagentGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then  --出现异常
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("ReagentRemainManager ==> Check(reagent) Error.")
            end
        else
            reagentGoBadTime = result
            if reagentGoBadTime - currentTime < 0 then
                local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.salt,})
                ExceptionHandler.MakeAlarm(exception)
                isSaltBad = true
            end
        end

    elseif reagent == setting.liquidType.waste then
        local temp = os.date("*t", config.remain.waste.lastTime)
        temp.month = temp.month + config.remain.waste.cycle

        local reagentGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then  --出现异常
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("ReagentRemainManager ==> Check(reagent) Error.")
            end
        else
            reagentGoBadTime = result
            if reagentGoBadTime - currentTime < 0 then
                local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.waste,})
                ExceptionHandler.MakeAlarm(exception)
                isWasteBad = true
            end
        end

    end

end

function CheckAllReagentDate()

    if config.system.reagentLackWarn == true then
        if not isBlankBad and isBlankEnough then

            Check(setting.liquidType.blank)
        end

        if not isStandardBad and isBacteriaEnough then

            Check(setting.liquidType.bacteria)
        end

        if not isStandardBad and isSaltEnough then

            Check(setting.liquidType.salt)
        end

        if not isStandardBad and isStandardEnough then

            Check(setting.liquidType.standard)
        end

        if not isWasteBad and isWasteEnough then

            Check(setting.liquidType.waste)
        end
    end
end

local function ReagentRemainCheck()

    if config.system.reagentLackWarn == true then
        if (REMAIN * config.remain.blank.total - status.remain.blank >= PRECISE) and isBlankEnough and not isBlankBad then
            isBlankEnough = false
            local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.blank,})
            ExceptionHandler.MakeAlarm(exception)
        end

        if (REMAIN * config.remain.standard.total  - status.remain.standard >= PRECISE) and isStandardEnough and not isStandardBad then
            isStandardEnough = false
            local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.standard,})
            ExceptionHandler.MakeAlarm(exception)
        end

        if (REMAIN * config.remain.bacteria.total  - status.remain.bacteria >= PRECISE) and isBacteriaEnough and not isBacteriaBad then
            isBacteriaEnough = false
            local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.bacteria,})
            ExceptionHandler.MakeAlarm(exception)
        end

        if (REMAIN * config.remain.salt.total  - status.remain.salt >= PRECISE) and isSaltEnough and not isSaltBad then
            isSaltEnough = false
            local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.salt,})
            ExceptionHandler.MakeAlarm(exception)
        end

        if (REMAIN * config.remain.waste.total  - status.remain.waste >= PRECISE) and isWasteEnough and not isWasteBad then
            isWasteEnough = false
            local exception = ReagentRemainWarn:new({liquidType = setting.liquidType.waste,})
            ExceptionHandler.MakeAlarm(exception)
        end
    end
end

function ReduceReagentRemain(reagentRemain, vol)
    if reagentRemain == status.remain.blank then
        status.remain.blank = status.remain.blank - vol
        if status.remain.blank < 0 then
            status.remain.blank = 0
        end
    elseif reagentRemain == status.remain.standard then
        status.remain.standard = status.remain.standard - vol
        if status.remain.standard < 0 then
            status.remain.standard = 0
        end
    elseif reagentRemain == status.remain.bacteria then
        status.remain.bacteria = status.remain.bacteria - vol
        if status.remain.bacteria < 0 then
            status.remain.bacteria = 0
        end
    elseif reagentRemain == status.remain.salt then
        status.remain.salt = status.remain.salt - vol
        if status.remain.salt < 0 then
            status.remain.salt = 0
        end
    end

    if reagentRemain ~= status.remain.waste then
        status.remain.waste = status.remain.waste - vol
        if status.remain.waste < 0 then
            status.remain.waste = 0
        end
    end

    ReagentRemainCheck()
end

function RecoverRemainCheckStatus()
    isBacteriaEnough = true    -- 发光菌充足
    isSaltEnough = true        -- 渗透压调节液充足
    isStandardEnough = true    -- 质控液充足
    isBlankEnough = true       -- 空白水充足
    isWasteEnough = true       -- 废液充足

    isBacteriaBad = false
    isSaltBad = false
    isStandardBad = false
    isBlankBad = false
    isWasteBad = false
end

function SaveRemainStatus()
    ConfigLists.SaveRemainStatus()

    local reagentRemain = ReagentRemain.new()
    local reagentRemainVector = ReagentRemainVector.new()

    reagentRemain:__set_name("bacteria")
    reagentRemain:__set_volume(status.remain.bacteria)
    reagentRemainVector:Push(reagentRemain)

    reagentRemain:__set_name("salt")
    reagentRemain:__set_volume(status.remain.salt)
    reagentRemainVector:Push(reagentRemain)

    reagentRemain:__set_name("standard")
    reagentRemain:__set_volume(status.remain.standard)
    reagentRemainVector:Push(reagentRemain)

    reagentRemain:__set_name("blank")
    reagentRemain:__set_volume(status.remain.blank)
    reagentRemainVector:Push(reagentRemain)

    reagentRemain:__set_name("waste")
    reagentRemain:__set_volume(status.remain.waste)
    reagentRemainVector:Push(reagentRemain)

    if reagentRemainVector:Size() > 0 then
        WqimcManager.Instance():updateReagentRemain(reagentRemainVector)
    end

end

return ReagentRemainManager
