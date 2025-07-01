#if !defined(CONTROLLER_API_MOTORCONTROLINTERFACE_H)
#define CONTROLLER_API_MOTORCONTROLINTERFACE_H

#include "ControllerPlugin/API/DeviceInterface.h"
#include "System/Types.h"
#include "../LuipShare.h"

using System::Uint8;

namespace Controller
{
namespace API
{

/**
 * @brief 步进电机运动参数。
 * @details
 */
struct MotorParam
{
    float acceleration;           ///加速度，单位为 步/平方秒
    float speed;                  ///最大速度，单位为 步/秒
};


/**
 * @brief 电机状态。
 * @details
 */
enum class MotorStatus
{
    Idle,                   ///电机空闲
    Failed,                 ///电机异常
    Busy                    ///电机忙碌
};

/**
 * @brief 电机运动方向。
 * @details
 */
enum class MotorDirection
{
    Front,                      ///正向操作
    Back,                       ///反向操作
};

/**
 * @brief 电机操作结果码。
 * @details
 */
enum class MotorOperateResult
{
    Finished,                   /// 电机操作正常完成
    Failed,                     /// 电机操作中途出现故障，未能完成
    Stopped,                    /// 电机操作被停止
    Collsion_0,                 /// 针碰撞
    Collsion_1,                 /// 针碰撞
    MoveOutSensorFail_X,        /// X轴移不出传感器
    MoveOutSensorFail_Z,        /// Z轴移不出传感器
    MoveInSensorFail_X,         /// X轴找不到传感器
    MoveInSensorFail_Z,         /// Z轴找不到传感器
    MoveOutSensorFail_SYR,      /// 注射器移不出传感器
    MoveInSensorFail_SYR,       /// 注射器找不到传感器
    DriverError,                /// 驱动错误
};

/**
 * @brief 电机运动模式
 * @details
 */
enum class MotorMoveMode
{
    Absolute,                   ///绝对模式
    Relative,                   ///相对模式
    Safe                        ///安全模式
};

/**
 * @brief 电机操作结果。
 * @details
 */
struct MotorResult
{
    Uint8 index;        /// 产生事件的电机索引
    int result;         /// 电机操作结果码,赋值为MotorOperateResult。
};

/**
 * @brief 步进电机控制接口。
 * @details 定义了一序列步进电机控制相关的操作。
 */
class LUIP_SHARE MotorControlInterface : public DeviceInterface
{
public:
    MotorControlInterface(DscpAddress addr);

    // 查询系统支持的电机数目。
    Uint16 GetTotalMotors();
    // 查询指定电机的运动参数。
    MotorParam GetMotorParam(int index);
    // 设置指定电机的运动参数。
    bool SetMotorParam(int index, MotorParam param);
    // 查询指定电机的工作状态。
    MotorStatus GetMotorStatus(int index);
    // 获取指定电机最大移动步数
    Uint16 GetMotorMaxSteps(int index);
    // 获取电机初始位置步数
    Uint16 GetMotorInitSteps(int index);
    // 获取电极当前位置步数
    Uint16 GetMotorCurrentSteps(int index);
    // 电机运动
    bool MotorMove(int index, int steps, MotorMoveMode mode);
    // 停止电机
    bool MotorStop(int index);
    // 电机复位
    bool MotorReset(int index);
    // 获取指定传感器状态
    bool GetSensorStatus(int index);
    // 所有电机驱动初始化
    bool MotorDriverReinit();
    // 指定电机状态检查
    Uint8 MotorDriverCheck(Uint8 index);


    //电机操作结果事件。
    //
    //电机运动操作结束时将产生该事件。
    MotorResult ExpectMotorResult(long timeout);


};

}

}

#endif // CONTROLLER_API_MOTORCONTROLINTERFACE_H
