#include "LF82ResultDetailWidget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <qapplication.h>
#include <qeventloop.h>
#include "LF82CurveManager.h"
#include "LuaEngine/LuaEngine.h"
#include "RecordDataPlugin/RecordDataProxy.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextCodec>

using namespace Lua;
using namespace OOLUA;
using namespace System;
using namespace Controller;

#ifdef _CS_ARM_LINUX
#define DEST_DIR "/mnt/udisk/LF82_DataCurve_"
#define DEV_DIR_NAME    "/dev"
#define USB_STORAGE_DEV_NAME    "sd"
#else
#define DEST_DIR "E:/LF82_DataCurve_"
#endif

namespace  ResultDetail
{
LF82ResultDetailWidget::LF82ResultDetailWidget():
    MeasureResultDetailWidget(),m_maxValue(0),m_minValue(0)
{
    resize(810, 625);
    move(-5, -5);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);                        //设置为无标题栏窗体
    this->setWindowModality(Qt::WindowModal);//模态

    QFont font;
    font.setPointSize(10);

    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();
    m_lua = luaEngine->GetEngine();

    Table dataTable;
    luaEngine->GetLuaValue(state, "setting.ui.resultDetail.measure.data", dataTable);

    int dataWidth = 630;
    int dataHigh = 120;
    dataTable.safe_at("width", dataWidth);
    dataTable.safe_at("high", dataHigh);

    int editWidth = 90;
    dataTable.safe_at("editWidth", editWidth);

    String xLabel = "";
    String yLabel = "";
    dataTable.safe_at("xLabel", xLabel);
    dataTable.safe_at("yLabel", yLabel);

    oolua_ipairs(dataTable)
    {
        Table itemTable;
        m_lua->pull(itemTable);

        ResultDetailItem item;

        itemTable.safe_at("name", item.name);
        itemTable.safe_at("text", item.text);
        itemTable.safe_at("row", item.row);
        itemTable.safe_at("col", item.col);
        itemTable.safe_at("width", item.width);

        QLabel *label = new QLabel();
        label->setText(QString(item.text.c_str()));
        label->setFixedSize(item.width, 30);
        label->setFont(font);
        label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        item.itemLabel = label;

        QLineEdit *edit = new QLineEdit();
        edit->setText(QString(item.name.c_str()));
        edit->setFixedSize(editWidth, 30);
        edit->setFont(font);
        edit->setEnabled(false);
        edit->setStyleSheet("QLineEdit{border:0.5px solid lightgray;}");
        item.itemEdit = edit;

        m_resultDetailItems.push_back(item);
    }
    oolua_ipairs_end()

    m_groupBox = new QGroupBox(this);
    m_groupBox->setStyleSheet("QGroupBox{border:1px solid lightgray;}");
    m_groupBox->setFixedSize(dataWidth, dataHigh);

    QGridLayout *gridLayout = new QGridLayout();
    for (std::vector<ResultDetailItem>::iterator it = m_resultDetailItems.begin();
         it != m_resultDetailItems.end(); it++)
    {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget((*it).itemLabel);
        layout->addWidget((*it).itemEdit);

        gridLayout->addLayout(layout, (*it).row - 1, (*it).col - 1);
    }
    m_groupBox->setLayout(gridLayout);

    Table waveTable;
    luaEngine->GetLuaValue(state, "setting.ui.resultDetail.measure.wave", waveTable);

    int waveWidth;
    int waveHigh;
    waveTable.safe_at("width", waveWidth);
    waveTable.safe_at("high", waveHigh);

    m_plot = new QwtPlot(); //地址容器，里面存放其他对象  // 不加QString转换会提示无法通过编译，无法进行类型转换
    m_plot->setFixedSize(waveWidth, waveHigh);

    //设置画布
    QwtPlotCanvas *tempCanvas = new QwtPlotCanvas();
    tempCanvas->setBorderRadius(2);//画布四周边框角半径
    m_plot->setCanvas(tempCanvas);//设置绘图的画布
    m_plot->plotLayout()->setAlignCanvasToScales(true);//设置对齐画布、坐标轴、刻度
    m_plot->setAutoReplot(true); // 设置自动重画，相当于更新

