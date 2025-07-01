#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>
#include "CalibrateCurve.h"
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include <algorithm>
#include "UI/Frame/MessageDialog.h"
#include "Log.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "RecordDataPlugin/RecordDataProxy.h"
#include "Common.h"
#include "Lua/App.h"
#include "LuaException.h"

using namespace Result;
using namespace OOLUA;
using namespace std;
using namespace Lua;
using namespace ResultData;
using namespace ResultDetail;

namespace UI
{

CalibrateCurve::CalibrateCurve(System::String resultDataname, System::String profileTableName, System::String profileFileName, String curveParamName, QWidget *parent) :
        QWidget(parent),m_dateTimeStr("dateTime"),m_curveBStr("curveB"), m_curveKStr("curveK"),m_resultDataname(resultDataname),
        m_profileTableName(profileTableName),m_profileFileName(profileFileName),m_curveParamName(curveParamName),m_isDisplayHiddenInfo(false)
{
    MaintainUserChange::Instance()->Register(this);
    ResultManager::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);

    nextFlag = 0;
    backFlag = 0;

    this->resize(650,400);                            //字体
    font.setPointSize(12);
	
    m_resultDetailFiles = ResultManager::Instance()->GetResultDetailUseCalibrateRecordFile(m_resultDataname);

    m_resultFiles = ResultManager::Instance()->GetCalibrateRecordFile(m_resultDataname);
	m_resultFields = m_resultFiles->GetRecordFields();

    m_resultDetailWidget = ResultDetailManager::Instance()->GetCalibrateResultDetailWidget();

    RecordField recordField;

    m_resultFields->GetField(m_dateTimeStr, recordField);
    m_dateTimeStart = recordField.start;

    m_resultFields->GetField(m_curveKStr, recordField);
    m_curveKStart = recordField.start;

    m_resultFields->GetField(m_curveBStr, recordField);
    m_curveBStart = recordField.start;

    LuaEngine* luaEngine = LuaEngine::Instance();
    m_lua = luaEngine->GetEngine();
    lua_State * state = luaEngine->GetThreadState();

    Table fieldTable;
    luaEngine->GetLuaValue(state, "setting.ui.calibrateCurve", fieldTable);

    m_isUnitChange = fieldTable.safe_at("unitChange", m_unitChangeFunc);

    fieldTable.at("defaultRestore", m_defaultRestore);
    fieldTable.at("saveFile", m_saveFile);
    m_isDisplayExtraInfo = fieldTable.safe_at("getExtraInfo", m_getExtraInfo);
    m_isExtraToMap = fieldTable.safe_at("extraToMap", m_extraToMap);
    fieldTable.at("toInput", m_toInput);

    string strType;
    fieldTable.at("curveType", strType);
    m_curveType = QString(strType.c_str());

    minTime = new QMyEdit();            //时间检索最小值
    minTime->setFixedHeight(32);
    minTime->setFixedWidth(115);
    minTime->setText(GetMinTime());
    minTime->setFont(font);

    toTime = new QLabel();               //时间检索"-"
    toTime->setText("-");
    toTime->setFixedSize(5,30);
    toTime->setFont(font);

    maxTime = new QMyEdit();          //时间检索最大值
    maxTime->setFixedHeight(32);
    maxTime->setFixedWidth(115);
    maxTime->setText(GetMaxTime());
    maxTime->setFont(font);

    toMapButton = new QPushButton();           //设为当前布局
    toMapButton->setObjectName("brownButton");
    toMapButton->setText("设为当前");
    toMapButton->setFont(font);
    toMapButton->setFixedSize(80,40);

    inputButton = new QPushButton();
    inputButton->setObjectName("brownButton");
    inputButton->setText("输入");
    inputButton->setFont(font);
    inputButton->setFixedSize(80,40);

    searchButton = new QPushButton();
    searchButton->setObjectName("brownButton");
    searchButton->setText("检索");
    searchButton->setFont(font);
    searchButton->setFixedSize(80,40);

    QHBoxLayout *leftBottomLayout = new QHBoxLayout();
    leftBottomLayout->addWidget(minTime);
    leftBottomLayout->addWidget(toTime);
    leftBottomLayout->addWidget(maxTime);
    leftBottomLayout->addStretch();
    leftBottomLayout->addWidget(toMapButton);
    leftBottomLayout->addWidget(inputButton);
    leftBottomLayout->addWidget(searchButton);
    leftBottomLayout->setContentsMargins(0, 5, 0, 0);

    int columnCount = 0;
    fieldTable.at("columnCount", columnCount);

    calibrateTableWidget = new MQtableWidget();
    calibrateTableWidget->setColumnCount(columnCount);
    calibrateTableWidget->setRowCount(10);
    calibrateTableWidget->setFixedSize(550,335);

