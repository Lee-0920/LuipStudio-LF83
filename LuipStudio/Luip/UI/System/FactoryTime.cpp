#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "Log.h"
#include "FactoryTime.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include "UI/System/ScreenCalibrate.h"
#include "LuaEngine/LuaEngine.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "System/Types.h"
#include "Setting/SettingManager.h"
#include "Setting/Environment.h"
#include "FlowManager/MeasureScheduler.h"

using namespace OOLUA;
using namespace Lua;
using namespace System;
using namespace Configuration;
using namespace Flow;

namespace UI
{

FactoryTimeWidget::FactoryTimeWidget(QWidget *parent):
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this); //向窗口更新器注册

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    this->resize(650,400);

    defaultPassword = new QPushButton();
    defaultFactory = new QPushButton();
    screenCalibrate = new QPushButton();
    activation = new QPushButton();
    clearData = new QPushButton();
    clearFault = new QPushButton();

    defaultPassword->setObjectName("brownButton");
    defaultPassword->setText("一键恢复默认密码");
    defaultPassword->setFixedSize(250,35);

    defaultFactory->setObjectName("brownButton");
    defaultFactory->setText("一键恢复出厂设置");
    defaultFactory->setFixedSize(250,35);

    screenCalibrate->setObjectName("brownButton");
    screenCalibrate->setText("触摸屏校准");
    screenCalibrate->setFixedSize(250,35);

    clearFault->setObjectName("brownButton");
    clearFault->setText("故障清除");
    clearFault->setFixedSize(250,35);

    clearData->setObjectName("brownButton");
    clearData->setText("一键清除数据");
    clearData->setFixedSize(250,35);
    clearData->hide();

    activation->setObjectName("brownButton");
    activation->setText("出厂激活");
    activation->setFixedSize(250,35);
    activation->hide();

    QVBoxLayout *maintainerLayout = new QVBoxLayout();
    maintainerLayout->addWidget(defaultPassword);
    maintainerLayout->addSpacing(20);
    maintainerLayout->addWidget(defaultFactory);
    maintainerLayout->addSpacing(20);
    maintainerLayout->addWidget(screenCalibrate);
    maintainerLayout->addSpacing(20);
    maintainerLayout->addWidget(clearFault);
    maintainerLayout->addSpacing(20);
    maintainerLayout->addWidget(clearData);
    maintainerLayout->addSpacing(20);
    maintainerLayout->addWidget(activation);

    defaultPassword->setFont(font);
    defaultFactory->setFont(font);
    screenCalibrate->setFont(font);
    clearData->setFont(font);
    activation->setFont(font);
    clearFault->setFont(font);
    maintainerLayout->setContentsMargins(30,50,0,0);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(maintainerLayout);

    mainLayout->addStretch();
    this->setLayout(mainLayout);
    OnUserChange();

    connect(defaultPassword, SIGNAL(clicked(bool)), this, SLOT(SlotDefaultPassword()));
    connect(defaultFactory, SIGNAL(clicked(bool)), this, SLOT(SlotDefaultFactory()));
    connect(screenCalibrate, SIGNAL(clicked(bool)), this, SLOT(SlotScreenCalibrate()));
    connect(clearData, SIGNAL(clicked(bool)), this, SLOT(SlotClearData()));
    connect(activation, SIGNAL(clicked(bool)), this, SLOT(SlotActivation()));
    connect(clearFault, SIGNAL(clicked(bool)), this, SLOT(SlotClearFault()));
}

FactoryTimeWidget::~FactoryTimeWidget()
{

}

void FactoryTimeWidget::SlotClearFault()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    Lua_function call(state);

    Lua_func_ref clearFault;
    luaEngine->GetLuaValue(state, "setting.ui.factoryTime.clearFault", clearFault);

    if(clearFault.valid())
    {
        call(clearFault);
        logger->info("清除故障");

        MeasureScheduler::Instance()->Update();
    }
}

