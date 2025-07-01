#include "UI/UserChangeManager/UserChangeManager.h"
#include "Log.h"
#include "UI/Frame/MessageDialog.h"
#include "UI/Frame/NumberKeyboard.h"
#include "LuaEngine/LuaEngine.h"
#include "AutoMeasureWidget.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "Setting/SettingManager.h"
#include "FlowManager/MeasureScheduler.h"

using namespace OOLUA;
using namespace std;
using namespace System;
using namespace Lua;
using namespace Configuration;
using namespace Flow;

namespace UI
{
AutoMeasureWidget::AutoMeasureWidget(System::String profileTableName, QWidget *parent):
    QWidget(parent)
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this); //向窗口更新器注册

    LuaEngine* luaEngine = LuaEngine::Instance();
    m_lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, profileTableName, itermsTable);

    String configTableName;
    int useRowCount = 0;
    int administratorRow = 0;
    int superRow = 0;

    itermsTable.at("updateEvent", m_eventName);
    itermsTable.at("rowCount", useRowCount);
        itermsTable.safe_at("superRow", superRow);
    itermsTable.at("administratorRow", administratorRow);
    itermsTable.at("isMeaParaml", m_isMeaParaml);
    itermsTable.at("defaultRestore", m_defaultRestore);
    itermsTable.at("saveFile", m_saveFile);
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
    if (m_tableInfo.userRow > 9)
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
    oolua_ipairs(itermsTable)
    {
        Table typeTable;
        String typeText;

        m_lua->pull(typeTable);

        typeTable.at("text", typeText);
        m_parameterTableWidget->item(row, 0)->setText(QString::fromUtf8(typeText.c_str()));//显示类别

        oolua_ipairs(typeTable)
        {
            Table itermTable;
            m_lua->pull(itermTable);

            int targetPrivilege = 0;
            bool isTarget = itermTable.safe_at("targetPrivilege", targetPrivilege);

            if (isTarget == false || (type & targetPrivilege))
            {
                ParameterItem parameterItem;
                String text, unit;

                itermTable.at("name", parameterItem.name);
                parameterItem.name = configTableName + parameterItem.name;
                itermTable.at("type", parameterItem.type);
                itermTable.at("writePrivilege", parameterItem.writePrivilege);
                itermTable.at("readPrivilege", parameterItem.readPrivilege);
                parameterItem.isCheckValue = itermTable.safe_at("checkValue", parameterItem.checkValueFunc);

                itermTable.at("text", text);
                m_parameterTableWidget->item(row, 1)->setText(QString::fromUtf8(text.c_str()));//显示名称

                if (itermTable.safe_at("unit", unit))
                {
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
                        comboBox->addItem(QString::fromUtf8(str.c_str()));
                    }
                    oolua_ipairs_end()
                    comboBox->setFont(comboBoxFont);
                    m_optionComboBox.insert(make_pair(parameterItem.name, comboBox));

                    m_parameterTableWidget->setCellWidget(row, 2, comboBox);
                }
                m_items.insert(make_pair(row, parameterItem));

                if (parameterItem.name == "config.scheduler.measure.timedPoint")
                {
                    m_setPointRow = row;
                }
                row++;
            }
        }
        oolua_ipairs_end()
    }
    oolua_ipairs_end()

    if(m_items.size() < m_parameterTableWidget->rowCount())
    {
        for(int index = m_items.size();index < m_parameterTableWidget->rowCount();index++)
        {
            m_parameterTableWidget->setRowHidden(index, true);
        }
    }

    ConfigInit();
    OnUserChange();
    SetPointDialogInit();

    connect(m_defaultButtom, SIGNAL(clicked(bool)), this, SLOT(DefaultRestoreSlot()));
    connect(m_parameterTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(DoubleClickedSlot(QTableWidgetItem*)));
    connect(m_confirmButtom, SIGNAL(clicked(bool)), this, SLOT(ConfigRefreshSlot()));
    connect(m_parameterTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(CheckValueSlot(QTableWidgetItem*)));
}

AutoMeasureWidget::~AutoMeasureWidget()
{

}

void AutoMeasureWidget::SpaceInit()
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

