#ifndef LF82RESULTDETAILPLUGIN_H
#define LF82RESULTDETAILPLUGIN_H

#include "ResultDetailPlugin/ResultDetailPlugin.h"
#include "LuipShare.h"
#include "oolua.h"
#include "Log.h"
#include <memory>
#include "System/Types.h"

namespace  ResultDetail
{

class LF82ResultDetailPlugin: public ResultDetailPlugin
{
public:
    virtual ~LF82ResultDetailPlugin();
    bool Init(log4cpp::Category* log);
    System::String GetVersion();
private:
    const System::String version = System::String("1.0.0.1");
};

}
// extern "C" 生成的导出符号没有任何修饰，方便主程序找到它
extern "C"
{
    LUIP_SHARE ResultDetail::ResultDetailPlugin *CreatePlugin();
}

#endif // LF82RESULTDETAILPLUGIN_H
