#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "Log.h"
#include "DeviceInformation.h"
#include <QTextStream>
#include <QFile>
#include <QSysInfo>
#include "Setting/SettingManager.h"
#include "LuaEngine/LuaEngine.h"
#include "Communication/CommunicationException.h"
#include "Controller/ControllerManager.h"
#include "AlarmManager/AlarmManager.h"

#define FileName  ("/opt/version")

using namespace System;
using namespace Configuration;
using namespace OOLUA;
using namespace std;
using namespace Controller;
using namespace Communication;
using namespace Measure;
using namespace Lua;

namespace UI
{

DeviceInformation::DeviceInformation(QWidget *parent) :
        QWidget(parent)
{
    QFont font;                           //字体
    font.setPointSize(12);                //字体大小
    this->resize(650, 400);

    Script *lua = LuaEngine::Instance()->GetEngine();
    Table table, itermsTable;
    itermsTable.bind_script(*lua);
    itermsTable.set_table("setting");
    itermsTable.at("ui", table);
    table.at("deviceInfomationItems", itermsTable);


    int deviceNum = 0;
    itermsTable.at("deviceNum", deviceNum);
    int rowCount = deviceNum * 7 + 9;

    deviceInformationTableWidget = new MQtableWidget();

    deviceInformationTableWidget->setColumnCount(3);
    if (rowCount <= 10)
    {
        deviceInformationTableWidget->setRowCount(10);
    }
    else
    {
        deviceInformationTableWidget->setRowCount(rowCount);
    }
    deviceInformationTableWidget->setFixedSize(550, 390);

    QFont headerFont;
    headerFont.setPointSize(10);

    deviceInformationTableWidget->horizontalHeader()->setFixedHeight(0); // 设置表头的高度

    QFont dataFont = deviceInformationTableWidget->font();
    dataFont.setPointSize(8);
    deviceInformationTableWidget->setFont(dataFont);
    deviceInformationTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑
    deviceInformationTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置表格为整行选择
    if (rowCount > 10)
    {
        deviceInformationTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");
    }

    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(deviceInformationTableWidget);
    measureResultTableLayout->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *leftLayout = new QVBoxLayout();

    leftLayout->addLayout(measureResultTableLayout);

    leftLayout->addStretch();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addStretch();
    rightLayout->setSpacing(0);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();

    this->setLayout(mainLayout);

    deviceInformationTableWidget->setColumnWidth(0, 150);    //设置每行或每列的高度或宽度
    deviceInformationTableWidget->setColumnWidth(1, 150);
    if (rowCount > 10)
    {
        deviceInformationTableWidget->setColumnWidth(2, 218);
    }
    else
    {
        deviceInformationTableWidget->setColumnWidth(2, 248);
    }

    deviceInformationTableWidget->setFont(font);
    for (int i = 0; i < 10; i++)
    {
        deviceInformationTableWidget->setRowHeight(i, 30.3);
    }

    SpaceInit();
    TableInit(deviceNum);

    Uint8 row = 9;

    oolua_ipairs(itermsTable)
    {
        Table typeTable;
        String typeText;
        String name;

        lua->pull(typeTable);

        typeTable.at("text", typeText);
        deviceInformationTableWidget->item(row, 0)->setText(QString::fromUtf8(typeText.c_str()));//显示类别

        typeTable.at("name", name);

        m_items.insert(make_pair(row, name));
        row += 7;
    }
    oolua_ipairs_end()

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(SlotUpdateData()));
}

void DeviceInformation::SpaceInit()
{
    for (int i = 0; i < deviceInformationTableWidget->rowCount(); i++)
        for (int j = 0; j < deviceInformationTableWidget->columnCount(); j++)
        {
            deviceInformationTableWidget->setItem(i, j, new QTableWidgetItem());
            deviceInformationTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *item = deviceInformationTableWidget->item(i, j);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(Qt::ItemIsEnabled);
        }
}

void DeviceInformation::TableInit(System::Uint8 deviceNum)
{
    deviceInformationTableWidget->item(0, 0)->setText("主控板");

    deviceInformationTableWidget->item(0, 1)->setText("类型");
    deviceInformationTableWidget->item(1, 1)->setText("型号");
    deviceInformationTableWidget->item(2, 1)->setText("序列号");
    deviceInformationTableWidget->item(3, 1)->setText("生产厂商");
#if OEM
    deviceInformationTableWidget->setRowHidden(3, true);
#endif
    deviceInformationTableWidget->item(4, 1)->setText("生产日期");
    deviceInformationTableWidget->item(5, 1)->setText("软件版本");
    deviceInformationTableWidget->item(6, 1)->setText("内核版本");
    deviceInformationTableWidget->item(7, 1)->setText("文件系统版本");
    deviceInformationTableWidget->item(8, 1)->setText("硬件版本");

    for (int i = 0; i < deviceNum; i++)
    {
        int shifting = i * 7;
        deviceInformationTableWidget->item(9 + shifting, 1)->setText("类型");
        deviceInformationTableWidget->item(10 + shifting, 1)->setText("型号");
        deviceInformationTableWidget->item(11 + shifting, 1)->setText("序列号");
        deviceInformationTableWidget->item(12 + shifting, 1)->setText("生产厂商");
#if OEM
        deviceInformationTableWidget->setRowHidden(12 + shifting, true);
#endif
        deviceInformationTableWidget->item(13 + shifting, 1)->setText("生产日期");
        deviceInformationTableWidget->item(14 + shifting, 1)->setText("软件版本");
        deviceInformationTableWidget->item(15 + shifting, 1)->setText("硬件版本");
    }
}

