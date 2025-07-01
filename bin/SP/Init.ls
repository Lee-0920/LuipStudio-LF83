log = Log.Instance():GetLogger()

package.path = [[?;?.lua;?.ls;?;./SP/Common/?.ls;./SP/Setting/?.ls]]
require("Serialization")
require("Setting")
package.path = [[?;?.lua;?.ls;?;./SP/Common/?.ls;./SP/Setting/?.ls;./SP/Config/?.ls]]
require("ControlNetException")
require("MeasureFlowException")

--脚本文件夹所在路径
scriptsPath = "./SP"
require("ConfigLists")
require("ConfigModifyRecord")

function ArguUnitChange(value, changeType)
     local ret = value
     local unitIndex = config.system.unitIndex
     if (unitIndex == 1 and setting.unit.valid == true) or (setting.unit.valid == false and unitIndex == 0)then
         if type(value) == "string" then
             if value == "" then
                 ret = "%"
             end
         elseif type(value) == "number" then
            if changeType == UnitChange.Read then
                ret = value*setting.unit[2].multiple
--                print("read value = " .. value .. ", ret = ", ret)
            elseif changeType == UnitChange.Write then
                ret = value/setting.unit[2].multiple
--                print("write value = " .. value .. ", ret = ", ret)
            end
         end
     end

     return ret
end
