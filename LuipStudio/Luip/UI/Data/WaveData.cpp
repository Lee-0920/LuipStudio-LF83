#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTime>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QDebug>
#include "UI/UserChangeManager/UserChangeManager.h"
#include "UI/Frame/LoginDialog.h"
#include "WaveData.h"
#include "LuaEngine/LuaEngine.h"
#include "UI/Frame/UpdateWidgetManager.h"
#include "Lua/App.h"
#include <QScrollArea>
#include <QGroupBox>

using namespace Result;
using namespace OOLUA;
using namespace std;
using namespace Lua;
using namespace ResultData;

int SortDataByTime(QPointF data1, QPointF data2);

namespace UI
{

WaveData::WaveData(System::String resultDataname, QWidget *parent)
    :QWidget(parent),m_resultDataname(resultDataname)
{
    MaintainUserChange::Instance()->Register(this);
    UpdateWidgetManager::Instance()->Register(this);
    ResultManager::Instance()->Register(this);

    m_resultFiles = ResultManager::Instance()->GetWaveUseMeasureRecordFile(m_resultDataname);

    m_lua = LuaEngine::Instance()->GetEngine();

    m_ObjBackDateTime = GetBackDateTime(6);
    m_ObjNextDateTime = GetBackDateTime(0).addDays(1);

    InitWave();
}

WaveData::~WaveData()
{

}

void WaveData::InitWave()
{
    this->resize(700, 400);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    Script *lua = LuaEngine::Instance()->GetEngine();
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();

    Table itermsTable;
    luaEngine->GetLuaValue(state, "setting.ui.waveData", itermsTable);

    oolua_ipairs(itermsTable)
    {
        Table waveTable;
        lua->pull(waveTable);

        String content;
        waveTable.at("content", content);

        String title;
        waveTable.at("title", title);

        float yUpLimit;
        waveTable.at("yUpLimit", yUpLimit);

        float yLowLimit;
        waveTable.at("yLowLimit", yLowLimit);

        int width;
        waveTable.at("width", width);

        int height;
        waveTable.at("height", height);

        WaveObject *wave = new WaveObject;
        wave->content = QString::fromStdString(content);
        wave->title = QString::fromStdString(title);
        wave->yUpLimit = yUpLimit;
        wave->yLowLimit = yLowLimit;
        wave->width = width;
        wave->height = height;

        wave->plot = new QwtPlot(); //地址容器，里面存放其他对象  // 不加QString转换会提示无法通过编译，无法进行类型转换
        wave->plot->setFixedSize(wave->width, wave->height); //640 350

        QwtLegend *tempLegend = new QwtLegend;//标注
        wave->plot->insertLegend(tempLegend, QwtPlot::RightLegend);//插入位置，可以分别是上下左右

        QwtPlotCanvas *tempCanvas = new QwtPlotCanvas(); //画布
        tempCanvas->setBorderRadius(2);//画布四周边框角半径

        wave->plot->setCanvas(tempCanvas);//设置绘图的画布
        wave->plot->plotLayout()->setAlignCanvasToScales(true);//设置对齐画布、坐标轴、刻度
        wave->plot->setAutoReplot(true); // 设置自动重画，相当于更新

        wave->plot->setAxisScaleDraw(QwtPlot::yLeft,new VScaleDraw());//设置y轴的刻度绘画
        wave->plot->setAxisScale(QwtPlot::yLeft, wave->yLowLimit, wave->yUpLimit);//设置y轴坐标刻度大小: 0.0 到 最大量程

        wave->plot->axisWidget(QwtPlot::xBottom)->setMinBorderDist(70, 18);
        wave->plot->setAxisScale(QwtPlot::xBottom, 0.0, 7 * 50); //设置x轴坐标刻度大小  0-350（每天50个点）
        wave->plot->setAxisScaleDraw(QwtPlot::xBottom, new CTimeScaleDraw(m_ObjBackDateTime, 7 , 50));//设置x轴的刻度绘画
        wave->plot->setAxisLabelRotation(QwtPlot::xBottom,-10.0);//标签旋转10度

        QFont font;
        font.setPointSize(10);

        QwtText titleText(wave->title);
        titleText.setFont(font);

        wave->plot->setTitle(titleText);

        //设置光标
        QwtPlotPicker *picker =  new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                      QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly,
                                            wave->plot->canvas());
        picker->setStateMachine(new QwtPickerDragPointMachine());
        picker->setRubberBandPen(QPen(QColor(255,200,150)));
        picker->setTrackerPen(QColor(Qt::black));

        // 设置网格线
        QwtPlotGrid *tempGrid = new QwtPlotGrid;
        tempGrid->enableX(false);
        tempGrid->enableY(true);
        tempGrid->setMajorPen(Qt::lightGray, 0, Qt::DashLine);
        tempGrid->attach(wave->plot);

        oolua_ipairs(waveTable)
        {
            Table curveTable;
            lua->pull(curveTable);

            String name;
            curveTable.at("name", name);

            String text;
            curveTable.at("text", text);

            Qt::GlobalColor color;
            curveTable.at("color", color);

            CurveCategory curveCategory;
            curveCategory.name = QString::fromStdString(name);
            curveCategory.text = QString::fromStdString(text);
            curveCategory.color = color;

            curveCategory.curve = new QwtPlotCurve(curveCategory.text);//曲线类
            curveCategory.curve->setPen(color, 2);                                    // 设置曲线颜色 粗细
            curveCategory.curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);    // 线条光滑化,设置渲染效果，抗锯齿
            curveCategory.curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);