    Table table;
    int col = 0;
    oolua_ipairs(fieldTable)
    {
            Table table;
            m_lua->pull(table);

            bool isNeedTarget = false;
            String targetStr;
            isNeedTarget = table.safe_at("targetStr", targetStr);
            QStringList targetList = QString::fromStdString(targetStr).split('+');

            if(!isNeedTarget || (isNeedTarget && FindStringInList(targetList, QString::fromStdString(m_resultDataname))))
            {
                bool isHiddenInfo;
                ShowField showField;
                String text;
                String format;
                int width;

                table.at("name", showField.name);

                table.at("text", text);
                m_columnName << QString::fromUtf8(text.c_str());

                if(table.safe_at("isHiddenInfo", isHiddenInfo))
                {
                    m_items.insert(make_pair(col, isHiddenInfo));
                }
                else
                {
                    m_items.insert(make_pair(col, false));
                }

                table.at("width", width);
                calibrateTableWidget->setColumnWidth(col++, width);

                if (table.safe_at("format", format))
                {
                    showField.format = format.c_str();
                }

                m_showFields.push_back(showField);
            }
    }
    oolua_ipairs_end()

    //表头
    calibrateTableWidget->setColumn(m_columnName);
    if(m_showFields.size() != columnCount)
    {
        calibrateTableWidget->setColumnCount(m_showFields.size());
    }

    calibrateTableWidget->horizontalHeader()->setFixedHeight(42); // 设置表头的高度

    QFont dataFont = calibrateTableWidget->font();
    dataFont.setPointSize(8);
    calibrateTableWidget->setFont(dataFont);
    calibrateTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑
    calibrateTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置表格为整行选择
    calibrateTableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{height:18px;}");
    calibrateTableWidget->horizontalScrollBar()->setVisible(false);
    calibrateTableWidget->horizontalScrollBar()->setDisabled(true);
    calibrateTableWidget->verticalScrollBar()->setVisible(false);
    calibrateTableWidget->verticalScrollBar()->setDisabled(true);

    QHBoxLayout *measureResultTableLayout = new QHBoxLayout();
    measureResultTableLayout->addWidget(calibrateTableWidget);

    for(int i =0;i<10;i++)
    {
        calibrateTableWidget->setRowHeight(i,29);
    }

    calibrateTableWidget->setFont(font);

    currentCurve = new QLabel();
    currentCurve->setFixedHeight(20);

    m_ExtraInfoLabel = new QLabel();
    m_ExtraInfoLabel->setFixedHeight(20);

    QHBoxLayout *leftMiddleLayout = new QHBoxLayout();   //左边中间布局
    leftMiddleLayout->addWidget(currentCurve);
    leftMiddleLayout->addWidget(m_ExtraInfoLabel);
    leftMiddleLayout->setSpacing(30);

    currentCurve->setFont(font);

    double curveK, curveB;
    luaEngine->GetLuaValue(state, String(m_curveParamName + ".curveK"), curveK);
    luaEngine->GetLuaValue(state, String(m_curveParamName + ".curveB"), curveB);

    this->DisplayCurrentCurve(curveK, curveB);//显示当前标线
    this->DisplayExtraInfo();//显示当前标点浓度

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(measureResultTableLayout);
    leftLayout->addStretch();
    leftLayout->addLayout(leftMiddleLayout);
    leftLayout->addLayout(leftBottomLayout);
    leftLayout->addStretch();
    leftLayout->setContentsMargins(0, 0, 0, 0);

    toTopButton = new QPushButton();
    toTopButton->setObjectName("brownButton");
    toTopButton->setText("|<");
    toTopButton->setFont(font);
    toTopButton->setFixedSize(80,40);

    toBackButton = new QPushButton();
    toBackButton->setObjectName("brownButton");
    toBackButton->setText("<<");
    toBackButton->setFont(font);
    toBackButton->setFixedSize(80,40);

    toNextButton = new QPushButton();
    toNextButton->setObjectName("brownButton");
    toNextButton->setText(">>");
    toNextButton->setFont(font);
    toNextButton->setFixedSize(80,40);

    toBottomButton = new QPushButton();
    toBottomButton->setObjectName("brownButton");
    toBottomButton->setText(">|");
    toBottomButton->setFont(font);
    toBottomButton->setFixedSize(80,40);

    exportButton = new QPushButton();
    exportButton->setObjectName("brownButton");
    exportButton->setText("导出");
    exportButton->setFont(font);
    exportButton->setFixedSize(80,40);

    clearButton = new QPushButton();
    clearButton->setObjectName("brownButton");
    clearButton->setText("清空");
    clearButton->setFont(font);
    clearButton->setFixedSize(80,40);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(toTopButton);
    rightLayout->addWidget(toBackButton);
    rightLayout->addWidget(toNextButton);
    rightLayout->addWidget(toBottomButton);
    rightLayout->addWidget(exportButton);
    rightLayout->addWidget(clearButton);
    if(m_resultDetailWidget != nullptr)
    {
        detailButton = new QPushButton();
        detailButton->setObjectName("brownButton");
        detailButton->setText("详情");
        detailButton->setFont(font);
        detailButton->setFixedSize(80,40);
        rightLayout->addWidget(detailButton);
        connect(detailButton,SIGNAL(clicked()), this, SLOT(SlotDetailButton()));
    }
    rightLayout->addStretch();
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);

    this->setLayout(mainLayout);

    minDayCaledar = new QCalendarWidget(this);
    maxDayCaledar = new QCalendarWidget(this);
    minDayCaledar->hide();
    maxDayCaledar->hide();

    connect(toTopButton, SIGNAL(clicked(bool)), this, SLOT(ToTop()));
    connect(toBottomButton, SIGNAL(clicked(bool)), this, SLOT(ToBottom()));
    connect(toNextButton, SIGNAL(clicked(bool)), this, SLOT(ToNext()));
    connect(toBackButton, SIGNAL(clicked(bool)), this, SLOT(ToBack()));
    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(SearchResult()));
    connect(exportButton,SIGNAL(clicked()), this, SLOT(SlotExportButton()));
    connect(clearButton,SIGNAL(clicked()), this, SLOT(SlotClearButton()));


    connect(toMapButton, SIGNAL(clicked(bool)), this, SLOT(ToMap()));
    connect(inputButton, SIGNAL(clicked(bool)), this, SLOT(ToInput()));
    connect(minTime, SIGNAL(LineEditClicked()), this, SLOT(ChoseMinDay()));
    connect(maxTime, SIGNAL(LineEditClicked()), this, SLOT(ChoseMaxDay()));
    connect(minDayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMinDay()));
    connect(maxDayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMaxDay()));
    ChangeBottomStatus();
    ReadResult();

    fieldTable.safe_at("getHiddenInfo", m_getHiddenInfo);
    if (m_getHiddenInfo.valid())
    {
        bool ret;
        m_lua->call(m_getHiddenInfo);
        m_lua->pull(ret);
        m_isDisplayHiddenInfo = ret;
        this->DisplayHiddenInfo();
    }
}

