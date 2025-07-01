#include "PT62DriveController.h"
#include "PT62DriveControllerPluginProxy.h"
#include "PT62DriveControllerPlugin.h"

using namespace std;
using namespace System;

const static String version = String("1.0.0.0");

namespace Controller
{

PT62DriveController * PT62DriveControllerPlugin::m_dc = nullptr;

PT62DriveControllerPlugin::PT62DriveControllerPlugin()
{
}

PT62DriveControllerPlugin::~PT62DriveControllerPlugin()
{

}

bool PT62DriveControllerPlugin::Init(log4cpp::Category *log)
{
    PT62DriveControllerPluginProxy::Proxy();
    logger = log;

    return true;
}

BaseController *PT62DriveControllerPlugin::Create(DscpAddress addr)
{
    if (!m_dc)
    {
        m_dc = new PT62DriveController(addr);
    }
    return m_dc;
}

PT62DriveController *PT62DriveControllerPlugin::GetDriveController()
{
    return m_dc;
}

String PT62DriveControllerPlugin::GetVersion()
{
    return version;
}

ControllerPlugin *CreatePlugin()
{
    return new PT62DriveControllerPlugin();
}

}
