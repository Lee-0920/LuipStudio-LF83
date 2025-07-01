
function  CreateControllers()
    local controllerManager = ControllerManager.Instance()
    for i, controller in pairs(setting.plugins.controllers) do
        local name = controller.name
        local text = controller.text
        local addr = DscpAddress.new(controller.address[1],  controller.address[2],  controller.address[3], controller.address[4])
        if type(name)  == "string" then
            if name == "LF82DriveControllerPlugin"then
                    dc = LF82DriveController.new(addr)
                    controllerManager:AddController(name, text, dc)
            end
        end

        addr = nil
    end
end

function  CreatePumps()

    pumps = {}
    for _,v in pairs(setting.liquidType)  do
        if v ~= setting.liquidType.map then
            if pumps[v.pump +1]  == nil then
                pump = PeristalticPump:new()
                pump.index =  v.pump
                pump.isRunning = false
                pump.peristalticPumpInterface = dc:GetIPeristalticPump()
                pumps[v.pump +1] = pump
            end
        end
    end
end

function CreateMotors()
    --motor x
    motorX = Motor:new()
    motorX.index = 0
    motorX.isRunning = false
    motorX.motorControlInterface = dc:GetIMotorControl()
    motorX.localPos = 9999

    --motor z
    motorZ = Motor:new()
    motorZ.index = 1
    motorZ.isRunning = false
    motorZ.motorControlInterface = dc:GetIMotorControl()
    motorZ.localPos = 9999
end

function CreateThermostats()

    thermostats = {}

    thermostats.measure = Thermostat:new()
    thermostats.measure.index =  0
    thermostats.measure.isRunning = false
    thermostats.measure.temperatureControlInterface = dc:GetITemperatureControl()
    thermostats.measure.refrigeratorIndex = 0
    thermostats.measure.heaterIndex = 7
    thermostats.measure.fanIndexList = {3,5}

    thermostats.bacteria = Thermostat:new()
    thermostats.bacteria.index =  1
    thermostats.bacteria.isRunning = false
    thermostats.bacteria.temperatureControlInterface = dc:GetITemperatureControl()
    thermostats.bacteria.refrigeratorIndex = 1
    thermostats.bacteria.heaterIndex = nil
    thermostats.bacteria.fanIndexList = {2,4}

end

function  CreateOperator()
    op = Operator:new()
end

function  StopOperator()
    op:Stop()
end

