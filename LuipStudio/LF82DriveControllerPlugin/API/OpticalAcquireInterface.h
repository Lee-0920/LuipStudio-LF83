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
#include <vector>

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
 * @brief 光学信号采集结果码。
 * @details
 */
enum OpticalAcquireResult
{
    Finished,                    ///光学信号采集操作正常完成；
    Failed,                     ///光学信号采集操作失败。
    Stopped                     ///光学信号采集操作被终止。
};

/**
 * @brief 静态AD调节控制结果。
 */
enum class StaticADControlResult
{
    Unfinished = 0,       ///<静态AD调节未完成。
    Finished = 1,     ///<  静态AD调节完成。
};

/**
 * @brief 光学信号采集的结果。
 * @details
 */
struct AcquiredResult
{
    OpticalAD ad;
    int result;         // 光学信号采集结果码,赋值为AcquiredOperateResult
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
    bool StartAcquirer(float acquireTime);

    // 停止采集过程。
    bool StopAcquirer();

    // 设置采集增益
    bool SetGain(int index, int gainCoefficient);

    // 获取采集增益
    int GetGain(int index);

    // PD板电源开启
    bool PDPowerOn();

    // PD板电源关闭
    bool PDPowerOff();

    // 启动静态AD电位控制
    bool StartStaticADControl(Uint8 index, Uint32 targetAD);
    // 停止静态AD电位控制
    bool StopStaticADControl();
    // 获取目标器件电位默认参数
    Uint16 GetStaticADControlParam(Uint8 index) const;
    // 设置目标器件电位默认参数
    bool SetStaticADControlParam(Uint8 index, Uint16 value);
    // 静态AD调节功能是否有效
    bool IsStaticADControlValid() const;

    //信号AD采集操作结果事件。
    AcquiredResult ExpectADAcquirer(long timeout);

    //启动调节后，调节完成将产生该事件。
    StaticADControlResult ExpectStaticADControlResult(long timeout);

    // 注册信号AD定时上报事件。
    void RegisterAcquireADNotice(IEventReceivable *handle);
};

}
}

#endif  //CONTROLLER_API_OPTICALACQUIREINTERFACE_H
