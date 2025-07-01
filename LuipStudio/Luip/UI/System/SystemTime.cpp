#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "SystemTime.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include <time.h>
#include <stdio.h>
#ifndef  _CS_X86_WINDOWS
#include <sys/time.h>
#endif
#include "Setting/SettingManager.h"
#include "Controller/ControllerManager.h"
#include "AlarmManager/AlarmManager.h"
#include "Log.h"
#include "System/Types.h"
#include "oolua.h"
#include "LuaEngine/LuaEngine.h"
#include "UI/Frame/MessageDialog.h"
#include "FlowManager/MeasureScheduler.h"
#include "FlowManager/FlowManager.h"
#include "System/SetSystemTime.h"

using namespace System;
using namespace Controller;
using namespace Communication;
using namespace Configuration;
using namespace Measure;
using namespace Lua;
using namespace Flow;
using namespace OOLUA;
using namespace Flow;

namespace UI
{

SystemTimeWidget::SystemTimeWidget(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    this->resize(650, 400);

    QLabel *dayLabel = new QLabel();
    dayLabel->setText("日期");
    dayLabel->setFixedSize(40, 30);
    dayEdit = new QMyEdit();
    dayEdit->setFixedSize(150, 30);
    QHBoxLayout *dayLayout = new QHBoxLayout();
    dayLayout->addWidget(dayLabel);
    dayLayout->addWidget(dayEdit);

    dayLabel->setFont(font);
    dayEdit->setFont(font);
    dayLayout->setContentsMargins(20, 30, 0, 0);
    dayLayout->addStretch();

    QLabel *timeLabel = new QLabel();
    timeLabel->setText("时间");
    timeLabel->setFixedSize(40, 30);
    timeEdit = new QLineEdit();
    timeEdit->setFixedSize(150, 30);
    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeEdit);
    timeLabel->setFont(font);
    timeEdit->setFont(font);
    timeLayout->setContentsMargins(20, 10, 0, 0);
    timeLayout->addStretch();

    QHBoxLayout *confirmLayout = new QHBoxLayout();
    confirmButton = new QPushButton();
    confirmButton->setObjectName("brownButton");
    confirmButton->setText("确认");
    confirmButton->setFont(font);
    confirmButton->setFixedSize(90, 35);
    confirmLayout->addWidget(confirmButton);

    confirmLayout->setContentsMargins(20, 10, 0, 0);
    confirmLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addLayout(dayLayout);
    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(confirmLayout);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    dayCaledar = new QCalendarWidget(this);
    dayCaledar->hide();

    timeEdit->installEventFilter(CNumberKeyboard::Instance());
    OnUserChange();

    connect(dayEdit, SIGNAL(LineEditClicked()), this, SLOT(ChoseMinDay()));

    connect(dayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMinDay()));

    connect(confirmButton, SIGNAL(clicked(bool)), this, SLOT(SetTime()));
}

