#include "UI/UserChangeManager/UserChangeManager.h"
#include "Log.h"
#include "UI/Frame/MessageDialog.h"
#include "UI/Frame/NumberKeyboard.h"
#include "LuaEngine/LuaEngine.h"
#include "ParameterWidget.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "Setting/SettingManager.h"
#include "Lua/App.h"

using namespace OOLUA;
using namespace std;
using namespace System;
using namespace Lua;
using namespace Configuration;

namespace UI
{
ParameterWidget::ParameterWidget(System::String profileTableName, QWidget *parent):
    QWidget(parent),
    m_isConfigInit(false),m_profileTableText("")
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this); //向窗口更新器注册

    LuaEngine* luaEngine = LuaEngine::Instance();

    m_lua = luaEngine->GetEngine();
    lua_State* state = luaEngine->GetThreadState();
    Table itermsTable;
    luaEngine->GetLuaValue(state, profileTableName, itermsTable);

    String configTableName;
    int useRowCount = 0;
    int administratorRow = 0;
    int superRow = 0;

    itermsTable.at("updateEvent", m_eventName);
    itermsTable.at("rowCount", useRowCount);
    itermsTable.at("administratorRow", administratorRow);
    itermsTable.safe_at("superRow", superRow);
    itermsTable.at("isMeaParaml", m_isMeaParaml);
    itermsTable.at("defaultRestore", m_defaultRestore);
    itermsTable.at("saveFile", m_saveFile);
    itermsTable.at("text", m_profileTableText);
    itermsTable.at("name", configTableName);
    configTableName = "config." + configTableName + ".";

    this->setFixedHeight(418);
    m_parameterTableWidget = new MQtableWidget();
    m_parameterTableWidget->setColumnCount(4);

    m_tableInfo.userRow = useRowCount;
    m_tableInfo.hiddenRow = administratorRow + superRow; //管理员和工程师权限需隐藏行数总和
    m_tableInfo.perPageRow = 9;
    m_tableInfo.realRow = 0;
    RowHiddenHandler::Instance()->CreateRow(m_parameterTableWidget, m_tableInfo);

    m_parameterTableWidget->setFixedSize(570,355);

    QFont headerFont;
    headerFont.setPointSize(10);

    QTableWidgetItem *column0 = new QTableWidgetItem(tr("类别"));
    column0->setFont(headerFont);
    m_parameterTableWidget->setHorizontalHeaderItem(0, column0);

    QTableWidgetItem *column1 = new QTableWidgetItem(tr("参数项"));
    column1->setFont(headerFont);
    m_parameterTableWidget->setHorizontalHeaderItem(1, column1);

    QTableWidgetItem *column2 = new QTableWidgetItem(tr("值"));
    column2->setFont(headerFont);
    m_parameterTableWidget->setHorizontalHeaderItem(2, column2);

    QTableWidgetItem *column3 = new QTableWidgetItem(tr("单位"));
    column3->setFont(headerFont);
    m_parameterTableWidget->setHorizontalHeaderItem(3, column3);

    m_parameterTableWidget->horizontalHeader()->setFixedHeight(38); // 设置表头的高度
    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        m_parameterTableWidget->verticalScrollBar()->setStyleSheet
                ("QScrollBar{background:transparent; width: 30px;}");
    }

    QFont dataFont = m_parameterTableWidget->font();
    dataFont.setPointSize(13);
    m_parameterTableWidget->setFont(dataFont);

    SpaceInit();

    for(int i = 0;i < m_parameterTableWidget->rowCount();i++)
    {
        m_parameterTableWidget->setRowHeight(i,35);
    }

    m_parameterTableWidget->setColumnWidth(0,135);    //设置每行或每列的高度或宽度
    m_parameterTableWidget->setColumnWidth(1,185);
    m_parameterTableWidget->setColumnWidth(2,125);
    if (m_tableInfo.userRow > m_tableInfo.perPageRow)
    {
        m_parameterTableWidget->setColumnWidth(3,92);
    }
    else
    {
        m_parameterTableWidget->setColumnWidth(3,122);
    }

    m_defaultButtom = new QPushButton();
    QFont buttomFont = m_defaultButtom->font();
    buttomFont.setPointSize(14);
    m_defaultButtom->setFont(buttomFont);
    m_defaultButtom->setObjectName("brownButton");
    m_defaultButtom->setText("恢复默认");
    m_defaultButtom->setFixedSize(90,33);

    m_confirmButtom = new QPushButton();
    m_confirmButtom->setFont(buttomFont);
    m_confirmButtom->setObjectName("brownButton");
    m_confirmButtom->setText("应用");
    m_confirmButtom->setFixedSize(90,33);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_parameterTableWidget);
    leftLayout->addStretch();

    QHBoxLayout *rightLayout = new QHBoxLayout();
    rightLayout->addWidget(m_defaultButtom);
    rightLayout->addStretch(440);
    rightLayout->addWidget(m_confirmButtom);
    rightLayout->addStretch(75);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);
    this->setLayout(mainLayout);

    QFont comboBoxFont = m_parameterTableWidget->font();
    comboBoxFont.setPointSize(13);

    int type = SettingManager::Instance()->GetParamType();

    Uint8 row = 0;
    bool IsSolidifyMeaParamFromUI = this->IsSolidifyMeaParamFromUI();

    oolua_ipairs(itermsTable)
    {
        Table typeTable;
        String typeText;

        m_lua->pull(typeTable);

        typeTable.at("text", typeText);
        m_parameterTableWidget->item(row, 0)->setText(QString::fromUtf8(typeText.c_str()));//显示类别

        bool isInFactory = SettingManager::Instance()->IsProducerStatus();

        oolua_ipairs(typeTable)
        {
            Table itermTable;
            m_lua->pull(itermTable);

            int targetPrivilege = 0;
            bool isTarget = itermTable.safe_at("targetPrivilege", targetPrivilege);

            bool isFactoryCtrl = false;
            itermTable.safe_at("isFactoryCtrl", isFactoryCtrl);

            bool isUsed = true;
            if(isFactoryCtrl && !isInFactory)
            {
                isUsed = false;
            }

            bool isSolidifyFromUI = false;
            itermTable.safe_at("isSolidifyFromUI", isSolidifyFromUI);

            bool isSolidify = false;
            if(isSolidifyFromUI && IsSolidifyMeaParamFromUI)
            {
                isSolidify = true;
            }

            if ((isTarget == false || (type & targetPrivilege)) && isUsed && !isSolidify)
            {
                ParameterItem parameterItem;
                String text, unit;

                itermTable.at("name", parameterItem.name);
                parameterItem.name = configTableName + parameterItem.name;
                itermTable.at("type", parameterItem.type);
                itermTable.at("writePrivilege", parameterItem.writePrivilege);
                itermTable.at("readPrivilege", parameterItem.readPrivilege);
                parameterItem.isCheckValue = itermTable.safe_at("checkValue", parameterItem.checkValueFunc);
                parameterItem.isUnitChange = itermTable.safe_at("unitChange", parameterItem.unitChangeFunc);

                itermTable.at("text", text);
                m_parameterTableWidget->item(row, 1)->setText(QString::fromUtf8(text.c_str()));//显示名称

                if (itermTable.safe_at("unit", unit))
                {
                    if (parameterItem.isUnitChange)
                    {
                        m_lua->call(parameterItem.unitChangeFunc, unit, UnitChange::Read);
                        m_lua->pull(unit);
                    }

                    m_parameterTableWidget->item(row, 3)->setText(QString::fromUtf8(unit.c_str()));//显示单位
                }

                if (parameterItem.type == DataType::Bool)
                {
                    QComboBox *comboBox = new QComboBox();
                    comboBox->addItem("关");
                    comboBox->addItem("开");
                    comboBox->setFont(comboBoxFont);
                    m_boolComboBox.insert(make_pair(parameterItem.name, comboBox));

                    m_parameterTableWidget->setCellWidget(row, 2, comboBox);
                }
                else if (parameterItem.type == DataType::Option)
                {
                    Table optionsTable;
                    itermTable.at("options", optionsTable);
                    QComboBox *comboBox = new QComboBox();
                    oolua_ipairs(optionsTable)
                    {
                        String str;
                        m_lua->pull(str);

                        if (parameterItem.isUnitChange)
                        {
                            m_lua->call(parameterItem.unitChangeFunc, str, UnitChange::Read);
                            m_lua->pull(str);
                        }

                        comboBox->addItem(QString::fromUtf8(str.c_str()));
                    }
                    oolua_ipairs_end()
                    comboBox->setFont(comboBoxFont);
                    m_optionComboBox.insert(make_pair(parameterItem.name, comboBox));

                    m_parameterTableWidget->setCellWidget(row, 2, comboBox);
                }
                m_items.insert(make_pair(row, parameterItem));
                row++;
            }
        }
        oolua_ipairs_end()
    }
    oolua_ipairs_end()

    if(m_items.size() >= 9)
    {
        for(int i = m_items.size();i < m_tableInfo.userRow; i++)
        {
            m_parameterTableWidget->setRowHidden(i,true);
        }
    }

    ConfigInit();
    OnUserChange();

    connect(m_defaultButtom, SIGNAL(clicked(bool)), this, SLOT(DefaultRestoreSlot()));
    connect(m_parameterTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(DoubleClickedSlot(QTableWidgetItem*)));
    connect(m_confirmButtom, SIGNAL(clicked(bool)), this, SLOT(ConfigRefreshSlot()));
    connect(m_parameterTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(CheckValueSlot(QTableWidgetItem*)));
}