void AutoMeasureWidget::ConfigInit()          //读取配置文件的值
{
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();
    QRegExp rx;
    rx.setPattern("(\\.){0,1}0+$");

    for (map<Uint8, ParameterItem>::iterator iter = m_items.begin();iter != m_items.end(); ++iter)
    {
        Uint8 row = iter->first;
        ParameterItem &item = iter->second;
        if (LoginDialog::userType < item.readPrivilege)
        {
            continue;
        }
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
                m_parameterTableWidget->item(row, 2)->setText(QString::number(ret));
            }
            break;
            case DataType::Float:
            {
                float ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                m_parameterTableWidget->item(row, 2)->setText(QString::number(ret));
            }
            break;
            case DataType::Double:
            {
                double ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                m_parameterTableWidget->item(row, 2)->setText(QString("%1").arg(ret, 0, 'f', 6).replace(rx,""));
            }
            break;
            case DataType::IP:
            {
                String ret;
                luaEngine->GetLuaValue(state, item.name, ret);
                m_parameterTableWidget->item(row, 2)->setText(ret.c_str());
            }
            break;
            case DataType::IntArray:
            if (row == m_setPointRow)
            {
                QString thePoint = "";
                vector<bool>timedPoint =  SettingManager::Instance()->GetTimedPoint();
                int k = 0;
                for (vector<bool>::iterator iter = timedPoint.begin(); iter != timedPoint.end(); ++iter, ++k)
                {
                    if (k >= 24)
                    {
                        break;
                    }

                    if(*iter)
                    {
                        thePoint += (QString::number(k)+",");
                        timePointSet[k] = 1;
                    }
                    else
                    {
                        timePointSet[k] = 0;
                    }
                }
                QString pointDispaly="";     //限定显示长度
                if(thePoint.length() > 25)
                {
                    pointDispaly = thePoint.left(23)+"...";
                }
                else
                {
                    pointDispaly = thePoint;
                }
                m_parameterTableWidget->item(m_setPointRow, 2)->setText(pointDispaly);
            }
            break;
        }
    }
}

bool AutoMeasureWidget::IsSolidifyMeaParamFromUI()
{
    bool ret;
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();

    luaEngine->GetLuaValue(state, "config.system.solidifyMeaParamFromUI", ret);
    return ret;
}

bool AutoMeasureWidget::IsDisplayMeaParamInGeneral()
{

    bool ret;
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();

    luaEngine->GetLuaValue(state, "config.system.displayMeaParamInGeneral", ret);
    return ret;
}

