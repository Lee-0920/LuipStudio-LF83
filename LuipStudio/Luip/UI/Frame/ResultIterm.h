#ifndef UI_FRAME_RESULTITERM_H
#define UI_FRAME_RESULTITERM_H

#include <QWidget>
#include <QLabel>
#include <QString>

namespace UI
{
class ResultIterm : public QWidget
{
    Q_OBJECT
public:
    explicit ResultIterm(QString strTarget, QString strResult,
                         QString strDateTime, QString strUnit, QWidget *parent = 0);
    ~ResultIterm();

    void SetMeasureTarget(QString strTarget);
    void SetMeasureResult(QString strResult);
    void SetMeasureTime(QString strDateTime);
    void SetResultAlarm(bool isAlarm);

private:

    void DateTimeStringAnalysis(QString &dateTime, QString &date, QString &time);

    QLabel * m_targetLabel;         // 测量参数类型
    QLabel *m_resultLabel;    //测量结果显示
    QLabel *m_dateLabel;     //测量结果日期
    QLabel *m_timeLabel;     //测量结果时间
    QLabel *m_unitLabel;     // 单位
};

}
#endif // UI_FRAME_RESULTITERM_H