ParameterWidget::~ParameterWidget()
{

}

void ParameterWidget::SpaceInit()
{
    for(int i = 0;i < m_parameterTableWidget->rowCount(); i++)//行
    {
        for(int j = 0;j < m_parameterTableWidget->columnCount(); j++)//列
        {
            m_parameterTableWidget->setItem(i, j, new QTableWidgetItem());
            m_parameterTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);//设置对齐方式
            if(j != 2 || (i >= m_tableInfo.userRow))//设置不可编辑的单元格
            {
                QTableWidgetItem *item = m_parameterTableWidget->item(i, j);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setFlags(Qt::ItemIsEnabled);
            }
        }
    }
}

void ParameterWidget::ConfigInit()          //读取配置文件的值
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();
    QRegExp rx;
    rx.setPattern("(\\.){0,1}0+$");

    m_isConfigInit = true;

    for (map<Uint8, ParameterItem>::iterator iter = m_items.begin();iter != m_items.end(); ++iter)
    {
        Uint8 row = iter->first;
        ParameterItem &item = iter->second;
        /*if (LoginDialog::userType < item.readPrivilege)
        {
            continue;
        }*/
        switch(item.type)
        {
            case DataType::Bool:
            {
                if (m_boolComboBox.count(item.name))
                {
                    bool ret;
                    luaEngine->GetLuaValue(state, item.name, ret);
                    if(ret)
                    {
                        m_boolComboBox[item.name]->setCurrentIndex(1);
                    }
                    else
                    {
                        m_boolComboBox[item.name]->setCurrentIndex(0);
                    }
                }
            }
            break;
            case DataType::Option:
            {
                if (m_optionComboBox.count(item.name))
                {
                    Byte ret;
                    luaEngine->GetLuaValue(state, item.name, ret);
                    m_optionComboBox[item.name]->setCurrentIndex(ret);
                }
            }
            break;
            case DataType::Int:
            {
                int ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                if (item.isUnitChange)
                {
                    m_lua->call(item.unitChangeFunc, ret, UnitChange::Read);
                    m_lua->pull(ret);
                }
                m_parameterTableWidget->item(row, 2)->setText(QString::number(ret));
            }
            break;
            case DataType::Float:
            {
                float ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                if (item.isUnitChange)
                {
                    m_lua->call(item.unitChangeFunc, ret, UnitChange::Read);
                    m_lua->pull(ret);
                }

                m_parameterTableWidget->item(row, 2)->setText(QString::number(ret));
            }
            break;
            case DataType::Double:
            {
                double ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                if (item.isUnitChange)
                {
                    m_lua->call(item.unitChangeFunc, ret, UnitChange::Read);
                    m_lua->pull(ret);
                }

                m_parameterTableWidget->item(row, 2)->setText(QString("%1").arg(ret, 0, 'f', 9).replace(rx,""));
            }
            break;
            case DataType::IP:
            {
                String ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                m_parameterTableWidget->item(row, 2)->setText(ret.c_str());
            }
            break;
            case DataType::String:
            {
                String ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                m_parameterTableWidget->item(row, 2)->setText(ret.c_str());
            }
            break;
            case DataType::IntArray:
            break;
        }
    }

    m_isConfigInit = false;
}

