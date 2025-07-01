#ifndef LF82DRIVECONTROLLERPLUGINPROXY_H
#define LF82DRIVECONTROLLERPLUGINPROXY_H

#include "oolua.h"
#include "LuipShare.h"
#include "OpticalADCurve.h"

using Controller::OpticalADCurve;
/**
 * @brief 光学信号曲线类。
 * @details
 */
OOLUA_PROXY(LUIP_SHARE,OpticalADCurve)
    OOLUA_TAGS(
        No_default_constructor //没有默认构造函数时可以使用此标签
    )
    OOLUA_CTORS(
        OOLUA_CTOR(int)
    )
    OOLUA_MFUNC(SetData)
    OOLUA_MFUNC(GetWholeData)
    OOLUA_MFUNC_CONST(GetData)
    OOLUA_MFUNC_CONST(GetLength)
    OOLUA_MFUNC_CONST(GetBegin)
    OOLUA_MFUNC_CONST(GetEnd)
OOLUA_PROXY_END

class LF82DriveControllerPluginProxy
{
public:
    LF82DriveControllerPluginProxy();

    static void Proxy();
};


#endif // LF82DRIVECONTROLLERPLUGINPROXY_H
