#include "Interface/Wqimc/WqimcManager.h"
#include "UseResourceWidget.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "System/Types.h"
#include "SensorCheckWidget.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MessageDialog.h"
#include "LuaEngine/LuaEngine.h"
#include "StatusManager/StatusManager.h"
#include "Controller/ControllerManager.h"
#include "FlowManager/FlowManager.h"
#include "Setting/SettingManager.h"

using namespace Configuration;
using namespace Measure;
using System::Uint32;
using namespace std;
using namespace OOLUA;
using namespace Lua;
using namespace Controller;
using namespace Flow;

namespace UI
{

SensorCheckWidget::SensorCheckWidget(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);
    LuaEngine* luaEngine = LuaEngine::Instance();
    Script* lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.diagnosis.sensorCheck", itermsTable);

    int useRowCount = 0;
    int administratorRow = 0;
    int superRow = 0;

    itermsTable.at("rowCount", useRowCount);
    itermsTable.at("administratorRow", administratorRow);
    itermsTable.safe_at("superRow", superRow);
    itermsTable.at("name", m_suitName);

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    sensorCheckTableWidget = new MQtableWidget();

    sensorCheckTableWidget->setColumnCount(4);

    m_tableInfo.userRow = useRowCount;
    m_tableInfo.hiddenRow = administratorRow + superRow; //管理员和工程师权限需隐藏行数总和
    m_tableInfo.perPageRow = 10;
    m_tableInfo.realRow = 0;

    RowHiddenHandler::Instance()->CreateRow(sensorCheckTableWidget, m_tableInfo);

    sensorCheckTableWidget->setFixedSize(550, 340);

    QFont headerFont;
    headerFont.setPointSize(10);

    QTableWidgetItem *column0 = new QTableWidgetItem(tr("类别"));
    column0->setFont(headerFont);
    sensorCheckTableWidget->setHorizontalHeaderItem(0, column0);

    QTableWidgetItem *column1 = new QTableWidgetItem(tr(""));
    column1->setFont(headerFont);
    sensorCheckTableWidget->setHorizontalHeaderItem(1, column1);

    QTableWidgetItem *column2 = new QTableWidgetItem(tr("检测项"));
    column2->setFont(headerFont);
    sensorCheckTableWidget->setHorizontalHeaderItem(2, column2);

    QTableWidgetItem *column3 = new QTableWidgetItem(tr("状态"));

    column3->setFont(headerFont);
    sensorCheckTableWidget->setHorizontalHeaderItem(3, column3);

    sensorCheckTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度
    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        sensorCheckTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");

    }

    QFont dataFont = sensorCheckTableWidget->font();
    dataFont.setPointSize(8);
    sensorCheckTableWidget->setFont(dataFont);
    sensorCheckTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑


    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(sensorCheckTableWidget);
    measureResultTableLayout->addStretch();
    measureResultTableLayout->setContentsMargins(0, 0, 0, 0);

    oneCheck = new QPushButton();
    oneCheck->setObjectName("brownButton");
    oneCheck->setText("一键检测");
    oneCheck->setFont(font);
    oneCheck->setFixedSize(100,40);

    QHBoxLayout *oneChechLayout = new QHBoxLayout();
    oneChechLayout->addStretch();
    oneChechLayout->addWidget(oneCheck);
    oneChechLayout->setContentsMargins(0,30,130,0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(measureResultTableLayout);
    mainLayout->addLayout(oneChechLayout);
    mainLayout->addStretch();

    this->setLayout(mainLayout);

    sensorCheckTableWidget->setColumnWidth(0,130);    //设置每行或每列的高度或宽度
    sensorCheckTableWidget->setColumnWidth(1,50);
    sensorCheckTableWidget->setColumnWidth(2,170);
    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        sensorCheckTableWidget->setColumnWidth(3, 167);
    }
    else
    {
        sensorCheckTableWidget->setColumnWidth(3, 197);
    }

    for(int i = 0;i < sensorCheckTableWidget->rowCount();i++)
    {
        sensorCheckTableWidget->setRowHeight(i,30);
    }

    sensorCheckTableWidget->setFont(font);

    SpaceInit();
    for(int i = 0;i < m_tableInfo.userRow; i++)
    {
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setObjectName("checkboxtwo");
        checkBox->setCheckState(Qt::Unchecked);
        sensorCheckTableWidget->setCellWidget(i, 1, checkBox);
        m_checkBox.insert(make_pair(i, checkBox));
    }

    int type = SettingManager::Instance()->GetParamType();

    Uint8 row = 0;
    oolua_ipairs(itermsTable)
    {
        Table typeTable;
        String typeText;

        lua->pull(typeTable);
        typeTable.at("text", typeText);
        sensorCheckTableWidget->item(row, 0)->setText(QString::fromUtf8(typeText.c_str()));
        oolua_ipairs(typeTable)
        {
            Table itermTable;
            lua->pull(itermTable);

            int targetPrivilege = 0;
            bool isTarget = itermTable.safe_at("targetPrivilege", targetPrivilege);

            if (isTarget == false || (type & targetPrivilege))
            {
                String text,name;
                Lua_func_ref func;

                itermTable.at("text", text);
                sensorCheckTableWidget->item(row, 2)->setText(QString::fromUtf8(text.c_str()));

                SensorCheckItem item;
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
    }
    oolua_ipairs_end()

    OnUserChange();
    connect(oneCheck,SIGNAL(clicked(bool)),this, SLOT(SlotOneCheck()));
}

void SensorCheckWidget::SpaceInit()
{
    for(int i = 0;i < sensorCheckTableWidget->rowCount();i++)
    {
        for(int j = 0;j < sensorCheckTableWidget->columnCount();j++)
        {
            sensorCheckTableWidget->setItem(i, j, new QTableWidgetItem());
            sensorCheckTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *item = sensorCheckTableWidget->item(i, j);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void SensorCheckWidget::SlotOneCheck()
{
    if (!FlowManager::Instance()->IsFlowEnable())  // 空闲状态
    {
        MessageDialog msg("请确认仪器状态是否为空闲！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()==QDialog::Accepted)
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
            sensorCheckTableWidget->item(row, 3)->setText(" ");
            for (vector<SensorCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
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

SensorCheckWidget::~SensorCheckWidget()
{
    if (sensorCheckTableWidget)
        delete sensorCheckTableWidget;
}

void SensorCheckWidget::OnUserChange()
{
    if(LoginDialog::userType == Super)
    {
        for (vector<SensorCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            Uint8 row = (*it).row;

            sensorCheckTableWidget->setRowHidden(row, false);
        }
    }
    else if(LoginDialog::userType == Administrator)
    {
        for (vector<SensorCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            Uint8 row = (*it).row;

            if (Administrator >= (*it).readPrivilege)
            {
                sensorCheckTableWidget->setRowHidden(row, false);
            }
            else
            {
                sensorCheckTableWidget->setRowHidden(row, true);
            }
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        for (vector<SensorCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            Uint8 row = (*it).row;
            if (Maintain >= (*it).readPrivilege)
            {
                sensorCheckTableWidget->setRowHidden(row, false);
            }
            else
            {
                sensorCheckTableWidget->setRowHidden(row, true);
            }
        }
    }

    RowHiddenHandler::Instance()->AdjustRowHidden(sensorCheckTableWidget, m_tableInfo);
}

void SensorCheckWidget::OnFlowActivated(String name, long time)
{
    (void)name;
    (void)time;
}

void SensorCheckWidget::OnFlowFinished(String name, String result)
{
    for (vector<SensorCheckItem>::iterator it = m_items.begin(); it != m_items.end(); ++it)
    {
        if ((*it).name == name)
        {
            sensorCheckTableWidget->item((*it).row, 3)->setText(QString(result.c_str()));
            //if ("诊断中..." != result)
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

void SensorCheckWidget::OnFlowThreadExit()
{

}

void SensorCheckWidget::OnUpdateWidget(UpdateEvent event, System::String message)
{
    (void)message;
    if (event == UpdateEvent::StartSensorCheck)
    {
        FlowManager::Instance()->RegisterFinishedNotifier(this);
        FlowManager::Instance()->StartFlow();
    }
}

}
