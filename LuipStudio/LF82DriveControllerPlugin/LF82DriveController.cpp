/**
 * @file
 * @brief 驱动控制器。
 * @details
 * @version 1.0.0
 * @author kim@erchashu.com
 * @date 2016/5/13
 */

#include <QTime>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include "Log.h"
#include "LF82DriveController.h"
#include "API/Code/OpticalMeterInterface.h"
#include "API/Code/TemperatureControlInterface.h"
#include "API/Code/OpticalAcquireInterface.h"
#include "LuaEngine/LuaEngine.h"
#include "Communication/EventHandler.h"
#include "Communication/CommunicationException.h"

using namespace std;
using namespace Communication;
using namespace Communication::Dscp;
using namespace OOLUA;
using namespace Lua;

namespace Controller
{

/**
 * @brief 驱动控制器构造。
 */
LF82DriveController::LF82DriveController(DscpAddress addr)
    : m_environmentTemp(0),m_sendEnvTempCount(0),BaseController(addr),
    IPeristalticPump(new PeristalticPumpInterface(addr)),
    ISolenoidValve(new SolenoidValveInterface(addr)),
    IOpticalMeter(new OpticalMeterInterface(addr)),
    ITemperatureControl(new TemperatureControlInterface(addr)),
    IOpticalAcquire(new OpticalAcquireInterface(addr)),
    IMotorControl(new MotorControlInterface(addr))
{
    m_environmentTemp = 0;

    for (int i = 0; i < 10; i++)
    {
        m_ThermostatTemps.push_back(0);
    }
}

LF82DriveController::~LF82DriveController()
{
    if (IPeristalticPump)
        delete IPeristalticPump;
    if (ISolenoidValve)
        delete ISolenoidValve;
    if (IOpticalMeter)
        delete IOpticalMeter;
    if (ITemperatureControl)
        delete ITemperatureControl;
    if (IOpticalAcquire)
        delete IOpticalAcquire;
    if (IMotorControl)
        delete IMotorControl;
}

/**
 * @brief 初始化 LiquidController 环境。
 */
bool LF82DriveController::Init()
{
//    IOpticalMeter->RegisterOpticalADNotice(this);
    ITemperatureControl->RegisterTemperatureNotice(this);
    IOpticalAcquire->RegisterAcquireADNotice(this);
    return true;
}

bool LF82DriveController::Uninit()
{
    return true;
}

void LF82DriveController::Register(ISignalNotifiable *handle)
{
    m_notifise.push_back(handle);
}

PeristalticPumpInterface *LF82DriveController::GetIPeristalticPump()
{
    return IPeristalticPump;
}

SolenoidValveInterface *LF82DriveController::GetISolenoidValve()
{
    return ISolenoidValve;
}

OpticalMeterInterface *LF82DriveController::GetIOpticalMeter()
{
    return IOpticalMeter;
}

TemperatureControlInterface *LF82DriveController::GetITemperatureControl()
{
    return ITemperatureControl;
}

OpticalAcquireInterface *LF82DriveController::GetIOpticalAcquire()
{
    return IOpticalAcquire;
}

MotorControlInterface *LF82DriveController::GetIMotorControl()
{
    return IMotorControl;
}

float LF82DriveController::GetDigestTemperature() const
{
    return m_ThermostatTemps[0];
}

float LF82DriveController::GetEnvironmentTemperature() const
{
    return m_environmentTemp;
}

float LF82DriveController::GetEnvironmentSignalTemp() const
{
    return m_environmentTemp;
}

float LF82DriveController::GetThermostatSignalTemp(Uint8 index) const
{
    return m_ThermostatTemps[index];
}

float LF82DriveController::GetCurrEnvironmentTemp()
{
    float temp;

    try
    {
        temp = this->ITemperatureControl->GetEnvironmentTemperature();
        m_environmentTemp = temp;
    }
    catch (CommandTimeoutException e)  // 命令应答超时异常。
    {
        if(this->GetConnectStatus())
        {
            temp = m_environmentTemp;
        }
        else
        {
            throw CommandTimeoutException(e.m_addr, e.m_code);
        }
    }
    catch (std::exception e)
    {
        if(this->GetConnectStatus())
        {
            temp = m_environmentTemp;
        }
        else
        {
            throw std::exception();
        }
    }

    return temp;
}

float LF82DriveController::GetCurrThermostatTemp(Uint8 index)
{
    float temp;

    try
    {
        temp = this->ITemperatureControl->GetThermostatTemperature(index);
        m_ThermostatTemps[index] = temp;
    }
    catch (CommandTimeoutException e)  // 命令应答超时异常。
    {
        if(this->GetConnectStatus())
        {
            temp = m_ThermostatTemps[index];
        }
        else
        {
            throw CommandTimeoutException(e.m_addr, e.m_code);
        }
    }
    catch (std::exception e)
    {
        if(this->GetConnectStatus())
        {
            temp = m_ThermostatTemps[index];
        }
        else
        {
            throw std::exception();
        }
    }

    return temp;
}

bool LF82DriveController::GetOpticalADVector(int length)
{
    bool status = false;

    std::vector<OpticalAD> emptyVector;

    this->m_opticalAD.swap(emptyVector);

//    if(this->IOpticalAcquire->GetOpticalADData(length, m_opticalAD))
//    {
//        if(length == this->m_opticalAD.size())
//        {
//            status = true;
//        }
//    }

    return status;
}

OpticalADCurve* LF82DriveController::GetRefADCurve(int length) const
{
    OpticalADCurve* refCurve = new OpticalADCurve(length);

    int num = min((int)m_opticalAD.size(),length);

    for(int i = 0; i < num; i++)
    {
        OpticalAD data = this->m_opticalAD.at(i);

        refCurve->SetData(i, data.reference);
    }

    return refCurve;
}

OpticalADCurve* LF82DriveController::GetMeaADCurve(int length) const
{
    OpticalADCurve* meaCurve = new OpticalADCurve(length);

    int num = min((int)m_opticalAD.size(),length);

    for(int i = 0; i < num; i++)
    {
        OpticalAD data = this->m_opticalAD.at(i);

        meaCurve->SetData(i, data.measure);
    }

    return meaCurve;
}

void LF82DriveController::OnReceive(DscpEventPtr event)
{
    switch (event->code)
    {
        case DSCP_EVENT_TCI_TEMPERATURE_NOTICE:     //温度上报事件
        {

            m_environmentTemp = *((float*)event->data);

            Uint8 num = *(event->data+4);

            for(Uint8 i = 0; i < num; i++)
            {
                m_ThermostatTemps[i] = *((float*)(event->data+5 + i*4));
            }

            if(!m_notifise.empty())
            {
                list<ISignalNotifiable*>::iterator it;
                for(it = m_notifise.begin(); it != m_notifise.end(); it++)
                {
                    (*it)->OnSignalChanged("eTemp", m_environmentTemp);
                    (*it)->OnSignalChanged("tTemp", m_ThermostatTemps[0]);

                    for(Uint8 i = 1; i < num; i++)
                    {
                        String str = "temp" + std::to_string(i);
                        (*it)->OnSignalChanged(str, m_ThermostatTemps[i]);
                    }
                }
            }

            //环境温度监控
            EnvironmentTemperatureMonitor(m_environmentTemp);
        }
            break;

        case DSCP_EVENT_OAI_SIGNAL_AD_NOTICE:   // 光学测量信号AD定时上报事件
        {
            double ref = *((unsigned long*)(event->data));
            double mea = *((unsigned long*)(event->data + 4));
//            double abs = 0;
//            if (0 < ref && 0 < mea)
//            {
//                abs = 1000 * log10(ref/mea);
//            }

            if(!m_notifise.empty())
            {
                list<ISignalNotifiable*>::iterator it;
                for(it = m_notifise.begin(); it != m_notifise.end(); it++)
                {
                    (*it)->OnSignalChanged("refAD", ref);
                    (*it)->OnSignalChanged("meaAD", mea);
                    //(*it)->OnSignalChanged("abs", abs);
                }
            }
        }
            break;

        default:
            break;
    }
}

void LF82DriveController::StopSignalUpload()
{
    if (this->GetConnectStatus())
    {
        //重置温度上传周期
        try
        {
            this->ITemperatureControl->SetTemperatureNotifyPeriod(0);
        }
        catch(CommandTimeoutException e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch(ExpectEventTimeoutException e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch (std::exception e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.what());
        }

        //重置信号AD上传周期
        try
        {
            this->IOpticalAcquire->SetAcquireADNotifyPeriod(0);
        }
        catch(CommandTimeoutException e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch(ExpectEventTimeoutException e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch (std::exception e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.what());
        }
    }
}

void LF82DriveController::StartSignalUpload()
{
    if (this->GetConnectStatus())
    {
        //重置温度上传周期
        try
        {
            this->ITemperatureControl->SetTemperatureNotifyPeriod(1);
        }
        catch(CommandTimeoutException e)
        {
            logger->warn("LF82DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch(ExpectEventTimeoutException e)
        {
            logger->warn("LF82DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch (std::exception e)
        {
            logger->warn("LF82DriveController::ResetHandler() => %s", e.what());
        }

        //重置电极AD上传周期
        try
        {
            this->IOpticalAcquire->SetAcquireADNotifyPeriod(1);
        }
        catch(CommandTimeoutException e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch(ExpectEventTimeoutException e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.What().c_str());
        }
        catch (std::exception e)
        {
            logger->warn("DriveController::ResetHandler() => %s", e.what());
        }
    }
}

void LF82DriveController::ClearThermostatRemainEvent()
{
    try
    {
        DscpAddress addr = ITemperatureControl->GetAddress();
        for (int i = 0; i < 10; i++)
        {
            DscpEventPtr event = EventHandler::Instance()->Expect(addr,DSCP_EVENT_TCI_THERMOSTAT_RESULT, 200);

            if (event != nullptr)
            {
                Uint8 index = *event->data;
                Uint8 result = *(event->data + 1);
                float temp = *((float*)(event->data + 2));
                logger->debug("{ClearThermostatRemainEvent} index = %d，result = %d，temp = %.1f",index, result, temp);
            }
        }
    }
    catch(CommandTimeoutException e)
    {
    }
    catch(ExpectEventTimeoutException e)
    {
    }
    catch (std::exception e)
    {

    }
}

void LF82DriveController::EnvironmentTemperatureMonitor(float temp)
{
    m_sendEnvTempCount++;

    if (m_sendEnvTempCount >= 60)
    {
        m_sendEnvTempCount = 0;

        lua_State * newState = LuaEngine::Instance()->GetThreadState();

        OOLUA::Lua_function call(newState);
        OOLUA::Lua_func_ref EnvTempHandle;

        try
        {
            if(OOLUA::get_global(newState, "EnvTempHandle", EnvTempHandle))
            {
                call(EnvTempHandle, temp);
            }
        }
        catch(OOLUA::Exception e)
        {
            logger->warn("DriveController::EnvTempHandle() => %s", e.what());
        }
        catch(std::exception e)
        {
            logger->warn("DriveController::EnvTempHandle() => %s", e.what());
        }
    }
}

}
