#include "Log.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include <QTableWidget>
#include <QFont>
#include "UI/Frame/NumberKeyboard.h"
#include "Setting/Environment.h"
#include "AlarmManager/AlarmManager.h"
#include "WarnMangerWidget.h"
#include "LuaEngine/LuaEngine.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "LuaException.h"

#define FileName  ("AlarmLog.txt")

using namespace Configuration;
using namespace System;
using namespace OOLUA;
using namespace Lua;

namespace UI
{

WarnManager::WarnManager(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);
    AlarmManager::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    m_nextFlag = 0;
    m_backFlag = 0;
    warnLog = new TextfileParser(QString(Environment::Instance()->GetAppDataPath().c_str()) + QString("/") + FileName);

    this->resize(650,400);

    minTime = new QMyEdit();
    minTime->setFixedSize(115,32);
    minTime->setText(GetMinTime());
    minTime->setFont(font);

    QHBoxLayout *minTimeLayout = new QHBoxLayout();
    minTimeLayout->addWidget(minTime);
    minTimeLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *toTime = new QLabel();
    toTime->setText("-");
    toTime->setFixedSize(5,30);

    QHBoxLayout *toTimeLayout = new QHBoxLayout();
    toTimeLayout->addWidget(toTime);
    toTimeLayout->setContentsMargins(0, 0, 0, 0);

    maxTime = new QMyEdit();
    maxTime->setFixedSize(115,32);
    maxTime->setFont(font);
    maxTime->setText(GetMaxTime());

    QHBoxLayout *maxTimeLayout = new QHBoxLayout();
    maxTimeLayout->addWidget(maxTime);
    maxTimeLayout->addStretch();
    maxTimeLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *dateModel = new QLabel();
    dateModel->setFixedSize(32,40);
    dateModel->setText("类型");
    dateModel->setFont(font);

    QHBoxLayout *dateModelLayout = new QHBoxLayout();
    dateModelLayout->addWidget(dateModel);
    dateModelLayout->setContentsMargins(0, 0, 0, 0);

    dateModelCombobox = new QComboBox();
    dateModelCombobox->setFixedSize(100,30);
    dateModelCombobox->setFont(font);

    Script *lua = LuaEngine::Instance()->GetEngine();

    Table table1, table2;
    table1.bind_script(*lua);
    table1.set_table("setting");
    table1.at("ui", table2);
    table2.at("warnManager", table1);
    table1.at("dateModel", table2);

    QStringList dateModelList;

    oolua_ipairs(table2)
    {
        String text;
        lua->pull(text);
        dateModelList << QString::fromUtf8(text.c_str());
    }
    oolua_ipairs_end()

    dateModelCombobox->insertItems(0, dateModelList);


    QHBoxLayout *dateModelComLayout = new QHBoxLayout();
    dateModelComLayout->addWidget(dateModelCombobox);
    dateModelComLayout->setContentsMargins(0, 0, 0, 0);

    searchButton = new QPushButton();
    searchButton->setObjectName("brownButton");
    searchButton->setText("检索");
    searchButton->setFont(font);
    searchButton->setFixedSize(80,40);

    QHBoxLayout *leftBottomLayout = new QHBoxLayout();
    leftBottomLayout->addLayout(minTimeLayout);
    leftBottomLayout->addLayout(toTimeLayout);
    leftBottomLayout->addLayout(maxTimeLayout);
    leftBottomLayout->addLayout(dateModelLayout);
    leftBottomLayout->addLayout(dateModelComLayout);
    leftBottomLayout->addWidget(searchButton);

    warnManngerTableWidget = new MQtableWidget();
    warnManngerTableWidget->resize(550,350);
    warnManngerTableWidget->setColumnCount(3);
    warnManngerTableWidget->setRowCount(10);
    warnManngerTableWidget->setFixedSize(550,340);

    QStringList columnName;
    columnName<<"时间"<<"类别"<<"内容";

    warnManngerTableWidget->setColumn(columnName);
    warnManngerTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度
    warnManngerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑
    warnManngerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置表格为整行选择
    warnManngerTableWidget->verticalScrollBar()->setVisible(false);
    warnManngerTableWidget->verticalScrollBar()->setDisabled(true);

    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(warnManngerTableWidget);
    measureResultTableLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(measureResultTableLayout);
    leftLayout->addStretch();
    leftLayout->addLayout(leftBottomLayout);

