/**

 * @file
 * @brief 步进电机控制接口。
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2015/3/7
 */

#include "Code/MotorControlInterface.h"
#include "Communication/Dscp/DscpStatus.h"
#include "Communication/SyncCaller.h"
#include "Communication/EventHandler.h"
#include "MotorControlInterface.h"

using namespace std;
using namespace Communication;
using namespace Communication::Dscp;
using namespace System;

namespace Controller
{
namespace API
{

/**
 * @brief 电机控制接口构造。
 * @param[in] addr 设备地址。
 */
MotorControlInterface::MotorControlInterface(DscpAddress addr)
    : DeviceInterface(addr)
{

}

/**
 * @brief 查询系统操作的步进电机数目。
 * @return 总电机数目， Uint16。
 */
Uint16 MotorControlInterface::GetTotalMotors()
{
    Uint16 motors = 0;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_TOTAL_MOTORS));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        motors = *((Uint16*)resp->data);
    }

    return motors;
}

/**
 * @brief 查询指定电机的运动参数。
 * @details 系统将根据设定的运动参数进行运动控制和规划。
 * @param index Uint8，要查询的电机索引。
 * @return 运动参数， 数据格式为：
 *  - acceleration Float32，加速度，单位:步/平方秒。
 *  - speed Float32，电机最大运转速度，单位: 步/秒。
 * @see DSCP_CMD_MCI_SET_MOTION_PARAM
 */
MotorParam MotorControlInterface::GetMotorParam(int index)
{
    MotorParam param;

    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_MOTION_PARAM, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        param = *((MotorParam*)resp->data);
    }

    return param;
}

/**
 * @brief 设置指定电机的运动参数。
 * @details 系统将根据设定的运动参数进行运动控制和规划。运动参数将永久保存。
 * @param index Uint8，要设置的电机索引。
 * @param acceleration Float32，加速度，单位:步/平方秒。
 * @param speed Float32，电机最大运转速度，单位: 步/秒。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 *  - @ref DSCP_ERROR_PARAM 参数错误，传入的参数有问题
 * @see DSCP_CMD_MCI_GET_MOTION_PARAM
 */
bool MotorControlInterface::SetMotorParam(int index, MotorParam param)
{
    Uint16 status = DscpStatus::Error;
    Uint8 cmdData[9];

    cmdData[0] = index;
    memcpy(cmdData+1, &param, sizeof(MotorParam));

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_SET_MOTION_PARAM, cmdData, sizeof(cmdData)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 查询指定电机的工作状态。
 * @param index Uint8，要查询的电机索引。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_IDLE 空闲；
 *  - @ref DSCP_BUSY 忙碌，需要停止后才能做下一个动作；
 */
MotorStatus MotorControlInterface::GetMotorStatus(int index)
{
    Uint16 ret;
    MotorStatus status = MotorStatus::Idle;

    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_MOTOR_STATUS, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        ret = *((Uint16*)resp->data);
    }
    if (DscpStatus::Idle == ret)
    {
        status = MotorStatus::Idle;
    }
    else if (DscpStatus::Busy == ret)
    {
        status = MotorStatus::Busy;
    }
    return status;
}

/**
 * @brief 查询指定电机的最大移动步数。
 * @param index Uint8，要查询的电机索引。
 * @return 步数， Uint16。
 */
Uint16 MotorControlInterface::GetMotorMaxSteps(int index)
{
    Uint16 steps = 0;
    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_MOTOR_MAX_STEPS, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        steps = *((Uint16*)resp->data);
    }

    return steps;
}

/**
 * @brief 查询指定电机的初始位置步数。
 * @param index Uint8，要查询的电机索引。
 * @return 步数， Uint16。
 */
Uint16 MotorControlInterface::GetMotorInitSteps(int index)
{
    Uint16 steps = 0;
    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_MOTOR_INIT_STEPS, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        steps = *((Uint16*)resp->data);
    }

    return steps;
}

/**
 * @brief 查询指定电机的当前移动步数。
 * @param index Uint8，要查询的电机索引。
 * @return 步数， Uint16。
 */
Uint16 MotorControlInterface::GetMotorCurrentSteps(int index)
{
    Uint16 steps = 0;
    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_MOTOR_CURRENT_STEPS, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        steps = *((Uint16*)resp->data);
    }

    return steps;
}

