#ifndef FACTORYTIME_H
#define FACTORYTIME_H

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QtWidgets/QListView>
#include <QtWidgets/QStackedWidget>
#include <QLineEdit>
#include <QtWidgets/QStackedWidget>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QStringListModel>
#include <QComboBox>
#include <QPushButton>
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include "UI/Frame/IUpdateWidgetNotifiable.h"

namespace UI
{

class FactoryTimeWidget: public QWidget,  public IUserChangeNotifiable, public IUpdateWidgetNotifiable
{
    Q_OBJECT
public:
    explicit FactoryTimeWidget(QWidget *parent = 0);
    ~FactoryTimeWidget();
    void OnUserChange();
    void OnUpdateWidget(UpdateEvent event, System::String message);

protected:
    void showEvent(QShowEvent *event);

private:
    QAbstractItemModel *standardItemModel;
    QLineEdit *textEdit;
    QStackedWidget *stackedWidget;
    QTableWidget *measureResultTableWidget;
    QTableWidget *otherwidget;
    QPushButton *defaultPassword;
    QPushButton *defaultFactory;
    QPushButton *screenCalibrate;
    QPushButton *clearFault;
    QPushButton *activation;
    QPushButton *clearData;
    QLineEdit *minTime;
    QLineEdit *maxTime;
    QLabel *dateModel;
    QLabel *dateType;
    QComboBox *dateTypeCombobox;
    QComboBox *dateModelCombobox;
    QAbstractItemModel *model;

private slots:
    void SlotDefaultPassword();
    void SlotDefaultFactory();
    void SlotScreenCalibrate();
    void SlotClearData();
    void SlotActivation();
    void SlotClearFault();
};

}

#endif // FACTORYTIME_H
