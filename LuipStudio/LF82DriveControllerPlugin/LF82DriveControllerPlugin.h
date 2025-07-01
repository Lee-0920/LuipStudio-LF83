#ifndef LF82DRIVECONTROLLERPLUGIN_H
#define LF82DRIVECONTROLLERPLUGIN_H

#include "lua.hpp"
#include "System/Types.h"
#include "Log.h"
#include "LuipShare.h"
#include "ControllerPlugin/BaseController.h"
#include "ControllerPlugin/ControllerPlugin.h"
#include "oolua.h"

using System::String;

class LF82DriveController;

namespace Controller
{

class LUIP_SHARE LF82DriveControllerPlugin : public ControllerPlugin
{

public:
    LF82DriveControllerPlugin();
    virtual ~LF82DriveControllerPlugin();
    bool Init(log4cpp::Category* log);
    BaseController* Create(DscpAddress addr);
    LF82DriveController * GetDriveController();
    String GetVersion();

private:
    static LF82DriveController * m_dc;
};

// extern "C" 生成的导出符号没有任何修饰，方便主程序找到它
extern "C"
{
    LUIP_SHARE ControllerPlugin *CreatePlugin();
}

}
#endif // LF82DRIVECONTROLLERPLUGIN_H

