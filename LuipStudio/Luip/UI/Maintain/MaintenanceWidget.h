#ifndef MAINTENANCEWIDGET_H
#define MAINTENANCEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QSignalMapper>
#include <QList>
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/MQtableWidget.h"
#include "System/Types.h"
#include "oolua.h"
#include "UI/Frame/RowHiddenHandler.h"

namespace UI
{

struct MaintenanceItem
{
    System::String text;
    RoleType writePrivilege;
    RoleType readPrivilege;
    OOLUA::Lua_func_ref maintenanceFunc;
};

class MaintenanceWidget: public QWidget , public IUserChangeNotifiable
{
    Q_OBJECT
public:
    explicit MaintenanceWidget(QWidget *parent = 0);
    ~MaintenanceWidget();
    void ConfigInit();
    void OnUserChange();
private:
    MQtableWidget *maintenanceTableWidget;
    QList<QPushButton *> m_buttonList;
    QPushButton *stopButton;
    std::map<System::Uint8, MaintenanceItem> m_items;
    TableWidgetInfo m_tableInfo;

    void SpaceInit();
    void ConnectInit(System::Uint8 row);
	
private slots:
    void StopMeasure();
    void measureStart(int);

};
}




#endif // MAINTENANCEWIDGET_H
