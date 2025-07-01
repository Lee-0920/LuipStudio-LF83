#ifndef DYNAMICPASSWORD_H
#define DYNAMICPASSWORD_H

#include <QString>
#include <QDateTime>

namespace System {

static const QString keyWord = "LuipAdmin@";

class DynamicPassword
{
public:
    static QString GeneratePassword(QDateTime time = QDateTime::currentDateTime());
    static QStringList GeneratePasswordList();

};

}


#endif // DYNAMICPASSWORD_H
