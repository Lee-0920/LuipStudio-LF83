#include <QDateTime>
#include "Log.h"
#include "oolua.h"
#include "LuaEngine/LuaEngine.h"
#include "Setting/Environment.h"
#include "AlarmManager/AlarmManager.h"
#include "FlowManager/MeasureScheduler.h"
#include "AuthorizationManager.h"

using namespace std;
using namespace OOLUA;
using namespace Lua;
using namespace Measure;
using namespace Flow;

namespace System
{
std::unique_ptr<AuthorizationManager> AuthorizationManager::m_instance(nullptr);

AuthorizationManager::~AuthorizationManager()
{

}

AuthorizationManager *AuthorizationManager::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new AuthorizationManager);
    }

    return m_instance.get();

}

String AuthorizationManager::GetValidity()
{
   String str = "0000-00-00 00:00:00";

   try
   {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State *lua = luaEngine->GetThreadState();

        Uint32 expirationDate;
        luaEngine->GetLuaValue(lua, "config.info.instrument.expirationDate", expirationDate);

        str = QDateTime::fromTime_t(expirationDate).toString("yyyy-MM-dd hh:mm:ss").toStdString();
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("AuthorizationManager::Validity() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("AuthorizationManager::Validity() => %s", e.what());
    }

   return str;
}

bool AuthorizationManager::GetAuthorization()
{
    bool isValid = true;

    try
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State *lua = luaEngine->GetThreadState();


        bool permanent = false;
        luaEngine->GetLuaValue(lua, "config.info.instrument.permanent", permanent);

        if (!permanent)
        {
             bool license = false;
             luaEngine->GetLuaValue(lua, "config.info.instrument.license", license);

             if (license)
             {
                if (!this->Check())
                {
                    isValid = false;
                }
             }
             else
             {
                isValid = false;
             }
        }
     }
     catch(OOLUA::Exception e)
     {
         logger->warn("AuthorizationManager::GetAuthorization() => %s", e.what());
     }
     catch(std::exception e)
     {
         logger->warn("AuthorizationManager::GetAuthorization() => %s", e.what());
     }

     return isValid;
}

void AuthorizationManager::SetAuthorization(Uint32 expirationDate)
{
    try
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State *lua = luaEngine->GetThreadState();
        Lua_function call(lua);

        Lua_func_ref SaveEncryptionFile;
        luaEngine->GetLuaValue(lua, "Serialization.SaveEncryptionFile", SaveEncryptionFile);

        Table instrument;
        luaEngine->GetLuaValue(lua, "config.info.instrument", instrument);

        instrument.set("expirationDate", expirationDate);
        instrument.set("permanent", false);

        String path = Configuration::Environment::Instance()->GetDeviceDataPath() + "/InstrumentInfomation.ls";
        call(SaveEncryptionFile, instrument, path, "config.info.instrument");

        this->Check();
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("AuthorizationManager::SetAuthorization() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("AuthorizationManager::SetAuthorization() => %s", e.what());
    }
}

bool AuthorizationManager::Check()
{
    bool license = false;

    try
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State *lua = luaEngine->GetThreadState();
        Lua_function call(lua);

        Lua_func_ref SaveEncryptionFile;
        luaEngine->GetLuaValue(lua, "Serialization.SaveEncryptionFile", SaveEncryptionFile);

        Table instrument;
        luaEngine->GetLuaValue(lua, "config.info.instrument", instrument);


        luaEngine->GetLuaValue(lua, "config.info.instrument.license", license);

        Uint32 expirationDate;
        luaEngine->GetLuaValue(lua, "config.info.instrument.expirationDate", expirationDate);

        Uint32 curTime = QDateTime::currentDateTime().toTime_t();

        if (curTime > expirationDate)
        {
            if (license)
            {
                Alarm alarm("系统异常", "运行权限", "过期", "Error");
                AlarmManager::Instance()->AddAlarm(alarm);

                instrument.set("license", false);
                String path = Configuration::Environment::Instance()->GetDeviceDataPath() + "/InstrumentInfomation.ls";
                call(SaveEncryptionFile, instrument, path, "config.info.instrument");

                license = false;
            }
        }
        else
        {
            if (!license)
            {
                instrument.set("license", true);
                String path = Configuration::Environment::Instance()->GetDeviceDataPath() + "/InstrumentInfomation.ls";
                call(SaveEncryptionFile, instrument, path, "config.info.instrument");

                license = true;
                MeasureScheduler::Instance()->Update();
            }
        }
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("AuthorizationManager::Check() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("AuthorizationManager::Check() => %s", e.what());
    }

    return license;
}

}