bool ParameterWidget::IsSolidifyMeaParamFromUI()
{
    bool ret = false;
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();

    luaEngine->GetLuaValue(state, "config.system.solidifyMeaParamFromUI", ret);
    return ret;
}

bool ParameterWidget::IsDisplayMeaParamInGeneral()
{
    bool ret;
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();

    luaEngine->GetLuaValue(state, "config.system.displayMeaParamInGeneral", ret);
    return ret;
}

void ParameterWidget::OnUserChange()
{
    if(LoginDialog::userType == Super)
    {
        for (map<Uint8, ParameterItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ParameterItem &parameterItem = iter->second;

            m_parameterTableWidget->setRowHidden(row, false);

            #if !OEM
                if (parameterItem.name == "config.system.useQrencode")   //不是OEM版本，隐藏二维码开关
                {
                    m_parameterTableWidget->setRowHidden(row, true);
                }
            #endif

            switch(parameterItem.type)
            {
                case DataType::Bool:
                    if (m_boolComboBox.count(parameterItem.name))
                    {
                        m_boolComboBox[parameterItem.name]->setEnabled(true);
                    }
                break;
                case DataType::Option:
                    if (m_optionComboBox.count(parameterItem.name))
                    {
                        m_optionComboBox[parameterItem.name]->setEnabled(true);
                    }
                break;
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:case DataType::String:
                {
                     QTableWidgetItem *item = m_parameterTableWidget->item(row, 2);
                     item->setFlags((Qt::ItemFlags)63);
                }
                break;
            }
        }
        m_confirmButtom->setHidden(false);
        m_defaultButtom->setHidden(false);
    }
    else if(LoginDialog::userType == Administrator)
    {
        bool isEnabledChange = true;
        if (m_isMeaParaml == true && IsSolidifyMeaParamFromUI() == true)
        {
            isEnabledChange = false;
            m_confirmButtom->setHidden(true);
            m_defaultButtom->setHidden(true);
        }
        else
        {
            m_confirmButtom->setHidden(false);
            m_defaultButtom->setHidden(false);
        }

        for (map<Uint8, ParameterItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ParameterItem &parameterItem = iter->second;

            if (Administrator >= parameterItem.readPrivilege)
            {
                m_parameterTableWidget->setRowHidden(row, false);
            }
            else
            {
                m_parameterTableWidget->setRowHidden(row, true);
            }

            switch(parameterItem.type)
            {
                case DataType::Bool:
                    if (m_boolComboBox.count(parameterItem.name))
                    {
                        if (isEnabledChange == true && Administrator >= parameterItem.writePrivilege)
                        {
                            m_boolComboBox[parameterItem.name]->setEnabled(true);
                        }
                        else
                        {
                            m_boolComboBox[parameterItem.name]->setEnabled(false);
                        }
                    }
                break;
                case DataType::Option:
                    if (m_optionComboBox.count(parameterItem.name))
                    {
                        if (isEnabledChange == true && Administrator >= parameterItem.writePrivilege)
                        {
                            m_optionComboBox[parameterItem.name]->setEnabled(true);
                        }
                        else
                        {
                            m_optionComboBox[parameterItem.name]->setEnabled(false);
                        }
                    }
                break;
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:case DataType::String:
                {
                     QTableWidgetItem *item = m_parameterTableWidget->item(row, 2);
                    if (isEnabledChange == true && Administrator >= parameterItem.writePrivilege)
                    {
                        item->setFlags((Qt::ItemFlags)63);
                    }
                    else
                    {
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                        item->setFlags(Qt::ItemIsEnabled);
                    }
                }
                break;
            }
        }
    }
    else if(LoginDialog::userType == Maintain)
    {
        bool isEnabledChange = true;
        if (m_isMeaParaml == true && IsSolidifyMeaParamFromUI() == true)
        {
            isEnabledChange = false;
	        m_confirmButtom->setHidden(true);
	        m_defaultButtom->setHidden(true);
        }
        else
        {
            m_confirmButtom->setHidden(false);
            m_defaultButtom->setHidden(false);
        }

        for (map<Uint8, ParameterItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ParameterItem &parameterItem = iter->second;

            if (Maintain >= parameterItem.readPrivilege)
            {
                m_parameterTableWidget->setRowHidden(row, false);
            }
            else
            {
                m_parameterTableWidget->setRowHidden(row, true);
            }

            switch(parameterItem.type)
            {
                case DataType::Bool:
                    if (m_boolComboBox.count(parameterItem.name))
                    {
                        if (isEnabledChange == true && Maintain >= parameterItem.writePrivilege)
                        {
                            m_boolComboBox[parameterItem.name]->setEnabled(true);
                        }
                        else
                        {
                            m_boolComboBox[parameterItem.name]->setEnabled(false);
                        }
                    }
                break;
                case DataType::Option:
                    if (m_optionComboBox.count(parameterItem.name))
                    {
                        if (isEnabledChange == true && Maintain >= parameterItem.writePrivilege)
                        {
                            m_optionComboBox[parameterItem.name]->setEnabled(true);
                        }
                        else
                        {
                            m_optionComboBox[parameterItem.name]->setEnabled(false);
                        }
                    }
                break;
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:case DataType::String:
                {
                     QTableWidgetItem *item = m_parameterTableWidget->item(row, 2);
                    if (isEnabledChange == true && Maintain >= parameterItem.writePrivilege)
                    {
                        item->setFlags((Qt::ItemFlags)63);
                    }
                    else
                    {
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                        item->setFlags(Qt::ItemIsEnabled);
                    }
                }
                break;
            }
        }
    }
    else
    {
        bool isEnabledDisplay = false;
        if (m_isMeaParaml == true && IsDisplayMeaParamInGeneral() == true)
        {
            isEnabledDisplay = true;
        }
        for (map<Uint8, ParameterItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ParameterItem &parameterItem = iter->second;

            if (parameterItem.readPrivilege == Maintain && isEnabledDisplay)
            {
                m_parameterTableWidget->setRowHidden(row, false);
            }
            else
            {
                m_parameterTableWidget->setRowHidden(row, true);
            }

            switch(parameterItem.type)
            {
                case DataType::Bool:
                    if (m_boolComboBox.count(parameterItem.name))
                    {
                        m_boolComboBox[parameterItem.name]->setEnabled(false);
                    }
                break;
                case DataType::Option:
                    if (m_optionComboBox.count(parameterItem.name))
                    {
                        m_optionComboBox[parameterItem.name]->setEnabled(false);
                    }
                break;
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:case DataType::String:
                {
                    QTableWidgetItem *item = m_parameterTableWidget->item(row, 2);
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    item->setFlags(Qt::ItemIsEnabled);
                }
                break;
            }
        }
        m_confirmButtom->setHidden(true);
        m_defaultButtom->setHidden(true);
    }

    RowHiddenHandler::Instance()->AdjustRowHidden(m_parameterTableWidget, m_tableInfo);
}