void SystemTimeWidget::SetTime()
{
    if (!FlowManager::Instance()->IsFlowEnable())  // 空闲状态
    {
        MessageDialog msg("非空闲，禁止修改系统时间！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    QString dayValue = dayEdit->text();
    QStringList dayList = dayValue.split("-");

    if(!dayValue.isEmpty())
    {
        if(dayList.at(0).toInt() < 1970 || dayList.at(0).toInt()>= 2038)
        {
            MessageDialog msg1("输入年份超范围！\n", this,MsgStyle::ONLYOK);
            if(msg1.exec()== QDialog::Accepted)
            {
                return;
            }
        }
    }

    QString timeValue = timeEdit->text();
    QStringList timeList = timeValue.split(":");

    QString tmpPattern("^\\d{1,2}$");  //匹配1-2位整数
    QRegExp tmpRx(tmpPattern);

    if(!timeValue.isEmpty())
    {
        if(timeList.size() != 3 ||
           !tmpRx.exactMatch(timeList.at(0)) || timeList.at(0).toInt() < 0 || timeList.at(0).toInt() > 23 ||
           !tmpRx.exactMatch(timeList.at(1)) || timeList.at(1).toInt() < 0 || timeList.at(1).toInt() > 59 ||
           !tmpRx.exactMatch(timeList.at(2)) || timeList.at(2).toInt() < 0 || timeList.at(2).toInt() > 59 )
        {
            MessageDialog msg2("输入时间格式不正确！\n", this,MsgStyle::ONLYOK);
            if(msg2.exec()== QDialog::Accepted)
            {
                return;
            }
        }
    }

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    bool bridgeMode;
    luaEngine->GetLuaValue(state, "config.system.bridgeMode", bridgeMode);
    if (bridgeMode)
    {
        MainController * MC = ControllerManager::Instance()->MC;

        QDateTime localTime(QDate(dayList.at(0).toInt(), dayList.at(1).toInt(), dayList.at(2).toInt()),
                            QTime(timeList.at(0).toInt(), timeList.at(1).toInt(), timeList.at(2).toInt()));
        QDateTime utcTime(localTime.date(),localTime.time(),Qt::UTC);
        qDebug("SetSystemTime %s , %d", utcTime.toString("yyyy-MM-dd hh:mm:ss").toLatin1().data(),
               utcTime.toTime_t());
        try
        {
            if (!MC->IInstrumentInfo->SetSystemTime(utcTime.toTime_t()))
            {
                qDebug("SetSystemTime false");
            }
        }
        catch(CommandTimeoutException e)
        {
            e.m_name = "主控板";
            AlarmManager::Instance()->AddCommunicationExceptionAlarm(e);
        }
        catch (std::exception e)
        {
            AlarmManager::Instance()->AddExceptionAlarm(e);
        }
    }
    else
    {
        struct tm *t_tm;
        time_t t_timet;

        t_timet=time(NULL);
        t_tm=localtime(&t_timet);
        if(timeList.size() == 3)
        {
            t_tm->tm_hour = timeList.at(0).toInt();
            t_tm->tm_min = timeList.at(1).toInt();
            t_tm->tm_sec = timeList.at(2).toInt();
        }

        if(dayList.size() == 3)
        {
            t_tm->tm_year = dayList.at(0).toInt() - 1900;
            t_tm->tm_mon = dayList.at(1).toInt() - 1;
            t_tm->tm_mday = dayList.at(2).toInt();
        }

        System::SetSystemTime::SetTime(t_tm->tm_year + 1900, t_tm->tm_mon + 1, t_tm->tm_mday,
                                       t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
        return;
    }
}

void SystemTimeWidget::ChoseMinDay()
{
    dayCaledar->setFixedSize(300, 300);
    dayCaledar->move(130, 120);
    dayCaledar->setLocale(QLocale(QLocale::Chinese, QLocale::China));

    dayCaledar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    if (dayCaledar->isHidden())
    {
        dayCaledar->show();
    }
    else
    {
        dayCaledar->hide();
    }
}

void SystemTimeWidget::ChangeMinDay()
{
    QDate date = dayCaledar->selectedDate();
    QString dateString;
    QString monthsign;
    QString daysign;
    if (date.month() >= 10)
    {
        monthsign = "-";
    }
    else
    {
        monthsign = "-0";
    }

    if (date.day() >= 10)
    {
        daysign = "-";
    }
    else
    {
        daysign = "-0";
    }

    dateString = QString::number(date.year())+monthsign+QString::number(date.month())+daysign+QString::number(date.day());

    dayEdit->setText(dateString);
    dayCaledar->hide();
}

void SystemTimeWidget::OnUserChange()
{
//    if((LoginDialog::userType == Administrator))
//    {
//        confirmButton->setEnabled(true);
//        dayEdit->setEnabled(true);
//        timeEdit->setEnabled(true);
//    }
//    else
//    {
//        confirmButton->setEnabled(false);
//        dayEdit->setEnabled(false);
//        timeEdit->setEnabled(false);
//    }
}

void SystemTimeWidget::showEvent(QShowEvent *event)
{
    (void)event;
    this->setFocus();
    if (!dayCaledar->isHidden())
    {
        dayCaledar->hide();
    }
}

SystemTimeWidget::~SystemTimeWidget()
{

}

}

