
#include "AppProxy.h"
#include "App.h"
#include "System/SetSystemTime.h"

using namespace Lua;
using namespace System;

OOLUA_PROXY(,App)

    OOLUA_CTORS(

    )

    OOLUA_SFUNC(Sleep)
    OOLUA_SFUNC(CreateUUID)
	OOLUA_SFUNC(SetScreenTime)
    OOLUA_SFUNC(SetSceenEnable)
    OOLUA_SFUNC(GetOSRunTime)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(App)
OOLUA_EXPORT_FUNCTIONS_CONST(App)

OOLUA_PROXY(,DataTime)

    OOLUA_CTORS(

    )
    OOLUA_MGET_MSET(year, GetYear, SetYear)
    OOLUA_MGET_MSET(month, GetMonth, SetMonth)
    OOLUA_MGET_MSET(day, GetDay, SetDay)
    OOLUA_MGET_MSET(hour, GetHour, SetHour)
    OOLUA_MGET_MSET(min, GetMin, SetMin)
    OOLUA_MGET_MSET(sec, GetSec, SetSec)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(DataTime, SetYear, SetMonth, SetDay, SetHour, SetMin, SetSec)
OOLUA_EXPORT_FUNCTIONS_CONST(DataTime, GetYear, GetMonth, GetDay, GetHour, GetMin, GetSec)

OOLUA_PROXY(,SetSystemTime)

    OOLUA_CTORS(

    )

    OOLUA_SFUNC(SetDataTime)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(SetSystemTime)
OOLUA_EXPORT_FUNCTIONS_CONST(SetSystemTime)

/**
 * @brief  单位转换类型。
 * @details
 */
OOLUA_PROXY(,UnitChange)
    OOLUA_TAGS(
        Register_class_enums  // 枚举需要特别标注，否则枚举值无效
    )
    OOLUA_ENUMS(
        OOLUA_ENUM(Read)    // 枚举值
        OOLUA_ENUM(Write)   // 枚举值
    )
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(UnitChange)  // 导出函数声明（没有函数也需要声明）

AppProxy::AppProxy()
{

}

void AppProxy::Proxy(OOLUA::Script *lua)
{
    lua->register_class<App>();
    lua->register_class_static<App>("Sleep",
             &OOLUA::Proxy_class<App>::Sleep); // 静态函数要单独注册
    lua->register_class_static<App>("CreateUUID",
             &OOLUA::Proxy_class<App>::CreateUUID); // 静态函数要单独注册
	lua->register_class_static<App>("SetScreenTime",
             &OOLUA::Proxy_class<App>::SetScreenTime);
    lua->register_class_static<App>("SetSceenEnable",
             &OOLUA::Proxy_class<App>::SetSceenEnable);
    lua->register_class_static<App>("GetOSRunTime",
             &OOLUA::Proxy_class<App>::GetOSRunTime);
    lua->register_class<DataTime>();
    lua->register_class<SetSystemTime>();
    lua->register_class_static<SetSystemTime>("SetDataTime",
             &OOLUA::Proxy_class<SetSystemTime>::SetDataTime);
    lua->register_class<UnitChange>();
}
