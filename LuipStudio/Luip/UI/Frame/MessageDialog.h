/*
 */

#ifndef UI_FRAME_MESSAGEBOXDIALOG_H
#define UI_FRAME_MESSAGEBOXDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "Common.h"
#include "UI/Frame/NumberKeyboard.h"
#include <QTimer>

namespace UI
{

enum MsgStyle
{
    ONLYOK,OKANDCANCEL,ONLYCANCEL
};

class MessageDialog: public QDialog
{
Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = 0);
    explicit MessageDialog(const QString &text,
            QWidget *parent = 0, MsgStyle msgstyle = MsgStyle::ONLYOK);
    explicit MessageDialog(const QString &text, int deadTime, QWidget *parent = 0,  MsgStyle msgstyle = MsgStyle::ONLYCANCEL);

    ~MessageDialog();
    void SetText(const QString &text);

signals:
    void SignalDeadTimeReboot();

private slots:
    void SlotConfirmButton();
    void SlotCancelButton();
    void SlotDeadTimeReboot();
    void SlotDeadTimeCount();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel *m_logoLabel;
    QPushButton *m_confirmButton;
    QPushButton *m_cancelButton;
    QLabel *m_textLabel;

    QTimer *deadTimer;
    int m_deadTime;
    QLabel *m_deadLabel;
    void Init(const QString &text, MsgStyle msgstyle, bool withDeadTime = false);
};

}

#endif //UI_FRAME_MESSAGEBOXDIALOG_H