            wave->curveList.push_back(curveCategory);
        }
        oolua_ipairs_end()

        m_waveList.push_back(wave);
    }
    oolua_ipairs_end()

    QVBoxLayout *plotLayout = new QVBoxLayout();
    plotLayout->addStretch();
    foreach (WaveObject* wave, m_waveList)
    {
        plotLayout->addWidget(wave->plot);
        plotLayout->addStretch();
    }

    QGroupBox* group = new QGroupBox();
    group->setLayout(plotLayout);
    group->setStyleSheet("QGroupBox{border:none}");

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(group);
    scrollArea->setWidgetResizable(false);  // 设置滚动区大小
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("QScrollArea{border:none;background-color:rgb(236,236,236)}");

    QFont btnFont;
    btnFont.setPointSize(18);

    backButton = new QPushButton(tr("<<"));
    backButton->setObjectName("brownButton");
    backButton->setFixedHeight(35);
    backButton->setFixedWidth(70);
    backButton->setFocusPolicy(Qt::NoFocus);
    backButton->setFont(btnFont);

    nextButton = new QPushButton(tr(">>"));
    nextButton->setObjectName("brownButton");
    nextButton->setFixedHeight(35);
    nextButton->setFixedWidth(70);
    nextButton->setFocusPolicy(Qt::NoFocus);
    nextButton->setFont(btnFont);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addSpacing(50);
    bottomLayout->addWidget(backButton);
    bottomLayout->addSpacing(20);
    bottomLayout->addWidget(nextButton);
    bottomLayout->addStretch();
    bottomLayout->setMargin(0);
    bottomLayout->setAlignment(Qt::AlignLeft);

    connect(backButton, SIGNAL(clicked()), this, SLOT(SlotBackButton()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(SlotNextButton()));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();

    LoadWaveData();
}

void WaveData::LoadWaveData()
{

    foreach (WaveObject* wave, m_waveList)
    {
        for(QList<CurveCategory>::Iterator it = wave->curveList.begin(); it != wave->curveList.end(); it++)
        {
            (*it).buffer.clear();
        }

        m_resultFiles->MoveToLast();
        int i = 0;
        while(m_resultFiles->HasPrevious() && i < 5000)
        {
            QDateTime dateTime;
            int dataTime_t;
            String resulTypeStr;
            QString resultType;
            double value;

            m_resultFiles->MovePrevious();

            String dataTimeMark = "dateTime";
            m_resultFiles->GetFieldCurrentRecordValue(dataTimeMark, dataTime_t);
            dateTime = QDateTime::fromTime_t(dataTime_t);

            String resultTypeMark = "resultType";
            m_resultFiles->GetFieldCurrentRecordEnumString(resultTypeMark, resulTypeStr);
            resultType = QString::fromStdString(resulTypeStr);

            m_resultFiles->GetFieldCurrentRecordValue(wave->content.toStdString(), value);

            if(dateTime >= m_ObjBackDateTime && dateTime <= m_ObjNextDateTime)
            {
                for(QList<CurveCategory>::Iterator it = wave->curveList.begin(); it != wave->curveList.end(); it++)
                {
                    if(resultType == (*it).text)
                    {
                        (*it).buffer.append(QPointF(DateTimeToInt(dateTime), value));
                        i++;
                    }
                }
            }
            else if(dateTime < m_ObjBackDateTime)
            {
                break;
            }
        }

        for(QList<CurveCategory>::Iterator it = wave->curveList.begin(); it != wave->curveList.end(); it++)
        {
            std::sort((*it).buffer.begin(), (*it).buffer.end(), SortDataByTime);
            (*it).curve->attach(wave->plot);
            (*it).curve->setSamples((*it).buffer);
        }
    }
}

