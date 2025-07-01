#ifndef WAVEDATA_H
#define WAVEDATA_H

#include <QWidget>
#include <QPushButton>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_scale_draw.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_picker_machine.h>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qwt_scale_widget.h>
#include <qpen.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <QTableWidget>
#include <QMap>
#include <qwt_scale_draw.h>
#include <QDateTime>
#include "ResultManager/ResultManager.h"
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "UI/Frame/IUpdateWidgetNotifiable.h"

using Result::IResultNotifiable;

namespace UI
{

struct CurveCategory
{
    QString name;
    QString text;
    Qt::GlobalColor color;
    QVector<QPointF> buffer;
    QwtPlotCurve *curve;
};

struct WaveObject
{
    QwtPlot *plot;
    QString content;
    QString title;
    float yUpLimit;
    float yLowLimit;
    int width;
    int height;
    QList<CurveCategory> curveList;
};

class WaveData: public QWidget , public IUserChangeNotifiable, public IUpdateWidgetNotifiable , public IResultNotifiable
{
    Q_OBJECT
public:
    explicit WaveData(System::String resultDataname, QWidget *parent = 0);
    ~WaveData();
    void OnUserChange();
    void OnUpdateWidget(UpdateEvent event, System::String message);
    virtual void OnMeasureResultAdded(String name, ResultData::RecordData result);
    virtual void OnCalibrateResultAdded(String name, ResultData::RecordData result);

private:
    void InitWave();
    void LoadWaveData();
    void UpdateData();

    int DateTimeToInt(QDateTime dateTime);
    QDateTime GetBackDateTime(int iBackDays);

private slots:
    void SlotBackButton();
    void SlotNextButton();

private:

    QDateTime m_ObjBackDateTime;
    QDateTime m_ObjNextDateTime;
    QPushButton *backButton;
    QPushButton *nextButton;

    Result::MeasureRecordFile *m_resultFiles;

    OOLUA::Script *m_lua;
    System::String m_resultDataname;

    QList<WaveObject*> m_waveList;
};

class CTimeScaleDraw : public QwtScaleDraw
{
public:
    CTimeScaleDraw(const QDateTime base, const int day, const int pointCount)
        : baseTime(base)
        , displayDay(day)
        , oneDayPoints(pointCount)
    {
    }

    virtual QwtText label(double v) const
    {
        QDateTime dateTime = baseTime.addSecs(static_cast<uint>(v) * 60 *60 * 24 / 50);

        return dateTime.toString("MM-dd");
    }

private:
    QDateTime baseTime;
    int displayDay;
    int oneDayPoints;
};

class VScaleDraw: public QwtScaleDraw        //自画坐标轴
{
public:
    VScaleDraw()
    {

    }
    virtual QwtText label(double v) const  //重绘坐标轴刻度值
    {
        return QwtText(QString::number(v));
    }
};

}
#endif // WAVEDATA_H
