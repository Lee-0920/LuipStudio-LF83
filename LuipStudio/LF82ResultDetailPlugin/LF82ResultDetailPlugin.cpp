#include "LF82ResultDetailPlugin.h"
#include "LF82ResultDetailPluginProxy.h"
#include "LF82CurveManager.h"

using namespace std;

namespace ResultDetail
{

LF82ResultDetailPlugin::~LF82ResultDetailPlugin()
{

}

bool LF82ResultDetailPlugin::Init(log4cpp::Category* log)
{
    logger = log;
    LF82ResultDetailPluginProxy::Proxy();
    LF82CurveManager::Instance()->Init();

    return true;
}

System::String LF82ResultDetailPlugin::GetVersion()
{
    return version;
}
}

ResultDetail::ResultDetailPlugin *CreatePlugin()
{
    return new ResultDetail::LF82ResultDetailPlugin();
}

