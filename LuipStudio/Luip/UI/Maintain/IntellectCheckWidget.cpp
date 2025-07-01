#include "Interface/Wqimc/WqimcManager.h"
#include "UseResourceWidget.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "System/Types.h"
#include "IntellectCheckWidget.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include "LuaEngine/LuaEngine.h"
#include "StatusManager/StatusManager.h"
#include "Controller/ControllerManager.h"
#include "FlowManager/FlowManager.h"
#include "Setting/SettingManager.h"

using namespace Configuration;
using namespace System;
using namespace Lua;
using namespace std;
using namespace OOLUA;
using namespace Measure;
using namespace Controller;
using namespace Flow;

namespace UI
{

IntellectCheck::IntellectCheck(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);
    LuaEngine* luaEngine = LuaEngine::Instance();
    Script* lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.diagnosis.smartDetect", itermsTable);

    int useRowCount = 0;
    int administratorRow = 0;
    int superRow = 0;

    itermsTable.at("rowCount", useRowCount);
    itermsTable.at("name", m_suitName);
    itermsTable.safe_at("superRow", superRow);
    itermsTable.at("administratorRow", administratorRow);

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    this->resize(650,430);

    intellectCheckTableWidget = new MQtableWidget();

    intellectCheckTableWidget->setColumnCount(3);

    m_tableInfo.userRow = useRowCount;
    m_tableInfo.hiddenRow = administratorRow + superRow; //管理员和工程师权限需隐藏行数总和
    m_tableInfo.perPageRow = 12;
    m_tableInfo.realRow = 0;
    RowHiddenHandler::Instance()->CreateRow(intellectCheckTableWidget, m_tableInfo);

    intellectCheckTableWidget->setFixedSize(432,400);

    QFont headerFont;
    headerFont.setPointSize(10);

    QTableWidgetItem *column0 = new QTableWidgetItem(tr("选择"));
    column0->setFont(headerFont);
    intellectCheckTableWidget->setHorizontalHeaderItem(0, column0);

    QTableWidgetItem *column1 = new QTableWidgetItem(tr("诊断项"));
    column1->setFont(headerFont);
    intellectCheckTableWidget->setHorizontalHeaderItem(1, column1);

    QTableWidgetItem *column2 = new QTableWidgetItem(tr("结果"));
    column2->setFont(headerFont);
    intellectCheckTableWidget->setHorizontalHeaderItem(2, column2);

    intellectCheckTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度
    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        intellectCheckTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");
    }

    QFont dataFont = intellectCheckTableWidget->font();
    dataFont.setPointSize(8);
    intellectCheckTableWidget->setFont(dataFont);
    intellectCheckTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑

    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(intellectCheckTableWidget);
    measureResultTableLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(measureResultTableLayout);
    leftLayout->addStretch();

    oneCheckAll = new QPushButton();
    oneCheckAll->setObjectName("brownButton");
    oneCheckAll->setText("全选");
    oneCheckAll->setFont(font);
    oneCheckAll->setFixedSize(100,40);

    oneCheck = new QPushButton();
    oneCheck->setObjectName("brownButton");
    oneCheck->setText("一键检测");
    oneCheck->setFont(font);
    oneCheck->setFixedSize(100,40);

    oneKeyStop = new QPushButton();
    oneKeyStop->setObjectName("brownButton");
    oneKeyStop->setText("一键停止");
    oneKeyStop->setFont(font);
    oneKeyStop->setFixedSize(100,40);

    stopButton = new QPushButton();
    stopButton->setObjectName("brownButton");
    stopButton->setText("停止");
    stopButton->setFont(font);
    stopButton->setFixedSize(100,40);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addStretch();
    rightLayout->addWidget(oneCheckAll);
    rightLayout->addWidget(oneCheck);
    rightLayout->addWidget(oneKeyStop);
    rightLayout->addWidget(stopButton);
    rightLayout->addStretch();
    rightLayout->setSpacing(30);
    rightLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->setSpacing(70);
    mainLayout->addLayout(rightLayout);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    intellectCheckTableWidget->setColumnWidth(0,40);    //设置每行或每列的高度或宽度
    intellectCheckTableWidget->setColumnWidth(1,190);

    if (m_tableInfo.userRow > 12)
    {
        intellectCheckTableWidget->setColumnWidth(2, 170);
    }
    else
    {
        intellectCheckTableWidget->setColumnWidth(2, 200);
    }


    for(int i = 0;i < intellectCheckTableWidget->rowCount(); i++)
    {
        intellectCheckTableWidget->setRowHeight(i,30);
    }
    intellectCheckTableWidget->setFont(font);

    SpaceInit();

    for(int i = 0;i < m_tableInfo.userRow; i++)
    {
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setObjectName("checkboxtwo");
        checkBox->setCheckState(Qt::Unchecked);
        intellectCheckTableWidget->setCellWidget(i, 0, checkBox);
        m_checkBox.insert(make_pair(i, checkBox));
    }

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
            String text;

            itermTable.at("text", text);
            intellectCheckTableWidget->item(row, 1)->setText(QString::fromUtf8(text.c_str()));//显示名称

            IntellectCheckItem item;
            itermTable.at("name", item.name);

            itermTable.at("createFlow", item.func);
            itermTable.at("writePrivilege", item.writePrivilege);
            itermTable.at("readPrivilege", item.readPrivilege);

            item.row = row;
            m_items.push_back(item);

            row++;
        }
    }
    oolua_ipairs_end()

    if(m_items.size() < intellectCheckTableWidget->rowCount())
    {
        for(int i = m_items.size(); i < m_tableInfo.userRow; i++)
        {
            intellectCheckTableWidget->setRowHidden(i, true);
        }
    }

    OnUserChange();

    connect(oneCheck,SIGNAL(clicked(bool)),this, SLOT(OneCheckConfig()));
    connect(stopButton,SIGNAL(clicked(bool)),this, SLOT(SlotStopDetect()));
    connect(oneCheckAll,SIGNAL(clicked(bool)),this, SLOT(OneCheckAllConfig()));
    connect(oneKeyStop,SIGNAL(clicked(bool)),this, SLOT(SlotOneKeyStop()));
}

