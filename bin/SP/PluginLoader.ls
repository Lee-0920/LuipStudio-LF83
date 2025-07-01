--[[
 * @brief 驱动器相关文件载入的条目。
--]]

package.path = [[?;?.lua;?.ls;?;./SP/Common/?.ls;./SP/Controller/?.ls;]]
require("ExceptionHandler")
require("ReagentRemainManager")
require("MaterialLifeManager")
require("PeristalticPump")
require("Motor")
require("Thermostat")
require("Operator")
require("ControllerCreater")


CreateControllers()     -- 创建各个驱动控制器对象，加入全局表
CreatePumps()           -- 创建各个液路的泵对象
CreateMotors()           -- 创建X轴Z轴电机对象
CreateThermostats()      -- 创建各个恒温器对象
CreateOperator() 	-- 创建各个液路、温度、信号操作方法的泵对象

setting.ui.resultDetail.measure.widget = LF82ResultDetailWidget.new()
ResultDetailManager.Instance():SetMeasureResultDetailWidget(setting.ui.resultDetail.measure.widget)