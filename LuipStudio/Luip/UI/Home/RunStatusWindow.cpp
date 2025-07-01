#include "RunStatusWindow.h"
#include "ResultManager/ResultManager.h"
#include "FlowManager/FlowManager.h"
#include "Setting/SettingManager.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "StatusManager/StatusManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include <QDebug>
#include "Log.h"
#include "UI/System/ScreenCalibrate.h"
#include "LuaEngine/LuaEngine.h"
#include "FlowManager/FlowManager.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "FlowManager/MeasureScheduler.h"
#include "Lua/App.h"

using namespace Measure;
using namespace Result;
using namespace std;
using namespace Configuration;
using namespace UI;
using namespace OOLUA;
using namespace Flow;
using namespace Lua;
using namespace ResultData;

RunStatusWindow::RunStatusWindow(QWidget *parent) :
        QWidget(parent) ,
        IScheduleNotifiable() ,
        IUserChangeNotifiable()

{
    MaintainUserChange::Instance()->Register(this);

    MeasureScheduler::Instance()->Register(this);
    ResultManager::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);

    //注册状态观察
    statusManager = StatusManager::Instance();
    statusManager->Register(this);

    mainSplitter = new QSplitter();
    mainSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainSplitter->setOrientation(Qt::Horizontal);//水平分割
    mainSplitter->setHandleWidth(1);

    this->initLeft();
    this->initLeftTop();
    this->initLeftBottom();
    leftSplitter->addWidget(leftTopWidget);
    mainSplitter->addWidget(leftSplitter);

    for(int i = 0; i < mainSplitter->count();i++)
    {
        QSplitterHandle *handle = mainSplitter->handle(i);
        handle->setEnabled(false);
    }

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(mainSplitter);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    OnUserChange();

    this->setLayout(mainLayout);
    this->resize(800,450);
    m_isMousePress = false;
}

RunStatusWindow::~RunStatusWindow()
{

}

void RunStatusWindow::OnScheduler(QString content)
{
    emit SignalUpdateExpectInfo(content);
}

void RunStatusWindow::SlotUpdateExpectInfo(QString str)
{
    estimateResultTimeLable->setText(str);
}

void RunStatusWindow::initLeft()
{
    leftWidget = new QWidget();

    leftSplitter = new QSplitter();

    leftSplitter->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    leftSplitter->setOrientation(Qt::Vertical);//垂直分割
    leftSplitter->setHandleWidth(0);
    leftWidget->resize(800,450);
}

