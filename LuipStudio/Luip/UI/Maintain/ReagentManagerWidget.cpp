#include "Interface/Wqimc/WqimcManager.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "Log.h"
#include "System/Types.h"
#include "Setting/SettingManager.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "ReagentManagerWidget.h"
#include "LuaEngine/LuaEngine.h"

using namespace Configuration;
using System::Uint32;
using namespace OOLUA;
using namespace std;
using namespace Lua;
using namespace Interface::Wqimc;

namespace UI
{

ReagentManagerWidget::ReagentManagerWidget(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);

    Script *lua = LuaEngine::Instance()->GetEngine();
    Table table, itermsTable;
    itermsTable.bind_script(*lua);
    itermsTable.set_table("setting");
    itermsTable.at("ui", table);
    table.at("reagentManager", itermsTable);


    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    this->resize(650,430);

    int useRowCount = 0;
    int administratorRow = 0;
    int superRow = 0;

    itermsTable.at("rowCount", useRowCount);
    itermsTable.safe_at("superRow", superRow);
    itermsTable.at("administratorRow", administratorRow);

    ReagentManagerTableWidget = new MQtableWidget();

    ReagentManagerTableWidget->setColumnCount(6);

    m_tableInfo.userRow = useRowCount;
    m_tableInfo.hiddenRow = administratorRow + superRow; //管理员和工程师权限需隐藏行数总和
    m_tableInfo.perPageRow = 10;
    m_tableInfo.realRow = 0;
    RowHiddenHandler::Instance()->CreateRow(ReagentManagerTableWidget, m_tableInfo);

    ReagentManagerTableWidget->setFixedSize(597,400);

    QStringList columnName;
    columnName<<"耗材"<<"上次维护"<<"到期时间"<<"体积(mL)"<<"周期(月)"<<"操作";
    ReagentManagerTableWidget->setColumn(columnName);

    ReagentManagerTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度

    QFont dataFont = ReagentManagerTableWidget->font();
    dataFont.setPointSize(8);
    ReagentManagerTableWidget->setFont(dataFont);
    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        ReagentManagerTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");
    }

    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(ReagentManagerTableWidget);
    measureResultTableLayout->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *leftLayout = new QVBoxLayout();

    leftLayout->addLayout(measureResultTableLayout);
    leftLayout->addStretch();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    ReagentManagerTableWidget->setColumnWidth(0,110);
    ReagentManagerTableWidget->setColumnWidth(1,115);
    ReagentManagerTableWidget->setColumnWidth(2,115);
    //设置每行或每列的高度或宽度
    if (m_tableInfo.userRow > 10)
    {
        ReagentManagerTableWidget->setColumnWidth(3,75);
        ReagentManagerTableWidget->setColumnWidth(4,70);
    }
    else
    {
        ReagentManagerTableWidget->setColumnWidth(3,90);
        ReagentManagerTableWidget->setColumnWidth(4,85);
    }
    ReagentManagerTableWidget->setColumnWidth(5,80);

    ReagentManagerTableWidget->setFont(font);

    for(int i = 0;i < ReagentManagerTableWidget->rowCount();i++)
    {
        ReagentManagerTableWidget->setRowHeight(i, 36);
    }
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
            String text;
            ReagentManagerItem reagentManagerItem;

            itermTable.at("name", reagentManagerItem.name);

            itermTable.at("text", text);
            ReagentManagerTableWidget->item(row, 0)->setText(QString::fromUtf8(text.c_str()));//显示名称

            itermTable.at("writePrivilege", reagentManagerItem.writePrivilege);
            itermTable.at("readPrivilege", reagentManagerItem.readPrivilege);

            m_items.insert(make_pair(row, reagentManagerItem));
            row++;
        }
    }
    oolua_ipairs_end()
    itermsTable.at("changeReagent", m_changeReagentFunc);
    itermsTable.at("checkReagentDate", m_checkReagentDate);

    ConnectInit(m_items.size());
    ConfigInit();
    OnUserChange();

    connect(ReagentManagerTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotDoubleClicked(QTableWidgetItem*)));
    connect(ReagentManagerTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(CheckValue(QTableWidgetItem*)));

    m_reagentDateTimer = new QTimer();
    m_reagentDateTimer->setInterval(1000 * 3600);
    m_reagentDateTimer->start();
    connect(m_reagentDateTimer,SIGNAL(timeout()),this,SLOT(SlotReagentDateCheck()));

}

void ReagentManagerWidget::SlotReagentDateCheck()
{
    Script *lua = LuaEngine::Instance()->GetEngine();
    lua->call(m_checkReagentDate);
}

