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
#include <vector>
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
 * @brief 光学信号采集的AD值。
 * @details
 */
struct OpticalAD
{
    Uint32 reference;                ///光学信号采集的参考AD值
    Uint32 measure;                  ///光学信号采集的测量AD值
};

/**
 * @brief 光学信号采集接口。
 * @details 定义了一序列光学信号采集相关的操作。
 */
class LUIP_SHARE OpticalAcquireInterface : public DeviceInterface
{
public:
    OpticalAcquireInterface(DscpAddress addr);
    // 设置采集信号AD上报周期。
    bool SetAcquireADNotifyPeriod(float period);
    // 启动采集过程。
    bool StartAcquirer(float samplePeriod);
    // 停止采集过程。
    bool StopAcquirer();
    // 打开汞灯。
    bool TurnOnHgLamp();
    // 关闭汞灯。
    bool TurnOffHgLamp();
    // 打开LED。
    bool TurnOnLED();
    // 关闭LED。
    bool TurnOffLED();

    // 信号AD采集完成事件。
    Uint32 ExpectADAcquirer(long timeout, std::vector<OpticalAD> &ad);
    // 注册信号AD定时上报事件。
    void RegisterAcquireADNotice(IEventReceivable *handle);
};

}
}

#endif  //CONTROLLER_API_OPTICALACQUIREINTERFACE_H