void RunStatusWindow::initLeftTop()
{
    leftTopWidget = new QWidget();
    leftTopWidget->resize(800, 300);

    // 产品类型
    productTypeLabel = new QLabel();
    productTypeLabel->setObjectName(QStringLiteral("productTypeLabel"));
    productTypeLabel->setFixedSize(600,35);
    QFont productTypeFont = productTypeLabel->font();
    productTypeFont.setPointSize(20);
    productTypeLabel->setFont(productTypeFont);
    productTypeLabel->setAlignment(Qt::AlignLeft);

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    String model;
    luaEngine->GetLuaValue(state, "config.info.instrument.model", model);
    String paramType = SettingManager::Instance()->GetParamTypeName();
#if !OEM
    productTypeLabel->setText((model + paramType).c_str());
#endif

    QHBoxLayout *productTypeLayout = new QHBoxLayout();
    productTypeLayout->addWidget(productTypeLabel);
    productTypeLayout->addStretch();
    productTypeLayout->setSpacing(0);
    productTypeLayout->setContentsMargins(20, 0, 0, 0);

    // 测量结果显示框
    m_lua = luaEngine->GetEngine();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.runStatus", itermsTable);
    m_isUnitChange = itermsTable.safe_at("unitChange", m_unitChangeFunc);

    Table targetTable;
    itermsTable.at("targets", targetTable);

    int type = SettingManager::Instance()->GetParamType();

    QStringList targetList;

    oolua_ipairs(targetTable)
    {
        Table itermTable;
        m_lua->pull(itermTable);

         int targetPrivilege = 0;
        bool isTarget = itermTable.safe_at("targetPrivilege", targetPrivilege);
        if (isTarget == false || (type & targetPrivilege))
        {
            RunStatusIterm iterm;

            itermTable.safe_at("name", iterm.name);
            itermTable.at("getData", iterm.getData);
			iterm.isNeedResultType = itermTable.safe_at("getResultType", iterm.getResultType);
            iterm.isAlarm = itermTable.safe_at("isAlarm", iterm.isAlarmFunc);
            m_items.push_back(iterm);

            targetList.append(QString(iterm.name.c_str()));
        }
    }
    oolua_ipairs_end()

    String unit;
    luaEngine->GetLuaValue(state, "setting.unit[1].text", unit);
    if (m_isUnitChange)
    {
        m_lua->call(m_unitChangeFunc, unit, UnitChange::Read);
        m_lua->pull(unit);
    }

    m_resultPanel = new MeasureResultPanel(targetList, tr(""), QString::fromStdString(unit), this);
//    m_resultPanel->setFixedSize(800, 400);

    QHBoxLayout * measureResultPanelLayout = new QHBoxLayout();
    measureResultPanelLayout->addWidget(m_resultPanel);
    measureResultPanelLayout->setContentsMargins(0,10,0,0);

    //测量预期标签
    estimateResultTimeLable = new QLabel();
    estimateResultTimeLable->setFixedSize(600,50);
    QFont estimateResultTypeFont = estimateResultTimeLable->font();
    estimateResultTypeFont.setPointSize(12);
    estimateResultTimeLable->setFont(estimateResultTypeFont);
    estimateResultTimeLable->setAlignment(Qt::AlignCenter);
    estimateResultTimeLable->setText(tr(" "));

    QHBoxLayout *estimateResultLayout = new QHBoxLayout();
    estimateResultLayout->addStretch();
    estimateResultLayout->addWidget(estimateResultTimeLable);
    estimateResultLayout->addStretch();
    estimateResultLayout->setSpacing(0);
    estimateResultLayout->setContentsMargins(0, 0, 0, 0);

    stopButton = new QPushButton();
    stopButton->setObjectName("brownButton");
    stopButton->setFixedSize(95,40);
    stopButton->setText("停止");
    QFont measureFont = stopButton->font();
    measureFont.setPointSize(12);
    stopButton->setFont(measureFont);

    oneKeyButton = new QPushButton();
    oneKeyButton->setObjectName("brownButton");
    oneKeyButton->setFixedSize(95,40);
    oneKeyButton->setText("一键运行");
    oneKeyButton->setFont(measureFont);
    connect(oneKeyButton, SIGNAL(clicked(bool)), SLOT(SlotOneKeyButton()));

    measureModeComBox = new QComboBox();
    measureModeComBox->setObjectName("brownComboBox");


    Table schedulerTable;
    luaEngine->GetLuaValue(state, "setting.ui.profile.scheduler", schedulerTable);

    QStringList modeList;
    oolua_ipairs(schedulerTable)
    {
        Table typeTable;
        String typeName;

        m_lua->pull(typeTable);
        typeTable.safe_at("name", typeName);

        oolua_ipairs(typeTable)
        {
          Table measureTable;
          String measureName;

          m_lua->pull(measureTable);
          measureTable.safe_at("name", measureName);
          if(measureName == "measure.mode")
          {
              Table optTable;
              measureTable.safe_at("options", optTable);
              oolua_ipairs(optTable)
              {
                  String modeStr;
                  m_lua->pull(modeStr);
                  modeList.append(QString::fromStdString(modeStr));
              }oolua_ipairs_end()
          }

        }oolua_ipairs_end()

    }oolua_ipairs_end()

    QStringList defaultModeList;
    defaultModeList << tr("触发测量") << tr("周期测量") << tr("整点测量")<< tr("连续测量");

    measureModeComBox->setFont(measureFont);
    if(modeList.count() > 0)
    {
        measureModeComBox->insertItems(0,modeList);
    }
    else
    {
        measureModeComBox->insertItems(0,defaultModeList);
    }
    MeasureMode theMode;
    luaEngine->GetLuaValue(state, "config.scheduler.measure.mode", theMode);
    measureModeComBox->setCurrentIndex((int)theMode);
    measureModeComBox->setFixedSize(95,40);

    reportModeComBox = new QComboBox();
    reportModeComBox->setObjectName("brownComboBox");

    QStringList reportModeList;
    Table reportModeTable;
    luaEngine->GetLuaValue(state, "setting.ui.runStatus.reportModeList", reportModeTable);
    oolua_ipairs(reportModeTable)
    {
        String text;
        m_lua->pull(text);
        reportModeList << QString::fromUtf8(text.c_str());
    }
    oolua_ipairs_end()
    reportModeComBox->insertItems(0, reportModeList);

    ReportMode reportMode;
    luaEngine->GetLuaValue(state, "config.interconnection.reportMode", reportMode);
    reportModeComBox->setCurrentIndex((int)reportMode);
    reportModeComBox->setFixedSize(95,40);


    QVBoxLayout *buttonLayout = new QVBoxLayout();

    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(reportModeComBox);
    buttonLayout->addWidget(oneKeyButton);
    buttonLayout->addWidget(measureModeComBox);
    buttonLayout->setSpacing(20);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *buttonHLayout = new QHBoxLayout();
    buttonHLayout->addStretch();
    buttonHLayout->addLayout(buttonLayout);
    buttonHLayout->setSpacing(0);
    buttonHLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *buttonVLayout = new QVBoxLayout();
    buttonVLayout->addStretch();
    buttonVLayout->addLayout(buttonHLayout);;
    buttonVLayout->setSpacing(0);
    buttonVLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *erLayout = new QVBoxLayout();
    erLayout->addStretch();
    erLayout->addLayout(measureResultPanelLayout);
    erLayout->addLayout(estimateResultLayout);
    erLayout->addStretch();
    erLayout->setSpacing(8); //20
    erLayout->setContentsMargins(0, 0, 0, 0);

    measureLayout = new QHBoxLayout();
    measureLayout->addStretch();
    measureLayout->addLayout(erLayout);
    measureLayout->addStretch();
    measureLayout->addLayout(buttonVLayout);
    measureLayout->setSpacing(0);
    measureLayout->setContentsMargins(100, 0, 20, 0);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(productTypeLayout);
    vLayout->addLayout(measureLayout);
    vLayout->setSpacing(20);
    vLayout->setContentsMargins(0, 20, 0, 10);//整个窗口
    leftTopWidget->setLayout(vLayout);

    ChangeBottomStatus();

    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(StopFlow()));
    connect(measureModeComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangeAutoMeasure()));
    connect(reportModeComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangeReportMode()));
    connect(this, SIGNAL(SignalUpdateExpectInfo(QString)), this, SLOT(SlotUpdateExpectInfo(QString)));


    MeasureResultPanelDisplayData();
    MeasureResultPanelUpdateStatus();
}


