#include "UI/Frame/LoginDialog.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/NumberKeyboard.h"
#include "RemoteMaintainWindow.h"
#include "LuaEngine/LuaEngine.h"

using namespace Lua;
using namespace OOLUA;

namespace UI
{

RemoteMainTainWindow::RemoteMainTainWindow(QWidget *parent) : QWidget(parent), reagentManager(nullptr)
{
    MaintainUserChange::Instance()->Register(this);

    listView = new QListView();
    listView->setFixedHeight(402);
    listView->setFixedWidth(101);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout *listViewLayout = new QVBoxLayout();
    listViewLayout->addWidget(listView);
    listViewLayout->setContentsMargins(0, 11, 0, 0);
    listViewLayout->addStretch();

    stackedWidget = new QStackedWidget();
    stackedWidget->setFixedHeight(425);
    stackedWidget->setFixedWidth(650);
    stackedWidget->setObjectName(QStringLiteral("stackedWidget"));

    QHBoxLayout *stackedWidgetLayout = new QHBoxLayout();
    stackedWidgetLayout->addWidget(stackedWidget);
    stackedWidgetLayout->addStretch();
    stackedWidgetLayout->setContentsMargins(10, 0, 0, 0);

    QVBoxLayout *vstackedWidgetLayout = new QVBoxLayout();
    vstackedWidgetLayout->addLayout(stackedWidgetLayout);
    vstackedWidgetLayout->addStretch();
    vstackedWidgetLayout->setContentsMargins(0, 3, 0, 0);

    LuaEngine* luaEngine = LuaEngine::Instance();
    Script* lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.remoteMaintain", itermsTable);

    oolua_pairs(itermsTable)
    {
        Table itermTable;
        lua->pull(itermTable);

        String name;
        itermTable.at("name", name);

        RemoteMaintainWidget subWidget;
        subWidget.remoteMaintainWidget = CreateSubRemoteMaintainWidget(QString::fromStdString(name));

        itermTable.at("readPrivilege", subWidget.privilege);
        itermTable.at("text", subWidget.text);

        m_remoteMaintainWidget.push_back(subWidget);

        String text;
        itermTable.at("text", text);

        m_strList.push_back(QString::fromStdString(text));
		
        m_privilegeList.push_back(subWidget.privilege);
    }
    oolua_pairs_end()

    model = new QStringListModel(m_strList);
    listView->setGridSize(QSize(50,40));
    listView->setModel(model);

    QFont listFont = listView->font();
    listFont.setPointSize(12);
    listView->setFont(listFont);

    std::vector<RemoteMaintainWidget>::iterator it;
    for(it = m_remoteMaintainWidget.begin(); it != m_remoteMaintainWidget.end(); it++)
    {
        stackedWidget->addWidget((*it).remoteMaintainWidget);
    }

    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
    QHBoxLayout *mainlayout = new QHBoxLayout();

    mainlayout->addLayout(listViewLayout);
    mainlayout->addLayout(vstackedWidgetLayout);
    mainlayout->setContentsMargins(10, 5, 0, 0);

    this->setLayout(mainlayout);

    ChangeBottomStatus();

    if(reagentManager != nullptr)
    {
        connect(this,SIGNAL(SignalOneKeyChangeReagentWindow()), reagentManager, SLOT(SlotOneKeyChangeReagent()));
    }

}

QWidget* RemoteMainTainWindow::CreateSubRemoteMaintainWidget(QString typeStr)
{
    QWidget* subWidget;

    if(typeStr == "Maintenance")
    {
        subWidget = new MaintenanceWidget();
    }
    else if(typeStr == "reagentManager")
    {
        subWidget = new ReagentManagerWidget();

        connect(this,SIGNAL(SignalOneKeyChangeReagentWindow()), (ReagentManagerWidget*)(subWidget), SLOT(SlotOneKeyChangeReagent()));
    }
    else if(typeStr == "userResource")
    {
        subWidget = new UseResourceWidget();
    }
    else if(typeStr == "pipeMannger")
    {
        subWidget = new PipeManngerWidget();
    }
    else if(typeStr == "motorManager")
    {
        subWidget = new MotorManagerWidget();
    }
    else if(typeStr == "hardwareCheck")
    {
        subWidget = new HardwareCheckWidget();
    }
    else if(typeStr == "intellectCheck")
    {
        subWidget = new IntellectCheck();
    }
    else if(typeStr == "communicationTest")
    {
        subWidget = new CommunicationTestWidget();
    }
    else if(typeStr == "sensorCheck")
    {
        subWidget = new SensorCheckWidget();
    }
    else if(typeStr == "hardwareTest")
    {
        subWidget = new HardwareTestWidget();
    }
    else if(typeStr == "updateWidget")
    {
        subWidget = new UpdateWidget();
    }

    return subWidget;
}

void RemoteMainTainWindow::itemClicked(QModelIndex index)
{
    int row = index.row();

    if(row >= 0 && row < m_remoteMaintainWidget.size())
    {
        stackedWidget->setCurrentWidget(m_remoteMaintainWidget.at(row).remoteMaintainWidget);
    }

    CNumberKeyboard::Instance()->hide();
}

RemoteMainTainWindow::~RemoteMainTainWindow()
{
    std::vector<RemoteMaintainWidget>::iterator it;
    for(it = m_remoteMaintainWidget.begin(); it != m_remoteMaintainWidget.end(); it++)
    {
        delete (*it).remoteMaintainWidget;
    }
}

void RemoteMainTainWindow::OnUserChange()
{
    ChangeBottomStatus();
}

void RemoteMainTainWindow::ChangeBottomStatus()
{
    if(LoginDialog::userType == Super)
    {
        stackedWidget->setCurrentWidget(m_remoteMaintainWidget.at(0).remoteMaintainWidget);
        QModelIndex index = model->index(0,0);
        listView->setCurrentIndex(index);

        for (int i = 0; i < m_strList.size(); i++)
        {
            listView->setRowHidden(i, false);
        }
    }
    else if(LoginDialog::userType == Administrator)
    {
        stackedWidget->setCurrentWidget(m_remoteMaintainWidget.at(0).remoteMaintainWidget);
        QModelIndex index = model->index(0,0);
        listView->setCurrentIndex(index);

        for (int i = 0; i < m_strList.size(); i++)
        {
            if(m_privilegeList.at(i)> Administrator)
            {
                listView->setRowHidden(i, true);
            }
            else
            {
                listView->setRowHidden(i, false);
            }
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        stackedWidget->setCurrentWidget(m_remoteMaintainWidget.at(0).remoteMaintainWidget);
        QModelIndex index = model->index(0,0);
        listView->setCurrentIndex(index);

        for (int i = 0; i < m_strList.size(); i++)
        {
            if(m_privilegeList.at(i)> Maintain)
            {
                listView->setRowHidden(i, true);
            }
            else
            {
                listView->setRowHidden(i, false);
            }
        }
    }
    else if(LoginDialog::userType == General)
    {
        for (int i = 0; i < m_strList.size(); i++)
        {
            listView->setRowHidden(i, true);
        }
    }
}

}

