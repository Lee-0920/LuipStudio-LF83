#include "RunLogWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QList>
#include <QDateTime>
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include "Setting/Environment.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include <QDebug>
#include "LuaEngine/LuaEngine.h"
#include "LuaException.h"
#include "System/Uncompress.h"
#include "System/AuthorizationManager.h"
#include "System/DynamicPassword.h"

#define LOGPWD  ("LuipSuper@2018")
#define FileName  ("log.txt")
#ifdef _CS_ARM_LINUX
#define SEPARATOR ("\n")
#else
#define SEPARATOR ("\r\n")
#endif
using namespace System;
using namespace Configuration;
using namespace Lua;

namespace UI
{

RunLog::RunLog(QWidget *parent) :
QWidget(parent),itemSumLine(0),currentItemLine(0)
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    m_nextFlag = 0;
    m_backFlag = 0;
    m_isLoaded = false;

    logFile = new TextfileParser(QString(Environment::Instance()->GetAppDataPath().c_str()) + QString("/") + FileName);

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
    maxTime->setText(GetMaxTime());
    maxTime->setFont(font);

    QHBoxLayout *maxTimeLayout = new QHBoxLayout();
    maxTimeLayout->addWidget(maxTime);
    maxTimeLayout->addStretch();
    maxTimeLayout->setContentsMargins(0, 0, 0, 0);

    searchButton = new QPushButton();
    searchButton->setObjectName("brownButton");
    searchButton->setText("检索");
    searchButton->setFont(font);
    searchButton->setFixedSize(80,40);

    QHBoxLayout *leftBottomLayout = new QHBoxLayout();

    leftBottomLayout->addLayout(minTimeLayout);
    leftBottomLayout->addLayout(toTimeLayout);
    leftBottomLayout->addLayout(maxTimeLayout);
    leftBottomLayout->addWidget(searchButton);

    runLogTableWidget = new MQtableWidget();
    runLogTableWidget->setColumnCount(3);
    runLogTableWidget->setRowCount(10);
    runLogTableWidget->setFixedSize(550,340);

    QStringList columnName;
    columnName<<"时间"<<"类别"<<"内容";
    runLogTableWidget->setColumn(columnName);
    runLogTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度
    runLogTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑
    runLogTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置表格为整行选择
    runLogTableWidget->verticalScrollBar()->setVisible(false);
    runLogTableWidget->verticalScrollBar()->setDisabled(true);

    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(runLogTableWidget);
    measureResultTableLayout->setContentsMargins(0, 0, 0, 0);

    m_loadHintLabel = new QLabel(this);
    m_loadHintLabel->setObjectName(QStringLiteral("logLoadHintLabel"));
    m_loadHintLabel->setFixedSize(200,18);
    QFont hintFont = m_loadHintLabel->font();
    hintFont.setPointSize(10);
    m_loadHintLabel->setFont(hintFont);
    m_loadHintLabel->setAlignment((Qt::AlignLeft | Qt::AlignVCenter));
    m_loadHintLabel->setText("");

    QHBoxLayout *loadHintLayout = new QHBoxLayout();
    loadHintLayout->addWidget(m_loadHintLabel);
    loadHintLayout->addStretch();

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(measureResultTableLayout);
    leftLayout->addStretch();
    leftLayout->addLayout(loadHintLayout);
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

    runLogTableWidget->setColumnWidth(0,175);    //设置每行或每列的高度或宽度
    runLogTableWidget->setColumnWidth(1,100);
    runLogTableWidget->setColumnWidth(2,273);

    runLogTableWidget->setFont(font);
    for(int i = 0;i < 10;i++)
    {
        runLogTableWidget->setRowHeight(i,30.3);
    }

    minDayCaledar = new QCalendarWidget(this);
    maxDayCaledar = new QCalendarWidget(this);
    minDayCaledar->hide();
    maxDayCaledar->hide();

    SpaceInit();

    connect(toTopButton,SIGNAL(clicked()), this, SLOT(ToTop()));
    connect(toBottomButton,SIGNAL(clicked()), this, SLOT(ToBottom()));
    connect(toBackButton,SIGNAL(clicked()), this, SLOT(ToBack()));
    connect(toNextButton,SIGNAL(clicked()), this, SLOT(ToNext()));
    connect(searchButton, SIGNAL(clicked()) ,this, SLOT(ToTop()));
    connect(exportButton,SIGNAL(clicked()), this, SLOT(SlotExportButton()));
    connect(clearButton,SIGNAL(clicked()), this, SLOT(SlotClearButton()));

