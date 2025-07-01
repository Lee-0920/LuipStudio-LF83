#ifndef MOTORMANAGERWIDGET_H
#define MOTORMANAGERWIDGET_H
#include <QPushButton>
#include <QFont>
#include <QTableWidget>
#include <QWidget>
#include <QDebug>
#include <QList>
#include "UI/Frame/NumberKeyboard.h"
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "UI/Frame/MQtableWidget.h"
#include "System/Types.h"
#include "oolua.h"
#include "UI/Frame/LoginDialog.h"
#include "UI/Frame/RowHiddenHandler.h"

namespace UI
{
struct MotorManagerItem
{
    RoleType writePrivilege;
    RoleType readPrivilege;
    OOLUA::Lua_func_ref createFlow;
};

class MotorManagerWidget: public QWidget , public IUserChangeNotifiable
{
    Q_OBJECT
public:
    explicit MotorManagerWidget(QWidget *parent = 0);
    ~MotorManagerWidget();
    void OnUserChange();

private:
    QAbstractItemModel *standardItemModel;
    //MQtableWidget *pipeManngerTableWidget; //
    MQtableWidget *motorManngerTableWidget;
    QPushButton *stopButton;
    QWidget *rightWidget;
    //QLabel *typeLabel;
    //QComboBox *typeComboBox;
    QAbstractItemModel *model;
    QList<QPushButton *> m_buttonList;
    //std::map<System::Uint8, PipeManngerItem> m_items;
    std::map<System::Uint8, MotorManagerItem> m_items;
    OOLUA::Lua_func_ref m_checkvalueFunc;
    TableWidgetInfo m_tableInfo;

    void SpaceInit();
    void CheckItem(QTableWidgetItem* item);

private slots:
    void MeasureStart(int index);
    void ChoseDay();
    void StopMeasure();
    void CheckValue(QTableWidgetItem* item);
    //void SlotIndexChanged(int);

};

}
#endif // MOTORMANAGERWIDGET_H