void AutoMeasureWidget::OnUserChange()
{
    if(LoginDialog::userType == Super)
    {
        for (map<Uint8, ParameterItem>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            Uint8 row = iter->first;
            ParameterItem &parameterItem = iter->second;

            m_parameterTableWidget->setRowHidden(row, false);

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
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:
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
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:
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
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:
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
                case DataType::Int:case DataType::Float:case DataType::IP:case DataType::IntArray:case DataType::Double:
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

void AutoMeasureWidget::SetPointDialogInit()
{
    QPushButton * confirmButton = new QPushButton();
    confirmButton->setText("确定");
    confirmButton->setFixedSize(80,30);

    QPushButton *cancelButton = new QPushButton();
    cancelButton->setText("取消");
    cancelButton->setFixedSize(80,30);

    m_checkAllButton = new QPushButton();
    m_checkAllButton->setText("全选");
    m_checkAllButton->setFixedSize(100,30);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_checkAllButton);
    buttonLayout->setSpacing(15);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setSpacing(15);
    buttonLayout->addWidget(confirmButton);

    QVBoxLayout *DialogLayout = new QVBoxLayout();

    m_setPointDialog = new QDialog(this, Qt::WindowSystemMenuHint);
    m_setPointDialog->setWindowTitle("整点设置");
    m_setPointDialog->setFixedSize(450,250);

    m_setPointDialog->setLayout(DialogLayout);

    for(int k = 0;k < 24;k++)
    {
       m_check[k] = new QCheckBox();
       m_check[k]->setObjectName("checkboxone");
       m_check[k]->setText(QString::number(k,10));
    }

    QHBoxLayout *checkBoxLayout[4];
    for(int k = 0;k < 4;k++)
    {
        checkBoxLayout[k] = new QHBoxLayout();
    }
    for(int i = 0;i < 4;i++)
    {
        for(int j =0;j< 6;j++)
        {
            checkBoxLayout[i]->addWidget(m_check[i*6+j]);
        }
    }
    for(int k = 0;k < 4;k++)
    {
        DialogLayout->addLayout(checkBoxLayout[k]);
    }
    DialogLayout->addLayout(buttonLayout);

    connect(confirmButton, SIGNAL(clicked()), m_setPointDialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), m_setPointDialog, SLOT(close()));
    connect(m_checkAllButton, SIGNAL(clicked()), this, SLOT(OneCheckAllSlot()));
    m_setPointDialog->hide();
}

void AutoMeasureWidget::SetPointDialogShow()
{
    vector<bool>timedPoint = SettingManager::Instance()->GetTimedPoint();
    int k = 0;
    for (vector<bool>::iterator iter = timedPoint.begin(); iter != timedPoint.end(); ++iter, ++k)
    {
        if (k >= 24)
        {
            break;
        }
        if (*iter)
        {
            m_check[k]->setCheckState(Qt::Checked);
        }
        else
        {
            m_check[k]->setCheckState(Qt::Unchecked);
        }
    }
    if(m_setPointDialog->exec() == QDialog::Rejected)
    {
        int j = 0;
        for (vector<bool>::iterator iter = timedPoint.begin(); iter != timedPoint.end(); ++iter, ++j)
        {
            if (j >= 24)
            {
                break;
            }
            if (*iter)
            {
                m_check[j]->setCheckState(Qt::Checked);
            }
            else
            {
                m_check[j]->setCheckState(Qt::Unchecked);
            }
        }
    }
    QString chosenPoint = "";
    for(int k = 0;k < 24;k++)
    {
        if(m_check[k]->checkState() == Qt::Checked)
        {
            chosenPoint+= QString::number(k,10)+",";
            timePointSet[k] = 1;
        }
        else
        {
            timePointSet[k] = 0;
        }
    }
    QString pointDispaly="";
    if(chosenPoint.length() > 25)
    {
        pointDispaly = chosenPoint.left(23)+"...";
    }
    else
    {
        pointDispaly = chosenPoint;
    }
    m_parameterTableWidget->item(m_setPointRow, 2)->setText(pointDispaly);
}

void AutoMeasureWidget::OneCheckAllSlot()
{
    int flag = false;
    for(int k = 0;k < 24;k++)
    {
        if(m_check[k]->checkState() == Qt::Unchecked)
        {
            flag = true;
            break;
        }
    }
    for(int k = 0;k < 24;k++)
    {
        if(!flag)
        {
            m_check[k]->setCheckState(Qt::Unchecked);
        }
        else
        {
            m_check[k]->setCheckState(Qt::Checked);
        }
    }
}

void AutoMeasureWidget::DoubleClickedSlot(QTableWidgetItem *item)
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
        if (item->row() == m_setPointRow)
        {
            CNumberKeyboard *numKbd = CNumberKeyboard::Instance();//让键盘隐藏
            (void)numKbd;
            SetPointDialogShow();
        }
        else if (m_items.count(item->row()))
        {
            ParameterItem parameterItem = m_items[item->row()];
            if (LoginDialog::userType >= parameterItem.writePrivilege
                    && (parameterItem.type == DataType::Int || parameterItem.type == DataType::Float || parameterItem.type == DataType::Double
                        || parameterItem.type == DataType::IP))
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

void AutoMeasureWidget::CheckValueSlot(QTableWidgetItem *item)
{
    if (item->column() == 2 && m_items.count(item->row()))
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

void AutoMeasureWidget::DefaultRestoreSlot()
{
    MessageDialog msg("是否恢复默认设置？\n", this, MsgStyle::OKANDCANCEL);
    if(msg.exec()==QDialog::Rejected)
    {
        return;
    }
    m_lua->call(m_defaultRestore, LoginDialog::userType);
    this->ConfigInit();
    UpdateWidgetManager::Instance()->SlotUpdateWidget(m_eventName, "AutoMeasureWidget");

    MeasureScheduler::Instance()->Update();
}

void AutoMeasureWidget::ConfigRefreshSlot()
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
            case DataType::Int:case DataType::Float: case DataType::Double:
                ret += m_parameterTableWidget->item(row, 2)->text().toStdString();
                isSet = true;
            break;
            case DataType::IP:
                ret += "\"" + m_parameterTableWidget->item(row, 2)->text().toStdString() +  "\"" ;
                isSet = true;
            break;
            case DataType::IntArray:
            if (row == m_setPointRow)
            {
                vector<bool>timedPoint(24, 0);
                int k = 0;
                for (vector<bool>::iterator iter = timedPoint.begin(); iter != timedPoint.end(); ++iter, ++k)
                {
                    if (k >= 24)
                    {
                        break;
                    }
                    *iter = timePointSet[k];
                }
                SettingManager::Instance()->SetTimedPoint(timedPoint);
            }
            break;
        }
        if (isSet)
        {
            LuaEngine::Instance()->RunChunk(ret);
        }
    }
    m_lua->call(m_saveFile, true);
    UpdateWidgetManager::Instance()->SlotUpdateWidget(m_eventName, "AutoMeasureWidget");

    MeasureScheduler::Instance()->Update();
}

void AutoMeasureWidget::showEvent(QShowEvent *event)
{
    (void)event;
    this->ConfigInit();
}

void AutoMeasureWidget::OnUpdateWidget(UpdateEvent event, System::String message)
{
    if (event == UpdateEvent::ChangeConfigParam || event == UpdateEvent::WqimcChangeParam)
    {
        this->ConfigInit();
    }
    else if (event == UpdateEvent::ChangeAutoMeasure && message != "AutoMeasureWidget")
    {
        this->ConfigInit();

        if(message == "Modbus")
        {
            MeasureScheduler::Instance()->Update();
        }
    }
}
}
