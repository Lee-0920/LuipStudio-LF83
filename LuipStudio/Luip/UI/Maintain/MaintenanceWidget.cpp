
#include "Log.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include "StatusManager/StatusManager.h"
#include "FlowManager/FlowManager.h"
#include "LuaEngine/LuaEngine.h"
#include "MaintenanceWidget.h"
#include "Setting/SettingManager.h"

using namespace Configuration;
using namespace OOLUA;
using namespace Measure;
using namespace std;
using namespace Flow;
using namespace Lua;

namespace UI
{

MaintenanceWidget::MaintenanceWidget(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);

    LuaEngine* luaEngine = LuaEngine::Instance();
    Script* lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.operation.maintain", itermsTable);

    int useRowCount = 0;
    int administratorRow = 0;
    int superRow = 0;

    itermsTable.at("rowCount", useRowCount);
    itermsTable.safe_at("superRow", superRow);
    itermsTable.at("administratorRow", administratorRow);

    maintenanceTableWidget = new MQtableWidget();
    maintenanceTableWidget->setColumnCount(2);

    m_tableInfo.userRow = useRowCount;
    m_tableInfo.hiddenRow = administratorRow + superRow; //管理员和工程师权限需隐藏行数总和
    m_tableInfo.perPageRow = 10;
    m_tableInfo.realRow = 0;
    RowHiddenHandler::Instance()->CreateRow(maintenanceTableWidget, m_tableInfo);

    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        maintenanceTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");
    }

    maintenanceTableWidget->setFixedSize(400,400);

    QStringList columnName;
    columnName<<"项目"<<"操作";
    maintenanceTableWidget->setColumn(columnName);
    maintenanceTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度

    QFont dataFont = maintenanceTableWidget->font();
    dataFont.setPointSize(12);

    maintenanceTableWidget->setFont(dataFont);
    maintenanceTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QHBoxLayout *leftLayout = new QHBoxLayout();
    leftLayout->addWidget(maintenanceTableWidget, 0, Qt::AlignTop | Qt::AlignLeft);

    stopButton = new QPushButton();
    stopButton->setObjectName("brownButton");
    stopButton->setText("停止");
    stopButton->setFont(dataFont);
    stopButton->setFixedSize(100,40);
    QHBoxLayout *stopButtonLayout = new QHBoxLayout();
    stopButtonLayout->addWidget(stopButton);
    stopButtonLayout->addStretch();
    stopButtonLayout->setContentsMargins(50, 50, 0, 0);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addLayout(stopButtonLayout);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    this->setLayout(mainLayout);
    for(int i = 0;i < maintenanceTableWidget->rowCount();i++)
    {
        maintenanceTableWidget->setRowHeight(i, 36);
    }
    if (m_tableInfo.userRow <= 10)
    {
        maintenanceTableWidget->setColumnWidth(0, 250);
    }
    else
    {
        maintenanceTableWidget->setColumnWidth(0, 220);
    }

    maintenanceTableWidget->setColumnWidth(1, 148);

    SpaceInit();

    int type = SettingManager::Instance()->GetParamType();

    Uint8 row = 0;
    oolua_ipairs(itermsTable)
    {
        Table itermTable;
        lua->pull(itermTable);

        int targetPrivilege = 0;
        bool isTarget = itermTable.safe_at("targetPrivilege", targetPrivilege);

        if (isTarget == false || (type & targetPrivilege))
        {
            MaintenanceItem maintenanceItem;

            itermTable.at("text", maintenanceItem.text);
            itermTable.at("writePrivilege", maintenanceItem.writePrivilege);
            itermTable.at("readPrivilege", maintenanceItem.readPrivilege);
            itermTable.safe_at("createFlow", maintenanceItem.maintenanceFunc);

            maintenanceTableWidget->item(row, 0)->setText(QString::fromUtf8(maintenanceItem.text.c_str()));//显示名称
            m_items.insert(make_pair(row, maintenanceItem));
            row++;

        }
    }
    oolua_ipairs_end()

    if(maintenanceTableWidget->rowCount() > row && maintenanceTableWidget->rowCount() > 10)
    {
        maintenanceTableWidget->setRowCount(row);
    }

    ConfigInit();
    ConnectInit(row);
    OnUserChange();

    connect(stopButton, SIGNAL(clicked()), this, SLOT(StopMeasure()));
}

void MaintenanceWidget::ConnectInit(Uint8 row)
{
    QFont font;                           //字体
    font.setPointSize(12);
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i < row; i++)
    {
        QPushButton *button = new QPushButton();
        button->setFixedSize(148,35);
        button->setFont(font);
        button->setText("执行");
        m_buttonList.append(button);
        maintenanceTableWidget->setCellWidget(i,1,button);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(measureStart(int)));
}

void MaintenanceWidget::SpaceInit()
{
    for(int i = 0;i < maintenanceTableWidget->rowCount();i++)
    {
        for(int j = 0;j < maintenanceTableWidget->columnCount();j++)
        {
            maintenanceTableWidget->setItem(i, j, new QTableWidgetItem());
            maintenanceTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *item = maintenanceTableWidget->item(i, j);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void MaintenanceWidget::StopMeasure()
{
    FlowManager::Instance()->StopFlow();
}

void MaintenanceWidget::measureStart(int index)
{   
    if (!FlowManager::Instance()->IsFlowEnable())  // 空闲状态
    {
        MessageDialog msg("请确认仪器状态是否为空闲！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    if (!FlowManager::Instance()->IsAuthorize())  // 运行权限
    {
        MessageDialog msg("运行权限已过期！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    if (m_items.count(index))
    {
        Script *lua = LuaEngine::Instance()->GetEngine();
        MaintenanceItem item = m_items[index];
        Lua_func_ref func = item.maintenanceFunc;
        if (lua->call(func))
        {
            FlowManager::Instance()->StartFlow();
        }
        else
        {
            logger->warn("MaintenanceWidget call the function Fail on lua");
        }
    }
}

void MaintenanceWidget::ConfigInit()
{

}

void MaintenanceWidget::OnUserChange()
{
    if(LoginDialog::userType == Super)
    {
        for (map<Uint8, MaintenanceItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;

            maintenanceTableWidget->setRowHidden(row, false);
        }
    }
    else if(LoginDialog::userType == Administrator)
    {
        for (map<Uint8, MaintenanceItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            MaintenanceItem &maintenanceItem = iter->second;
            if (Administrator >= maintenanceItem.readPrivilege)
            {
                maintenanceTableWidget->setRowHidden(row, false);
            }
            else
            {
                maintenanceTableWidget->setRowHidden(row, true);
            }
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        for (map<Uint8, MaintenanceItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            MaintenanceItem &maintenanceItem = iter->second;
            if (Maintain >= maintenanceItem.readPrivilege)
            {
                maintenanceTableWidget->setRowHidden(row, false);
            }
            else
            {
                maintenanceTableWidget->setRowHidden(row, true);
            }
        }
    }

    RowHiddenHandler::Instance()->AdjustRowHidden(maintenanceTableWidget, m_tableInfo);
}

MaintenanceWidget::~MaintenanceWidget()
{

}

}
