﻿/**

 * @file
 * @brief 温度控制接口。
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2015/3/7
 */
#include "Log.h"
#include "Code/TemperatureControlInterface.h"
#include "Communication/Dscp/DscpStatus.h"
#include "Communication/SyncCaller.h"
#include "Communication/EventHandler.h"
#include "TemperatureControlInterface.h"
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
 * @brief 温度控制接口。
 * @param[in] addr 设备地址。
 */
TemperatureControlInterface::TemperatureControlInterface(DscpAddress addr)
    : DeviceInterface(addr)
{

}

/**
 * @brief 查询温度传感器的校准系数。
 * @return  负输入分压，Float32。
 * @return 参考电压 ，Float32。
 * @return 校准电压 Float32。
 * @see DSCP_CMD_TCI_SET_CALIBRATE_FACTOR
 */

TempCalibrateFactor TemperatureControlInterface::GetCalibrateFactor()
{
    TempCalibrateFactor factor = {0, 0, 0};

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_GET_CALIBRATE_FACTOR));
    SyncCaller  syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        factor.negativeInput = *((float*)resp->data);
        factor.referenceVoltage = *((float*)(resp->data + 4));
        factor.calibrationVoltage = *((float*)(resp->data + 8));
    }

    return factor;
}

/**
 * @brief 设置温度传感器的校准系数。
 * @details 因为个体温度传感器有差异，出厂或使用时需要校准。该参数将永久保存。
 * @param negativeInput Float32，负输入分压。
 * * @param referenceVoltage Float32，参考电压。
 * * @param calibrationVoltage Float32，校准电压。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_OK  操作成功；
 *  - @ref DSCP_ERROR 操作失败；
 */
bool TemperatureControlInterface::SetCalibrateFactor(TempCalibrateFactor factor)
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_SET_CALIBRATE_FACTOR, &factor, sizeof(factor)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 查询当前温度。
 * @return 当前温度，包括控制室温度和环境温度，格式如下：
 *     - thermostatTemp float，恒温室温度，单位为摄氏度。
 *     - environmentTemp float，环境温度，单位为摄氏度。
 */
Temperature TemperatureControlInterface::GetTemperature()
{
    Temperature temp;
    memset(&temp, 0, sizeof(temp));

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_GET_TEMPERATURE));
    SyncCaller  syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        temp = *((Temperature*)resp->data);
    }

    return temp;
}

/**
 * @brief 查询恒温控制参数。
 * @return 恒温控制参数，格式如下：
 *  - proportion float，PID的比例系数。
 *  - integration float，PID的积分系数。
 *  - differential float，PID的微分系数。
 * @see DSCP_CMD_TCI_SET_THERMOSTAT_PARAM
 */
ThermostatParam TemperatureControlInterface::GetThermostatParam()
{
    ThermostatParam param;
    memset(&param, 0, sizeof(param));

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_GET_THERMOSTAT_PARAM));
    SyncCaller  syncCaller(m_retries);
    DscpRespPtr resp = syncCaller.Send(cmd);
    if (resp)
    {
        param = *((ThermostatParam*)resp->data);
    }

    return param;
}

/**
 * @brief 设置恒温控制参数。
 * @details 恒温系统将根据设置的参数进行温度调节。
 *  该参数掉电后将丢失，每次恒温前都需要重新设置。
 * @param[in] proportion float，PID的比例系数。
 * @param[in] integration float，PID的积分系数。
 * @param[in] differential float，PID的微分系数。
* @return 返回状态，操作是否成功。
 */
bool TemperatureControlInterface::SetThermostatParam(ThermostatParam param)
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_SET_THERMOSTAT_PARAM, &param, sizeof(param)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 查询恒温器的工作状态。
 * @return 状态回应，Uint16，支持的状态有：
 *  - @ref DSCP_IDLE 空闲；
 *  - @ref DSCP_BUSY 忙碌，需要停止后才能做下一个动作；
 */

Uint16 TemperatureControlInterface::GetThermostatStatus()
{
    Uint16 status = DscpStatus::Busy;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_GET_THERMOSTAT_STATUS));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return status;
}

