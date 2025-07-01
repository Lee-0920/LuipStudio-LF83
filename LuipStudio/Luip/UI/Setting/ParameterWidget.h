#ifndef UI_SETTING_PARAMETERWIDGET_H
#define UI_SETTING_PARAMETERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QShowEvent>
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "UI/Frame/MQtableWidget.h"
#include "System/Types.h"
#include "UI/Frame/LoginDialog.h"
#include "oolua.h"
#include "UI/Frame/IUpdateWidgetNotifiable.h"
#include "UI/Frame/RowHiddenHandler.h"

namespace UI
{

enum class DataType
{
    Bool = 0,                //1个字节，bool类型, 下拉列表，只有开和关两个选项
    Option = 1,              //1个字节, Byte类型, 下拉列表，选项由脚本提供
    Int = 2,           //4个字节，int 类型
    Float = 3,         //4个字节, float 类型
    IP = 4,                  //String 类型
    IntArray = 5,            //4 * len 个字节，int 类型数组
    Double = 9,
    String = 10,
};

struct ParameterItem
{
    System::String name;
    DataType type;
    RoleType writePrivilege;
    RoleType readPrivilege;
    OOLUA::Lua_func_ref checkValueFunc;
    bool isCheckValue;
    OOLUA::Lua_func_ref unitChangeFunc;
    bool isUnitChange;
};

class ParameterWidget: public QWidget, public IUserChangeNotifiable, public IUpdateWidgetNotifiable
{
    Q_OBJECT
public:
    explicit ParameterWidget(System::String profileTableName, QWidget *parent = 0);
    ~ParameterWidget();

   void OnUserChange();
   void OnUpdateWidget(UpdateEvent event, System::String message);

protected:
    void showEvent(QShowEvent *event);

private:
    MQtableWidget *m_parameterTableWidget;
    System::String m_profileTableText;
    QPushButton *m_defaultButtom;
    QPushButton *m_confirmButtom;
    std::map<System::String, QComboBox*> m_boolComboBox;
    std::map<System::String, QComboBox*> m_optionComboBox;
    std::map<System::Uint8, ParameterItem> m_items;
    OOLUA::Script *m_lua;
    bool m_isMeaParaml;
    OOLUA::Lua_func_ref m_defaultRestore;
    OOLUA::Lua_func_ref m_saveFile;
    UpdateEvent m_eventName;
    TableWidgetInfo m_tableInfo;

    bool m_isConfigInit;

    void ConfigInit();
    void SpaceInit();
    bool IsSolidifyMeaParamFromUI();
    bool IsDisplayMeaParamInGeneral();

private slots:
    void DefaultRestoreSlot();
    void ConfigRefreshSlot();
    void DoubleClickedSlot(QTableWidgetItem *item);
    void CheckValueSlot(QTableWidgetItem *item);
};
}
#endif // UI_SETTING_PARAMETERWIDGET_H
