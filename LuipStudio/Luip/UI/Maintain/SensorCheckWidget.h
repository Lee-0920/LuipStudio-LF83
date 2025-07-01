#ifndef SENSORCHECKWIDGET_H
#define SENSORCHECKWIDGET_H

#include <QPushButton>
#include <QFont>
#include <QTableWidget>
#include <QWidget>
#include <QDebug>
#include "UI/Frame/NumberKeyboard.h"
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "UI/Frame/MQtableWidget.h"
#include "System/Types.h"
#include "oolua.h"
#include "FlowManager/IFlowNotifiable.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/RowHiddenHandler.h"

using Flow::IFlowNotifiable;

namespace UI
{

struct SensorCheckItem
{
    System::Uint8 row;
    System::String name;
    OOLUA::Lua_func_ref func;
    RoleType writePrivilege;
    RoleType readPrivilege;
};

class SensorCheckWidget: public QWidget, public IUserChangeNotifiable, public IFlowNotifiable, public IUpdateWidgetNotifiable
{
    Q_OBJECT
public:
    explicit SensorCheckWidget(QWidget *parent = 0);
    ~SensorCheckWidget();
    void OnUserChange();
    void OnFlowActivated(String name, long time);
    void OnFlowFinished(String name, String result);
    void OnFlowThreadExit();
    void OnUpdateWidget(UpdateEvent event, System::String message);
private:
    QAbstractItemModel *standardItemModel;
    //MQtableWidget *communicationTestTableWidget;
    MQtableWidget *sensorCheckTableWidget;
    QWidget *rightWidget;
    QAbstractItemModel *model;
    QPushButton *oneCheck;
    std::map<System::Uint8, QCheckBox *>m_checkBox;
    std::vector<SensorCheckItem> m_items;
    System::String m_suitName;
    TableWidgetInfo m_tableInfo;

    void SpaceInit();
private slots:
    void SlotOneCheck();
};

}

#endif // SENSORCHECKWIDGET_H

