#ifndef UI_FRAME_MQTABLEWIDGET_H
#define UI_FRAME_MQTABLEWIDGET_H
#include <QtGui>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>

class MQtableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MQtableWidget(QWidget *parent = 0);
    void setColumn(QStringList columnName);
};

#endif // UI_FRAME_MQTABLEWIDGET_H