void ParameterWidget::DoubleClickedSlot(QTableWidgetItem *item)
{
    bool isEnabledChange = true;
    if (m_isMeaParaml == true && IsSolidifyMeaParamFromUI() == true)
    {
        isEnabledChange = false;
    }
    if(item->column() == 2
        && item->row() < m_tableInfo.userRow
        && (LoginDialog::userType == Super
        || (LoginDialog::userType == Administrator && isEnabledChange)
        || (LoginDialog::userType == Maintain && isEnabledChange)))
    {
        if (m_items.count(item->row()))
        {
            ParameterItem parameterItem = m_items[item->row()];
            if (LoginDialog::userType >= parameterItem.writePrivilege
                    && (parameterItem.type == DataType::Int || parameterItem.type == DataType::Float || parameterItem.type == DataType::Double
                        || parameterItem.type == DataType::IP || parameterItem.type == DataType::String))
            {
                int curX = cursor().pos().x();
                int curY = cursor().pos().y();
                int curWidth = m_parameterTableWidget->columnWidth(item->column());
                int curHeight = m_parameterTableWidget->rowHeight(item->row());

                CNumberKeyboard *numKbd = CNumberKeyboard::Instance();

                if(true == numKbd->isHidden())
                {
                    numKbd->myMove(curX, curY, curWidth, curHeight);
                    numKbd->show();
                }
            }
        }
    }
}

