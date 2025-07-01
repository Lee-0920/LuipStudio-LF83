#include "WaterCollector.h"
#include "Log.h"

#ifdef _DEV_USING_CONTROL
    #include <stdlib.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/kernel.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <linux/ioctl.h>
#endif

#define DEV_NAME "/dev/Relay"
#define SWITCH_PORT 1

#define LABSUN_IO(n)     (_IO('z',n))
#define CMD_RELAY_ON     LABSUN_IO(5)
#define CMD_RELAY_OFF    LABSUN_IO(6)

using namespace std;

namespace Interface
{
namespace Control
{

unique_ptr<WaterCollector> WaterCollector::m_instance(nullptr);
bool WaterCollector::m_status = false;

WaterCollector *WaterCollector::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new WaterCollector);
    }
    return m_instance.get();
}

void WaterCollector::Init()
{
    #ifdef _DEV_USING_CONTROL
        int fd = open(DEV_NAME,O_RDWR);
        if(fd < 0)
        {
            logger->warn("采水样继电器驱动打开失败");
        }
        ::close(fd);
    #endif
}

void WaterCollector::TurnOn()
{
    #ifdef _DEV_USING_CONTROL
        int fd = open(DEV_NAME,O_RDWR);
        if(fd < 0)
        {
            logger->warn("采水样继电器驱动打开失败");
        }
        else
        {
            unsigned long  port = SWITCH_PORT;
            ioctl(fd, CMD_RELAY_ON, &port);

            m_status = true;
        }

        ::close(fd);
    #endif
}

void WaterCollector::TurnOff()
{
    #ifdef _DEV_USING_CONTROL
        int fd = open(DEV_NAME,O_RDWR);
        if(fd < 0)
        {
            logger->warn("采水样继电器驱动打开失败");
        }
        else
        {
            unsigned long  port = SWITCH_PORT;
            ioctl(fd, CMD_RELAY_OFF, &port);

            m_status = false;
        }

        ::close(fd);
    #endif
}

bool WaterCollector::GetStatus() const
{
    return m_status;
}
}
}
