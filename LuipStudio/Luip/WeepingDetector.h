#ifndef WEEPINGDETECTOR_H
#define WEEPINGDETECTOR_H
#include <memory>
#include <list>
#include <QThread>
#include "System/Types.h"

using System::String;

namespace Weeping
{
class WeepingDetector : public QThread
{
    Q_OBJECT
public:
    static WeepingDetector *Instance();
    bool Init();

    bool Start();
    void Stop();

protected:
    void run();

private:
    void Detect();

private:
    bool m_isWeeping;
    String m_fileName;
    bool m_isRuning;
    static std::unique_ptr<WeepingDetector> m_instance;

};
}

#endif // WEEPINGDETECTOR_H
