#ifndef AuthorizationManager_H
#define AuthorizationManager_H

#include <memory>
#include "System/Types.h"

namespace System
{
class AuthorizationManager
{
public:
    ~AuthorizationManager();

    static AuthorizationManager* Instance();
    String GetValidity();
    bool GetAuthorization();
    void SetAuthorization(Uint32 expirationDate);

protected:
    bool Check();

private:
    static std::unique_ptr<AuthorizationManager> m_instance;
    bool m_isAlarm;

};

}

#endif // AuthorizationManager_H