void ParameterWidget::CheckValueSlot(QTableWidgetItem *item)
{
    if (!m_isConfigInit && item->column() == 2 && m_items.count(item->row()))
    {
        ParameterItem &parameterItem = m_items[item->row()];
        if (parameterItem.type == DataType::IP)
        {
            QString tmpPattern("^([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])$");  //检查数字是否合法
            QRegExp tmpRx(tmpPattern);
            if(false == tmpRx.exactMatch(item->text()))
            {
                String ret;
                LuaEngine* luaEngine = LuaEngine::Instance();
                lua_State * state = luaEngine->GetThreadState();

                luaEngine->GetLuaValue(state, parameterItem.name, ret);
                item->setText(ret.c_str());
            }
        }
        else if (parameterItem.isCheckValue)
        {
            String ret =  item->text().toStdString();
            m_lua->call(parameterItem.checkValueFunc, ret);
            m_lua->pull(ret);
            item->setText(ret.c_str());
        }
    }
}

void ParameterWidget::DefaultRestoreSlot()
{
    MessageDialog msg("是否恢复默认设置？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }
    m_lua->call(m_defaultRestore, LoginDialog::userType);

    bool isRestart = false;
    m_lua->pull(isRestart);

    this->ConfigInit();
    UpdateWidgetManager::Instance()->SlotUpdateWidget(m_eventName, "ParameterWidget");

    if(isRestart == true)
    {
        QString text = QString::fromStdString(m_profileTableText) + "更改，重启仪器后生效，是否立即重启？\n";
        MessageDialog msg(text, 10, this, MsgStyle::ONLYCANCEL);
        if(msg.exec()==QDialog::Rejected)
        {
            return;
        }
    }
}

void ParameterWidget::ConfigRefreshSlot()
{
    MessageDialog msg("是否确认更改设置？\n", this,MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }

    for (map<Uint8, ParameterItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        Uint8 row = iter->first;
        ParameterItem &item = iter->second;
        if (LoginDialog::userType < item.readPrivilege)
        {
            continue;
        }
        String ret = item.name + " = ";
        bool isSet = false;
        switch(item.type)
        {
            case DataType::Bool:
                if (m_boolComboBox.count(item.name))
                {
                    if (m_boolComboBox[item.name]->currentIndex())
                    {
                        ret += "true";
                    }
                    else
                    {
                        ret += "false";
                    }
                    isSet = true;
                }
            break;
            case DataType::Option:
                if (m_optionComboBox.count(item.name))
                {
                    Byte number = m_optionComboBox[item.name]->currentIndex();
                    ret += QString::number(number).toStdString();
                    isSet = true;
                }
            break;
            case DataType::Int:
                if (item.isUnitChange)
                {
                    int data = m_parameterTableWidget->item(row, 2)->text().toInt();
                    m_lua->call(item.unitChangeFunc, data, UnitChange::Write);
                    m_lua->pull(data);
                    ret += QString::number(data).toStdString();
                }
                else
                {
                    ret += m_parameterTableWidget->item(row, 2)->text().toStdString();
                }
                isSet = true;
            break;
            case DataType::Float:

                if (item.isUnitChange)
                {
                    float data = m_parameterTableWidget->item(row, 2)->text().toFloat();
                    m_lua->call(item.unitChangeFunc, data, UnitChange::Write);
                    m_lua->pull(data);
                    ret += QString::number(data).toStdString();
                }
                else
                {
                    ret += m_parameterTableWidget->item(row, 2)->text().toStdString();
                }
                isSet = true;
            break;
            case DataType::Double:
                if (item.isUnitChange)
                {
                    double data = m_parameterTableWidget->item(row, 2)->text().toDouble();
                    m_lua->call(item.unitChangeFunc, data, UnitChange::Write);
                    m_lua->pull(data);
                    ret += QString::number(data, 'f', 9).toStdString();
                }
                else
                {
                    ret += m_parameterTableWidget->item(row, 2)->text().toStdString();
                }
                isSet = true;
            break;
            case DataType::IP:
                ret += "\"" + m_parameterTableWidget->item(row, 2)->text().toStdString() +  "\"" ;
                isSet = true;
            break;
            case DataType::String:
                ret += "\"" + m_parameterTableWidget->item(row, 2)->text().toStdString() +  "\"" ;
                isSet = true;
            break;
            case DataType::IntArray:
            break;
        }
        if (isSet)
        {
            LuaEngine::Instance()->RunChunk(ret);
        }
    }
    m_lua->call(m_saveFile, true);

    bool isRestart = false;
    m_lua->pull(isRestart);

    UpdateWidgetManager::Instance()->SlotUpdateWidget(m_eventName, "ParameterWidget");

    if(isRestart == true)
    {
        QString text = QString::fromStdString(m_profileTableText) + "更改，重启仪器后生效，是否立即重启？\n";
        MessageDialog msg(text, 10, this, MsgStyle::ONLYCANCEL);
        if(msg.exec()==QDialog::Rejected)
        {
            return;
        }
    }
}

void ParameterWidget::showEvent(QShowEvent *event)
{
    (void)event;
    this->ConfigInit();
}

void ParameterWidget::OnUpdateWidget(UpdateEvent event, System::String message)
{
    if (event == UpdateEvent::ChangeConfigParam || event == UpdateEvent::WqimcChangeParam)
    {
        this->ConfigInit();
    }
    else if (event == UpdateEvent::ChangeInterconnectionParam && m_eventName == UpdateEvent::ChangeInterconnectionParam && message != "ParameterWidget")
    {
        this->ConfigInit();
    }
    else if (event == UpdateEvent::ChangeMeasureParam && m_eventName == UpdateEvent::ChangeMeasureParam && message == "CalibrateCurve")
    {
        this->ConfigInit();
    }
    else if (event == UpdateEvent::NewCalibrateCurve && m_eventName == UpdateEvent::ChangeMeasureParam)
    {
        this->ConfigInit();
    }
    else if (event == UpdateEvent::ModbusChangeParam && m_eventName == UpdateEvent::ChangeMeasureParam)
    {
        this->ConfigInit();
    }
}
}
