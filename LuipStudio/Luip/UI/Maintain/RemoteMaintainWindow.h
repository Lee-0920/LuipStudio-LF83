#ifndef UI_REMOTEMAINTAINWINDOW_H
#define UI_REMOTEMAINTAINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QStackedWidget>
#include <QStringListModel>
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "MaintenanceWidget.h"
#include "ReagentManagerWidget.h"
#include "UseResourceWidget.h"
#include "PipeManngerWidget.h"
#include "MotorManagerWidget.h"
#include "HardwareCheckWidget.h"
#include "HardwareTestWidget.h"
#include "IntellectCheckWidget.h"
#include "CommunicationTestWidget.h"
#include "SensorCheckWidget.h"
#include "UpdateWidget.h"

namespace UI
{

struct RemoteMaintainWidget
{
    QWidget* remoteMaintainWidget;
    System::String text;
    System::Uint8 index;
    RoleType privilege;
};

class RemoteMainTainWindow: public QWidget, public IUserChangeNotifiable
{
    Q_OBJECT
public:
    explicit RemoteMainTainWindow(QWidget *parent = 0);
    ~RemoteMainTainWindow();

    void OnUserChange();
private:
    void ChangeBottomStatus();

private:
    QListView *listView;
    QWidget *autoWidget;
   // QGroupBox *autoMeasure;
    MaintenanceWidget *Maintenance;
    QAbstractItemModel *model;
    QStackedWidget *stackedWidget;
    ReagentManagerWidget *reagentManager;
    UseResourceWidget *userResource;
    PipeManngerWidget *pipeMannger;
    //MotorManagerWidget *motorManager;
    HardwareCheckWidget *hardwareCheck;
    HardwareTestWidget *hardwareTest;
    IntellectCheck *intellectCheck;
    CommunicationTestWidget *communicationTest;
    UpdateWidget *updateWidget;

    QList<RoleType> m_privilegeList;
    QStringList m_strList;
    std::vector<RemoteMaintainWidget> m_remoteMaintainWidget;

private:
    QWidget* CreateSubRemoteMaintainWidget(QString typeStr);

signals:
    void SignalOneKeyChangeReagentWindow();
private slots:
    void itemClicked(QModelIndex index);
};

}

#endif // REMOTEMAINTAINWINDOW_H