void RunStatusWindow::MeasureResultPanelDisplayData()
{
    try
    {
        if (!m_items.empty())
        {
            lua_State * newState = LuaEngine::Instance()->GetThreadState();
            OOLUA::Lua_function call(newState);

            for(int i = 0; i < (int)m_items.size(); ++i)
            {
                RunStatusIterm iterm = m_items.at(i);

                int dateTime;
                float consistency;

                String strResultType = "";

                call(iterm.getData);
                OOLUA::pull(newState, consistency);
                OOLUA::pull(newState, dateTime);

                if (m_isUnitChange)
                {
                    m_lua->call(m_unitChangeFunc, consistency, UnitChange::Read);
                    m_lua->pull(consistency);
                }

                QString strConsistency = ResultManager::Instance()->DisplayResult(consistency);
                QString strTime = QDateTime::fromTime_t(dateTime).toString("yyyy-MM-dd hh:mm:ss");

                if (iterm.isNeedResultType)
                {
                    Byte resultType;
                    call(iterm.getResultType);
                    OOLUA::pull(newState, resultType);

                    if ((MeasureType)resultType != MeasureType::None)
                    {
                        LuaEngine* luaEngine = LuaEngine::Instance();
                        lua_State* state = luaEngine->GetThreadState();

                        Table measureTable;
                        luaEngine->GetLuaValue(state, "setting.fileFormat.measure", measureTable);

                        int currentId = 0;
                        Table formatTable;

                        measureTable.at("current", currentId);
                        measureTable.at(currentId, formatTable);

                        Table optionTable;

                        oolua_ipairs(formatTable)
                        {
                            Table itermTable;
                            m_lua->pull(itermTable);

                            String name;
                            itermTable.at("name", name);
                            if (name == "resultType")
                            {
                                itermTable.at("option", optionTable);
                            }
                        }
                        oolua_ipairs_end()

                        optionTable.safe_at((int)resultType + 1, strResultType);
                    }
                }

                m_resultPanel->SetResultInfo(QString(iterm.name.c_str()), strConsistency, strTime, QString(strResultType.c_str()));
            }
        }
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("RunStatusWindow::MeasureResultPanelDisplayData() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("RunStatusWindow::MeasureResultPanelDisplayData() => %s", e.what());
    }
}

void RunStatusWindow::MeasureResultPanelUpdateStatus()
{
    try
    {
        if (!m_items.empty())
        {
            lua_State * newState = LuaEngine::Instance()->GetThreadState();
            OOLUA::Lua_function call(newState);
            for(int i = 0; i < (int)m_items.size(); ++i)
            {
                RunStatusIterm iterm = m_items.at(i);
                if (iterm.isAlarm)
                {
                    bool ret;
                    call(iterm.isAlarmFunc);
                    OOLUA::pull(newState, ret);
                    m_resultPanel->SetResultAlarm(QString(iterm.name.c_str()), ret);
                }
            }
        }
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("RunStatusWindow::MeasureResultPanelUpdateStatus() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("RunStatusWindow::MeasureResultPanelUpdateStatus() => %s", e.what());
    }
}

void RunStatusWindow::StopFlow()
{   
    MessageDialog msg("是否停止当前动作？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Accepted)
    {
        if(measureModeComBox->currentIndex() == 3)
        {
           measureModeComBox->setCurrentIndex(0);  //连续测量时手动停止后换成触发测量
        }
        FlowManager::Instance()->StopFlow();
    }
}

void RunStatusWindow::initLeftBottom()
{

}

void RunStatusWindow::ChangeBottomStatus()
{
    if(LoginDialog::userType == Super)
    {
        stopButton->setEnabled(true);
        oneKeyButton->setEnabled(true);
        measureModeComBox->setEnabled(true);
        reportModeComBox->setEnabled(true);
    }
    else if(LoginDialog::userType == Administrator)
    {
        stopButton->setEnabled(true);
        oneKeyButton->setEnabled(true);
        measureModeComBox->setEnabled(true);
        reportModeComBox->setEnabled(true);
    }
    else if(LoginDialog::userType == Maintain)
    {
        stopButton->setEnabled(true);
        oneKeyButton->setEnabled(true);
        measureModeComBox->setEnabled(true);
        reportModeComBox->setEnabled(true);
    }
    else if(LoginDialog::userType == General)
    {
        stopButton->setEnabled(false);
        oneKeyButton->setEnabled(false);
        measureModeComBox->setEnabled(false);
        reportModeComBox->setEnabled(false);
    }
}


void RunStatusWindow::SlotOneKeyButton()
{
    StatusManager *statusManager = StatusManager::Instance();

    Script *lua = LuaEngine::Instance()->GetEngine();
    Table table, itermsTable;
    itermsTable.bind_script(*lua);
    itermsTable.set_table("setting");
    itermsTable.at("ui", table);
    table.at("runStatus", itermsTable);

    Lua_func_ref func;
    itermsTable.at("oneKeyMaintain", func);

    if (statusManager->IsIdleStatus() && statusManager->IsIdleAction())
    {
        emit SignalOneKeyChangeReagent();

        reportModeComBox->setCurrentIndex((int)ReportMode::OnLine);
        SlotChangeReportMode();

        if (lua->call(func))
        {
            FlowManager::Instance()->StartFlow();
        }
        else
        {
            logger->warn("RunStatusWindow call the function Fail on lua");
        }
    }
    else
    {
        MessageDialog msg("请确认仪器状态是否为空闲！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }
}

void RunStatusWindow::OnMeasureResultAdded(String name, RecordData recordData)
{
    (void)name;
    (void)recordData;
    MeasureResultPanelDisplayData();
    MeasureResultPanelUpdateStatus();
}

void RunStatusWindow::SlotChangeAutoMeasure()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    luaEngine->SetLuaValue("config.scheduler.measure.mode", measureModeComBox->currentIndex());
    SettingManager::Instance()->SchedulerSave();
    UpdateWidgetManager::Instance()->SlotUpdateWidget(UpdateEvent::ChangeAutoMeasure, "RunStatusWindow");

    MeasureScheduler::Instance()->Update();
}

void RunStatusWindow::RefreshAutoMeasure()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    MeasureMode autoMode;
    luaEngine->GetLuaValue(state, "config.scheduler.measure.mode", autoMode);
    measureModeComBox->setCurrentIndex((int)autoMode);
}

void RunStatusWindow::SlotChangeReportMode()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    luaEngine->SetLuaValue("config.interconnection.reportMode", reportModeComBox->currentIndex());
    SettingManager::Instance()->InterconnectionSave();
    UpdateWidgetManager::Instance()->SlotUpdateWidget(UpdateEvent::ChangeInterconnectionParam, "RunStatusWindow");
}

void RunStatusWindow::RefreshReportMode()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    ReportMode reportMode;
    luaEngine->GetLuaValue(state, "config.interconnection.reportMode", reportMode);
    reportModeComBox->setCurrentIndex((int)reportMode);
}

void RunStatusWindow::OnUserChange()
{ 
    ChangeBottomStatus();
}
void RunStatusWindow::OnCalibrateResultAdded(String name, RecordData result)
{
    (void)name;
    (void)result;
}

void RunStatusWindow::mousePressEvent(QMouseEvent * event)
{
    m_isMousePress = true;
    m_pressPoint = event->pos();
    m_pressTime = QDateTime::currentDateTime();
}

void RunStatusWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //符合条件:是否被按下过（有些区域识别不到按下），是否按下的位置和放开的位置X或Y相差50，是否按下的时间超过3秒
    if(true == m_isMousePress &&
            (abs(m_pressPoint.x() - event->x()) > 50
            || abs(m_pressPoint.y() - event->y()) > 50)
            && m_pressTime.addSecs(30) <  QDateTime::currentDateTime())
    {
        logger->notice("主界面启动触摸屏校准 按下坐标：(%d, %d) 松开坐标：(%d, %d)",
                       m_pressPoint.x(), m_pressPoint.y(), event->x(), event->y());
        ScreenCalibrate screenCalibrate(this);
    }
    m_isMousePress = false;
}