    //设置光标
    QwtPlotPicker *picker =  new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                                  QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly,
                                        m_plot->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());
    picker->setRubberBandPen(QPen(QColor(255,200,150)));
    picker->setTrackerPen(QColor(Qt::black));

    //设置网格线
    QwtPlotGrid *tempGrid = new QwtPlotGrid;
    tempGrid->enableX(false);
    tempGrid->enableY(true);
    tempGrid->setMajorPen(Qt::lightGray, 0, Qt::DashLine);
    tempGrid->attach(m_plot);

    QwtLegend *tempLegend = new QwtLegend;//标注
    m_plot->insertLegend(tempLegend, QwtPlot::RightLegend);//插入位置，可以分别是上下左右
    m_plot->setAxisAutoScale(QwtPlot::yLeft, true);  //光标
    m_plot->setAxisTitle(QwtPlot::yLeft, QString::fromStdString(yLabel));
    m_plot->setAxisTitle(QwtPlot::xBottom, QString::fromStdString(xLabel));
    m_plot->setTitle(QString("Curve"));

    oolua_ipairs(waveTable)
    {
        Table itemTable;
        m_lua->pull(itemTable);

        CurveItem item;

        itemTable.safe_at("name", item.curveName);
        itemTable.safe_at("text", item.curveText);
        itemTable.safe_at("acronym", item.curveAcronym);
        itemTable.safe_at("color", item.curveColor);
        item.curveIndex = 0;
        item.curveLength = 0;

        System::String text = item.curveText + "(" + item.curveAcronym + ")";
        item.curvePlot = new QwtPlotCurve(QString(text.c_str())); //曲线类
        item.curvePlot->setPen(item.curveColor, 2);             //设置曲线颜色粗细
        item.curvePlot->setRenderHint(QwtPlotItem::RenderAntialiased, true);    //线条光滑化
        item.curvePlot->setLegendAttribute(item.curvePlot->LegendShowLine);
        item.curvePlot->attach(m_plot);
        item.curvePlot->setSamples(item.curveBufer);

        QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse,
        QBrush(item.curveColor), QPen(item.curveColor, 2), QSize(6, 6));//设置样本点的颜色形状大小
        item.curvePlot->setSymbol(symbol);  //添加样本点形状

        m_curveItems.push_back(item);
    }
    oolua_ipairs_end()

    QFont btnFont;
    btnFont.setPointSize(12);

    m_exportButton = new QPushButton(tr("曲线导出"));
    m_exportButton->setObjectName("brownButton");
    m_exportButton->setFixedHeight(40);
    m_exportButton->setFixedWidth(80);
    m_exportButton->setFocusPolicy(Qt::NoFocus);
    m_exportButton->setFont(btnFont);

    m_closeButton = new QPushButton(tr("关闭"));
    m_closeButton->setObjectName("brownButton");
    m_closeButton->setFixedHeight(40);
    m_closeButton->setFixedWidth(80);
    m_closeButton->setFocusPolicy(Qt::NoFocus);
    m_closeButton->setFont(btnFont);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_exportButton, 0, Qt::AlignCenter);
    buttonLayout->addWidget(m_closeButton, 0, Qt::AlignCenter);
    buttonLayout->addStretch();
    buttonLayout->setSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(m_groupBox, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(m_plot, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(Close()));
    connect(m_exportButton, SIGNAL(clicked()), this, SLOT(SlotExportButton()));

    luaEngine->GetLuaValue(state, "setting.ui.resultDetail.measure.updateRecord", this->updateRecordFunc);
}

void  LF82ResultDetailWidget::Show(QWidget *parent, int roleType)
{
    if(roleType >= 4)
    {
        m_exportButton->setVisible(true);
    }
    else
    {
        m_exportButton->setVisible(false);
    }
    this->setParent(parent, Qt::FramelessWindowHint | Qt::Dialog);
    this->show();
}