bool CalibrateCurve::FindStringInList(QStringList srcList, QString dstStr)
{
    bool ret = false;
    QStringListIterator it(srcList);
    while(it.hasNext())
    {
        if(dstStr == it.next())
        {
            ret = true;
            break;
        }
    }

    return ret;
}

void CalibrateCurve::SlotDetailButton()
{
    int row = calibrateTableWidget->currentIndex().row();  //获取当前的行
    if(row!=-1)
    {
        Int64 pos = m_resultDetailFiles->GetReadPos();
        if (m_resultDetailFiles->MovePrevious(row + 1))
        {
            RecordData recordData = m_resultDetailFiles->GetCurrentRecord();

            m_resultDetailFiles->SetReadPos(pos);

            if (m_resultDetailWidget != nullptr)
            {
                m_resultDetailWidget->SetRecordData(recordData);
                m_resultDetailWidget->UpdateRecord();
                m_resultDetailWidget->Show(this);
            }
        }
    }
}

void CalibrateCurve::ToMap()
{
    try
    {
        int j = calibrateTableWidget->currentIndex().row();  //获取当前的行
        if(j!=-1)
        {
            QString currentCalibrate = calibrateTableWidget->item(j ,1)->text();
            if (!currentCalibrate.isEmpty())
            {
                currentCurve->setText("当前标线: " + m_curveType + " = " + currentCalibrate);

                m_currentCurveKValue = m_currentCurveK[j];
                m_currentCurveBValue = m_currentCurveB[j];

                QString curveKStr = QString::number(m_currentCurveKValue,'f',9);
                LuaEngine::Instance()->RunChunk(String(m_curveParamName + ".curveK = " + curveKStr.toStdString()));

                QString curveBStr = QString::number(m_currentCurveBValue,'f',9);
                LuaEngine::Instance()->RunChunk(String(m_curveParamName + ".curveB = " + curveBStr.toStdString()));

                m_lua->call(m_saveFile, this->m_profileTableName, this->m_profileFileName, true);


                UpdateWidgetManager::Instance()->SlotUpdateWidget(UpdateEvent::ChangeMeasureParam, "CalibrateCurve");

                if (m_isExtraToMap)
                {
                    Int64 pos = m_resultDetailFiles->GetReadPos();
                    if (m_resultDetailFiles->MovePrevious(j + 1))
                    {
                        RecordData recordData = m_resultDetailFiles->GetCurrentRecord();
                        m_resultDetailFiles->SetReadPos(pos);
                        m_lua->call(m_extraToMap, &recordData, this->m_profileTableName, this->m_curveParamName, this->m_profileFileName);
                    }
                }
            }
        }
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("CalibrateCurve::ToMap => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("CalibrateCurve::ToMap => %s", e.what());
    }
}

void CalibrateCurve::ShowRow(Uint16 row)
{
    Uint16 column = 0;
    for (vector<ShowField>::iterator iter = m_showFields.begin(); iter != m_showFields.end(); ++iter)
    {
        if (column == 1 && iter->name == "curve")
        {
            double curveK, curveB;
            m_resultFiles->GetFieldCurrentRecordValue(m_curveKStr, curveK);
            m_resultFiles->GetFieldCurrentRecordValue(m_curveBStr, curveB);

            m_currentCurveK[row] = curveK;
            m_currentCurveB[row] = curveB;

            if (m_isUnitChange)
            {
                m_lua->call(m_unitChangeFunc, curveK, UnitChange::Read);
                m_lua->pull(curveK);
            }

            QString curveKString;
            QString curveBString;

            if (iter->format.empty())
            {
                curveKString = QString::number(curveK,'f',9);
                curveBString = QString::number(fabs(curveB),'f',9);
            }
            else
            {
                curveKString = QString::asprintf(iter->format.c_str(), curveK);
                curveBString = QString::asprintf(iter->format.c_str(), fabs(curveB));
            }

            if(curveB < 0) //负数求绝对值
            {
                calibrateTableWidget->item(row, column++)->setText(curveKString +" * C - "+ curveBString);
            }
            else
            {
                calibrateTableWidget->item(row, column++)->setText(curveKString +" * C + "+ curveBString);
            }
        }
        else
        {
            FieldType type;
            if (!m_resultFiles->GetRecordFields()->GetFieldType(iter->name, type))
            {
                column++;
                continue;
            }
            switch(type)
            {
                case FieldType::Bool:
                    {
                        bool ret;
                        m_resultFiles->GetFieldCurrentRecordValue(iter->name, ret);
                        calibrateTableWidget->item(row, column++)->setText(QString::number(ret));
                    }
                    break;
                case FieldType::Byte:
                    {
                        Byte ret;
                        m_resultFiles->GetFieldCurrentRecordValue(iter->name, ret);
                        if (iter->format.empty())
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::number(ret));
                        }
                        else
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::asprintf(iter->format.c_str(), ret));
                        }
                    }
                    break;
                case FieldType::Int:
                    {
                        int ret;
                        m_resultFiles->GetFieldCurrentRecordValue(iter->name, ret);
                        if (iter->format.empty())
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::number(ret));
                        }
                        else
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::asprintf(iter->format.c_str(), ret));
                        }
                    }
                    break;
                case FieldType::Float:
                    {
                        float ret;
                        m_resultFiles->GetFieldCurrentRecordValue(iter->name, ret);
                        if (iter->format.empty())
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::number(ret));
                        }
                        else
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::asprintf(iter->format.c_str(), ret));
                        }
                    }
                    break;
                case FieldType::Double:
                    {
                        double ret;
                        m_resultFiles->GetFieldCurrentRecordValue(iter->name, ret);
                        if (iter->format.empty())
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::number(ret,'f',9));
                        }
                        else
                        {
                            calibrateTableWidget->item(row, column++)->setText(QString::asprintf(iter->format.c_str(), ret));
                        }
                    }
                    break;
                case FieldType::IntArray:
                    break;
                case FieldType::Time:
                    {
                        int ret;
                        m_resultFiles->GetFieldCurrentRecordValue(iter->name, ret);
                        calibrateTableWidget->item(row, column++)->setText(QDateTime::fromTime_t(ret).toString(iter->format.c_str()));
                    }
                    break;
                case FieldType::Enum:
                    {
                        String str;
                        m_resultFiles->GetFieldCurrentRecordEnumString(iter->name, str);
                        calibrateTableWidget->item(row, column++)->setText(str.c_str());
                    }
                    break;
            }
        }
    }
}

