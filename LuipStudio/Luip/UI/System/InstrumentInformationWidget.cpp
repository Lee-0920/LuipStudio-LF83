#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "InstrumentInformationWidget.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "Setting/SettingManager.h"
#include "Log.h"
#include "UI/Frame/NumberKeyboard.h"
#include "Communication/CommunicationException.h"
#include "Controller/ControllerManager.h"
#include "AlarmManager/AlarmManager.h"
#include "LuaEngine/LuaEngine.h"
#include "System/PlatformInfo.h"
#include <qrencode.h>
#include <QDebug>
#include "Interface/Wqimc/WqimcManager.h"

using namespace Configuration;
using namespace System;
using namespace Controller;
using namespace Communication;
using namespace Measure;
using namespace Lua;
using namespace Interface::Wqimc;

namespace UI
{

InstrumentInformationWidget::InstrumentInformationWidget(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);

    QFont font;
    font.setPointSize(12);

    QLabel *logoLabel = new QLabel();
    logoLabel->setFixedSize(160, 74);
#if !OEM
    logoLabel->setPixmap(QPixmap(":/img/logo"));
#endif
    logoLabel->setScaledContents(true);
    QHBoxLayout *logoLabelLayout = new QHBoxLayout();
    logoLabelLayout->addWidget(logoLabel);

    QLabel *type = new QLabel();
    type->setText("仪器型号 :");
    type->setFixedSize(80, 35);
    type->setFont(font);

    typeName = new QLabel();
    typeName->setFixedSize(400, 35);
    typeName->setFont(font);

    QHBoxLayout *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(type);
    typeLayout->addWidget(typeName);
    typeLayout->setContentsMargins(60, 0, 0, 0);
    QLabel *name = new QLabel();
    name->setText("仪器名称 :");
    name->setFixedSize(80, 35);
    name->setFont(font);

    nameValue = new QLabel();
    nameValue->setFixedSize(400, 35);
    nameValue->setFont(font);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(name);
    nameLayout->addWidget(nameValue);
    nameLayout->setContentsMargins(60, 0, 0, 0);

    QLabel *argument = new QLabel();
    argument->setText("测定项目 :");
    argument->setFixedSize(80, 35);
    argument->setFont(font);

    argumentName = new QLabel();
    argumentName->setFixedSize(400, 35);
    argumentName->setFont(font);

    QHBoxLayout *argumentLayout = new QHBoxLayout();
    argumentLayout->addWidget(argument);
    argumentLayout->addWidget(argumentName);
    argumentLayout->setContentsMargins(60, 0, 0, 0);

    QLabel *serialnumber = new QLabel();
    serialnumber->setText("序列号:");
    serialnumber->setFixedSize(80, 35);
    serialnumber->setFont(font);

    serialnumberName = new QLabel();
    serialnumberName->setFixedSize(400, 35);
    serialnumberName->setFont(font);

    QHBoxLayout *serialnumberLayout = new QHBoxLayout();
    serialnumberLayout->addWidget(serialnumber);
    serialnumberLayout->addWidget(serialnumberName);
    serialnumberLayout->setContentsMargins(60, 0, 0, 0);

    QLabel *typeNumber = new QLabel();
    typeNumber->setText("仪器型码:");
    typeNumber->setFixedSize(80, 35);
    typeNumber->setFont(font);

    typeNumberName = new QLabel();
    typeNumberName->setFixedSize(400, 35);
    typeNumberName->setFont(font);

    QHBoxLayout *typeNumberLayout = new QHBoxLayout();
    typeNumberLayout->addWidget(typeNumber);
    typeNumberLayout->addWidget(typeNumberName);
    typeNumberLayout->setContentsMargins(60, 0, 0, 0);

    QLabel *version = new QLabel();
    version->setText("仪器版本 :");
    version->setFixedSize(80, 35);
    version->setFont(font);

    versionName = new QLabel();
    versionName->setFixedSize(400, 35);
    versionName->setFont(font);

    QHBoxLayout *versionLayout = new QHBoxLayout();
    versionLayout->addWidget(version);
    versionLayout->addWidget(versionName);
    versionLayout->setContentsMargins(60, 0, 0, 0);

    QLabel *platform = new QLabel();
    platform->setText("平台版本 :");
    platform->setFixedSize(80, 35);
    platform->setFont(font);

