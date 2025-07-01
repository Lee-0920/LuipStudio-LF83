#ifndef UI_SINGALWINDOW_H
#define UI_SINGALWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QStackedWidget>
#include <QStringListModel>
#include "SignalWidget.h"

namespace UI
{

class SingalWindow: public QWidget
{
Q_OBJECT
public:
    explicit SingalWindow(QWidget *parent = 0);
    ~SingalWindow();

public:

private:
    QListView *listView;
    QWidget *autoWidget;
    SignalWidget *basicSignal;
    QAbstractItemModel *model;
    QStackedWidget *stackedWidget;
private slots:
    void itemClicked(QModelIndex index);
};

}

#endif // SINGALWINDOW_H