void LF82ResultDetailWidget::SlotExportButton()
{
    int length = 0;
    for (std::vector<CurveItem>::iterator it = m_curveItems.begin();it != m_curveItems.end(); it++)
    {
        if((*it).curveLength <= 0)
        {
            logger->info("曲线数据异常");
            return;
        }
        if(length != 0 && length != (*it).curveLength)
        {
            logger->info("曲线数据异常");
            return;
        }
        length = (*it).curveLength;
    }

#ifdef _CS_ARM_LINUX
    QDir dev("/dev/");
    QStringList nameFilters;
    nameFilters<<"sd*";
    QStringList sdList = dev.entryList(nameFilters, QDir::System);
    if(sdList.empty())
    {
        logger->info("曲线导出失败");
        return;
    }
#endif

    std::string sn;
    LuaEngine* luaEngine = LuaEngine::Instance();
    lua_State * state = luaEngine->GetThreadState();
    luaEngine->GetLuaValue(state, "config.info.instrument.sn", sn);


    QString dirPath = DEST_DIR + QString::fromStdString(sn);
    QDir dir;
    if(!dir.mkpath(dirPath))
    {
        logger->info("曲线导出失败");
        return;
    }

    QString fileName = "Curve";
    for (std::vector<CurveItem>::iterator it = m_curveItems.begin();it != m_curveItems.end(); it++)
    {
        fileName += QString("_%1%2").arg(QString::fromStdString((*it).curveAcronym)).arg(QString::number((*it).curveIndex));
    }
    fileName += ".csv";

    QString fPath = dirPath+"/"+fileName;
    QFile file(fPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        logger->info("曲线导出失败");
        return;
    }

    QTextStream out(&file);
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    out.setCodec(codec);

    for (std::vector<CurveItem>::iterator it = m_curveItems.begin();
         it != m_curveItems.end(); it++)
    {
        QString curveName = QString::fromStdString((*it).curveName);
        out<<curveName<<" ,";
    }
    out<<"\n";

    for(int i = 0; i< length; i++)
    {
        for (std::vector<CurveItem>::iterator it = m_curveItems.begin();
             it != m_curveItems.end(); it++)
        {
            out<<(*it).curveBufer.at(i).y()<<" ,";
        };
        out<<"\n";
    }
    out<<endl;

    file.close();

#ifdef    _CS_ARM_LINUX
    system("sync");
#endif
    logger->info("曲线导出成功");
}

void LF82ResultDetailWidget::Close()
{
    m_plot->setTitle(QString("Curve"));
    this->close();
}

bool LF82ResultDetailWidget::UpdateRecord()
{
    bool ret = false;

    try
    {
        lua_State * newState = LuaEngine::Instance()->GetThreadState();
        OOLUA::Lua_function call(newState);
        ret = call(this->updateRecordFunc, &m_recordData);
    }
    catch(OOLUA::Exception e)
    {
        logger->warn("LF82CalibrateCurveWidget::UpdateRecord() => %s", e.what());
    }
    catch(std::exception e)
    {
        logger->warn("LF82CalibrateCurveWidget::UpdateRecord() => %s", e.what());
    }

    return ret;
}

void LF82ResultDetailWidget::UpdateDataInfo(System::String dataName, System::String value)
{
    //qDebug()<<"LF82ResultDetailWidget::UpdateDataInfo "<<QString(dataName.c_str())<<" = "<<QString(value.c_str());
    for (std::vector<ResultDetailItem>::iterator it = m_resultDetailItems.begin();
         it != m_resultDetailItems.end(); it++)
    {
        if((*it).name == dataName)
        {
            (*it).itemEdit->setText(QString(value.c_str()));
        }
    }
}

void LF82ResultDetailWidget::UpdateWaveInfo(System::String curveName, System::Uint32 curveIndex)
{
    //qDebug()<<"LF82ResultDetailWidget::UpdateWaveInfo "<<QString(curveName.c_str())<<" curveIndex = "<<curveIndex;
    for (std::vector<CurveItem>::iterator it = m_curveItems.begin();
         it != m_curveItems.end(); it++)
    {
        if((*it).curveName == curveName)
        {
            LF82CurveData curve = LF82CurveManager::Instance()->GetCurve(curveIndex);
            (*it).curveIndex = curveIndex;
            (*it).curveLength = curve.GetLength();

            QString fileName = m_plot->title().text();
            fileName += QString("_%1%2").arg(QString::fromStdString((*it).curveAcronym)).arg(QString::number((*it).curveIndex));
            m_plot->setTitle(fileName);

            if(curve.GetMaxData() > m_maxValue)
            {
                m_maxValue = curve.GetMaxData();
            }
            if(curve.GetMinData() < m_minValue)
            {
                m_minValue = curve.GetMinData();
            }
            if (curve.GetLength() > 0)
            {
                Uint32 length = curve.GetLength();

                (*it).curveBufer.clear();

                for (Uint32 i = 0; i < length; i++)
                {
                    float current = curve.GetData(i);
                    (*it).curveBufer.append(QPointF(i, current));
                }
            }
            else
            {
                (*it).curveBufer.clear();
            }
            (*it).curvePlot->attach(m_plot);
            (*it).curvePlot->setSamples((*it).curveBufer);
        }
    }
    //qDebug()<<"m_plot UpdateWaveInfo m_minValue = "<<m_minValue<<"m_maxValue = "<<m_maxValue;
    m_plot->setAxisScale(QwtPlot::yLeft, m_minValue, m_maxValue*1.2);
}

void LF82ResultDetailWidget::SetResultInfo(QString info)
{
    m_infoLabel->setText(info);
}

void LF82ResultDetailWidget::ResetScaleValue()
{
    m_minValue = 0;
    m_maxValue = 0.000001;
}

}