void CalibrateCurve::ReadResult()
{
    Uint16 row = 0;

    ViewRefresh();
    SpaceInit();

    m_resultFiles->MoveToLast();
    m_resultDetailFiles->SetReadPos(m_resultFiles->GetReadPos());
    while(m_resultFiles->HasPrevious() && row < 10)
    {
        m_resultFiles->MovePrevious();
        ShowRow(row++);
    }
}

void CalibrateCurve::SearchResult()
{
    QDateTime theDate;
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text()+" 00:00:00","yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text()+" 23:59:59","yyyy-MM-dd hh:mm:ss");
    Uint16 row = 0;

    ViewRefresh();
    SpaceInit();

    m_resultFiles->MoveToLast();
    m_resultDetailFiles->SetReadPos(m_resultFiles->GetReadPos());
    while(m_resultFiles->HasPrevious() && row < 10)
    {
        int ret;
        m_resultFiles->MovePrevious();

        m_resultFiles->GetFieldCurrentRecordValue(m_dateTimeStr, ret);
        theDate = QDateTime::fromTime_t(ret);


        if(theDate <= theMaxDateTime && theDate >= theMinDateTime)
        {
            ShowRow(row++);
        }
    }
}

void CalibrateCurve::ToTop()      // 对应按钮|<
{
    nextFlag = 0;
    backFlag = 0;
    SearchResult();
}

void CalibrateCurve::ToBottom()    // 对应按钮|>
{
    nextFlag = 0;
    backFlag = 0;

    QDateTime theDate;
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text()+" 00:00:00","yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text()+" 23:59:59","yyyy-MM-dd hh:mm:ss");
    Uint16 row = 0;

    ViewRefresh();
    SpaceInit();

    m_resultFiles->MoveToFirst();

    while(m_resultFiles->HasNext() && row < 10)
    {
        int ret;
        m_resultFiles->MoveNext();

        m_resultFiles->GetFieldCurrentRecordValue(m_dateTimeStr, ret);
        theDate = QDateTime::fromTime_t(ret);

        if(theDate <= theMaxDateTime && theDate >= theMinDateTime)
        {
            row++;
            ShowRow(10 - row);
        }
    }
    m_resultDetailFiles->SetReadPos(m_resultFiles->GetReadPos());
    if(row < 10)
    {
        ToTop();
    }
}

void CalibrateCurve::ToBack()    //对应按钮 << ,最新数据
{
    QDateTime theDate;
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text()+" 00:00:00","yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text()+" 23:59:59","yyyy-MM-dd hh:mm:ss");
    Uint16 row = 0;

    SpaceInit();

    if(m_resultFiles->GetRemainNum() <= 10)
    {
        ToTop();
    }
    else
    {
        if(nextFlag == 1)
        {
            int k = 10;
            while(m_resultFiles->HasNext()&& k > 0)
            {
                m_resultFiles->MoveNext();
                k--;
            }
            nextFlag = 0;
            backFlag = 1;
        }
        while(m_resultFiles->HasNext() && row < 10)
        {
            int ret;
            m_resultFiles->MoveNext();

            m_resultFiles->GetFieldCurrentRecordValue(m_dateTimeStr, ret);
            theDate = QDateTime::fromTime_t(ret);

            if(theDate <= theMaxDateTime && theDate >= theMinDateTime)
            {
                row++;
                ShowRow(10 - row);
            }
        }
        m_resultDetailFiles->SetReadPos(m_resultFiles->GetReadPos());
        backFlag = 1;
        if(row < 10)
        {
            ToTop();
        }
    }
}