void IntellectCheck::SpaceInit()
{
    for(int i = 0;i < intellectCheckTableWidget->rowCount();i++)
    {
        for(int j = 0;j < intellectCheckTableWidget->columnCount();j++)
        {
            intellectCheckTableWidget->setItem(i, j, new QTableWidgetItem());
            intellectCheckTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *item = intellectCheckTableWidget->item(i, j);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void IntellectCheck::OneCheckConfig()
{
    if (!FlowManager::Instance()->IsFlowEnable())  // 空闲状态
    {
        MessageDialog msg("请确认仪器状态是否为空闲！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    bool isSelected = false;
    Script *lua = LuaEngine::Instance()->GetEngine();

    for (map<Uint8, QCheckBox *>::iterator iter = m_checkBox.begin(); iter != m_checkBox.end(); ++iter)
    {
        QCheckBox *checkBox = iter->second;
        if (checkBox->checkState() == Qt::Checked)
        {
            Uint8 row = iter->first;
            intellectCheckTableWidget->item(row, 2)->setText(" ");

            for (vector<IntellectCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
            {
                if ((*it).row == row)
                {
                    Lua_func_ref func = (*it).func;
                    lua->call(func);
                    isSelected = true;
                }
            }
        }
    }

    if (isSelected)
    {
        FlowManager::Instance()->RegisterFinishedNotifier(this);
        FlowManager::Instance()->StartFlow();
    }
}

void IntellectCheck::SlotStopDetect()
{
    FlowManager::Instance()->StopFlow();
}

void IntellectCheck::SlotOneKeyStop()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    Script* lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.diagnosis.smartDetect", itermsTable);

    oolua_ipairs(itermsTable)
    {
        Table itermTable;
        lua->pull(itermTable);

        String name;
        itermTable.at("name", name);

        FlowManager::Instance()->RemoveFlow(name);
    }
    oolua_ipairs_end()
}

void IntellectCheck::OneCheckAllConfig()
{
    int flag = false;
    for (map<Uint8, QCheckBox *>::iterator iter = m_checkBox.begin(); iter != m_checkBox.end(); ++iter)
    {
        QCheckBox *checkBox = iter->second;
        if (checkBox->checkState() == Qt::Unchecked)
        {
            flag = true;
            break;
        }
    }
    for (map<Uint8, QCheckBox *>::iterator iter = m_checkBox.begin(); iter != m_checkBox.end(); ++iter)
    {
        QCheckBox *checkBox = iter->second;
        if(!flag)
        {
            checkBox->setCheckState(Qt::Unchecked);
        }
        else
        {
            checkBox->setCheckState(Qt::Checked);
        }
    }
}

IntellectCheck::~IntellectCheck()
{
    if (intellectCheckTableWidget)
        delete intellectCheckTableWidget;
}


void IntellectCheck::OnUserChange()
{
    if(LoginDialog::userType == Super)
    {
        for (vector<IntellectCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            Uint8 row = (*it).row;

            intellectCheckTableWidget->setRowHidden(row, false);
        }
    }
    else if(LoginDialog::userType == Administrator)
    {
        for (vector<IntellectCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            Uint8 row = (*it).row;
            if (Administrator >= (*it).readPrivilege)
            {
                intellectCheckTableWidget->setRowHidden(row, false);
            }
            else
            {
                intellectCheckTableWidget->setRowHidden(row, true);
            }
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        for (vector<IntellectCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            Uint8 row = (*it).row;
            if (Maintain >= (*it).readPrivilege)
            {
                intellectCheckTableWidget->setRowHidden(row, false);
            }
            else
            {
                intellectCheckTableWidget->setRowHidden(row, true);
            }
        }
    }

    RowHiddenHandler::Instance()->AdjustRowHidden(intellectCheckTableWidget, m_tableInfo);
}

void IntellectCheck::OnFlowActivated(String name, long time)
{
    (void)name;
    (void)time;
}

void IntellectCheck::OnFlowFinished(String name, String result)
{
    for (vector<IntellectCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        if ((*it).name == name)
        {
            intellectCheckTableWidget->item((*it).row, 2)->setText(QString(result.c_str()));
//            if ("诊断中..." != result)
            {
                Interface::Wqimc::Api::Manager::DiagnosisResult diagnosisResult;
                diagnosisResult.__set_suit(m_suitName);
                diagnosisResult.__set_name(name);
                diagnosisResult.__set_result(result);
                Interface::Wqimc::WqimcManager::Instance()->notifyDiagnosisResult(diagnosisResult);
            }
        }
    }
}

void IntellectCheck::OnFlowThreadExit()
{

}

void IntellectCheck::OnUpdateWidget(UpdateEvent event, System::String message)
{
    (void)message;
    if (event == UpdateEvent::StartIntellectCheck)
    {
        FlowManager::Instance()->RegisterFinishedNotifier(this);
        FlowManager::Instance()->StartFlow();
    }
}

}
