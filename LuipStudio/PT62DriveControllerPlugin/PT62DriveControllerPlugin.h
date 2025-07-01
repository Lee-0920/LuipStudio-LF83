#ifndef PT62DRIVECONTROLLERPLUGIN_H
#define PT62DRIVECONTROLLERPLUGIN_H

#include "lua.hpp"
#include "System/Types.h"
#include "Log.h"
#include "LuipShare.h"
#include "ControllerPlugin/BaseController.h"
#include "ControllerPlugin/ControllerPlugin.h"
#include "oolua.h"

using System::String;

class PT62DriveController;

namespace Controller
{

class LUIP_SHARE PT62DriveControllerPlugin : public ControllerPlugin
{

public:
    PT62DriveControllerPlugin();
    virtual ~PT62DriveControllerPlugin();
    bool Init(log4cpp::Category* log);
    BaseController* Create(DscpAddress addr);
    PT62DriveController * GetDriveController();
    String GetVersion();

private:
    static PT62DriveController * m_dc;
};

// extern "C" 生成的导出符号没有任何修饰，方便主程序找到它
extern "C"
{
    LUIP_SHARE ControllerPlugin *CreatePlugin();
}

}
#endif // PT62DRIVECONTROLLERPLUGIN_H

