/**
 * @file
 * @brief 光学信号采集接口。
 * @details 
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2015/3/7
 */


#if !defined(CONTROLLER_API_OPTICALACQUIREINTERFACE_H)
#define CONTROLLER_API_OPTICALACQUIREINTERFACE_H

#include "../LuipShare.h"
#include "Communication/IEventReceivable.h"
#include "ControllerPlugin/API/DeviceInterface.h"
#include "System/Types.h"

using System::Uint16;
using System::Uint32;
using Communication::IEventReceivable;

namespace Controller
{
namespace API
{    

/**
 * @brief 双通道测量信号类型
 * @details
 */
typedef enum
{
    MEASURE_TYPE_NONE = 0,   /// 未处于测量状态
    MEASURE_TYPE_TP,         /// 测试总磷
    MEASURE_TYPE_TN,         /// 测量总氮
}enumMeasureType;

/**
 * @brief 光学信号采集的AD值。
 * @details
 */
struct OpticalAD
{
    Uint32 reference;                ///光学信号采集的参考AD值
    Uint32 measure;                  ///光学信号采集的测量AD值
};

/**
 * @brief LED控制参数。
 * @details
 */
struct LedParam
{
    float proportion;     // PID的比例系数
    float integration;    // PID的积分系数
    float differential;   // PID的微分系数
};

/**
 * @brief 光学信号采集接口。
 * @details 定义了一序列光学信号采集相关的操作。
 */
class LUIP_SHARE OpticalAcquireInterface : public DeviceInterface
{
public:
    OpticalAcquireInterface(DscpAddress addr);
    // 打开LED灯。
    bool TurnOnLED();
    // 关闭LED灯。
    bool TurnOffLED();
    // 打开氙灯。
    bool TurnOnXonen();
    // 关闭氙灯。
    bool TurnOffXonen();
    // 设置采集信号AD上报周期。
    bool SetAcquireADNotifyPeriod(float period);
    // 启动采集过程。
    bool StartAcquirer(float acquireTime);
    // 停止采集过程。
    bool StopAcquirer();

    // 信号AD采集完成事件。
    OpticalAD ExpectADAcquirer(long timeout);


    // 注册信号AD定时上报事件。
    void RegisterAcquireADNotice(IEventReceivable *handle);
};

}
}

#endif  //CONTROLLER_API_OPTICALACQUIREINTERFACE_H
