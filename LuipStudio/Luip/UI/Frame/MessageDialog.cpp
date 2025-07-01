#include "MessageDialog.h"
#include <QDebug>
#include <QProcess>
#include <QDebug>

namespace UI
{

MessageDialog::MessageDialog(QWidget *parent):QDialog(parent),deadTimer(nullptr)
{
    Init("", MsgStyle::ONLYOK);
}

MessageDialog::MessageDialog(const QString &text,
      QWidget *parent, MsgStyle msgstyle) :QDialog(parent),deadTimer(nullptr)
{
    Init(text, msgstyle);
}

MessageDialog::MessageDialog(const QString &text,int deadTime, QWidget *parent, MsgStyle msgstyle) :QDialog(parent),deadTimer(nullptr)
{
    m_deadTime = deadTime;
    if(m_deadTime > 0)
    {
        Init(text, msgstyle, true);
    }
    else
    {
        Init(text, msgstyle, false);
    }
}

void MessageDialog::Init(const QString &text, MsgStyle msgstyle, bool withDeadTime)
{
#ifdef _CS_X86_WINDOWS
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
#endif

    this->resize(280, 205);

    m_logoLabel = new QLabel(this);

    m_logoLabel->setFixedSize(36, 36);
#if !OEM
    m_logoLabel->setPixmap(QPixmap(":/img/WQIMC"));
#endif
    m_logoLabel->setScaledContents(true);

    QHBoxLayout *logoLayout = new QHBoxLayout();
    logoLayout->addStretch();
    logoLayout->addWidget(m_logoLabel,1,Qt::AlignRight|Qt::AlignVCenter);
    logoLayout->setSpacing(0);

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addLayout(logoLayout);
    topLayout->addStretch();
    topLayout->setSpacing(0);

    QFont font;
    font.setPointSize(14);

    m_textLabel = new QLabel(this);
    m_textLabel->setObjectName(QStringLiteral("whiteLabel"));
    m_textLabel->setFont(font);
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_textLabel->setText(text);
    m_textLabel->setWordWrap(true);
    m_textLabel->adjustSize();

    if(withDeadTime == true)
    {
        m_deadLabel = new QLabel(this);
        m_deadLabel->setObjectName(QStringLiteral("whiteLabel"));
        m_deadLabel->setFont(font);
        m_deadLabel->setFixedSize(20, 20);
        m_deadLabel->setAlignment(Qt::AlignCenter);
        m_deadLabel->setText(QString::number(m_deadTime));
    }

    m_confirmButton = new QPushButton(this);
    m_confirmButton->setObjectName(QStringLiteral("brownButton"));
    m_confirmButton->setFixedSize(60, 30);
    m_confirmButton->setFont(font);
    m_confirmButton->setFocus();
    m_confirmButton->setDefault(true);
    m_confirmButton->setText("确认");
    connect(m_confirmButton, SIGNAL(clicked()), this, SLOT(SlotConfirmButton()));

    m_cancelButton = new QPushButton(this);
    m_cancelButton->setObjectName(QStringLiteral("brownButton"));
    m_cancelButton->setFixedSize(60, 30);
    m_cancelButton->setFont(font);
    m_cancelButton->setFocus();
    m_cancelButton->setDefault(true);
    m_cancelButton->setText("取消");
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(SlotCancelButton()));

    QHBoxLayout *bottomBtnLayout = new QHBoxLayout();
    switch(msgstyle)
    {
    case MsgStyle::ONLYOK:
        bottomBtnLayout->addWidget(m_confirmButton);
        m_cancelButton->hide();
        break;
    case MsgStyle::OKANDCANCEL:
        bottomBtnLayout->addWidget(m_confirmButton);
        bottomBtnLayout->addWidget(m_cancelButton);
        break;
    case MsgStyle::ONLYCANCEL:
        m_confirmButton->hide();
        bottomBtnLayout->addWidget(m_cancelButton);
        break;
    }
    bottomBtnLayout->setSpacing(10);

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(m_textLabel,1,Qt::AlignHCenter);
    if(withDeadTime == true)
    {
        bottomLayout->addWidget(m_deadLabel, 1, Qt::AlignHCenter);
    }
    bottomLayout->addStretch(2);
    bottomLayout->addLayout(bottomBtnLayout,1);
    bottomLayout->addStretch(1);
    bottomLayout->setSpacing(10);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout,1);
    mainLayout->addLayout(bottomLayout,3);
    mainLayout->setSpacing(10);

    if(withDeadTime == true)
    {
        deadTimer = new QTimer(this);
        deadTimer->setInterval(1000);

        connect(this->deadTimer, SIGNAL(timeout()), this, SLOT(SlotDeadTimeCount()));

        connect(this, SIGNAL(SignalDeadTimeReboot()), this, SLOT(SlotDeadTimeReboot()));

        deadTimer->start();
    }

    this->setLayout(mainLayout);
}

MessageDialog::~MessageDialog()
{

}

void MessageDialog::SlotConfirmButton()
{
    if(deadTimer != nullptr)
    {
        deadTimer->stop();
    }
    this->accept();
}

void MessageDialog::SlotCancelButton()
{
    if(deadTimer != nullptr)
    {
        deadTimer->stop();
    }
    this->reject();
}

void MessageDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    int height = 50;
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawPixmap(
            QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - 2 * SHADOW_WIDTH,
                  this->height() - 2 * SHADOW_WIDTH), QPixmap(DEFAULT_SKIN)); //DEFAULT_SKIN
    painter.drawRect(
            QRect(SHADOW_WIDTH, height, this->width() - 2 * SHADOW_WIDTH,
                  this->height() - height - SHADOW_WIDTH));
}


void MessageDialog::SlotDeadTimeCount()
{
    m_deadTime--;
    this->m_deadLabel->setText(QString::number(m_deadTime));
    if(m_deadTime <= 0)
    {
        if(deadTimer != nullptr)
        {
            deadTimer->stop();
        }
        emit SignalDeadTimeReboot();
    }
}

void MessageDialog::SlotDeadTimeReboot()
{
    #ifdef _CS_ARM_LINUX
        QProcess::execute("reboot");
    #endif
    this->accept();
}

void MessageDialog::SetText(const QString &text)
{
    m_textLabel->setText(text);
}
}
