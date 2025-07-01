#include "Log.h"
#include "RelayControl.h"

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

#define LABSUN_IO(n)     (_IO('z',n))
#define CMD_RELAY_ON     LABSUN_IO(5)
#define CMD_RELAY_OFF    LABSUN_IO(6)

using namespace std;

namespace Interface
{
namespace Control
{

unique_ptr<RelayControl> RelayControl::m_instance(nullptr);

RelayControl* RelayControl::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new RelayControl);
    }

    return m_instance.get();
}

bool RelayControl::Init()
{
#ifdef _DEV_USING_CONTROL
    int fd = open(DEV_NAME,O_RDWR);
    if(fd < 0)
    {
        logger->warn("继电器驱动打开失败");
    }
    ::close(fd);
#endif

    return true;
}

void RelayControl::TurnOn(unsigned long  port)
{
#ifdef _DEV_USING_CONTROL
    int fd = open(DEV_NAME,O_RDWR);
    if(fd < 0)
    {
        logger->warn("继电器驱动打开失败");
    }
    else
    {
        ioctl(fd, CMD_RELAY_ON, &port);
    }

    ::close(fd);

#else
    (void)port;
#endif
}

void RelayControl::TurnOff(unsigned long  port)
{
#ifdef _DEV_USING_CONTROL
    int fd = open(DEV_NAME,O_RDWR);
    if(fd < 0)
    {
        logger->warn("继电器驱动打开失败");
    }
    else
    {
        ioctl(fd, CMD_RELAY_OFF, &port);
    }

    ::close(fd);

#else
    (void)port;
#endif
}

}
}