void CalibrateCurve::ToNext()     //对应按钮 >>
{
    QDateTime theDate;
    QDateTime theMinDateTime  = QDateTime::fromString(minTime->text()+" 00:00:00","yyyy-MM-dd hh:mm:ss");
    QDateTime theMaxDateTime  = QDateTime::fromString(maxTime->text()+" 23:59:59","yyyy-MM-dd hh:mm:ss");
    Uint16 row = 0;


    SpaceInit();

    if(m_resultFiles->GetHeadNum() <= 10)
    {
        ToBottom();
    }
    else
    {
        if(backFlag ==1)
        {
            int k = 10;
            while(m_resultFiles->HasPrevious()&& k > 0)
            {
               m_resultFiles->MovePrevious();
               k--;
            }
            backFlag = 0;
        }
        m_resultDetailFiles->SetReadPos(m_resultFiles->GetReadPos());
        while(m_resultFiles->HasPrevious() && row < 10)
        {
            int ret;
            m_resultFiles->MovePrevious();

            m_resultFiles->GetFieldCurrentRecordValue(m_dateTimeStr, ret);
            theDate = QDateTime::fromTime_t(ret);

            if(theDate <= theMaxDateTime && theDate >= theMinDateTime)
            {
                ShowRow(row++);
            }
        }
        nextFlag = 1;
        if(row < 10)
        {
            ToBottom();
        }
    }
}

QString CalibrateCurve::GetMinTime()
{
    QString minTime = QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd");
    m_resultFiles->MoveToLast();
    if(m_resultFiles->HasPrevious())
    {
        m_resultFiles->MovePrevious();

        int ret = 0;
        m_resultFiles->GetFieldCurrentRecordValue(m_dateTimeStr, ret);
        minTime = QDateTime::fromTime_t(ret).addDays(-30).toString("yyyy-MM-dd");
    }
    return minTime;
}

QString CalibrateCurve::GetMaxTime()
{
    QString maxTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    m_resultFiles->MoveToLast();
    if(m_resultFiles->HasPrevious())
    {
        m_resultFiles->MovePrevious();

        int ret = 0;
        m_resultFiles->GetFieldCurrentRecordValue(m_dateTimeStr, ret);
        maxTime = QDateTime::fromTime_t(ret).toString("yyyy-MM-dd");
    }
    return maxTime;
}

void CalibrateCurve::DisplayCurrentCurve( double curveK, double curveB)
{
    m_currentCurveKValue = curveK;
    m_currentCurveBValue = curveB;
    for (vector<ShowField>::iterator iter = m_showFields.begin(); iter != m_showFields.end(); ++iter)
    {
        if (iter->name == "curve")
        {
            QString curveKString;
            QString curveBString;

            if (m_isUnitChange)
            {
                m_lua->call(m_unitChangeFunc, curveK, UnitChange::Read);
                m_lua->pull(curveK);
            }

            if (iter->format.empty())
            {
                curveKString = QString::number(curveK,'f',9);
                curveBString = QString::number(fabs(curveB),'f',9);
            }
            else
            {
                curveKString = QString::asprintf(iter->format.c_str(), curveK);
                curveBString = QString::asprintf(iter->format.c_str(), fabs(curveB));
            }

            if(curveB < 0) //负数求绝对值
            {
                currentCurve->setText("当前标线: " + m_curveType + " = " + curveKString + " * C - "+ curveBString);
            }
            else
            {
                currentCurve->setText("当前标线: " + m_curveType + " = " + curveKString + " * C + "+curveBString);
            }

            break;
        }
    }
}