void RunStatusWindow::OnStattusChanged(RunStatus status)
{
    (void)status;
}

void RunStatusWindow::OnStattusDelay(int time)
{
    (void)time;
}

void RunStatusWindow::OnActionChanged(RunAction action)
{
    (void)action;
}

void RunStatusWindow::OnActionDelay(int time)
{
    (void)time;
}

void RunStatusWindow::OnUpdateWidget(UpdateEvent event, System::String message)
{
    if (event == UpdateEvent::ChangeConfigParam || event == UpdateEvent::WqimcChangeParam)//一键恢复出厂设置
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State* state = luaEngine->GetThreadState();

        MeasureMode autoMode;
        luaEngine->GetLuaValue(state, "config.scheduler.measure.mode", autoMode);
        measureModeComBox->setCurrentIndex((int)autoMode);

        ReportMode reportMode;
        luaEngine->GetLuaValue(state, "config.interconnection.reportMode", reportMode);
        reportModeComBox->setCurrentIndex((int)reportMode);
    }
    if (event == UpdateEvent::ChangeInterconnectionParam && message != "RunStatusWindow")
    {
        RefreshReportMode();
    }
    else if (event == UpdateEvent::ChangeMeaModeOnHome)    // 刷新主界面的测量模式
    {
        RefreshAutoMeasure();
    }
    else if (event == UpdateEvent::ChangeAutoMeasure && message != "RunStatusWindow")
    {
        RefreshAutoMeasure();
    }
    else if (event == UpdateEvent::OneKeyClearData)
    {
        MeasureResultPanelDisplayData();
    }
}