    platformName = new QLabel();
    platformName->setFixedSize(400, 35);
    platformName->setFont(font);

    QHBoxLayout *platformLayout = new QHBoxLayout();
    platformLayout->addWidget(platform);
    platformLayout->addWidget(platformName);
    platformLayout->setContentsMargins(60, 0, 0, 0);

    QLabel *firm = new QLabel();
    firm->setText("生产厂商 :");
    firm->setFixedSize(80, 35);
    firm->setFont(font);

    firmName = new QLabel();
    firmName->setFixedSize(400, 35);
    firmName->setFont(font);

    QHBoxLayout *firmLayout = new QHBoxLayout();
    firmLayout->addWidget(firm);
    firmLayout->addWidget(firmName);
    firmLayout->setContentsMargins(60, 0, 0, 0);

    QFont wordFont;
    wordFont.setPointSize(14);
    QLabel *wordLabel = new QLabel();
    wordLabel->setAlignment(Qt::AlignCenter);
    wordLabel->setText("[微信扫码  轻松售后]");
    //wordLabel->setStyleSheet("color:#191970;");
    wordLabel->setFixedSize(200, 30);
    wordLabel->setFont(wordFont);

    qrencodeLabel = new QLabel();
    qrencodeLabel->setFixedSize(200,200);

    m_setSNButton = new QPushButton();
    m_setSNButton->setObjectName("brownButton");
    m_setSNButton->setText("修改序列号");
    QFont SNButtonFont = m_setSNButton->font();
    SNButtonFont.setPointSize(12);
    m_setSNButton->setFont(SNButtonFont);
    m_setSNButton->setFixedSize(100, 40);
    m_setSNButton->setContentsMargins(0, 0, 20, 0);

    QHBoxLayout *SNButtonLayout = new QHBoxLayout();
    SNButtonLayout->addStretch();
    SNButtonLayout->addWidget(m_setSNButton);
    SNButtonLayout->addStretch();

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addStretch();
#if !OEM
    leftLayout->addLayout(typeLayout);
#endif
    leftLayout->addLayout(nameLayout);
    leftLayout->addLayout(argumentLayout);
    leftLayout->addLayout(typeNumberLayout);
    leftLayout->addLayout(serialnumberLayout);
    leftLayout->addLayout(versionLayout);
    leftLayout->addLayout(platformLayout);
#if !OEM
    leftLayout->addLayout(firmLayout);
#endif
    leftLayout->addStretch();

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addLayout(logoLabelLayout);
    rightLayout->addStretch();
    rightLayout->addWidget(wordLabel);
    rightLayout->addWidget(qrencodeLabel);
    rightLayout->addStretch();
    rightLayout->addLayout(SNButtonLayout);

#if OEM
    bool useQrencode = true;
    luaEngine->GetLuaValue(state, "config.system.useQrencode", useQrencode);
    if(useQrencode == false)
    {
        wordLabel->setVisible(false);
        qrencodeLabel->setVisible(false);
    }
#endif

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    this->setLayout(mainLayout);
    this->InfoDisplay();
    this->ChangeBottomStatus();
    this->UpdateQrencode();

    connect(m_setSNButton, SIGNAL(clicked(bool)), this, SLOT(SlotSetSNButton()));
}

void InstrumentInformationWidget::showEvent(QShowEvent *event)
{
    (void)event;
    this->InfoDisplay();
}

void InstrumentInformationWidget::InfoDisplay()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    bool bridgeMode;
    luaEngine->GetLuaValue(state, "config.system.bridgeMode", bridgeMode);
    if (bridgeMode)
    {
        MainController * MC = ControllerManager::Instance()->MC;
        try
        {
            typeName->setText(MC->IInstrumentInfo->GetModel().c_str());
            serialnumberName->setText(MC->IInstrumentInfo->GetSn().c_str());
            typeNumberName->setText(MC->IInstrumentInfo->GetType().c_str());
            firmName->setText(MC->IInstrumentInfo->GetManuFacturer().c_str());
            versionName->setText(MC->IInstrumentInfo->GetSoftwareVersion().ToString().c_str());
            platformName->setText(MC->IInstrumentInfo->GetPlatFormVersion().ToString().c_str());
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

        String model;
        luaEngine->GetLuaValue(state, "config.info.instrument.model", model);
        String paramType = SettingManager::Instance()->GetParamTypeName();
        typeName->setText((model + paramType).c_str());

        String sn;
        luaEngine->GetLuaValue(state, "config.info.instrument.sn", sn);
        serialnumberName->setText(sn.c_str());

        String type;
        luaEngine->GetLuaValue(state, "config.info.instrument.type", type);
        typeNumberName->setText(type.c_str());

        String manuFacturer;
        luaEngine->GetLuaValue(state, "config.info.instrument.manuFacturer", manuFacturer);
        firmName->setText(manuFacturer.c_str());

        String systemVersion;
        luaEngine->GetLuaValue(state, "setting.version.system", systemVersion);

        #if OEM
            systemVersion += " BL";
        #endif

        versionName->setText(systemVersion.c_str());

        platformName->setText(System::PlatformVersion.c_str());
    }
    String instrumentInfoName;
    luaEngine->GetLuaValue(state, "setting.instrument.name", instrumentInfoName);
    nameValue->setText(instrumentInfoName.c_str());

    argumentName->setText(SettingManager::Instance()->GetValidInstrumentInfoArgus().c_str());
}

