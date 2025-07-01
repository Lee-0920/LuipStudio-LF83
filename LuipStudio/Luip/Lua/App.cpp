#include <QThread>
#include "App.h"
#include <QUuid>
#include "System/LockScreen/ScreenProtection.h"
#include "Log.h"
#ifdef _CS_ARM_LINUX
    #include <sys/sysinfo.h>
#endif

namespace Lua
{

App::App()
{

}

void App::Sleep(int ms)
{
    QThread::msleep(ms);
}

System::String App::CreateUUID(void)
{
    return QUuid::createUuid().toString().toStdString();
}

void App::SetScreenTime(float darkTime, float closeTime, float userExitTime)
{
    System::Screen::Screenprotection::Instance()->SetScreenTime(darkTime, closeTime, userExitTime);
}

void App::SetSceenEnable(bool isEnable)
{
    System::Screen::Screenprotection::Instance()->SetSceenEnable(isEnable);
}

long App::GetOSRunTime()
{
    long runTime = -1;

#ifdef _CS_ARM_LINUX
    struct sysinfo info;

    if (sysinfo(&info))
    {
        logger->warn("Failed to get sysinfo, errno:%u, reason:%s",errno, strerror(errno));
        return -1;
    }

    runTime = info.uptime;
#endif

    return runTime;
}

}
