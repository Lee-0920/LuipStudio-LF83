#ifndef UI_HOME_RUNSTATUSWINDOW_H
#define UI_HOME_RUNSTATUSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QSplitter>
#include <QProgressBar>
#include <QComboBox>
#include <QGroupBox>
#include <QPainter>
#include <QPoint>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QPushButton>
#include <QDialog>
#include "UI/Frame/MeasureResultPanel.h"
#include "FlowManager/IScheduleNotifiable.h"
#include "FlowManager/MeasureScheduler.h"
#include "ResultManager/IResultNotifiable.h"
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include <QDateTime>
#include "StatusManager/StatusManager.h"
#include "StatusManager/IStatusNotifiable.h"
#include "ResultManager/ResultManager.h"
#include "UI/Frame/UpdateWidgetManager.h"

using Flow::IScheduleNotifiable;
using Measure::IStatusNotifiable;
using UI::MeasureResultPanel;
using UI::IUserChangeNotifiable;
using Measure::RunStatus;
using Measure::RunAction;
using Measure::StatusManager;

namespace Ui {
class Widget;
}

struct RunStatusIterm
{
    System::String name;
    OOLUA::Lua_func_ref getData;
    bool isNeedResultType;
    OOLUA::Lua_func_ref getResultType;
    OOLUA::Lua_func_ref isAlarmFunc;
    bool isAlarm;
};

class RunStatusWindow:
    public QWidget,
    public IScheduleNotifiable,
    public Result::IResultNotifiable,
    public IUserChangeNotifiable,
    public IStatusNotifiable,
    public IUpdateWidgetNotifiable
{
Q_OBJECT
public:
    explicit RunStatusWindow(QWidget *parent = 0);
    ~RunStatusWindow();

public:
    void SetProductType(char *){}
    void SetMeasureTime(char *){}
    void SetMeasureResult(char *){}
    void SetEstimateResultTime(char *){}
    void SetOnline(bool){}
    void SetOutTime(bool){}

    void OnScheduler(QString content);
    void OnUserChange();
    void OnMeasureResultAdded(String name, ResultData::RecordData recordData);
    void OnCalibrateResultAdded(String name, ResultData::RecordData recordData);

    void OnStattusChanged(RunStatus status);
    void OnStattusDelay(int time);
    void OnActionChanged(RunAction action);
    void OnActionDelay(int time);

    void OnUpdateWidget(UpdateEvent event, System::String message);

protected:   
    QSplitter *mainSplitter;

    QSplitter *leftSplitter;
    QWidget *leftWidget;
    QWidget *leftTopWidget;
    QLabel *productTypeLabel;      //产品型号
    QLabel *measureResultLabel;    //测量结果显示
    QLabel *measureTimeLabel;     //测量结果时间
    QLabel *estimateResultTimeLable;    //定标时间
    QPushButton *stopButton;
    QPushButton *oneKeyButton;

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::Widget *ui;
    void initLeft();
    void initLeftTop();
    void initLeftBottom();
    void ChangeBottomStatus();

    void MeasureResultPanelDisplayData();
    void MeasureResultPanelUpdateStatus();

    void RefreshAutoMeasure();
    void RefreshReportMode();

    StatusManager* statusManager;

    char *productType;
    char *measureTime;
    char *measureResult;
    char *estimateResultTime;
    QComboBox *measureModeComBox;
    QComboBox *reportModeComBox;
    QHBoxLayout *measureLayout;
    std::vector<RunStatusIterm> m_items;

    MeasureResultPanel *m_resultPanel;

    OOLUA::Script *m_lua;
    QPoint m_pressPoint;//按下的点坐标
    bool m_isMousePress;//是否按下
    QDateTime m_pressTime;//按下的时间

    bool m_isUnitChange;
    OOLUA::Lua_func_ref m_unitChangeFunc;
	
signals:
    void SignalOneKeyChangeReagent();
    void SignalUpdateExpectInfo(QString str);

private slots:
    void StopFlow();
    void SlotOneKeyButton();
    void SlotChangeAutoMeasure();
    void SlotUpdateExpectInfo(QString str);
    void SlotChangeReportMode();
};

#endif // UI_HOME_RUNSTATUSWINDOW_H
