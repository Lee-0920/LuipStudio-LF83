#ifndef LF82RESULTDETAILWIDGET_H
#define LF82RESULTDETAILWIDGET_H

#include "ResultDetailPlugin/MeasureResultDetailWidget.h"
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
#include<qwt_plot.h>
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
#include <qpen.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <QDateTime>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include "oolua.h"

namespace  ResultDetail
{

struct ResultDetailItem
{
    System::String name;
    System::String text;
    int row;
    int col;
    int width;
    QLabel* itemLabel;
    QLineEdit* itemEdit;
};

struct CurveItem
{
    System::String curveName;
    System::String curveText;
    System::String curveAcronym;
    Qt::GlobalColor curveColor;
    QwtPlotCurve *curvePlot;
    QVector<QPointF> curveBufer;
    int curveIndex;
    int curveLength;
};

class LF82ResultDetailWidget : public MeasureResultDetailWidget
{
    Q_OBJECT
public:
    LF82ResultDetailWidget();
    void Show(QWidget *parent, int roleType = 0);
    bool UpdateRecord();
    void SetResultInfo(QString info);

    void UpdateDataInfo(System::String dataName, System::String value);
    void UpdateWaveInfo(System::String curveName, System::Uint32 curveIndex);
    void ResetScaleValue();
private:
    OOLUA::Script *m_lua;

    QwtPlot *m_plot;
    QwtPlotCurve *m_sampleCurve;
    QwtPlotCurve *m_blankCurve;
    QwtPlotCurve *m_standardCurve;

    QLabel *m_infoLabel;

//    QVector<QPointF> m_sampleBuffer;
//    QVector<QPointF> m_blankBuffer;
//    QVector<QPointF> m_standardBuffer;

    std::vector<CurveItem> m_curveItems;

    QPushButton *m_exportButton;
    QPushButton *m_closeButton;
    OOLUA::Lua_func_ref updateRecordFunc;

    QGroupBox *m_groupBox;
    std::vector<ResultDetailItem> m_resultDetailItems;

    float m_maxValue;
    float m_minValue;

    int m_index;
public slots:
    void Close();
    void SlotExportButton();
};

}
#endif // LF82RESULTDETAILWIDGET_H
