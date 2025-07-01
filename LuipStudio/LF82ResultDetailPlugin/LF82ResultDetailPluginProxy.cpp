#include "LF82ResultDetailPluginProxy.h"
#include "LuaEngine/LuaEngine.h"
#include "LF82ResultDetailWidget.h"
#include "LuipShare.h"
#include "oolua.h"
#include "ResultDetailPlugin/ResultDetailPluginProxy.h"
#include "LF82CurveManager.h"
#include "LF82CurveData.h"

using namespace OOLUA;
using namespace Lua;
using namespace  ResultDetail;

OOLUA_PROXY(, LF82ResultDetailWidget, MeasureResultDetailWidget)
    OOLUA_MFUNC(UpdateDataInfo)
    OOLUA_MFUNC(UpdateWaveInfo)
    OOLUA_MFUNC(ResetScaleValue)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(LF82ResultDetailWidget, UpdateDataInfo, UpdateWaveInfo, ResetScaleValue)
OOLUA_EXPORT_FUNCTIONS_CONST(LF82ResultDetailWidget)

OOLUA_PROXY(, LF82CurveData)
    OOLUA_TAGS(
        No_default_constructor //没有默认构造函数时可以使用此标签
    )
    OOLUA_CTORS(
        OOLUA_CTOR(int)
    )
    OOLUA_MFUNC_CONST(GetData)
    OOLUA_MFUNC(SetData)
    OOLUA_MFUNC_CONST(GetLength)
    OOLUA_MFUNC_CONST(GetSize)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(LF82CurveData, SetData)
OOLUA_EXPORT_FUNCTIONS_CONST(LF82CurveData, GetData, GetLength, GetSize)

OOLUA_PROXY(, LF82CurveFile)
    OOLUA_TAGS(
        No_public_constructors  // 不能在 lua 中创建对象
    )
    OOLUA_MFUNC(ClearCurve)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(LF82CurveFile, ClearCurve)
OOLUA_EXPORT_FUNCTIONS_CONST(LF82CurveFile)

OOLUA_PROXY(, LF82CurveManager)
    OOLUA_TAGS(
        No_public_constructors  // 单例，不能在 lua 中创建对象
    )
    OOLUA_SFUNC(Instance)   //Lua 中访问代码：ResultManager.Instance()
    OOLUA_MFUNC(AddCurve)
    OOLUA_MFUNC(GetCurve)
    OOLUA_MFUNC(GetCurveFile)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(LF82CurveManager, AddCurve, GetCurve, GetCurveFile)
OOLUA_EXPORT_FUNCTIONS_CONST(LF82CurveManager)

namespace  ResultDetail
{
LF82ResultDetailPluginProxy::LF82ResultDetailPluginProxy()
{

}

void LF82ResultDetailPluginProxy::Proxy()
{
    Script *lua = LuaEngine::Instance()->GetEngine();

    OOLUA::register_class<LF82ResultDetailWidget>(*lua);
    OOLUA::register_class<LF82CurveData>(*lua);
    OOLUA::register_class<LF82CurveFile>(*lua);
    OOLUA::register_class<LF82CurveManager>(*lua);
    OOLUA::register_class_static<LF82CurveManager>(*lua, "Instance",
                 &OOLUA::Proxy_class<LF82CurveManager>::Instance);
}
}