void ReagentManagerWidget::ConnectInit(Uint8 useRowCount)
{
    QFont font;                           //字体
    font.setPointSize(12);
    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i = 0; i < useRowCount; i++)
    {
        QPushButton *button = new QPushButton();
        button->setFixedSize(78,35);
        button->setFont(font);

        button->setText(tr("更换"));

        m_buttonList.append(button);
        ReagentManagerTableWidget->setCellWidget(i,5,button);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(button, i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(SlotUseMeasure(int)));
}

void ReagentManagerWidget::SlotOneKeyChangeReagent()
{
    for(int i = 0; i < (int)m_items.size(); i++ )
    {
        this->SlotUseMeasure(i);
        QThread::msleep(20);
    }
}

void ReagentManagerWidget::SlotUseMeasure(int index)
{    
    QDateTime oldDate;
    oldDate = QDateTime::currentDateTime();
    float vol = 0;
    LuaEngine *luaEngine = LuaEngine::Instance();

    if (m_items.count(index))
    {
        Script *lua = LuaEngine::Instance()->GetEngine();
        ReagentManagerItem &reagentManagerItem = m_items[index];
        String name = reagentManagerItem.name;

        luaEngine->SetLuaValue(String("config.remain." + name + ".lastTime"), oldDate.toTime_t());
        luaEngine->SetLuaValue(String("config.remain." + name + ".cycle"), ReagentManagerTableWidget->item(index, 4)->text().toInt());
        vol = ReagentManagerTableWidget->item(index, 3)->text().toFloat();
        luaEngine->SetLuaValue(String("config.remain." + name + ".total"), vol);
        lua->call(m_changeReagentFunc, name, vol);
        String strLog = "更换" + ReagentManagerTableWidget->item(index, 0)->text().toStdString();
        logger->info(strLog);
    }

    ReagentManagerTableWidget->item(index, 1)->setText(oldDate.toString("yyyy.MM.dd"));

    int cycle = ReagentManagerTableWidget->item(index,4)->text().toInt();
    QString strNewDate = oldDate.addMonths(cycle).toString("yyyy.MM.dd");
    ReagentManagerTableWidget->item(index, 2)->setText(strNewDate);

    SettingManager::Instance()->RemainSave();
    WqimcManager::Instance()->uploadReagentProfile();
}
void ReagentManagerWidget::showEvent(QShowEvent *event)
{
    (void)event;
    this->ConfigInit();
}

void ReagentManagerWidget::ConfigInit()
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();

    for (map<Uint8, ReagentManagerItem>::iterator iter = m_items.begin();iter != m_items.end(); ++iter)
    {
        Uint8 row = iter->first;
        ReagentManagerItem &reagentManagerItem = iter->second;
        String name = reagentManagerItem.name;

        float cycle;
        luaEngine->GetLuaValue(state, String("config.remain."+ name +".cycle"), cycle);

        int lastTime;
        luaEngine->GetLuaValue(state, String("config.remain."+ name +".lastTime"), lastTime);

        ReagentManagerTableWidget->item(row, 1)->setText(QDateTime::fromTime_t(lastTime).toString("yyyy.MM.dd"));
        ReagentManagerTableWidget->item(row, 2)->setText(QDateTime::fromTime_t(lastTime).addMonths(cycle).toString("yyyy.MM.dd"));

        float total;
        luaEngine->GetLuaValue(state, String("config.remain."+ name +".total"), total);
        ReagentManagerTableWidget->item(row, 3)->setText(QString::number(total));
        ReagentManagerTableWidget->item(row, 4)->setText(QString::number(cycle));
    }
}

void ReagentManagerWidget::slotDoubleClicked(QTableWidgetItem *item)
{
    int useRow = 0;
    if (m_items.size() < m_tableInfo.userRow)
    {
        useRow = m_items.size();
    }
    else
    {
        useRow = m_tableInfo.userRow;
    }
    if((item->column() == 3 || item->column() == 4) &&
        item->row() < useRow)
    {
        int curX = cursor().pos().x();
        int curY = cursor().pos().y();
        int curWidth = ReagentManagerTableWidget->columnWidth(item->column());
        int curHeight = ReagentManagerTableWidget->rowHeight(item->row());

        CNumberKeyboard *numKbd = CNumberKeyboard::Instance();
        if(true == numKbd->isHidden())
        {
            numKbd->myMove(curX, curY, curWidth, curHeight);
            numKbd->show();
        }
    }
}

