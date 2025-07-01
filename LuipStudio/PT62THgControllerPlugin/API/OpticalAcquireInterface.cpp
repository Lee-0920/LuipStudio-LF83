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
 * @details 启动后，将采样时间内得到的数据将以事件 @ref DSCP_EVENT_EAI_AD_ACQUIRED
 *  的形式上传给上位机，按照设置的采样周期进行采样，采样结束即上传数据，无需对数据进行处理。
 * @param samplePeriod Float32，采样周期。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
bool OpticalAcquireInterface:: StartAcquirer(float samplePeriod)
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_START_ACQUIRER, &samplePeriod, sizeof(samplePeriod)));
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
 * @brief 打开汞灯
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
bool OpticalAcquireInterface:: TurnOnHgLamp()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_ON_MERCURY_LAMP));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 关闭汞灯
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
bool OpticalAcquireInterface:: TurnOffHgLamp()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_OFF_MERCURY_LAMP));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/** * @brief 打开LED
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
bool OpticalAcquireInterface::TurnOnLED()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_ON_LED));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 关闭LED
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
bool OpticalAcquireInterface::TurnOffLED()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_OAI_TURN_OFF_LED));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 信号AD采集完成事件。
 * @details 启动采集后，按照设置的采样周期采完数据时将产生该事件。
 * @param number Uint32，采样时间内得到的数据个数，决定了后面的变长参数长度。
 * @param value[number][2] Uint32，采样时间内得到的AD值。value格式如下：
 *      - value[number][0] reference Uint32，参考端AD。
 *      - value[number][1] measure Uint32，测量端AD。
 * @see DSCP_CMD_OAI_START_ACQUIRER
 */
Uint32 OpticalAcquireInterface::ExpectADAcquirer(long timeout, vector<OpticalAD> &ad)
{
    Uint32 number = 0;
    DscpEventPtr event = this->Expect(DSCP_EVENT_OAI_AD_ACQUIRED ,timeout);

    if (event != nullptr)
    {
        number = *((Uint32*)event->data);

        for(Uint32 i = 0; i < number *2; i += 2)
        {
            OpticalAD data;

            data.reference = *((Uint32*)(event->data+4*(i+1)));
            data.measure = *((Uint32*)(event->data+4*(i+2)));
            ad.push_back(data);
        }
    }

    return number;
}

/**
 * @brief 信号AD定时上报事件。
 * @details 系统将根据设置的上报周期，定时向上汇报信号AD。
 *  上报周期可通过命令 @ref DSCP_CMD_TCI_SET_TEMPERATURE_NOTIFY_PERIOD 设定。
 * @param reference Uint32，参考端AD。
 * @param measure Uint32，测量端AD。
 */
void OpticalAcquireInterface::RegisterAcquireADNotice(IEventReceivable *handle)
{
    EventHandler::Instance()->Register(m_addr, DSCP_EVENT_OAI_SIGNAL_AD_NOTICE, handle);

}

}
}