    toTopButton = new QPushButton();
    toTopButton->setObjectName("brownButton");
    toTopButton->setText("|<");
    toTopButton->setFont(font);
    toTopButton->setFixedSize(80,40);

    toBackButton = new QPushButton();
    toBackButton->setObjectName("brownButton");
    toBackButton->setText("<<");
    toBackButton->setFont(font);
    toBackButton->setFixedSize(80,40);

    toNextButton = new QPushButton();
    toNextButton->setObjectName("brownButton");
    toNextButton->setText(">>");
    toNextButton->setFont(font);
    toNextButton->setFixedSize(80,40);

    toBottomButton = new QPushButton();
    toBottomButton->setObjectName("brownButton");
    toBottomButton->setText(">|");
    toBottomButton->setFont(font);
    toBottomButton->setFixedSize(80,40);

    exportButton = new QPushButton();
    exportButton->setObjectName("brownButton");
    exportButton->setText("导出");
    exportButton->setFont(font);
    exportButton->setFixedSize(80,40);

    clearButton = new QPushButton();
    clearButton->setObjectName("brownButton");
    clearButton->setText("清空");
    clearButton->setFont(font);
    clearButton->setFixedSize(80,40);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(toTopButton);
    rightLayout->addWidget(toBackButton);
    rightLayout->addWidget(toNextButton);
    rightLayout->addWidget(toBottomButton);
    rightLayout->addWidget(exportButton);
    rightLayout->addWidget(clearButton);
    rightLayout->addStretch();
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);

    this->setLayout(mainLayout);

    warnManngerTableWidget->setColumnWidth(0,175);    //设置每行或每列的高度或宽度
    warnManngerTableWidget->setColumnWidth(1,100);
    warnManngerTableWidget->setColumnWidth(2,273);

    warnManngerTableWidget->setFont(font);
    for(int i = 0;i < 10;i++)
    {
        warnManngerTableWidget->setRowHeight(i,30.3);
    }

    minDayCaledar = new QCalendarWidget(this);
    maxDayCaledar = new QCalendarWidget(this);
    minDayCaledar->hide();
    maxDayCaledar->hide();

    connect(toTopButton,SIGNAL(clicked()), this, SLOT(ToTop()));
    connect(toBottomButton,SIGNAL(clicked()), this, SLOT(ToBottom()));
    connect(toBackButton,SIGNAL(clicked()), this, SLOT(ToBack()));
    connect(toNextButton,SIGNAL(clicked()), this, SLOT(ToNext()));
    connect(searchButton, SIGNAL(clicked()) ,this, SLOT(ToTop()));
    connect(exportButton,SIGNAL(clicked()), this, SLOT(SlotExportButton()));
    connect(clearButton, SIGNAL(clicked()) ,this, SLOT(SlotClearButton()));

    connect(minTime, SIGNAL(LineEditClicked()) , this, SLOT(ChoseMinDay()));
    connect(maxTime, SIGNAL(LineEditClicked()), this, SLOT(ChoseMaxDay()));
    connect(minDayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMinDay()));
    connect(maxDayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMaxDay()));
    connect(dateModelCombobox, SIGNAL(currentTextChanged(QString)), this, SLOT(ChangePage()));
    connect(this, SIGNAL(UpdaterWarnSignals()), this, SLOT(UpdaterWarnSlot()));

    SpaceInit();
    ToTop();
    this->ChangeBottomStatus();
}