void InstrumentInformationWidget::UpdateQrencode()
{
    QString urlStr = "https://www.watertestcloud.com";

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    String type;
    luaEngine->GetLuaValue(state, "config.info.instrument.type", type);
    QString typeStr;
    typeStr = "t=" + QString::fromStdString(type);

    String sn;
    luaEngine->GetLuaValue(state, "config.info.instrument.sn", sn);
    QString snStr;
    snStr = "s=" + QString::fromStdString(sn);

    String systemVersion;
    luaEngine->GetLuaValue(state, "setting.version.system", systemVersion);
    QString verStr;
    verStr = "v=" + QString::fromStdString(systemVersion);
    #if OEM
        verStr = verStr + "_BL";
    #endif

    String modbus;
    try
    {
        OOLUA::get_global(state, "modbusStr", modbus);
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("OOLUA::get_global modbusStr fail. OOLUA::Exception ==> %s.", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("OOLUA::get_global modbusStr fail. std::exception ==> %s.", e.what());
    }
    QString modbusStr;
    modbusStr = "m=" + QString::fromStdString(modbus);

    QStringList strList;
    strList.append(typeStr);
    strList.append(snStr);
    strList.append(verStr);
    strList.append(modbusStr);
    QString paramStr = strList.join('&');

    QString tempStr = urlStr + "?" + paramStr;
    //qDebug()<<"qrencode text = "<<tempStr;

    QRcode *qrcode; //二维码数据
    qrcode = QRcode_encodeString(tempStr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);

    qint32 tempWidth = qrencodeLabel->width(); //二维码图片的大小
    qint32 tempHeight = qrencodeLabel->height();
    qint32 qrcodeWidth = qrcode->width > 0 ? qrcode->width : 1;

    double scaleX = (double)tempWidth / (double)qrcodeWidth; //二维码图片的缩放比例
    double scaleY = (double) tempHeight / (double) qrcodeWidth;

    QImage img = QImage(tempWidth, tempHeight, QImage::Format_ARGB32);

    QPainter painter(&img);

    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, tempWidth, tempHeight);

    QColor foreground(Qt::black);
    painter.setBrush(foreground);

    for( qint32 y = 0; y < qrcodeWidth; y ++)
    {
        for(qint32 x = 0; x < qrcodeWidth; x++)
        {
            unsigned char b = qrcode->data[y * qrcodeWidth + x];
            if(b & 0x01)
            {
                QRectF r(x * scaleX, y * scaleY, scaleX, scaleY);
                painter.drawRects(&r, 1);
            }
        }
    }

    QPixmap pixmap = QPixmap::fromImage(img);
    qrencodeLabel->setPixmap(pixmap);
    //qrencodeLabel->setVisible(true);
}