void WaveData::UpdateData()
{
    QDateTime objDateTime = QDateTime::currentDateTime();
    if (objDateTime > m_ObjNextDateTime)
    {
        while(true)
        {
            m_ObjBackDateTime = m_ObjBackDateTime.addDays(7);
            m_ObjNextDateTime = m_ObjNextDateTime.addDays(7);
            if (objDateTime <= m_ObjNextDateTime && objDateTime >= m_ObjBackDateTime)
            {
                break;
            }
        }
    }

    foreach (WaveObject* wave, m_waveList)
    {
        wave->plot->setAxisScale(QwtPlot::xBottom, 0.0, 7 * 50);
        wave->plot->setAxisScaleDraw(QwtPlot::xBottom,new CTimeScaleDraw(m_ObjBackDateTime, 7 , 50));
        wave->plot->setAxisLabelRotation(QwtPlot::xBottom,-10.0);
    }

    this->LoadWaveData();
}

void WaveData::OnMeasureResultAdded(String name, RecordData result)
{
    (void)name;
    (void)result;
	if (m_resultDataname == name)
	{
        UpdateData();
	}
}

void WaveData::OnCalibrateResultAdded(String name, RecordData result)
{
    (void)name;
    (void)result;
}

void WaveData::OnUserChange()
{

}

void WaveData::SlotBackButton()
{
    m_ObjBackDateTime = m_ObjBackDateTime.addDays(-7);
    m_ObjNextDateTime = m_ObjNextDateTime.addDays(-7);

    foreach (WaveObject* wave, m_waveList)
    {
        wave->plot->setAxisScale(QwtPlot::xBottom, 0.0, 7 * 50);
        wave->plot->setAxisScaleDraw(QwtPlot::xBottom,new CTimeScaleDraw(m_ObjBackDateTime, 7 , 50));
        wave->plot->setAxisLabelRotation(QwtPlot::xBottom,-10.0);
    }

    this->LoadWaveData();
}

void WaveData::SlotNextButton()
{
    m_ObjBackDateTime = m_ObjBackDateTime.addDays(7);
    m_ObjNextDateTime = m_ObjNextDateTime.addDays(7);

    foreach (WaveObject* wave, m_waveList)
    {
        wave->plot->setAxisScale(QwtPlot::xBottom, 0.0, 7 * 50);
        wave->plot->setAxisScaleDraw(QwtPlot::xBottom,new CTimeScaleDraw(m_ObjBackDateTime, 7 , 50));
        wave->plot->setAxisLabelRotation(QwtPlot::xBottom,-10.0);
    }

    this->LoadWaveData();
}

QDateTime WaveData::GetBackDateTime(int iBackDays)
{
    // 设置开始的时间
    QDateTime objDateTime = QDateTime::currentDateTime();

    m_resultFiles->MoveToLast();//文件位置移到最后
    if(m_resultFiles->HasPrevious())//判断一下文件是否有内容
    {
        m_resultFiles->MovePrevious();//向前移一个数据
        int ret = 0;
        String dataTimeMark = "dateTime";
        m_resultFiles->GetFieldCurrentRecordValue(dataTimeMark, ret);
        objDateTime = QDateTime::fromTime_t(ret);
    }

    objDateTime.setTime(QTime(0, 0, 0));//清零时分秒
    if(iBackDays < 0)
    {
        iBackDays = 0;
    }
    objDateTime = objDateTime.addDays(-1 * iBackDays);//往前减iBackDays天

    return objDateTime;
}

int WaveData::DateTimeToInt(QDateTime dateTime)
{
    if(dateTime.secsTo(m_ObjBackDateTime) <= 0)
    {
        return m_ObjBackDateTime.secsTo(dateTime) * 50  / (60 * 60 *24);
    }
    else
    {
        return 0;
    }
}

void WaveData::OnUpdateWidget(UpdateEvent event, System::String message)
{
    if ((event == UpdateEvent::ClearMeasureData && message == m_resultDataname) || (event == UpdateEvent::OneKeyClearData))//清除测量数据
    {
        m_ObjBackDateTime = GetBackDateTime(6);
        m_ObjNextDateTime = GetBackDateTime(0).addDays(1);

        foreach (WaveObject* wave, m_waveList)
        {
            wave->plot->setAxisScale(QwtPlot::xBottom, 0.0, 7 * 50);
            wave->plot->setAxisScaleDraw(QwtPlot::xBottom,new CTimeScaleDraw(m_ObjBackDateTime, 7 , 50));
            wave->plot->setAxisLabelRotation(QwtPlot::xBottom,-10.0);
        }

        this->LoadWaveData();
    }

}

}

int SortDataByTime(QPointF data1, QPointF data2)
{
    return data1.x() < data2.x();
}




