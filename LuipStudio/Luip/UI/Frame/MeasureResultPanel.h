#ifndef UI_FRAME_MeasureResultPanel_H
#define UI_FRAME_MeasureResultPanel_H


#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QString>
#include "ResultIterm.h"

namespace UI
{
    struct TargetIterm
    {
        QString name;
        ResultIterm * iterm;
    };

class MeasureResultPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MeasureResultPanel( QList<QString> targets, QString resultType, QString unitStr, QWidget *parent = 0);
    ~MeasureResultPanel();

    void SetResultInfo(QString target, QString result, QString dateTime, QString resultType);
    void SetResultAlarm(QString target, bool isAlarm);

private:
    QVBoxLayout *measureResultLayout;  //测量面板布局
    QVector <TargetIterm> m_resultMap;

    int m_targetNum;
    QLabel *m_resultTypeLabel;  // 结果类型

};

}
#endif // UI_FRAME_MeasureResultPanel_H
