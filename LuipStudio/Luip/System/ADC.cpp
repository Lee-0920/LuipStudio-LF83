#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <QString>
#include <QThread>

#ifdef _DEV_USING_CURRENT
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif

#include "ADC.h"

namespace System
{


ADC::ADC(String driveFile)
{
#ifdef _DEV_USING_CURRENT


    const char *device = driveFile.c_str();

    m_fd = open(device, O_RDONLY);
    if(m_fd < 0)
    {
        qDebug(" %s can't open device", device);
    }
    else
    {

    }
#else
    (void)driveFile;
#endif
}

ADC::~ADC()
{
#ifdef _DEV_USING_CURRENT
    ::close(m_fd);
#endif
}

Uint16 System::ADC::ReadValue()
{
    Uint16 adVal = 0;

#ifdef _DEV_USING_CURRENT

    char buffer[24] = "";                    //����һ��buffer
    memset(buffer,0,sizeof(buffer));

    int len = ::read(m_fd, buffer, sizeof(buffer));            //��adc�豸�ж�ȡ����Ϊsizeof��buffer�����ݵ�buffer��

    if (len > 0)
    {
        sscanf(buffer, "%d", &adVal);        //��buffer�е����ݴ浽value��
    }


#endif

    return adVal;
}

}