void DeviceInformation::showEvent(QShowEvent *event)
{
    (void)event;
    timer->start(200);
}

void DeviceInformation::SlotUpdateData()
{
    timer->stop();
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    bool bridgeMode;
    luaEngine->GetLuaValue(state, "config.system.bridgeMode", bridgeMode);
    if (bridgeMode)
    {
        MainController * MC = ControllerManager::Instance()->MC;
        try
        {
            deviceInformationTableWidget->item(0, 2)->setText(MC->IDeviceInfo->GetType().c_str());
            deviceInformationTableWidget->item(1, 2)->setText(MC->IDeviceInfo->GetModel().c_str());
            deviceInformationTableWidget->item(2, 2)->setText(MC->IDeviceInfo->GetSerialNumber().c_str());
            deviceInformationTableWidget->item(3, 2)->setText(MC->IDeviceInfo->GetManufacturer().c_str());
            deviceInformationTableWidget->item(4, 2)->setText(MC->IDeviceInfo->GetManufDate().ToString().c_str());
            deviceInformationTableWidget->item(5, 2)->setText(MC->IDeviceInfo->GetSoftwareVersion().ToString().c_str());
            deviceInformationTableWidget->item(8, 2)->setText(MC->IDeviceInfo->GetHardwareVersion().ToString().c_str());
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
        String type;
        luaEngine->GetLuaValue(state, "config.info.mainBoard.type", type);
        deviceInformationTableWidget->item(0, 2)->setText(type.c_str());

        String model;
        luaEngine->GetLuaValue(state, "config.info.mainBoard.model", model);
        deviceInformationTableWidget->item(1, 2)->setText(model.c_str());

        String sn;
        luaEngine->GetLuaValue(state, "config.info.mainBoard.sn", sn);
        deviceInformationTableWidget->item(2, 2)->setText(sn.c_str());

        String manuFacturer;
        luaEngine->GetLuaValue(state, "config.info.mainBoard.manuFacturer", manuFacturer);
        deviceInformationTableWidget->item(3, 2)->setText(manuFacturer.c_str());

        int year = 1970, month = 1, day = 1;
        luaEngine->GetLuaValue(state, "config.info.mainBoard.manuDate.year", year);
        luaEngine->GetLuaValue(state, "config.info.mainBoard.manuDate.month", month);
        luaEngine->GetLuaValue(state, "config.info.mainBoard.manuDate.day", day);
        deviceInformationTableWidget->item(4, 2)->setText(ManufDate(year, month, day).ToString().c_str());

        String version;
        luaEngine->GetLuaValue(state, "setting.version.software", version);
        deviceInformationTableWidget->item(5, 2)->setText(version.c_str());

        String hardwareVersion;
        luaEngine->GetLuaValue(state, "setting.version.hardware", hardwareVersion);
        deviceInformationTableWidget->item(8, 2)->setText(hardwareVersion.c_str());
    }
    QString version = QSysInfo::kernelVersion();
    version.replace(QString("-EmbedSky"), QString(""));
    deviceInformationTableWidget->item(6, 2)->setText(QSysInfo::kernelType() + "-" + version);
    deviceInformationTableWidget->item(7, 2)->setText(GetFileSystemVersion());


    for (map<Uint8,  String>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        Uint8 row = iter->first;
        String &name = iter->second;

        BaseController* controller = ControllerManager::Instance()->GetController(name);

        if (controller->GetConnectStatus())
        {
            try
            {
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetType().c_str());
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetModel().c_str());
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetSerialNumber().c_str());
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetManufacturer().c_str());
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetManufDate().ToString().c_str());
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetSoftwareVersion().ToString().c_str());
                deviceInformationTableWidget->item(row++, 2)->setText(controller->GetIDeviceInfo()->GetHardwareVersion().ToString().c_str());
            }
            catch(CommandTimeoutException e)
            {
                e.m_name = deviceInformationTableWidget->item(iter->first, 0)->text().toStdString();
                AlarmManager::Instance()->AddCommunicationExceptionAlarm(e);
            }
            catch (CommunicationException e)
            {
                e.m_name = deviceInformationTableWidget->item(iter->first, 0)->text().toStdString();
                AlarmManager::Instance()->AddCommunicationExceptionAlarm(e);
            }
            catch (std::exception e)
            {
                AlarmManager::Instance()->AddExceptionAlarm(e);
            }
        }
    }

//    InterfaceManager::Instance()->UpdateInfo();

}

QString DeviceInformation::GetFileSystemVersion()
{
    QString version;
    QFile file(FileName);
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            version = in.readLine();
            file.close();
        }
    }
    return version;
}

DeviceInformation::~DeviceInformation()
{

}

}

