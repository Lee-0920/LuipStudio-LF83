#include "LF82DriveController.h"
#include "LF82DriveControllerPluginProxy.h"
#include "LF82DriveControllerPlugin.h"

using namespace std;
using namespace System;

const static String version = String("1.4.0.0");

namespace Controller
{

LF82DriveController * LF82DriveControllerPlugin::m_dc = nullptr;

LF82DriveControllerPlugin::LF82DriveControllerPlugin()
{
}

LF82DriveControllerPlugin::~LF82DriveControllerPlugin()
{

}

bool LF82DriveControllerPlugin::Init(log4cpp::Category *log)
{
    LF82DriveControllerPluginProxy::Proxy();
    logger = log;

    return true;
}

BaseController *LF82DriveControllerPlugin::Create(DscpAddress addr)
{
    if (!m_dc)
    {
        m_dc = new LF82DriveController(addr);
    }
    return m_dc;
}

LF82DriveController *LF82DriveControllerPlugin::GetDriveController()
{
    return m_dc;
}

String LF82DriveControllerPlugin::GetVersion()
{
    return version;
}

ControllerPlugin *CreatePlugin()
{
    return new LF82DriveControllerPlugin();
}

}