void FactoryTimeWidget::SlotClearData()
{
    MessageDialog msg("是否一键清除数据？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }

    logger->info("一键清除数据");

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    Lua_function call(state);

    Lua_func_ref clearData;
    luaEngine->GetLuaValue(state, "setting.ui.factoryTime.clearData", clearData);

    if(clearData.valid())
    {
        call(clearData);
    }

    UpdateWidgetManager::Instance()->SlotUpdateWidget(UpdateEvent::OneKeyClearData, "FactoryTime");
}

void FactoryTimeWidget::SlotDefaultFactory()
{
    MessageDialog msg("是否恢复出厂设置？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }

    logger->info("一键恢复出厂设置")
            ;
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    OOLUA::Script* lua = luaEngine->GetEngine();
    Lua_function call(state);

    Lua_func_ref defaultRestore;
    luaEngine->GetLuaValue(state, "setting.ui.factoryTime.defaultRestore", defaultRestore);

    call(defaultRestore, LoginDialog::userType);

    bool isRestart = false;
    lua->pull(isRestart);

    UpdateWidgetManager::Instance()->SlotUpdateWidget(UpdateEvent::ChangeConfigParam);

    if(isRestart == true)
    {
        QString text = "关键参数更改，重启仪器后生效，是否立即重启？\n";
        MessageDialog msg(text, 10, this, MsgStyle::ONLYCANCEL);
        if(msg.exec()==QDialog::Rejected)
        {
            return;
        }
    }
}

void FactoryTimeWidget::showEvent(QShowEvent *event)
{
    (void)event;
    if (LoginDialog::userType == RoleType::Super)
    {
        if (SettingManager::Instance()->IsProducerStatus())
        {
            activation->show();
        }
        else
        {
            activation->hide();
        }
        clearData->show();
    }
    else
    {
        activation->hide();
        clearData->hide();
    }
}

void FactoryTimeWidget::SlotDefaultPassword()
{
    MessageDialog msg("是否恢复默认密码？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }

    logger->info("恢复默认密码");

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    Lua_function call(state);

    Lua_func_ref LoadAuthorization;
    Table defaultAuthorization;
    luaEngine->GetLuaValue(state, "ConfigLists.LoadAuthorization", LoadAuthorization);
    call(LoadAuthorization, true);
    OOLUA::pull(state, defaultAuthorization);

    Table authorization;
    luaEngine->GetLuaValue(state, "config.authorization", authorization);

    String normalUserPassword;
    defaultAuthorization.at("normalUserPassword", normalUserPassword);
    authorization.set("normalUserPassword", normalUserPassword);

    String maintenancePassword;
    defaultAuthorization.at("maintenancePassword", maintenancePassword);
    authorization.set("maintenancePassword", maintenancePassword);

    String administratorPassword;
    defaultAuthorization.at("administratorPassword", administratorPassword);
    authorization.set("administratorPassword", administratorPassword);

    String superPassword;
    defaultAuthorization.at("superPassword", superPassword);
    authorization.set("superPassword", superPassword);

    Lua_func_ref SaveAuthorization;
    luaEngine->GetLuaValue(state, "ConfigLists.SaveAuthorization", SaveAuthorization);
    call(SaveAuthorization);

    QString text = "密码已重置！";
    MessageDialog msgRet(text, 10, this, MsgStyle::ONLYOK);
    msgRet.exec();
}

void FactoryTimeWidget::OnUserChange()
{
    if (LoginDialog::userType == RoleType::Super)
    {
        if (SettingManager::Instance()->IsProducerStatus())
        {
            activation->show();
        }
        else
        {
            activation->hide();
        }
        clearData->show();
        defaultPassword->show();
    }
    else
    {
        defaultPassword->hide();
        activation->hide();
        clearData->hide();
    }
}

void FactoryTimeWidget::SlotScreenCalibrate()
{
    logger->info("系统设置，触摸屏校准");
    ScreenCalibrate screenCalibrate(this);
}

void FactoryTimeWidget::SlotActivation()
{
    MessageDialog msg("是否激活出厂模式？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }

    logger->info("激活出厂模式");

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    Lua_function call(state);

    Lua_func_ref SaveEncryptionFile;
    luaEngine->GetLuaValue(state, "Serialization.SaveEncryptionFile", SaveEncryptionFile);

    Table producerAuthorization;
    luaEngine->GetLuaValue(state, "config.producerAuthorization", producerAuthorization);

    producerAuthorization.set("enable", false);

    String path = Configuration::Environment::Instance()->GetAppPath() + "/SP/Config/ProducerAuthorization.ls";
    call(SaveEncryptionFile, producerAuthorization, path, "config.producerAuthorization");

    activation->hide();
}

void FactoryTimeWidget::OnUpdateWidget(UpdateEvent event, System::String message)
{
    if (event == UpdateEvent::RecoverInFactoryMode && message == "InstrumentAgentHandler")
    {
        activation->show();
    }
    else if (event == UpdateEvent::ActivateOutFactoryMode && message == "InstrumentAgentHandler")
    {
        activation->hide();
    }
    else if (event == UpdateEvent::FaultClear && message == "InstrumentAgentHandler")
    {
        emit this->clearFault->click();
    }

    this->OnUserChange();
}

}

