﻿/**
 * @file
 * @brief 温度控制接口。
 * @details 
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2015/3/7
 */


#if !defined(CONTROLLER_API_TEMPERATURECONTROLINTERFACE_H)
#define CONTROLLER_API_TEMPERATURECONTROLINTERFACE_H

#include "Communication/IEventReceivable.h"
#include "ControllerPlugin/API/DeviceInterface.h"
#include "System/Types.h"
#include "../LuipShare.h"

using System::Uint16;
using Communication::IEventReceivable;

namespace Controller
{
namespace API
{    

/**
 * @brief 温度。
 * @details
 */
struct Temperature
{
    float thermostatTemp;     // 恒温室温度，单位为摄氏度
    float environmentTemp;    // 环境温度，单位为摄氏度
};

/**
 * @brief 温度控制参数。
 * @details
 */
struct ThermostatParam
{
    float proportion;     // PID的比例系数
    float integration;    // PID的积分系数
    float differential;   // PID的微分系数
};

/**
 * @brief 温度校准参数。
 * @details
 */
struct TempCalibrateFactor
{
    float negativeInput;           ///<负输入分压
    float referenceVoltage;        ///<参考电压
    float calibrationVoltage;      ///<校准电压
};

/**
 * @brief 恒温模式。
 */
enum class ThermostatMode
{
    Auto = 0,           ///< 自动模式，根据需要及硬件能力综合使用加热器和制冷器。
    Heater = 1,         ///< 纯加热模式，不使用制冷器。
    Refrigerate= 2,     ///< 纯制冷模式，不使用加热器。
    Natural = 3         ///< 自然模式，加热器和制冷器都不参与，靠环境传递热量，自然升温或冷却。
};

/**
 * @brief 恒温操作结果。
 */
enum class ThermostatOperateResult
{
    Reached = 0,    // 恒温目标达成，目标温度在规定时间内达成，后续将继续保持恒温，直到用户停止。
    Failed = 1,     // 恒温中途出现故障，未能完成。
    Stopped= 2,     // 恒温被停止。
    Timeout= 3      // 恒温超时，指定时间内仍未达到目标温度。
};
/**
 * @brief 恒温结果。
 * @details
 */
struct ThermostatResult
{
    int result;     // 恒温操作结果。
    float temp;                       // 当前温度。
};


/**
 * @brief 温度控制接口。
 * @details 定义了一序列温度控制相关的操作。
 */
class LUIP_SHARE TemperatureControlInterface : public DeviceInterface
{
public:
    TemperatureControlInterface(DscpAddress addr);
    // 查询温度传感器的校准系数。
    TempCalibrateFactor GetCalibrateFactor();
    // 设置温度传感器的校准系数。
    bool SetCalibrateFactor(TempCalibrateFactor factor);
    // 查询当前温度。
    Temperature GetTemperature();
    // 查询恒温控制参数。
    ThermostatParam GetThermostatParam();
    // 设置恒温控制参数。
    bool SetThermostatParam(ThermostatParam param);
    // 查询恒温器的工作状态。
    Uint16 GetThermostatStatus();
    // 开始恒温。
    bool StartThermostat(ThermostatMode mode, float targetTemp, float toleranceTemp, float timeout);
    // 停止恒温控制。
    bool StopThermostat();
    // 设置温度上报周期。
    bool SetTemperatureNotifyPeriod(float period);
    // 恒温结果事件。
    ThermostatResult ExpectThermostat(long timeout);
    // 注册温度定时上报事件。
    void RegisterTemperatureNotice(IEventReceivable *handle);
    //机箱风扇输出
    bool BoxFanSetOutput(float level);
};

}
}

#endif  //CONTROLLER_API_TEMPERATURECONTROLINTERFACE_H
