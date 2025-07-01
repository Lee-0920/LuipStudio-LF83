#include <vector>
#include<QDebug>
#include<QFile>
#include "Log.h"
#include "Common.h"
#include "System/ADC.h"
#include "LuaEngine/LuaEngine.h"
#include "AlarmManager/AlarmManager.h"
#include "WeepingDetector.h"

using namespace std;
using namespace System;
using namespace OOLUA;
using namespace Lua;
using namespace Measure;

namespace Weeping
{

bool AscendingSort(const int &v1, const int &v2);

unique_ptr<WeepingDetector> WeepingDetector::m_instance(nullptr);
/**
 * @brief 获取全局唯一的 WeepingDetector 对象（单间）。
 */
WeepingDetector* WeepingDetector::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new WeepingDetector);
    }
    return m_instance.get();
}
bool WeepingDetector::Init()
{
#ifdef _DEV_USING_CONTROL

    m_isWeeping = false;
    bool weepingDetect = false;

    m_fileName = "/sys/bus/iio/devices/iio:device0/in_voltage4_raw";

    QFile file(QString(m_fileName.c_str()));
    if (!file.exists())
    {
        logger->debug("%s is not exists", m_fileName.c_str());
        return false;
    }

    try
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State * state = luaEngine->GetThreadState();

        Table commonTable;
        luaEngine->GetLuaValue(state, "config.system", commonTable);

        commonTable.safe_at("weepingDetect", weepingDetect);
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("WeepingDetector::Init => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("WeepingDetector::Init => %s", e.what());
    }

    if (weepingDetect)
    {
        this->Start();
    }
    else
    {
        logger->warn("Not open weeping detect function !!!");
        return false;
    }
#endif
    return true;
}


bool WeepingDetector::Start()
{
    m_isRuning = true;
    start();
    return true;
}

void WeepingDetector::Stop()
{
    m_isRuning = false;
}

void WeepingDetector::run()
{
    logger->debug("WeepingDetector Thread ID = %d", QThread::currentThreadId());
    while(m_isRuning)
    {
        sleep(120);
        this->Detect();
    }
}

void WeepingDetector::Detect()
{
#ifdef _DEV_USING_CONTROL

    int i = 10;
    std::vector<Uint16> adValues;

    while(i--)
    {
        ADC adc(m_fileName);
        int val = adc.ReadValue();
        adValues.push_back(val);
//        logger->debug("ADC4 val = %d", val);
//        qDebug("ADC4 val = %d", val);
        QThread::sleep(1);
    }

    if (!adValues.empty())
    {
        std::sort(adValues.begin(), adValues.end(), AscendingSort);

        adValues.erase(adValues.begin());
        adValues.pop_back();

        Uint16 total = 0;
        for (std::vector<Uint16>::iterator iter = adValues.begin(); iter != adValues.end(); iter++)
        {
            total += *iter;
        }

        Uint16 vag = total/adValues.size();
        Uint16 alarmVal = 4095/1.8 *1.6;

    //    logger->debug("vag = %d, alarmVal = %d", vag, alarmVal);
//        qDebug("vag = %d, alarmVal = %d", vag, alarmVal);
        if (vag < alarmVal )
        {
            if (!m_isWeeping)
            {
                m_isWeeping = true;
                Alarm alarm("仪器故障", "漏液", "", "Error");
                AlarmManager::Instance()->AddAlarm(alarm);
            }
        }
        else
        {
            if (m_isWeeping)
            {
                m_isWeeping = false;
            }
        }
    }
#endif
}

bool AscendingSort(const int &v1, const int &v2)
{
    return v1 < v2;   //升序排列
}

}