void WarnManager::ToTop()
{
    ViewRefresh();
    SpaceInit();
    m_nextFlag = 0;
    m_backFlag = 0;

    QString pattern("(.*)( .*)( .*)");
    int row = 0;
    m_currentBlock = 0;

    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    while(row < 10 && m_currentBlock < warnLog->TotalBlock())
    {
        char *s = warnLog->SeekBlock(m_currentBlock);
        QString str(s);
        QStringList strlist;
        QStringList newStrlist;
        strlist = str.split("\r\n");

        newStrlist = SetTypeAndDate(dateModelCombobox->currentText(), theMinDateTime, theMaxDateTime, strlist, pattern);

        m_currentPage = newStrlist.size();

        while(m_currentPage > 0 && row < 10)
        {
            m_currentPage--;
            QString m = newStrlist.at(m_currentPage);
            if(m.size() > 0)
            {
                QRegExp rx(pattern);
                m.indexOf(rx);
                if(rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
                {
                    warnManngerTableWidget->item(row ,0)->setText(rx.cap(1).trimmed());
                    warnManngerTableWidget->item(row ,1)->setText(rx.cap(2).trimmed());
                    warnManngerTableWidget->item(row ,2)->setText(rx.cap(3).trimmed());
                    row++;
                }
            }
        }

        if(row < 10)
        {
            if(m_currentBlock + 1 < warnLog->TotalBlock())
            {
                m_currentBlock++;
            }
            else
            {
                break;
            }
        }
    }
}

void WarnManager::ToNext()        //返回老数据，文件指针前移动
{
    if (m_backFlag == 1)
    {
        m_backFlag = 0;
        ToNext();
    }

    ViewRefresh();
    SpaceInit();
    QString pattern("(.*)( .*)( .*)");
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    char *s =  warnLog->SeekBlock(m_currentBlock);
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split("\r\n");

    newStrlist = SetTypeAndDate(dateModelCombobox->currentText(), theMinDateTime, theMaxDateTime, strlist, pattern);

    int row = 0;

    while (row < 10)
    {
        while (m_currentPage > 0 && row < 10)
        {
             m_currentPage--;
            if (newStrlist.at(m_currentPage).size() > 0)
            {
                QRegExp rx(pattern);
                QString m = newStrlist.at(m_currentPage);
                m.indexOf(rx);

                if(rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
                {
                    warnManngerTableWidget->item(row ,0)->setText(rx.cap(1).trimmed());
                    warnManngerTableWidget->item(row ,1)->setText(rx.cap(2).trimmed());
                    warnManngerTableWidget->item(row ,2)->setText(rx.cap(3).trimmed());
                    row++;
                }
            }
        }

        if(row < 10)
        {
            if (m_currentBlock + 1 < warnLog->TotalBlock())
            {
                m_currentBlock++;
                char *s = warnLog->SeekBlock(m_currentBlock);
                QString str(s);
                strlist.clear();
                newStrlist.clear();
                strlist = str.split("\n");
                newStrlist = SetTypeAndDate(dateModelCombobox->currentText(), theMinDateTime, theMaxDateTime, strlist, pattern);
                m_currentPage = newStrlist.size();
            }
            else
            {
                ToBottom();
                break;
            }
        }
        else
        {
            m_nextFlag = 1;
        }
    }
}

void WarnManager::ToBottom()
{
    ViewRefresh();
    SpaceInit();

    m_nextFlag = 0;
    m_backFlag = 0;

    QString pattern("(.*)( .*)( .*)");
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    m_currentBlock = warnLog->TotalBlock() - 1;//最旧的块
    int row = 0;

    while (row < 10)
    {
        char * s = warnLog->SeekBlock(m_currentBlock);

        QString str(s);
        QStringList strlist;
        QStringList newStrlist;
        strlist = str.split("\r\n");

        newStrlist = SetTypeAndDate(dateModelCombobox->currentText(), theMinDateTime, theMaxDateTime, strlist, pattern);

        m_currentPage = 0;//最旧的串

        while (m_currentPage < newStrlist.size() && row < 10 && newStrlist.size() > 0)
        {
            if (newStrlist.at(m_currentPage).size() > 0)
            {
                QRegExp rx(pattern);
                QString m = newStrlist.at(m_currentPage);
                m.indexOf(rx);
                if (rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
                {
                    warnManngerTableWidget->item(9-row ,0)->setText(rx.cap(1).trimmed());
                    warnManngerTableWidget->item(9-row ,1)->setText(rx.cap(2).trimmed());
                    warnManngerTableWidget->item(9-row ,2)->setText(rx.cap(3).trimmed());
                    row++;
                }
            }
            m_currentPage++;
        }

        if(row < 10)
        {
            if(m_currentBlock > 0)
            {
                m_currentBlock--;
            }
            else
            {
                ToTop();
                break;
            }
        }
    }
}

void WarnManager::ToBack()       //返回新数据，文本指针后移
{
    if (m_nextFlag == 1)
    {
        m_nextFlag = 0;
        ToBack();
    }

    ViewRefresh();
    SpaceInit();
    QString pattern("(.*)( .*)( .*)");
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    int row = 0;

    char *s =  warnLog->SeekBlock(m_currentBlock);
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split("\r\n");

    newStrlist = SetTypeAndDate(dateModelCombobox->currentText(), theMinDateTime, theMaxDateTime, strlist, pattern);

    while(row < 10)
    {
        while(m_currentPage < newStrlist.size() && row < 10)
        {
            if(newStrlist.at(m_currentPage).trimmed().size()>0)
            {
                QRegExp rx(pattern);
                QString m = newStrlist.at(m_currentPage);

                m.indexOf(rx);
                if(rx.cap(1).size( )> 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
                {
                    warnManngerTableWidget->item(9-row ,0)->setText(rx.cap(1).trimmed());
                    warnManngerTableWidget->item(9-row ,1)->setText(rx.cap(2).trimmed());
                    warnManngerTableWidget->item(9-row ,2)->setText(rx.cap(3).trimmed());
                    row++;
                }
            }
            m_currentPage++;
        }

        if(row < 10)
        {
            if(m_currentBlock > 0)
            {
                m_currentBlock--;

                char *s =  warnLog->SeekBlock(m_currentBlock);
                QString str(s);
                strlist.clear();
                newStrlist.clear();
                strlist = str.split("\r\n");

                newStrlist = SetTypeAndDate(dateModelCombobox->currentText(), theMinDateTime, theMaxDateTime, strlist, pattern);
                m_currentPage = 0;
            }
            else
            {
                ToTop();
                break;
            }
        }
        else
        {
            m_backFlag = 1;
        }
    }
}


void WarnManager::SlotClearButton()
{
    MessageDialog msg("是否确认清空告警记录?", this, MsgStyle::OKANDCANCEL);
    if(QDialog::Accepted != msg.exec())
    {
        return;
    }
    else
    {
        warnLog->ClearRecord();
        minTime->setText(GetMinTime());
        maxTime->setText(GetMaxTime());
        m_currentPage = 0;
        ToTop();
        emit SignalAlarmClear();
    }
}

void WarnManager::SlotExportButton()
{
    QString result = false;
    CopyFileAction copyFileAction;
    QString strDir = copyFileAction.GetTargetDir().c_str();
    QDir dir(strDir);

    QString sPath = Environment::Instance()->GetAppDataPath().c_str() + QString("/") + QString(FileName);
    QString dPath = dir.filePath(FileName);

    bool isFail = false;

    if (!copyFileAction.ExMemoryDetect()) //U盘检测
    {
        MessageDialog msg("U盘不存在，告警数据导出失败", this);
        msg.exec();
        return;
    }

    if (!copyFileAction.TargetDirCheck(dir)) //拷贝目录检测
    {
        MessageDialog msg("不可在U盘创建目录，告警数据导出失败", this);
        msg.exec();
        return;
    }

    if(copyFileAction.CopyFileToPath(sPath.toStdString(), dPath.toStdString(), true))
    {
        if (!QFile::exists(dPath))
        {
            isFail = true;
        }
    }
    else
    {
        isFail = true;
    }

    if (isFail)
    {
        result = "告警数据导出失败\n";
        logger->info("告警数据导出失败");
    }
    else
    {
        result = "告警数据导出成功\n";
        logger->info("告警数据导出成功");
    }
    MessageDialog msg(result, this);
    msg.exec();
    return;
}

QStringList WarnManager::SetTypeAndDate(QString type, QDateTime& theMinDateTime, QDateTime& theMaxDateTime, QStringList& strlist, QString pattern)
{
    QStringList newStrlist;
    for(int i = 0; i < strlist.size() - 1 ; i++)
    {
        QString m = strlist.at(i);
        QRegExp rx(pattern);
        m.indexOf(rx);
        QDateTime dateTime = QDateTime::fromString(rx.cap(1), "yyyy-MM-dd hh:mm:ss");
        if(rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
        {
            if((type == "所有告警" || rx.cap(2).trimmed() == type)
                && (dateTime > theMinDateTime) && (dateTime < theMaxDateTime))
            {
                newStrlist.append(m);
            }
        }
    }
    return newStrlist;
}

void WarnManager::SpaceInit()
{
    for(int i = 0; i < warnManngerTableWidget->rowCount(); i++)
    {
        for(int j = 0; j < warnManngerTableWidget->columnCount(); j++)
        {
            warnManngerTableWidget->setItem(i, j, new QTableWidgetItem());
            warnManngerTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void WarnManager::OnAlamChanged(Alarm alarmAll)
{
    (void)alarmAll;
    emit UpdaterWarnSignals();
}

void WarnManager::UpdaterWarnSlot()
{
    minTime->setText(QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd"));
    maxTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    ToTop();
}

QDate WarnManager::GetCurrentMinDate()
{
    QString dateString = minTime->text();
    QStringList dateList = dateString.split("-");
    QString year = dateList.at(0);
    QString month = dateList.at(1);
    QString day = dateList.at(2);

    QDate minDate = QDate(year.toInt(),month.toInt(),day.toInt());
    return minDate;
}

QDate WarnManager::GetCurrentMaxDate()
{
    QString dateString = maxTime->text();
    QStringList dateList = dateString.split("-");
    QString year = dateList.at(0);
    QString month = dateList.at(1);
    QString day = dateList.at(2);

    QDate maxDate = QDate(year.toInt(),month.toInt(),day.toInt());
    return maxDate;
}

void WarnManager::ChoseMinDay()
{
    minDayCaledar->setFixedSize(300,300);
    minDayCaledar->move(130,120);
    minDayCaledar->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    minDayCaledar->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
    minDayCaledar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    if(minDayCaledar->isHidden())
    {
        minDayCaledar->show();
        maxDayCaledar->hide();
    }
    else
    {
        minDayCaledar->hide();
        maxDayCaledar->hide();
    }
}

void WarnManager::ChangeMinDay()
{
    QDate date = minDayCaledar->selectedDate();
    QDate curMaxDate = this->GetCurrentMaxDate();
    if(date > curMaxDate)
    {
        minDayCaledar->hide();
        MessageDialog msg("检索起始日期不能晚于结束日期！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    QString dateString;
    QString monthsign;
    QString daysign;
    if(date.month()>=10)
    {
        monthsign = "-";
    }
    else
    {
        monthsign = "-0";
    }

    if(date.day()>=10)
    {
        daysign = "-";
    }
    else
    {
        daysign = "-0";
    }

    dateString = QString::number(date.year())+monthsign+QString::number(date.month())+daysign+QString::number(date.day());

    minTime->setText(dateString);
    minDayCaledar->hide();
}

void WarnManager::ChoseMaxDay()
{
    maxDayCaledar->setFixedSize(300,300);
    maxDayCaledar->move(260,120);
    maxDayCaledar->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    maxDayCaledar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    if(maxDayCaledar->isHidden())
    {
        maxDayCaledar->show();
        minDayCaledar->hide();
    }
    else
    {
        maxDayCaledar->hide();
        minDayCaledar->hide();
    }
}

void WarnManager::ChangeMaxDay()
{
    QDate date = maxDayCaledar->selectedDate();
    QDate curMinDate = this->GetCurrentMinDate();
    if(date < curMinDate)
    {
        maxDayCaledar->hide();
        MessageDialog msg("检索结束日期不能早于起始日期！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    QString dateString;
    QString monthsign;
    QString daysign;
    if(date.month()>=10)
    {
        monthsign = "-";
    }
    else
    {
        monthsign = "-0";
    }

    if(date.day()>=10)
    {
        daysign = "-";
    }
    else
    {
        daysign = "-0";
    }

    dateString = QString::number(date.year())+monthsign+QString::number(date.month())+daysign+QString::number(date.day());
    maxTime->setText(dateString);
    maxDayCaledar->hide();
}

QString WarnManager::GetMinTime()
{
    char *s = warnLog->SeekBlock(0);
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split("\r\n");
    if (strlist.size() > 0)
    {
        for (int i = 0; i < strlist.size() - 1; i++)
        {
            QString pattern("(.*)( .*)( .*)( .*)");
            QRegExp rx(pattern);
            QString m = strlist.at(i);

            m.indexOf(rx);
            if (rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
            {
                newStrlist.append(m);
            }
        }
    }
    else
    {
        return QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd");
    }
    if (newStrlist.size() > 0)
    {
        QString minTime;
        QString pattern("(.*)( .*)( .*)");
        QRegExp rx(pattern);
        QString m = newStrlist.at(newStrlist.size()-1);
        m.indexOf(rx);
        minTime = rx.cap(1);
        QDateTime dateTime = QDateTime::fromString(minTime, "yyyy-MM-dd hh:mm:ss");
        QString fileMinTime = dateTime.addDays(-30).toString("yyyy-MM-dd");
        return fileMinTime;
    }
    else
    {
        return QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd");
    }

}

QString WarnManager::GetMaxTime()
{
    char *s = warnLog->SeekBlock(0);
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split("\r\n");
    if (strlist.size() > 0)
    {
        for (int i = 0; i < strlist.size() - 1; i++)
        {
            QString pattern("(.*)( .*)( .*)( .*)");
            QRegExp rx(pattern);
            QString m = strlist.at(i);

            m.indexOf(rx);
            if (rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
            {
                newStrlist.append(m);
            }
        }
    }
    else
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
    if (newStrlist.size() > 0)
    {
        QString maxTime;
        QString pattern("(.*)( .*)( .*)");
        QRegExp rx(pattern);
        QString m = newStrlist.at(newStrlist.size() - 1);
        m.indexOf(rx);
        maxTime = rx.cap(1);
        QDateTime dateTime = QDateTime::fromString(maxTime, "yyyy-MM-dd hh:mm:ss");
        QString fileMaxTime = dateTime.toString("yyyy-MM-dd");
        return fileMaxTime;
    }
    else
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
}

void WarnManager::ChangePage()
{
    m_currentPage = 0;
}

void WarnManager::ViewRefresh()
{
    warnManngerTableWidget->clear();
    QStringList columnName;
    columnName<<"时间"<<"类别"<<"内容";
    warnManngerTableWidget->setColumn(columnName);
}

void WarnManager::ChangeBottomStatus()
{
    bool solidifyMeaParamFromUI = false;

    try
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State * state = luaEngine->GetThreadState();

        luaEngine->GetLuaValue(state, "config.system.solidifyMeaParamFromUI", solidifyMeaParamFromUI);
    }
    catch(OOLUA::LuaException e)
    {
        logger->warn("MeasureData::ChangeBottomStatus() => %s", e.What().c_str());
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("MeasureData::ChangeBottomStatus() => %s", e.what());
    }
	
    if(LoginDialog::userType == Super)
    {
        exportButton->show();
        clearButton->show();
    }
    else if(LoginDialog::userType == Administrator)
    {
        exportButton->show();
		if (solidifyMeaParamFromUI)
        {
            clearButton->hide();
        }
        else
        {
            clearButton->show();
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        exportButton->hide();
        if (solidifyMeaParamFromUI)
        {
            clearButton->hide();
        }
        else
        {
            clearButton->show();
        }
    }
    else if(LoginDialog::userType == General)
    {
        exportButton->hide();
        clearButton->hide();
    }
}

void WarnManager::OnUserChange()
{
    this->ChangeBottomStatus();
}

void WarnManager::showEvent(QShowEvent *event)
{
    (void)event;
    if(!minDayCaledar->isHidden())
    {
      minDayCaledar->hide();
    }
    if(!maxDayCaledar->isHidden())
    {
      maxDayCaledar->hide();
    }
    this->setFocus();
}

void WarnManager::OnUpdateWidget(UpdateEvent event, System::String message)
{
    (void)message;
    if(event == UpdateEvent::OneKeyClearData)
    {
        warnLog->ClearRecord();
        minTime->setText(GetMinTime());
        maxTime->setText(GetMaxTime());
        m_currentPage = 0;
        ToTop();
        emit SignalAlarmClear();
    }
}


WarnManager::~WarnManager()
{

}

}

