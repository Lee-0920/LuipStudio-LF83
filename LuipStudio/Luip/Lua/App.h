#ifndef LUA_APP_H
#define LUA_APP_H

#include "System/Types.h"

namespace Lua
{

/**
 * @brief  单位转换类型。
 * @details
 */
enum class UnitChange
{
  Read = 0,
  Write = 1,
};

class App
{

public:
    App();

    static void Sleep(int ms);
    static System::String CreateUUID(void);
    static void SetScreenTime(float darkTime, float closeTime, float userExitTime);
    static void SetSceenEnable(bool isEnable);
    static long GetOSRunTime();
};

}


#endif // LUA_APP_H