/**
 * @brief 电机移动。
 * @details 启动后，不管成功与否，操作结果都将以事件的形式上传给上位机。关联的事件有：
 *   - @ref DSCP_EVENT_MCI_MOTOR_RESULT
 * @param index Uint8，要操作的电机索引。
 * @param steps short16，电机移动步数，正数时为正向转动，负数时为反向移动。
 * @param mode Uint8, 电机移动模式,（ @ref MotorMoveMode ）, 定义如下：
 *  - @ref MOTOR_MOVE_ABSOLUTE_MODE, 绝对模式,此时steps参数指电机相对初始位置转动的步数；
 *  - @ref MOTOR_MOVE_RELATIVE_MODE, 相对模式,此时steps参数指电机相对当前位置转动的步数；
 *  - @ref MOTOR_MOVE_SAFE_MODE, 安全模式，此时steps参数指电机相对当前位置转动的步数，如Z轴电机不在初始位置，移动X轴电机会先复位Z轴电机。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败，如电机正在运转，无法启动电机，需要先停止；
 *  - @ref DSCP_ERROR_PARAM 参数错误，传入的参数有问题
 * @note 该命令将立即返回，电机移动完成将以事件的形式上报。
 */
bool MotorControlInterface::MotorMove(int index, int steps, MotorMoveMode mode)
{
    Uint16 status = DscpStatus::Error;
    Int16 num = (Int16)steps;

    Uint8 cmdData[4] = {0};

    cmdData[0] = (Uint8)index;
    memcpy(cmdData+1, &num, sizeof(num));
    cmdData[3] = (Uint8)mode;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_MOTOR_MOVE, cmdData, sizeof(cmdData)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 停止电机。
 * @param index Uint8，要操作的电机索引。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 *  - @ref DSCP_ERROR_PARAM 参数错误，传入的参数有问题
 */
bool MotorControlInterface::MotorStop(int index)
{
    Uint16 status = DscpStatus::Error;

    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_MOTOR_STOP, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 电机复位。
 * @details 电机回到0位置，不管成功与否，操作结果都将以事件的形式上传给上位机。关联的事件有：
 *   - @ref DSCP_EVENT_MCI_MOTOR_RESULT
 * @param index Uint8，要操作的电机索引。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 *  - @ref DSCP_ERROR_PARAM 参数错误，传入的参数有问题
 */
bool MotorControlInterface::MotorReset(int index)
{
    Uint16 status = DscpStatus::Error;

    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_MOTOR_RESET, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 查询指定位置传感器的状态。
 * @param index Uint8，要查询的位置传感器索引。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_IDLE 空闲, 传感器未被遮挡；
 *  - @ref DSCP_BUSY 忙碌，传感器被遮挡。
 */
bool MotorControlInterface::GetSensorStatus(int index)
{
    Uint16 status = DscpStatus::Idle;

    Uint8 num = (Uint8)index;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_GET_SENSOR_STATUS, &num, sizeof(num)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        status = *((Uint16*)resp->data);
    }

    return (status == DscpStatus::Idle);
}

/**
 * @brief 所有步进电机驱动配置初始化。
 * @details 所有步进电机驱动配置初始化，不管成功与否，操作结果都将以事件的形式上传给上位机。关联的事件有：
 *   - @ref DSCP_EVENT_MCI_MOTOR_RESULT
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 *  - @ref DSCP_ERROR_PARAM 参数错误，传入的参数有问题
 */
bool MotorControlInterface::MotorDriverReinit()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_DRIVER_REINIT));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 检查指定电机驱动的状态。
 * @param index Uint8，要查询的位置电机驱动索引。
 * @return gstat Uint8，驱动全局寄存器的值
 */
Uint8 MotorControlInterface::MotorDriverCheck(Uint8 index)
{
    Uint8 status = 0;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_MCI_DRIVER_CHECK, &index, sizeof(index)));
    SyncCaller syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        status = *((Uint8*)resp->data);
    }

    return status;
}


/**
 * @brief 电机操作结果事件。
 * @details 电机移动操作结束时将产生该事件。
 * @param index Uint8，产生事件的泵索引，0号泵为光学定量泵。
 * @param result Uint8，泵操作结果码（ @ref MotorResult ），定义如下：
 *  - @ref MOTOR_RESULT_FINISHED  电机操作正常完成；
 *  - @ref MOTOR_RESULT_FAILED  电机操作中途出现故障，未能完成。
 *  - @ref MOTOR_RESULT_STOPPED  电机操作被停止。
 */
MotorResult MotorControlInterface::ExpectMotorResult(long timeout)
{
    MotorResult motorResult;
    memset(&motorResult, 0, sizeof(MotorResult));

    DscpEventPtr event = this->Expect(DSCP_EVENT_MCI_MOTOR_RESULT, timeout);

    if (event != nullptr)
    {
        motorResult.index = *event->data;
        motorResult.result = (int)(*(event->data+1));
    }

    return motorResult;

}

}
}