void CalibrateCurve::DisplayExtraInfo()
{
    try
    {
        if (m_isDisplayExtraInfo)
        {
            String ret;
            m_lua->call(m_getExtraInfo, this->m_profileTableName);
            m_lua->pull(ret);
            m_ExtraInfoLabel->setText(ret.c_str());
            m_ExtraInfoLabel->setFont(font);
        }
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("CalibrateCurve::DisplayExtraInfo() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("CalibrateCurve::DisplayExtraInfo() => %s", e.what());
    }
}

QDate CalibrateCurve::GetCurrentMinDate()
{
    QString dateString = minTime->text();
    QStringList dateList = dateString.split("-");
    QString year = dateList.at(0);
    QString month = dateList.at(1);
    QString day = dateList.at(2);

    QDate minDate = QDate(year.toInt(),month.toInt(),day.toInt());
    return minDate;
}

QDate CalibrateCurve::GetCurrentMaxDate()
{
    QString dateString = maxTime->text();
    QStringList dateList = dateString.split("-");
    QString year = dateList.at(0);
    QString month = dateList.at(1);
    QString day = dateList.at(2);

    QDate maxDate = QDate(year.toInt(),month.toInt(),day.toInt());
    return maxDate;
}

void CalibrateCurve::ChoseMinDay()
{
    minDayCaledar->setFixedSize(300,300);
    minDayCaledar->move(130,120);
    minDayCaledar->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    minDayCaledar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    if(minDayCaledar->isHidden())
    {
        minDayCaledar->show();
        maxDayCaledar->hide();
    }
    else
    {
        minDayCaledar->hide();
        maxDayCaledar->hide();
    }
}

void CalibrateCurve::ChangeMinDay()
{
    QDate date = minDayCaledar->selectedDate();
    QDate curMaxDate = this->GetCurrentMaxDate();
    if(date > curMaxDate)
    {
        minDayCaledar->hide();
        MessageDialog msg("检索起始日期不能晚于结束日期！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    QString dateString;
    QString monthsign;
    QString daysign;
    if(date.month()>=10)
    {
        monthsign = "-";
    }
    else
    {
        monthsign = "-0";
    }

    if(date.day()>=10)
    {
        daysign = "-";
    }
    else
    {
        daysign = "-0";
    }

    dateString = QString::number(date.year())+monthsign+QString::number(date.month())+daysign+QString::number(date.day());

    minTime->setText(dateString);
    minDayCaledar->hide();
}

void CalibrateCurve::ChoseMaxDay()
{
    maxDayCaledar->setFixedSize(300,300);
    maxDayCaledar->move(260,120);
    maxDayCaledar->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    maxDayCaledar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    if(maxDayCaledar->isHidden())
    {
        maxDayCaledar->show();
        minDayCaledar->hide();
    }
    else
    {
        maxDayCaledar->hide();
        minDayCaledar->hide();
    }
}

void CalibrateCurve::ChangeMaxDay()
{
    QDate date = maxDayCaledar->selectedDate();
    QDate curMinDate = this->GetCurrentMinDate();
    if(date < curMinDate)
    {
        maxDayCaledar->hide();
        MessageDialog msg("检索结束日期不能早于起始日期！\n", this,MsgStyle::ONLYOK);
        if(msg.exec()== QDialog::Accepted)
        {
            return;
        }
    }

    QString dateString;
    QString monthsign;
    QString daysign;
    if(date.month()>=10)
    {
        monthsign = "-";
    }
    else
    {
        monthsign = "-0";
    }

    if(date.day()>=10)
    {
        daysign = "-";
    }
    else
    {
        daysign = "-0";
    }

    dateString = QString::number(date.year())+monthsign+QString::number(date.month())+daysign+QString::number(date.day());
    maxTime->setText(dateString);
    maxDayCaledar->hide();
}

void CalibrateCurve::SpaceInit()
{
    for(int i = 0;i < calibrateTableWidget->rowCount();i++)
    {
        for(int j = 0;j < calibrateTableWidget->columnCount();j++)
        {
            calibrateTableWidget->setItem(i, j, new QTableWidgetItem());
            calibrateTableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void CalibrateCurve::ViewRefresh()
{
    calibrateTableWidget->clear();
    calibrateTableWidget->setColumn(m_columnName);
}

void CalibrateCurve::ChangeBottomStatus()
{
    bool solidifyMeaParamFromUI = false;

    try
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State * state = luaEngine->GetThreadState();

        luaEngine->GetLuaValue(state, "config.system.solidifyMeaParamFromUI", solidifyMeaParamFromUI);
    }
    catch(OOLUA::LuaException e)
    {
        logger->warn("MeasureData::ChangeBottomStatus() => %s", e.What().c_str());
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("MeasureData::ChangeBottomStatus() => %s", e.what());
    }
	
    if(LoginDialog::userType == RoleType::Super)
    {
        toMapButton->show();
        inputButton->show();
        exportButton->show();
        clearButton->show();
    }
    else if(LoginDialog::userType == RoleType::Administrator)
    {
        toMapButton->show();
        exportButton->show();
        inputButton->hide();

        if (solidifyMeaParamFromUI)
        {
            clearButton->hide();
        }
        else
        {
            clearButton->show();
        }
    }
    else if(LoginDialog::userType == RoleType::Maintain)
    {
        toMapButton->show();
        exportButton->show();
        inputButton->hide();

        if (solidifyMeaParamFromUI)
        {
            clearButton->hide();
        }
        else
        {
            clearButton->show();
        }
    }
    else if(LoginDialog::userType == RoleType::General)
    {
        toMapButton->hide();
        inputButton->hide();
        exportButton->hide();
        clearButton->hide();
    }


    if(m_resultDetailWidget != nullptr)
    {
        RoleType showPrivilege;
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State * state = luaEngine->GetThreadState();
        luaEngine->GetLuaValue(state, "setting.ui.resultDetail.showPrivilege", showPrivilege);

        if(LoginDialog::userType >= showPrivilege)
        {
            detailButton->show();
        }
        else
        {
            detailButton->hide();
        }
    }
}

void CalibrateCurve::DisplayHiddenInfo()
{
    int width = 0;
    for (map<Uint8, bool>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        Uint8 column = iter->first;
        bool isHiddenInfo = iter->second;
        if (!m_isDisplayHiddenInfo && isHiddenInfo)
        {
            calibrateTableWidget->setColumnHidden(column, true);
        }
        else
        {
            calibrateTableWidget->setColumnHidden(column, false);
            width = width + calibrateTableWidget->columnWidth(column);
        }
    }

    //调整表格大小适应显示
    if(width > calibrateTableWidget->width())
    {
        this->TableExtraInfoAdjust(true);
    }
    else
    {
        this->TableExtraInfoAdjust(false);
    }

}

void CalibrateCurve::TableExtraInfoAdjust(bool isEnable)
{
    if(isEnable)
    {
        calibrateTableWidget->horizontalScrollBar()->setVisible(true);
        calibrateTableWidget->horizontalScrollBar()->setDisabled(false);
        for(int i =0;i<10;i++)
        {
            calibrateTableWidget->setRowHeight(i,28);
        }
        calibrateTableWidget->horizontalHeader()->setFixedHeight(35);
    }
    else
    {
        calibrateTableWidget->horizontalScrollBar()->setVisible(false);
        calibrateTableWidget->horizontalScrollBar()->setDisabled(true);
        for(int i =0;i<10;i++)
        {
            calibrateTableWidget->setRowHeight(i,30);
        }
        calibrateTableWidget->horizontalHeader()->setFixedHeight(33);
    }
}

void CalibrateCurve::OnUserChange()
{
    this->ChangeBottomStatus();
}

void CalibrateCurve::OnCalibrateResultAdded(String name, RecordData result)
{
    if (m_resultDataname == name)
    {
        double curveK, curveB;
        result.GetFieldData(m_curveKStart, curveK);
        result.GetFieldData(m_curveBStart, curveB);
        UpdaterCalibrate(curveB, curveK);
    }
}

void CalibrateCurve::UpdaterCalibrate(double curveB, double curveK)
{
    this->DisplayCurrentCurve(curveK, curveB);
    this->DisplayExtraInfo();//显示当前标点浓度

    minTime->setText(QDateTime::currentDateTime().addDays(-30).toString("yyyy-MM-dd"));
    maxTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));

    ReadResult();
}

void CalibrateCurve::OnMeasureResultAdded(String name, RecordData result)
{
    (void)name;
    (void)result;
}

void CalibrateCurve::ToInput()
{
    QHBoxLayout *inputLayout = new QHBoxLayout();

    QDialog *checkDialog = new QDialog();

    CNumberKeyboard *numberKey = new CNumberKeyboard(checkDialog);
    checkDialog->setFixedSize(450,150);
    checkDialog->move(this->width()/2 -100,this->height()/2);

    QLabel *curveK;
    curveK = new QLabel();
    curveK->setText("K值");
    curveK->setFixedSize(30,40);
    curveK->setFont(font);

    QLabel *curveB;
    curveB = new QLabel();
    curveB->setText("B值");
    curveB->setFixedSize(30,40);
    curveB->setFont(font);

    QLineEdit *curveKEdit;
    curveKEdit = new QLineEdit();
    curveKEdit->setFixedHeight(30);
    curveKEdit->setFixedWidth(80);
    curveKEdit->setFont(font);
    curveKEdit->installEventFilter(numberKey);

    QLineEdit *curveBEdit;
    curveBEdit = new QLineEdit();
    curveBEdit->setFixedHeight(30);
    curveBEdit->setFixedWidth(80);
    curveBEdit->setFont(font);
    curveBEdit->installEventFilter(numberKey);

    QPushButton *isConfirm;
    isConfirm = new QPushButton();
    isConfirm->setObjectName("brownButton");
    isConfirm->setText("确定");
    isConfirm->setFixedSize(60,35);
    isConfirm->setFont(font);

    QPushButton *isConcel;
    isConcel = new QPushButton();
    isConcel->setObjectName("brownButton");
    isConcel->setText("取消");
    isConcel->setFixedSize(60,35);
    isConcel->setFont(font);

    inputLayout->addWidget(curveK);
    inputLayout->addWidget(curveKEdit);
    inputLayout->addWidget(curveB);
    inputLayout->addWidget(curveBEdit);
    inputLayout->addWidget(isConfirm);
    inputLayout->addWidget(isConcel);

    checkDialog->setLayout(inputLayout);
    connect(isConfirm,SIGNAL(clicked()),checkDialog,SLOT(accept()));
    connect(isConcel, SIGNAL(clicked()),checkDialog, SLOT(close()));
    if(checkDialog->exec() == QDialog::Rejected)
    {
        return;
    }
    else
    {
        QString pattern("^([+-]?)(\\d{1,6})(\\.\\d{1,6})?$");
        QRegExp rx(pattern);

        if(rx.exactMatch(curveKEdit->text())&&rx.exactMatch(curveBEdit->text()))
        {
            double curveK, curveB;

            curveK = curveKEdit->text().toDouble();
            curveB = curveBEdit->text().toDouble();

            if (m_isUnitChange)
            {
                m_lua->call(m_unitChangeFunc, curveK, UnitChange::Write);
                m_lua->pull(curveK);
            }

            LuaEngine::Instance()->RunChunk(String(m_curveParamName + ".curveK = " + QString::number(curveK,'f',9).toStdString()));
            LuaEngine::Instance()->RunChunk(String(m_curveParamName + ".curveB = " + curveBEdit->text().toStdString()));

            m_lua->call(m_saveFile, this->m_profileTableName, this->m_profileFileName, true);

            RecordData recordData(ResultManager::Instance()->GetCalibrateRecordFile(m_resultDataname)->GetRecordFields()->GetFieldsSize());
            int time = QDateTime::currentDateTime().toTime_t();

            m_lua->call(m_toInput, &recordData, time, curveK, curveB);
            m_lua->push(&recordData);

            //给结果管理添加定标曲线，结果管理通过观察者更新注册的所有界面，会调用 UpdaterCalibrateSlot 函数更新表格和当前曲线
            ResultManager::Instance()->AddCalibrateRecordSlots(m_resultDataname, recordData);
        }

        if (numberKey)
            delete numberKey;
        if (checkDialog)
            delete checkDialog;
    }
}

void CalibrateCurve::SlotClearButton()
{
    MessageDialog msg("是否确认清空标线记录?", this, MsgStyle::OKANDCANCEL);
    if(QDialog::Accepted != msg.exec())
    {
        return;
    }
    else
    {
        ResultManager::Instance()->ClearBackupCalibrateRecordFile(m_resultDataname);
        ResultManager::Instance()->GetCalibrateRecordFile(m_resultDataname)->ClearRecord();
        minTime->setText(GetMinTime());
        maxTime->setText(GetMaxTime());
        ViewRefresh();
        SpaceInit();
        this->DisplayCurrentCurve(1, 0);
        m_lua->call(m_defaultRestore, this->m_profileTableName, this->m_profileFileName, this->m_curveParamName);
    }
}

void CalibrateCurve::SlotExportButton()
{
    QString result = false;
    CopyFileAction copyFileAction;
    String strDir = copyFileAction.GetTargetDir();
    QDir dir(strDir.c_str());

    if (!copyFileAction.ExMemoryDetect()) //U盘检测
    {
        MessageDialog msg("U盘不存在，定标数据导出失败", this);
        msg.exec();
        return;
    }

    if (!copyFileAction.TargetDirCheck(dir)) //拷贝目录检测
    {
        MessageDialog msg("不可在U盘创建目录，定标数据导出失败", this);
        msg.exec();
        return;
    }

    if  (m_resultFiles->ExportFiles(LoginDialog::userType))
    {
        result = "定标数据导出成功";
        logger->info("定标数据导出成功");
    }
    else
    {
        result = "定标数据导出失败";
        logger->info("定标数据导出失败");
    }
    MessageDialog msg(result, this);
    msg.exec();
    return;
}

void CalibrateCurve::showEvent(QShowEvent *event)
{
    (void)event;
    if(!minDayCaledar->isHidden())
    {
      minDayCaledar->hide();
    }
    if(!maxDayCaledar->isHidden())
    {
      maxDayCaledar->hide();
    }
    this->setFocus();

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();

    double curveK, curveB;
    luaEngine->GetLuaValue(state, String(m_curveParamName + ".curveK"), curveK);
    luaEngine->GetLuaValue(state, String(m_curveParamName + ".curveB"), curveB);

    this->DisplayCurrentCurve(curveK, curveB);//显示当前标线
    DisplayExtraInfo();
}

void CalibrateCurve::OnUpdateWidget(UpdateEvent event, System::String message)
{
    if ((event == UpdateEvent::ChangeMeasureParam && message != "CalibrateCurve") || //更改测量参数
         event == UpdateEvent::ModbusChangeParam ||
          event ==  UpdateEvent::ChangeConfigParam ||
            event ==  UpdateEvent::WqimcChangeParam)
    {
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State * state = luaEngine->GetThreadState();

        double curveK, curveB;
        luaEngine->GetLuaValue(state, String(m_curveParamName + ".curveK"), curveK);
        luaEngine->GetLuaValue(state, String(m_curveParamName + ".curveB"), curveB);

        if (qAbs(curveB - m_currentCurveBValue) > PRECISE || qAbs(curveK - m_currentCurveKValue) > PRECISE)
        {
            RecordData recordData(ResultManager::Instance()->GetCalibrateRecordFile(m_resultDataname)->GetRecordFields()->GetFieldsSize());
            int time = QDateTime::currentDateTime().toTime_t();
            m_lua->call(m_toInput, &recordData, time, curveK, curveB);
            m_lua->push(&recordData);
            //给结果管理添加定标曲线，结果管理通过观察者更新注册的所有界面，会调用 UpdaterCalibrateSlot 函数更新表格和当前曲线
            ResultManager::Instance()->AddCalibrateRecordSlots(m_resultDataname, recordData);
        }

        //是否显示隐藏信息开关
        if (m_getHiddenInfo.valid())
        {
            bool ret;
            m_lua->call(m_getHiddenInfo);
            m_lua->pull(ret);
            m_isDisplayHiddenInfo = ret;
            this->DisplayHiddenInfo();
        }
    }
    else if(event == UpdateEvent::OneKeyClearData)
    {
        ResultManager::Instance()->ClearBackupCalibrateRecordFile(m_resultDataname);
        ResultManager::Instance()->GetCalibrateRecordFile(m_resultDataname)->ClearRecord();
        minTime->setText(GetMinTime());
        maxTime->setText(GetMaxTime());
        ViewRefresh();
        SpaceInit();
        this->DisplayCurrentCurve(1, 0);
        m_lua->call(m_defaultRestore, this->m_profileTableName, this->m_profileFileName, this->m_curveParamName);
    }
}

CalibrateCurve::~CalibrateCurve()
{

}

}