void ReagentManagerWidget::SpaceInit()
{
    for(int i = 0;i < ReagentManagerTableWidget->rowCount();i++)
    {
        for(int j = 0;j < ReagentManagerTableWidget->columnCount();j++)
        {
            ReagentManagerTableWidget->setItem(i, j, new QTableWidgetItem());
            ReagentManagerTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
            if((j!= 3 && j!= 4) || (i >= m_tableInfo.userRow))
            {
                QTableWidgetItem *item = ReagentManagerTableWidget->item(i, j);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setFlags(Qt::ItemIsEnabled);
            }
        }
    }
}

void ReagentManagerWidget::CheckValue(QTableWidgetItem *item)
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State* state = luaEngine->GetThreadState();
    if (item)
    {
        if (item->column() == 3)
        {
            if (m_items.count(item->row()))
            {
                ReagentManagerItem &reagentManagerItem = m_items[item->row()];
                String name = reagentManagerItem.name;
                float total;
                luaEngine->GetLuaValue(state, String("config.remain." + name + ".total"), total);
                CheckVolumeItem(item->text(), total);
            }
        }
        else if (item->column() == 4)
        {
            if (m_items.count(item->row()))
            {
                ReagentManagerItem &reagentManagerItem = m_items[item->row()];
                String name = reagentManagerItem.name;
                float cycle;
                luaEngine->GetLuaValue(state, String("config.remain."+ name +".cycle"), cycle);
                CheckItem(item->text(), cycle);
            }
        }
    }
}

void ReagentManagerWidget::CheckVolumeItem(QString stringValue, float &volume)
{
    bool valueValid = true;
    int index = ReagentManagerTableWidget->currentRow();
    int column = ReagentManagerTableWidget->currentColumn();
    QString tmpPattern("^\\d{1,6}$");  //匹配1-6位整数
    QRegExp tmpRx(tmpPattern);

    valueValid = tmpRx.exactMatch(stringValue);
    if(false == valueValid)
    {
        QTableWidgetItem *item = ReagentManagerTableWidget->item(index, column);
        if (item)
        {
            item->setText(QString::number(volume));
        }
    }
}

void ReagentManagerWidget::CheckItem(QString stringValue, int &cycleValue)
{
    bool valueValid = true;
    int index = ReagentManagerTableWidget->currentRow();
    int column = ReagentManagerTableWidget->currentColumn();
    QString tmpPattern("^\\d{1,2}$");  //匹配1-2位整数
    QRegExp tmpRx(tmpPattern);

    valueValid = tmpRx.exactMatch(stringValue);
    if(false == valueValid)
    {
        QTableWidgetItem *item = ReagentManagerTableWidget->item(index, column);
        if (item)
        {
            item->setText(QString::number(cycleValue));
        }
    }
}

void ReagentManagerWidget::CheckItem(QString stringValue, float &cycleValue)
{
    bool valueValid = true;
    int index = ReagentManagerTableWidget->currentRow();
    int column = ReagentManagerTableWidget->currentColumn();
    QString tmpPattern("^\\d{1,2}$");  //匹配1-2位整数
    QRegExp tmpRx(tmpPattern);
    valueValid = tmpRx.exactMatch(stringValue);
    if(false == valueValid)
    {
        QTableWidgetItem *item = ReagentManagerTableWidget->item(index, column);
        if (item)
        {
            item->setText(QString::number(cycleValue));
        }
    }
}

void ReagentManagerWidget::OnUserChange()
{
    if(LoginDialog::userType == Super)
    {
        for (map<Uint8, ReagentManagerItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;

            ReagentManagerTableWidget->setRowHidden(row, false);
        }
    }
    else if(LoginDialog::userType == Administrator)
    {
        for (map<Uint8, ReagentManagerItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ReagentManagerItem &reagentManagerItem = iter->second;
            if (Administrator >= reagentManagerItem.readPrivilege)
            {
                ReagentManagerTableWidget->setRowHidden(row, false);
            }
            else
            {
                ReagentManagerTableWidget->setRowHidden(row, true);
            }
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        for (map<Uint8, ReagentManagerItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ReagentManagerItem &reagentManagerItem = iter->second;
            if (Maintain >= reagentManagerItem.readPrivilege)
            {
                ReagentManagerTableWidget->setRowHidden(row, false);
            }
            else
            {
                ReagentManagerTableWidget->setRowHidden(row, true);
            }
        }
    }

    RowHiddenHandler::Instance()->AdjustRowHidden(ReagentManagerTableWidget, m_tableInfo);
}

ReagentManagerWidget::~ReagentManagerWidget()
{

}

}
