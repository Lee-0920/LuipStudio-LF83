#ifndef INSTRUMENTINFORMATIONWIDGET_H
#define INSTRUMENTINFORMATIONWIDGET_H
#include <QPushButton>
#include <QFont>
#include <QTableWidget>
#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include "UI/UserChangeManager/IUserChangeNotifiable.h"
#include <iostream>

namespace UI
{

class InstrumentInformationWidget: public QWidget, public IUserChangeNotifiable
{
Q_OBJECT
public:
    explicit InstrumentInformationWidget(QWidget *parent = 0);
    ~InstrumentInformationWidget();
    void OnUserChange();
private:
    QLabel *typeName;
    QLabel *nameValue;
    QLabel *argumentName;
    QLabel *serialnumberName;
    QLabel *typeNumberName;
    QLabel *versionName;
    QLabel *platformName;
    QLabel *firmName;
    QPushButton *m_setSNButton;

    QLabel *qrencodeLabel;

    void ChangeBottomStatus();
    void InfoDisplay();

protected:
    void showEvent(QShowEvent *event);

private slots:
    void SlotSetSNButton();
    void UpdateQrencode();

    //   void slotDoubleClicked(QTableWidgetItem *item);
};

}
#endif // INSTRUMENTINFORMATIONWIDGET_H
