local _G = _ENV

local os = os
local pcall = pcall
local type = type
local log = log
local setting = setting
local config = config
local ExceptionHandler = ExceptionHandler
local MaterialGoBadException = MaterialGoBadException

local P = {}
MaterialLifeManager = P
_ENV = P

local isPumpPipeGoBad = false
local isPTFEPipeGoBad = false

function Reset(material)
    if material == setting.materialType.pumpPipe then
        isPumpPipeGoBad = false

    elseif material == setting.materialType.PTFEPipe then
        isPTFEPipeGoBad = false
    end
end

local function Check(material)

   local currentTime = os.time()

    if material == setting.materialType.pumpPipe then
        local temp = os.date("*t", config.consumable.pumpPipe.lastTime)
        temp.month = temp.month + config.consumable.pumpPipe.cycle

        local materialGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("MaterialLifeManager ==> Check(material) Error.")
            end
        else
            materialGoBadTime = result
            if materialGoBadTime - currentTime < 0 then
                ExceptionHandler.MakeAlarm(MaterialGoBadException:new({materialType = setting.materialType.pumpPipe,}))
                isPumpPipeGoBad = true
            end
        end

    elseif material == setting.materialType.PTFEPipe then
        local temp = os.date("*t", config.consumable.PTFEPipe.lastTime)
        temp.month = temp.month + config.consumable.PTFEPipe.cycle

        local materialGoBadTime = currentTime
        local err,result = pcall(function() return os.time(temp) end)
        if not err then
            if type(result) == "string" then
                log:warn(result)
            else
                log:warn("MaterialLifeManager ==> Check(material) Error.")
            end
        else
            materialGoBadTime = result
            if materialGoBadTime - currentTime < 0 then
                ExceptionHandler.MakeAlarm(MaterialGoBadException:new({materialType = setting.materialType.PTFEPipe,}))
                isPTFEPipeGoBad = true
            end
        end
    end

end

function CheckAllMaterialLife()

    if not isPumpPipeGoBad then
        Check(setting.materialType.pumpPipe)
    end

    if not isPTFEPipeGoBad then
        Check(setting.materialType.PTFEPipe)
    end
end

function RecoverMaterialLifeCheckStatus()
    isPumpPipeGoBad = false
    isPTFEPipeGoBad = false
end

return MaterialLifeManager
