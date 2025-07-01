
#include <QApplication>
#include <QProcess>
#include "ScreenCalibrate.h"
#include "Log.h"
#include <QFile>
#include <QThread>
#include "Log.h"
#include "Setting/Environment.h"

using namespace Configuration;

namespace UI
{
ScreenCalibrate::ScreenCalibrate(QWidget *parent):
QWidget(parent),m_timerCnt(10)
{
    m_timer = nullptr;
    m_msgBox = nullptr;

    QFile file("/opt/tq335x/tslib/bin/ts_calibrate");
    if (file.exists())
    {
         m_timer = new QTimer;
         connect(m_timer, SIGNAL(timeout()), this, SLOT(StartCalibrate()));

         m_msgBox = new MessageDialog("校准会终止当前的测量流程\n完成后系统将自动复位\n即将进行触摸屏校准:\n10",
                                         this ,MsgStyle::ONLYCANCEL);
         m_timer->start(1000);//启动倒计时定时器
         int ret = m_msgBox->exec();
         if(ret == QDialog::Rejected)//按下取消键或者10秒倒计时时间到
         {
             m_timer->stop();
         }
    }
    else
    {
        logger->warn("校准程序不存在");
        MessageDialog msg("校准程序不存在\n", this);
        msg.exec();
    }
}

ScreenCalibrate::~ScreenCalibrate()
{
   if( nullptr != m_timer)
   {
       m_timer->stop();
       delete m_timer;
   }
   if( nullptr != m_msgBox)
   {
       delete m_msgBox;
   }
}

void ScreenCalibrate::StartCalibrate()
{
    m_timerCnt--;
    m_msgBox->SetText("校准会终止当前的测量流程\n完成后系统将自动复位\n即将进行触摸屏校准:\n" + QString::number(m_timerCnt));//每秒提醒框时间减1
    if(m_timerCnt <= 0)
    {
        m_timerCnt = 10;
        m_msgBox->close();//关闭提醒框

        if (false == QProcess::startDetached(Environment::Instance()->GetAppPath().c_str() + QString("/Calibrate")))//启动触摸屏校准启动进程
        {
            logger->debug("触摸屏校准启动失败");
            logger->debug("工作目录 %s", QDir::currentPath().toLatin1().data());
        }
    }
}

}