/**
 * @brief 开始恒温。
 * @details 恒温开始后，系统将根据设定的恒温控制参数进行自动温度控制，尝试达到指定温度。
 *  不管成功与否，操作结果都将以事件的形式上传给上位机。关联的事件有：
 *   - @ref DSCP_EVENT_TCI_THERMOSTAT_RESULT
 * @param[in] mode Uint8，恒温模式，支持的模式见： @ref ThermostatMode 。
 * @param[in] targetTemp float，恒温目标温度。
 * @param[in] toleranceTemp float，容差温度，与目标温度的差值在该参数范围内即认为到达目标温度。
 * @param[in] timeout float，超时时间，单位为秒。超时仍未达到目标温度，也将返回结果事件。
* @return 返回状态，操作是否成功。
 * @note 该命令将立即返回，恒温完成将以事件的形式上报。
 */
bool TemperatureControlInterface::StartThermostat(ThermostatMode mode, float targetTemp, float toleranceTemp, float timeout)
{
    Uint16 status = DscpStatus::Error;
    Uint8 cmdData[13] = {0};
    memcpy(cmdData, &mode, sizeof(Uint8));
    memcpy(cmdData+1, &targetTemp, sizeof(float));
    memcpy(cmdData+5, &toleranceTemp, sizeof(float));
    memcpy(cmdData+9, &timeout, sizeof(float));

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_START_THERMOSTAT, cmdData, sizeof(cmdData)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);

    if (status != DscpStatus::OK)
    {
        logger->debug("启动恒温命令应答状态：%d", (int)status);
    }
    return (status == DscpStatus::OK);
}

/**
 * @brief 停止恒温控制。
 * @details 停止后，加热器和冷却器将不工作。
* @return 返回状态，操作是否成功。
 * @note 该命令将立即返回。
 */
bool TemperatureControlInterface::StopThermostat()
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_STOP_THERMOSTAT));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);

    if (status != DscpStatus::OK)
    {
        logger->debug("停止恒温命令应答状态：%d", (int)status);
    }
    return (status == DscpStatus::OK);
}

/**
 * @brief 设置温度上报周期。
 * @details 系统将根据设定的周期，定时向上发出温度上报事件。
 * @param[in] period float，温度上报周期，单位为秒。0表示不需要上报，默认为0。
* @return 返回状态，操作是否成功。
 * @see DSCP_EVENT_TCI_TEMPERATURE_NOTICE
 * @note 所设置的上报周期将在下一次启动时丢失，默认为0，不上报。
 */
bool TemperatureControlInterface::SetTemperatureNotifyPeriod(float period)
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_SET_TEMPERATURE_NOTIFY_PERIOD, &period, sizeof(period)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

/**
 * @brief 恒温结果事件。
 * @details 恒温操作结束时将产生该事件。
 * @param[in] timeout 恒温操作事件等待超时时间。
 * @returen 恒温结果（ @ref Thermostat ），定义如下：
 *  - @ref result Uint8，恒温结果码（ @ref ThermostatResult ）。
 *  - @ref temp float，当前温度。
 */
ThermostatResult TemperatureControlInterface::ExpectThermostat(long timeout)
{
    ThermostatResult thermostatResult;
    memset(&thermostatResult, 0, sizeof(thermostatResult));

    DscpEventPtr event = this->Expect(DSCP_EVENT_TCI_THERMOSTAT_RESULT,timeout);

    if (event != nullptr)
    {
        thermostatResult.result = (int)(*event->data);
        thermostatResult.temp = *((float*)(event->data+1));
    }

    return thermostatResult;
}

/**
 * @brief 注册温度定时上报事件。
 * @details 系统将根据设置的上报周期，定时向上汇报温度。
 *  上报周期可通过命令 @ref DSCP_CMD_TCI_SET_TEMPERATURE_NOTIFY_PERIOD 设定。
 * @param[in] handle 定时接收温度上报事件的对象。
 */
void TemperatureControlInterface::RegisterTemperatureNotice(IEventReceivable *handle)
{
    EventHandler::Instance()->Register(m_addr,DSCP_EVENT_TCI_TEMPERATURE_NOTICE,handle);
}

/**
 * @brief 设置机箱风扇。
 * @details 根据设定的占空比，调节风扇速率
 * @param[in] level unsigned char ,风扇速率，对应高电平占空比。默认为0，机箱风扇关闭。
 */
bool TemperatureControlInterface::BoxFanSetOutput(float level)
{
    Uint16 status = DscpStatus::Error;

    DscpCmdPtr cmd(new DscpCommand(m_addr, DSCP_CMD_TCI_TURN_BOXFAN , &level, sizeof(level)));
    SyncCaller syncCaller(m_retries);
    status = syncCaller.SendWithStatus(cmd);
    return (status == DscpStatus::OK);
}

}
}