    connect(minTime, SIGNAL(LineEditClicked()), this, SLOT(ChoseMinDay()));
    connect(maxTime, SIGNAL(LineEditClicked()), this, SLOT(ChoseMaxDay()));
    connect(minDayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMinDay()));
    connect(maxDayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMaxDay()));
//    connect(dateModelCombobox, SIGNAL(currentTextChanged(QString)), this, SLOT(ChangePage()));

//    ToTop();
    this->ChangeBottomStatus();
}

QFileInfoList RunLog::GetLogFileList()
{
    QDir dir = QDir(Environment::Instance()->GetAppDataPath().c_str());

    QStringList nameFilters;
    nameFilters<<"log.txt*";

    QFileInfoList fileInfoList = dir.entryInfoList(nameFilters, QDir::Files);

    return fileInfoList;
}

QString RunLog::TranslateLauguage(QString englishStr)
{
    if(englishStr == "[INFO]:")
    {
        englishStr = "信息";
    }
    else if(englishStr == "[DEBUG]:")
    {
        englishStr = "调试";
    }
    else if(englishStr == "[WARN]:")
    {
        englishStr = "提示";
    }
    else if(englishStr == "[ALERT]:")
    {
        englishStr = "警告";
    }
    else if(englishStr == "[FATAL]:")
    {
        englishStr = "故障";
    }
    else if(englishStr == "[ERROR]:")
    {
        englishStr = "错误";
    }
    else if(englishStr == "[NOTICE]:")
    {
        englishStr = "通知";
    }
    else if(englishStr == "[EMERG]:")
    {
        englishStr = "紧急";
    }
    else if(englishStr == "[NOTSET]:")
    {
        englishStr = "未设置";
    }
    else if(englishStr == "[CRIT]:")
    {
        englishStr = "致命";
    }
    return englishStr;
}

void RunLog::DisplayHint()
{
    m_loadHintLabel->setText("日志加载中，请等待 ······");
}

void RunLog::ClearHint()
{
    m_loadHintLabel->setText(" ");
}

