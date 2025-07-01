
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include "log4cpp/PropertyConfigurator.hh"
#include "Log.h"

using namespace std;
using namespace log4cpp;

log4cpp::Category* logger = NULL;

unique_ptr<Log> Log::m_instance(nullptr);

Log *Log::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new Log);
    }

    return m_instance.get();
}

void Log::Init()
{
    // 使用指定目录的配置文件初始化日志系统
    QString strFile = QApplication::applicationDirPath() + "/log4cpp.conf";

    // 不存在则把二进制程序自带的资源中的原始配置文件拷贝出来
    if (!QFile::exists(strFile))
    {
        QFile sourceFile(":/config/log4cpp.conf");
        QFile destFile(strFile);
        if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) &&
            destFile.open(QIODevice::WriteOnly))
        {
            while (!sourceFile.atEnd())
            {
                destFile.write(sourceFile.readLine());
            }
        }
    }

    log4cpp::PropertyConfigurator::configure(strFile.toStdString());

    // 实例化category对象
    log4cpp::Category& root = log4cpp::Category::getRoot();
    logger = &log4cpp::Category::getInstance(std::string("RootLog"));
}

log4cpp::Category *Log::GetLogger()
{
    return logger;
}

OOLUA_PROXY(,Category)
    OOLUA_TAGS(
        No_default_constructor
    )

//    OOLUA_MEM_FUNC(void, debug, const char*, ) // 改名版本：手工指定代理函数名
//    OOLUA_MEM_FUNC(void, debug, cpp_in_p<std::string&>)   // 改名版本：手工指定代理函数名
    OOLUA_MEM_FUNC(void, debug, const std::string&)
    OOLUA_MEM_FUNC(void, info, const std::string&)
    OOLUA_MEM_FUNC(void, notice, const std::string&)
    OOLUA_MEM_FUNC(void, warn, const std::string&)
    OOLUA_MEM_FUNC(void, error, const std::string&)
    OOLUA_MEM_FUNC(void, crit, const std::string&)
    OOLUA_MEM_FUNC(void, alert, const std::string&)
    OOLUA_MEM_FUNC(void, emerg, const std::string&)
    OOLUA_MEM_FUNC(void, fatal, const std::string&)

OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(Category, debug, info, notice, warn, error, crit, alert, fatal)
OOLUA_EXPORT_FUNCTIONS_CONST(Category)


OOLUA_PROXY(,Log)
    OOLUA_TAGS(
        //No_default_constructor,
        No_public_constructors  // 单例，不能在 lua 中创建对象
    )
    OOLUA_SFUNC(Instance)  // 单便入口函数
//    OOLUA_MEM_FUNC(maybe_null<lua_return<Category*> >, GetLogger)
    OOLUA_MFUNC(GetLogger)


OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(Log, GetLogger)
OOLUA_EXPORT_FUNCTIONS_CONST(Log)

void Log::Proxy(OOLUA::Script * lua)
{
    lua->register_class<Category>();
    lua->register_class<Log>();
    lua->register_class_static<Log>("Instance",
             &OOLUA::Proxy_class<Log>::Instance); // 静态函数要单独注册

}