void InstrumentInformationWidget::SlotSetSNButton()
{
    QDialog *setSNDialog = new QDialog();
    setSNDialog->setFixedSize(380, 180);
    setSNDialog->setWindowFlags(Qt::WindowSystemMenuHint);
    setSNDialog->setWindowTitle("SN设置");
    setSNDialog->move(this->width() / 2 - 100, this->height() / 2);

    QFont SNFont;
    SNFont.setPointSize(12);

    QLabel *SNLabel = new QLabel();
    SNLabel->setText("SN:");
    SNLabel->setFixedSize(30, 30);
    SNLabel->setFont(SNFont);

    QLabel *charLabel = new QLabel();
    charLabel->setText("");
    charLabel->setFixedSize(10, 30);
    charLabel->setFont(SNFont);

    QLineEdit *SNEdit = new QLineEdit();
    SNEdit->setFixedSize(150, 30);
    SNEdit->setFont(SNFont);
    SNEdit->clearFocus();

    MainController * MC = ControllerManager::Instance()->MC;

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    bool bridgeMode;
    luaEngine->GetLuaValue(state, "config.system.bridgeMode", bridgeMode);
    if (bridgeMode)
    {
        try
        {
            SNEdit->setText(MC->IInstrumentInfo->GetSn().c_str());
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
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State* state = luaEngine->GetThreadState();

        String sn;
        luaEngine->GetLuaValue(state, "config.info.instrument.sn", sn);

        QString strSN = QString(sn.c_str());

        QString tmpPattern("^([A-Z]+)(\\d{1,8})$");  //匹配n位大写字母+1~8位整数
        QRegExp tmpRx(tmpPattern);
        strSN.indexOf(tmpRx);

        QString head = tmpRx.cap(1);
        charLabel->setFixedWidth(head.size()*10);
        charLabel->setText(head);

        SNEdit->setText(tmpRx.cap(2));
    }

    CNumberKeyboard *numberKey = new CNumberKeyboard(SNEdit);
    SNEdit->installEventFilter(numberKey);

    QHBoxLayout *SNLayout = new QHBoxLayout();
    SNLayout->addWidget(SNLabel);
    SNLayout->addWidget(charLabel);
    SNLayout->addWidget(SNEdit);
    SNLayout->addStretch();
    SNLayout->setContentsMargins(20, 0, 0, 0);

    QPushButton *OKButton = new QPushButton();
    OKButton->setObjectName("brownButton");
    OKButton->setText("确定");
    OKButton->setFixedSize(80, 40);
    OKButton->setFont(SNFont);

    QPushButton *ConcelButton = new QPushButton();
    ConcelButton->setObjectName("brownButton");
    ConcelButton->setText("取消");
    ConcelButton->setFixedSize(80, 40);
    ConcelButton->setFont(SNFont);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(OKButton);
    buttonLayout->addWidget(ConcelButton);
    buttonLayout->setSpacing(30);
    buttonLayout->setContentsMargins(0, 0, 0, 30);

    QVBoxLayout *dialogLayout = new QVBoxLayout();
    dialogLayout->addStretch();
    dialogLayout->addLayout(SNLayout);
    dialogLayout->addStretch();
    dialogLayout->addLayout(buttonLayout);

    setSNDialog->setLayout(dialogLayout);

    connect(OKButton, SIGNAL(clicked()), setSNDialog, SLOT(accept()));
    connect(ConcelButton, SIGNAL(clicked()), setSNDialog, SLOT(close()));
    if (setSNDialog->exec() == QDialog::Accepted)
    {
        QString strSN = charLabel->text() +SNEdit->text();

        bool valueValid = false;
        QString tmpPattern("^\\d{1,8}$");  //匹配1-8位整数
        QRegExp tmpRx(tmpPattern);
        valueValid = tmpRx.exactMatch(SNEdit->text());

        if(valueValid == true)
        {
            if (bridgeMode)
            {
                try
                {
                     MC->IInstrumentInfo->SetSn(strSN.toStdString());
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
                luaEngine->SetLuaValue("config.info.instrument.sn", strSN.toStdString());
                SettingManager::Instance()->InstrumentInfoSave();
            }
        }

        this->InfoDisplay();
        this->UpdateQrencode();
        WqimcManager::Instance()->ChangeSN();

        if (numberKey)
        {
            delete numberKey;
        }
    }
}

void InstrumentInformationWidget::OnUserChange()
{
    this->ChangeBottomStatus();
}

void InstrumentInformationWidget::ChangeBottomStatus()
{
    if (LoginDialog::userType == Super)
    {
        m_setSNButton->show();
    }
    else if (LoginDialog::userType == Administrator)
    {
        m_setSNButton->hide();
    }
    else if (LoginDialog::userType == Maintain)
    {
        m_setSNButton->hide();
    }
    else if (LoginDialog::userType == General)
    {
        m_setSNButton->hide();
    }
}

InstrumentInformationWidget::~InstrumentInformationWidget()
{

}
}
