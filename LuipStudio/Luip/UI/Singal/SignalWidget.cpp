#include <QWidget>
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "LuaEngine/LuaEngine.h"
#include "SignalWidget.h"
#include "SignalManager/SignalManager.h"
#include <float.h>

using namespace OOLUA;
using namespace Lua;

namespace UI
{
SignalWidget::SignalWidget(QWidget *parent) :
        QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);
    SignalManager::Instance()->Register(this);

    Script *lua = LuaEngine::Instance()->GetEngine();
    Table table, signalItermsTable;
    signalItermsTable.bind_script(*lua);
    signalItermsTable.set_table("setting");
    signalItermsTable.at("ui", table);
    table.at("signalIterms", signalItermsTable);

    int rowCount = 0;
    signalItermsTable.at("rowCount", rowCount);

    signalTableWidget = new MQtableWidget();
    signalTableWidget->setColumnCount(4);
    if (rowCount <= 10)
    {
        signalTableWidget->setRowCount(10);////设置行数
    }
    else
    {
        signalTableWidget->setRowCount(rowCount);
    }
    signalTableWidget->setFixedSize(550,400);

    QFont headerFont;
    headerFont.setPointSize(10);

    QStringList columnName;
    columnName<<"类别"<<"名称"<<"值"<<"单位";
    signalTableWidget->setColumn(columnName);
    signalTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度
    if (rowCount > 10)
    {
        signalTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");
    }
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *leftLayout = new QVBoxLayout();

    leftLayout->addWidget(signalTableWidget, 0, Qt::AlignLeft);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->addStretch();

    mainLayout->addLayout(leftLayout);
    this->setLayout(mainLayout);

    SpaceInit();

    for(int i = 0;i < signalTableWidget->rowCount();i++)
    {
        signalTableWidget->setRowHeight(i,36);
    }

    OnUserChange();

    signalTableWidget->setColumnWidth(0,130);    //设置每行或每列的高度或宽度
    signalTableWidget->setColumnWidth(1,160);
    signalTableWidget->setColumnWidth(2,130);
    signalTableWidget->setColumnWidth(3,128);
    if (rowCount > 10)
    {
        signalTableWidget->setColumnWidth(3,98);
    }
    else
    {
        signalTableWidget->setColumnWidth(3,128);
    }

    Uint8 row = 0;

    oolua_ipairs(signalItermsTable)
    {
        Table typeTable;
        String typeText;

        lua->pull(typeTable);

        typeTable.at("text", typeText);
        signalTableWidget->item(row, 0)->setText(QString::fromUtf8(typeText.c_str()));//显示类别

        oolua_ipairs(typeTable)
        {
            Signal signal;
            Table signalTable;
            String text, unit;

            lua->pull(signalTable);
            signalTable.at("name", signal.name);
            signalTable.at("format", signal.format);

            signalTable.at("text", text);
            signalTableWidget->item(row, 1)->setText(QString::fromUtf8(text.c_str()));//显示名称

            if (signalTable.safe_at("unit", unit))
            {
                signalTableWidget->item(row, 3)->setText(QString::fromUtf8(unit.c_str()));//显示单位
            }
            m_signals.push_back(signal);
            row++;
        }
        oolua_ipairs_end()
    }
    oolua_ipairs_end()
}

void SignalWidget::SpaceInit()
{
    for(int i = 0;i < signalTableWidget->rowCount();i++)
    {
        for(int j = 0;j < signalTableWidget->columnCount();j++)
        {
            signalTableWidget->setItem(i, j, new QTableWidgetItem());
            signalTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
            QTableWidgetItem *item = signalTableWidget->item(i, j);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(Qt::ItemIsEnabled);
        }
    }
}

SignalWidget::~SignalWidget()
{

}

void SignalWidget::OnUpdateSignal()
{
    double value;
    SignalManager *signalManager = SignalManager::Instance();
    Uint8 row = 0;
    QString str;

    for (std::vector<Signal>::iterator iter = m_signals.begin(); iter != m_signals.end(); iter++)
    {
        value = signalManager->GetSignal(iter->name);
        if (value != DBL_MIN)
        {
            str.sprintf(iter->format.c_str(), value);
            signalTableWidget->item(row, 2)->setText(str);
        }
        row++;
    }
}

void SignalWidget::OnUserChange()
{
    if((LoginDialog::userType == Administrator) || (LoginDialog::userType == Super))
    {
        signalTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else
    {
         signalTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
}
