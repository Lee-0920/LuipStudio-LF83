/**

 * @file
 * @brief 光学信号采集接口。
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2015/3/7
 */

#include "Code/OpticalAcquireInterface.h"
#include "Communication/Dscp/DscpStatus.h"
#include "Communication/SyncCaller.h"
#include "Communication/EventHandler.h"
#include "OpticalAcquireInterface.h"
#include "ControllerPlugin/API/Code/DeviceStatusInterface.h"

using namespace std;
using namespace Communication;
using namespace Communication::Dscp;
using namespace System;

namespace Controller
{
namespace API
{

/**
 * @brief 光学信号采集接口构造。
 * @param[in] addr 设备地址。
 */
OpticalAcquireInterface::OpticalAcquireInterface(DscpAddress addr)
    : DeviceInterface(addr)
{

}

/**
 * @brief 打开LED灯。
 * @return 返回状态，操作是否成功。
 */
bool OpticalAcquireInterface:: TurnOnLED()
{
    Uint16 status = DscpStatus::Error;
    Byte on = 1;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_ON_LED, &on, 1));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 关闭LED灯。
 * @return 返回状态，操作是否成功。
 */
bool OpticalAcquireInterface:: TurnOffLED()
{
    Uint16 status = DscpStatus::Error;
    Byte off = 0;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_ON_LED, &off, 1));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 打开氙灯。
 * @return 返回状态，操作是否成功。
 */
bool OpticalAcquireInterface:: TurnOnXonen()
{
    Uint16 status = DscpStatus::Error;
    Byte on = 1;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_ON_XONEN, &on, 1));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);

    return (status == DscpStatus::OK);
}

/**
 * @brief 关闭氙灯。
 * @return 返回状态，操作是否成功。
 */
bool OpticalAcquireInterface:: TurnOffXonen()
{
    Uint16 status = DscpStatus::Error;
    Byte off = 0;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_ON_XONEN, &off, 1));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);

    return (status == DscpStatus::OK);
}

/**
 * @brief 设置信号AD上报周期。
 * @details 系统将根据设定的周期，定时向上发出信号AD上报事件。
 * @param period Float32，信号AD上报周期，单位为秒。0表示不需要上报，默认为0。
 * @see DSCP_EVENT_OAI_AD_ACQUIRED_NOTICE
 * @note 所设置的上报周期将在下一次启动时丢失，默认为0，不上报。
 */
bool OpticalAcquireInterface::SetAcquireADNotifyPeriod(float period)
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_SET_SIGNAL_AD_NOTIFY_PERIOD, &period, sizeof(period)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 启动采集过程。
 * @details 启动后，将自动打开LED并采样，采集到的信号数据将以事件 @ref DSCP_EVENT_OAI_AD_ACQUIRED
 *  的形式上传给上位机。
 * @param acquireTime float，采样时间，单位为秒，0表示只采1个样本。
 * @param needTurnOn Bool，是否打开LED，0为不打开，1为打开。
 * @return 返回状态，操作是否成功。
 */
bool OpticalAcquireInterface:: StartAcquirer(float acquireTime)
{
    Uint16 status = DscpStatus::Error;

    Uint8 cmdData[5];

    memcpy(cmdData, &acquireTime, sizeof(acquireTime));
    cmdData[4] = MEASURE_TYPE_TN;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_START_ACQUIRER, cmdData, sizeof(cmdData)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 停止采集过程。
 * @return 状态回应，Uint16，支持的状态有：
 * @return 返回状态，操作是否成功。
 */
bool OpticalAcquireInterface:: StopAcquirer()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_STOP_ACQUIRER));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 信号采集完成事件。
 * @details 启动采集后，采完数据时将产生该事件。
 * @param[in] timeout Uint32，时间等待超时，单位ms。
 * @param[out] reference Uint32，参考端AD。
 * @param[out] measure Uint32，测量端AD。
 * @see DSCP_EVENT_OAI_TN_AD_ACQUIRED
 */
OpticalAD OpticalAcquireInterface:: ExpectADAcquirer(long timeout)
{
    OpticalAD AD;
    memset(&AD, 0, sizeof(AD));

    DscpEventPtr event = this->Expect(DSCP_EVENT_OAI_TN_AD_ACQUIRED,timeout);

    if (event != nullptr)
    {
        AD.reference = *((Uint32*)event->data);
        AD.measure = *((Uint32*)(event->data+4));
    }

    return AD;
}

/**
 * @brief 注册信号AD定时上报事件。
 * @details 系统将根据设置的上报周期，定时向上汇报信号AD。
 *  上报周期可通过命令 @ref DSCP_CMD_OAI_SET_SIGNAL_AD_NOTIFY_PERIOD 设定。
 * @param[in] handle 定时接收信号AD上报事件的对象。
 */
void OpticalAcquireInterface::RegisterAcquireADNotice(IEventReceivable *handle)
{
    EventHandler::Instance()->Register(m_addr, DSCP_EVENT_OAI_SIGNAL_AD_NOTICE, handle);

}

}
}
