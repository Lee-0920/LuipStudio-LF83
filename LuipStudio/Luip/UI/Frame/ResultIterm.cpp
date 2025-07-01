#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ResultIterm.h"

namespace UI
{

ResultIterm::ResultIterm(QString strTarget, QString strResult,
                         QString strDateTime, QString strUnit, QWidget *parent)
    : QWidget(parent)
{
    m_targetLabel  = new QLabel();
    m_targetLabel->setObjectName(QStringLiteral("measureLabel"));
    m_targetLabel->setFixedSize(100, 50);
    QFont measureParamFont = m_targetLabel->font();
    measureParamFont.setPointSize(20);
    m_targetLabel->setFont(measureParamFont);
    m_targetLabel->setAlignment(Qt::AlignCenter);
    m_targetLabel->setText(strTarget);

    m_resultLabel = new QLabel();
    m_resultLabel->setStyleSheet("color:rgb(0, 255, 0);");
    m_resultLabel->setFixedSize(220, 80);
    QFont measureResultFont = m_resultLabel->font();
    measureResultFont.setPointSize(48);
    //measureResultFont.setBold(true);
    m_resultLabel->setFont(measureResultFont);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    m_resultLabel->setMargin(0);
    m_resultLabel->setText(strResult);

    m_unitLabel = new QLabel();
    m_unitLabel->setObjectName(QStringLiteral("measureLabel"));
    m_unitLabel->setFixedSize(80, 50);
    QFont uintFont = m_unitLabel->font();
    uintFont.setPointSize(32);
    m_unitLabel->setFont(uintFont);
    m_unitLabel->setAlignment(Qt::AlignCenter);
    m_unitLabel->setText(strUnit);

    m_timeLabel = new QLabel();
    m_timeLabel->setObjectName(QStringLiteral("measureLabel"));
    m_timeLabel->setFixedSize(360,40);
    QFont timeFont = m_timeLabel->font();
    timeFont.setPointSize(14);
    m_timeLabel->setFont(timeFont);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setText(strDateTime);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(m_targetLabel);
    centerLayout->addWidget(m_resultLabel);
    centerLayout->addWidget(m_unitLabel);
    centerLayout->setSpacing(2);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(m_timeLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    mainLayout->setSpacing(30);

    this->setLayout(mainLayout);
    ///this->setStyleSheet("QWidget{border:0.5px solid red;}");
}

ResultIterm::~ResultIterm()
{

}
void ResultIterm::SetMeasureTarget(QString strTarget)
{
    m_targetLabel->setText(strTarget);
}

void ResultIterm::SetMeasureResult(QString strResult)
{
    m_resultLabel->setText(strResult);
}

void ResultIterm::SetMeasureTime(QString strDateTime)
{
    m_timeLabel->setText(strDateTime);
}

void ResultIterm::DateTimeStringAnalysis(QString &dateTime, QString &date, QString &time)
{
    QStringList strList = dateTime.split(QRegExp("[ ]"));

    if (strList.size() == 2)
    {
        date = strList.at(0);
        time = strList.at(1);
    }
}

void ResultIterm::SetResultAlarm(bool isAlarm)
{
    if (isAlarm)
    {
        m_resultLabel->setStyleSheet("color:rgb(255, 0, 0);");
    }
    else
    {
        m_resultLabel->setStyleSheet("color:rgb(0, 255, 0);");
    }
}

}