QStringList RunLog::SetDate(QDateTime& theMinDateTime, QDateTime& theMaxDateTime, QStringList& strlist, QString pattern)
{
    QStringList newStrlist;
    for(int i = 0; i < strlist.size() - 1 ; i++)
    {
        QString m = strlist.at(i);
        if(m.contains("[INFO]",Qt::CaseSensitive))
        {
            QRegExp rx(pattern);
            m.indexOf(rx);
            QDateTime dateTime = QDateTime::fromString(rx.cap(1), "yyyy-MM-dd hh:mm:ss");
            if(rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
            {
                if((TranslateLauguage(rx.cap(2).trimmed()) == "信息")
                    && (dateTime > theMinDateTime) && (dateTime < theMaxDateTime))
                {
                    newStrlist.append(m);
                }
            }
        }
    }
    return newStrlist;
}

void RunLog::ToTop()
{
    m_nextFlag = 0;
    m_backFlag = 0;

    bool isFull = false;
    DisplayHint();

    usefulList.clear();
    usefulFileList = GetLogFileList();

    currentFile = 0;    //最新的文件
    if(logFile != nullptr)
    {
        delete logFile;
        logFile = nullptr;
    }
    logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
    m_currentBlock = 0;   //最新的块

    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    char *s = logFile->SeekBlock(m_currentBlock);
    QString pattern("(.*)( .*: )(.*)");
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split(SEPARATOR);
    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
    m_currentpage = newStrlist.size();  //最新的串

    while(currentFile < usefulFileList.size())      //  文件层
    {
        while(!isFull && m_currentBlock < logFile->TotalBlock())    // 块层
        {
            while(!isFull && m_currentpage > 0) // 页层
            {
                QRegExp rx(pattern);
                m_currentpage--; //向旧翻页搜索
                QString m = newStrlist.at(m_currentpage);
                m.indexOf(rx);
                if(m.size()> 0 && rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
                {
                    usefulList.push_back(m);
                }
                if(usefulList.size() > 9)
                {
                    isFull = true;
                    break;
                }
            }
            if(isFull)
            {
                break;
            }
            else
            {
                if(m_currentBlock + 1 < logFile->TotalBlock())
                {
                    m_currentBlock++; //向旧翻块搜索

                    char *s = logFile->SeekBlock(m_currentBlock);
                    QString str(s);
                    strlist.clear();
                    newStrlist.clear();
                    strlist = str.split(SEPARATOR);
                    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                    m_currentpage = newStrlist.size();  //翻块后页号重置该页最新
                }
                else
                {
                    break;
                }
            }
        }//end while block
        if(isFull)
        {
            break;
        }
        else
        {
            currentFile++;  //向旧文件搜索
            if(currentFile < usefulFileList.size())
            {
                if(logFile != nullptr)
                {
                 delete logFile;
                 logFile = nullptr;
                }
                logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
                m_currentBlock = 0;  //重置为该文件最新的块
                char *s = logFile->SeekBlock(m_currentBlock);
                QString str(s);
                strlist.clear();
                newStrlist.clear();
                strlist = str.split(SEPARATOR);
                newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                m_currentpage = newStrlist.size();  //翻块后页号重置该页最新
            }
        }
    }//end while file
    if(currentFile>usefulFileList.size()-1)
    {
        currentFile = usefulFileList.size()-1; //文件修正
    }
    FillTable();
    ClearHint();
    m_isLoaded = true;
}

void RunLog::ToNext()
{
    if (m_backFlag == 1)
    {
        m_backFlag = 0;
        ToNext();
    }

    bool isFull = false;
    DisplayHint();
    usefulList.clear();
    usefulFileList = GetLogFileList();

    if(logFile != nullptr)
    {
        delete logFile;
        logFile = nullptr;
    }
    logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
    char *s = logFile->SeekBlock(m_currentBlock);
    QString pattern("(.*)( .*: )(.*)");
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split(SEPARATOR);
    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);

    while (currentFile < usefulFileList.size())      //  文件层
    {
        while (!isFull && m_currentBlock < logFile->TotalBlock())    // 块层
        {
            while (!isFull && m_currentpage > 0) // 页层
            {
                QRegExp rx(pattern);
                m_currentpage--;  //向前(旧)翻页
                if(m_currentpage < newStrlist.size())
                {
                    QString m = newStrlist.at(m_currentpage);
                    m.indexOf(rx);
                    if (m.size() > 0 && rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
                    {
                        usefulList.push_back(m);
                    }
                    if (usefulList.size() > 9)
                    {
                        m_nextFlag = 1;
                        isFull = true;
                        break;
                    }
                }
            }
            if(isFull)
            {
                break;
            }
            else
            {
                if(m_currentBlock + 1 < logFile->TotalBlock())
                {
                    m_currentBlock++;  //向旧翻块搜索

                    char *s = logFile->SeekBlock(m_currentBlock);
                    QString str(s);
                    strlist.clear();
                    newStrlist.clear();
                    strlist = str.split(SEPARATOR);
                    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                    m_currentpage = newStrlist.size();  //翻块后页号重置该页最新
                }
                else
                {
                    break;
                }
            }
        }//end while block
        if(isFull)
        {
            break;
        }
        else
        {
            currentFile++;  //向旧文件搜索
            if(currentFile < usefulFileList.size())
            {
                if(logFile != nullptr)
                {
                    delete logFile;
                    logFile = nullptr;
                }
                logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
                m_currentBlock = 0;   //重置为该文件最新的块
                char *s = logFile->SeekBlock(m_currentBlock);
                QString str(s);
                strlist.clear();
                newStrlist.clear();
                strlist = str.split(SEPARATOR);
                newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                m_currentpage = newStrlist.size();  //翻块后页号重置该页最新
            }
        }
    }//end while file
    if(currentFile>usefulFileList.size()-1)
    {
        currentFile = usefulFileList.size()-1; //文件修正
        ToBottom();
        return;
    }
    FillTable();
    ClearHint();
}

void RunLog::ToBottom()
{
    m_nextFlag = 0;
    m_backFlag = 0;

    bool isFull = false;
    DisplayHint();
    usefulList.clear();
    usefulFileList = GetLogFileList();

    currentFile = usefulFileList.size()-1;  //最旧的文件
    if(logFile != nullptr)
    {
     delete logFile;
     logFile = nullptr;
    }
    logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
    m_currentBlock = logFile->TotalBlock() - 1;   //最旧的块
    char *s = logFile->SeekBlock(m_currentBlock);
    QString pattern("(.*)( .*: )(.*)");
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split(SEPARATOR);
    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
    m_currentpage = 0;  //最旧的串

    while(currentFile > -1)      //  文件层
    {
        while(!isFull && m_currentBlock >= 0)    // 块层
        {
            while(!isFull && m_currentpage<newStrlist.size()) // 页层
            {
                QRegExp rx(pattern);
                QString m = newStrlist.at(m_currentpage);
                m.indexOf(rx);
                if(m.size()> 0 && rx.cap(1).size()>1 && rx.cap(2).size()>1 && rx.cap(3).size()>1)
                {
                    usefulList.push_front(m);
                }
                m_currentpage++;  //向新翻页
                if(usefulList.size()>9)
                {
                    isFull = true;
                    break;
                }
            }
            if(isFull)
            {
                break;
            }
            else
            {
                if(m_currentBlock > 0)
                {
                    m_currentBlock--;  //向新块搜索

                    char *s = logFile->SeekBlock(m_currentBlock);
                    QString str(s);
                    strlist.clear();
                    newStrlist.clear();
                    strlist = str.split(SEPARATOR);
                    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                    m_currentpage = 0;  //重置为新块最旧的页
                }
                else
                {
                    break;
                }
            }
        } //end while block
        if(isFull)
        {
            break;
        }
        else
        {
            currentFile--;  //向新文件搜索
            if(currentFile > -1)
            {
                if(logFile != nullptr)
                {
                 delete logFile;
                 logFile = nullptr;
                }
                logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
                m_currentBlock = logFile->TotalBlock()-1;  //重置为新文件最旧的块
                char *s = logFile->SeekBlock(m_currentBlock);
                QString str(s);
                strlist.clear();
                newStrlist.clear();
                strlist = str.split(SEPARATOR);
                newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                m_currentpage = 0;  //重置为新块最旧的页
            }
        }
    }//end while file
    if(currentFile < 0)
    {
        currentFile = 0; //文件修正
    }
    FillTable();
    ClearHint();
}

void RunLog::ToBack()
{
    if (m_nextFlag == 1)
    {
        m_nextFlag = 0;
        ToBack();
    }

    bool isFull = false;
    DisplayHint();
    usefulList.clear();
    usefulFileList = GetLogFileList();

    if(logFile != nullptr)
    {
     delete logFile;
     logFile = nullptr;
    }
    logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
    char *s = logFile->SeekBlock(m_currentBlock);
    QString pattern("(.*)( .*: )(.*)");
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text() + " 00:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text() + " 23:59:59", "yyyy-MM-dd hh:mm:ss");

    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split(SEPARATOR);
    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);

    while(currentFile > -1)      //  文件层
    {
        while(!isFull&&m_currentBlock >= 0)    // 块层
        {
            while(!isFull && m_currentpage < newStrlist.size()) // 页层
            {
                QRegExp rx(pattern);
                QString m = newStrlist.at(m_currentpage);
                m.indexOf(rx);
                if(m.size()> 0 && rx.cap(1).size()>1 && rx.cap(2).size()>1 && rx.cap(3).size()>1)
                {
                    usefulList.push_front(m);
                }
                m_currentpage++;  //向新翻页
                if(usefulList.size()>9)
                {
                    m_backFlag = 1;
                    isFull = true;
                    break;
                }
            }
            if(isFull)
            {
                break;
            }
            else
            {
                if(m_currentBlock > 0)
                {
                    m_currentBlock--;  //向新块搜索

                    char *s = logFile->SeekBlock(m_currentBlock);
                    QString str(s);
                    strlist.clear();
                    newStrlist.clear();
                    strlist = str.split(SEPARATOR);
                    newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                    m_currentpage = 0;  //重置为新块最旧的页
                }
                else
                {
                    break;
                }
            }
        } //end while block
        if(isFull)
        {
            break;
        }
        else
        {
            currentFile--;  //向新文件搜索
            if(currentFile > -1)
            {
                if(logFile != nullptr)
                {
                 delete logFile;
                 logFile = nullptr;
                }
                logFile = new TextfileParser(usefulFileList.at(currentFile).absoluteFilePath());
                m_currentBlock = logFile->TotalBlock() - 1; //重置为新文件最旧的块
                char *s = logFile->SeekBlock(m_currentBlock);
                QString str(s);
                strlist.clear();
                newStrlist.clear();
                strlist = str.split(SEPARATOR);
                newStrlist = SetDate(theMinDateTime, theMaxDateTime, strlist, pattern);
                m_currentpage = 0;  //重置为新块最旧的页
            }
        }
    }//end while file
    if(currentFile < 0)
    {
        currentFile = 0; //文件修正
        ToTop();
        return;
    }
    FillTable();
    ClearHint();
}

void RunLog::FillTable()
{
     ViewRefresh();
     SpaceInit();
     int max = usefulList.size()-1;
     if(max>-1)
     {
         for(int i=0;i<usefulList.size();i++)
         {
             QString pattern("(.*)( .*: )(.*)");
             QRegExp rx(pattern);
             QString m = usefulList.at(i);
             m.indexOf(rx);

             runLogTableWidget->item(i ,0)->setText(rx.cap(1).trimmed());
             runLogTableWidget->item(i ,1)->setText(TranslateLauguage(rx.cap(2).trimmed()));
             runLogTableWidget->item(i ,2)->setText(rx.cap(3).trimmed());

         }
     }
}

QDate RunLog::GetCurrentMinDate()
{
    QString dateString = minTime->text();
    QStringList dateList = dateString.split("-");
    QString year = dateList.at(0);
    QString month = dateList.at(1);
    QString day = dateList.at(2);

    QDate minDate = QDate(year.toInt(),month.toInt(),day.toInt());
    return minDate;
}

QDate RunLog::GetCurrentMaxDate()
{
    QString dateString = maxTime->text();
    QStringList dateList = dateString.split("-");
    QString year = dateList.at(0);
    QString month = dateList.at(1);
    QString day = dateList.at(2);

    QDate maxDate = QDate(year.toInt(),month.toInt(),day.toInt());
    return maxDate;
}

void RunLog::ChoseMinDay()
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

void RunLog::ChangeMinDay()
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

void RunLog::ChoseMaxDay()
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

void RunLog::ChangeMaxDay()
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

QString RunLog::GetMinTime()
{
    char *s = logFile->SeekBlock(0);
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split(SEPARATOR);
    if(strlist.size() > 0)
    {
        for(int i = 0;i < strlist.size() - 1; i++)
        {
            QString pattern("(.*)( .*: )(.*)");
            QRegExp rx(pattern);
            QString m = strlist.at(i);

            m.indexOf(rx);
            if(rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
            {
                newStrlist.append(m);
            }
        }
    }
    else
    {
        return QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd");
    }
    if(newStrlist.size() > 0)
    {
        QString minTime;
        QString pattern("(.*)( .*: )(.*)");
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

QString RunLog::GetMaxTime()
{
    char *s = logFile->SeekBlock(0);
    QString str(s);
    QStringList strlist;
    QStringList newStrlist;
    strlist = str.split(SEPARATOR);
    if(strlist.size() > 0)
    {
        for (int i = 0; i < strlist.size() - 1; i++)
        {
            QString pattern("(.*)( .*: )(.*)");
            QRegExp rx(pattern);
            QString m = strlist.at(i);

            m.indexOf(rx);
            if(rx.cap(1).size() > 1 && rx.cap(2).size() > 1 && rx.cap(3).size() > 1)
            {
                newStrlist.append(m);
            }
        }
    }
    else
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }

    if(newStrlist.size() > 0)
    {
        QString maxTime;
        QString pattern("(.*)( .*: )(.*)");
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

void RunLog::ViewRefresh()
{
    runLogTableWidget->clear();
    QStringList columnName;
    columnName<<"时间"<<"类别"<<"内容";
    runLogTableWidget->setColumn(columnName);
}

void RunLog::SpaceInit()
{
    for(int i = 0;i < runLogTableWidget->rowCount();i++)
    {
        for(int j = 0;j < runLogTableWidget->columnCount();j++)
        {
            runLogTableWidget->setItem(i, j, new QTableWidgetItem());
            runLogTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void RunLog::ChangeBottomStatus()
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

void RunLog::ChangePage()
{
    m_currentpage = 0;
}

void RunLog::SlotClearButton()
{
    MessageDialog msg("是否确认清空日志记录?", this, MsgStyle::OKANDCANCEL);
    if(QDialog::Accepted != msg.exec())
    {
        return;
    }
    else
    {
        if(!usefulFileList.isEmpty())
        {
            foreach (QFileInfo file, usefulFileList)
            {
                if (file.isFile() && file.fileName() != FileName)
                {
                    file.dir().remove(file.fileName());
                }
            }
        }
        usefulFileList.clear();
        if(logFile!=nullptr)
        {
            delete logFile;
            logFile = nullptr;
        }
        logFile = new TextfileParser(QString(Environment::Instance()->GetAppDataPath().c_str()) + QString("/") + FileName);

        logFile->ClearRecord();
        minTime->setText(GetMinTime());
        maxTime->setText(GetMaxTime());
        ToTop();
    }
}

void RunLog::SlotExportButton()
{
    QString result;
    CopyFileAction copyFileAction;
    QString strDir = copyFileAction.GetTargetDir().c_str();
    QDir dir(strDir);

    bool isFail = false;

    if (!copyFileAction.ExMemoryDetect()) //U盘检测
    {
        MessageDialog msg("U盘不存在，日志数据导出失败", this);
        msg.exec();
        return;
    }

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State *state = luaEngine->GetThreadState();

    bool isValid = false;
    luaEngine->GetLuaValue(state, "config.producerAuthorization.enable", isValid);

    if(isValid == true)
    {
        if (!copyFileAction.TargetDirCheck(dir)) //拷贝目录检测
        {
            MessageDialog msg("不可在U盘创建目录，日志数据导出失败", this);
            msg.exec();
            return;
        }

        QString sPath = Environment::Instance()->GetAppDataPath().c_str() + QString("/") +QString(FileName);
        QString dPath = dir.filePath(FileName);
        QList<QString> dPathList;

       if(copyFileAction.CopyFileToPath(sPath.toStdString(), dPath.toStdString(), true))
       {
           dPathList.append(dPath);

           for(int index = 1; index <= 10; index++)
           {
               QString logName;

               if(index != 10)
               {
                   logName = QString(FileName) + ".0" + QString::number(index);
               }
               else
               {
                   logName = QString(FileName) + "." + QString::number(index);
               }

               sPath = Environment::Instance()->GetAppDataPath().c_str() + QString("/") +QString(logName);
               dPath = dir.filePath(logName);

               QFileInfo fileInfo(sPath);

               if (QFile::exists(sPath)&&fileInfo.isFile())
               {
                   dPathList.append(dPath);

                   if (!copyFileAction.CopyFileToPath(sPath.toStdString(), dPath.toStdString(), true))
                   {
                       isFail = true;
                   }
               }
           }
       }
       else
       {
           isFail = true;
       }

       if (!isFail)
       {
           for(QList<QString> ::iterator it = dPathList.begin(); it != dPathList.end(); it++)
           {
               if (!QFile::exists((*it)))
               {
                   logger->info("日志数据导出失败");
                   MessageDialog msg("日志数据导出失败", this);
                   msg.exec();
                   return;
               }
           }
           result = "日志数据导出成功";
           logger->info("日志数据导出成功");
       }
       else
       {
           result = "日志数据导出失败";
           logger->info("日志数据导出失败");
       }
    }
    else
    {
        std::list<String> logPathList;

        QFileInfoList logFileList = this->GetLogFileList();
        if(!logFileList.isEmpty())
        {
            foreach (QFileInfo file, logFileList)
            {
                QString logPath = file.absoluteFilePath();
                logPathList.push_back(logPath.toStdString());
            }
        }

        String zipPath = strDir.toStdString() + ".upg";

        if(Uncompress::CompressFileList(logPathList, zipPath, LOGPWD))
        {
            result = "日志数据导出成功";
            logger->info("日志数据导出成功");
        }
        else
        {
            result = "日志数据导出失败";
            logger->info("日志数据导出失败");
        }
    }
    MessageDialog msg(result, this);
    msg.exec();

#ifdef    _CS_ARM_LINUX
    system("sync");
#endif

    return;
}

void RunLog::OnUserChange()
{
    this->ChangeBottomStatus();
}

void RunLog::showEvent(QShowEvent *event)
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
    if (m_isLoaded == false)
    {
        ToTop();
   }
}

void RunLog::OnUpdateWidget(UpdateEvent event, System::String message)
{
    (void)message;
    if(event == UpdateEvent::OneKeyClearData)
    {
        if(!usefulFileList.isEmpty())
        {
            foreach (QFileInfo file, usefulFileList)
            {
                if (file.isFile() && file.fileName() != FileName)
                {
                    file.dir().remove(file.fileName());
                }
            }
        }
        usefulFileList.clear();
        if(logFile!=nullptr)
        {
            delete logFile;
            logFile = nullptr;
        }
        logFile = new TextfileParser(QString(Environment::Instance()->GetAppDataPath().c_str()) + QString("/") + FileName);

        logFile->ClearRecord();
        minTime->setText(GetMinTime());
        maxTime->setText(GetMaxTime());
        ToTop();
    }
}

RunLog::~RunLog()
{
    if (logFile)
        delete logFile;
}

}

