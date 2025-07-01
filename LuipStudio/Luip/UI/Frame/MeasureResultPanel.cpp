
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMap>
#include <QString>

#include "MeasureResultPanel.h"

namespace UI
{

MeasureResultPanel::MeasureResultPanel(QList<QString> targets, QString resultType, QString unitStr, QWidget *parent) :
        QWidget(parent)
{
    if (!targets.empty())
    {
        m_resultTypeLabel  = new QLabel();
        m_resultTypeLabel->setObjectName(QStringLiteral("measureLabel"));
        m_resultTypeLabel->setFixedSize(100, 30);
        QFont resultTypeLabelFont = m_resultTypeLabel->font();
        resultTypeLabelFont.setPointSize(12);
        m_resultTypeLabel->setFont(resultTypeLabelFont);
        m_resultTypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft );
        m_resultTypeLabel->setText(resultType);

        QHBoxLayout *typeHLayout = new QHBoxLayout();
        typeHLayout->addWidget(m_resultTypeLabel);
        typeHLayout->addStretch();
        typeHLayout->setSpacing(0);
        typeHLayout->setContentsMargins(2, 2, 0, 0);

        QVBoxLayout *typeVLayout = new QVBoxLayout();
        typeVLayout->addLayout(typeHLayout);

        for (int i = 0; i < targets.size(); i++)
        {
            TargetIterm targetIterm;
            targetIterm.name = targets.at(i);
            targetIterm.iterm = new ResultIterm(targets.at(i), tr("0.0000"),
                                                 tr("0000-00-00 00:00:00"), unitStr, this);
            m_resultMap.append(targetIterm);
        }

        QGroupBox *meaResultGroupBox = new QGroupBox();
        meaResultGroupBox->setObjectName(QStringLiteral("meaResultGroupBox"));
        meaResultGroupBox->setFixedWidth(480);
        meaResultGroupBox->setMinimumHeight(60);
        meaResultGroupBox->setMaximumHeight(300);

        QVBoxLayout *itermLayout = new QVBoxLayout();
        itermLayout->addStretch();
        QVector <TargetIterm> ::Iterator it;
        for(it = m_resultMap.begin(); it != m_resultMap.end(); ++it)
        {
            itermLayout->addWidget((*it).iterm);
        }
        itermLayout->addStretch();
        ///itermLayout->setSpacing(5);
        itermLayout ->setContentsMargins(10, 0, 10, 20);

        QVBoxLayout *resultPanelLayout = new QVBoxLayout();
        resultPanelLayout->addLayout(typeVLayout);
        resultPanelLayout->addStretch();
        resultPanelLayout->addLayout(itermLayout);
        resultPanelLayout->addStretch();
        resultPanelLayout->setSpacing(0);

        meaResultGroupBox->setLayout(resultPanelLayout);

        QHBoxLayout *meaResultGroupLayout = new QHBoxLayout();
        meaResultGroupLayout->addStretch();
        meaResultGroupLayout->addWidget(meaResultGroupBox);
        meaResultGroupLayout->addStretch();
        meaResultGroupLayout->setSpacing(0);
        meaResultGroupLayout->setContentsMargins(0, 0, 0, 0);
        this->setLayout(meaResultGroupLayout);
    }
}

MeasureResultPanel::~MeasureResultPanel()
{

}

void MeasureResultPanel::SetResultAlarm(QString target, bool isAlarm)
{
    QVector <TargetIterm> ::Iterator it;
    for(it = m_resultMap.begin(); it != m_resultMap.end(); ++it)
    {
        if ((*it).name == target)
        {
            ResultIterm* iterm = (*it).iterm;
            iterm->SetResultAlarm(isAlarm);
        }
    }
}

void MeasureResultPanel::SetResultInfo(QString target, QString result, QString dateTime, QString resultType)
{
    QVector <TargetIterm> ::Iterator it;
    for(it = m_resultMap.begin(); it != m_resultMap.end(); ++it)
    {
        if ((*it).name == target)
        {
            m_resultTypeLabel->setText(resultType);

            ResultIterm* iterm = (*it).iterm;

            iterm->SetMeasureTarget(target);
            iterm->SetMeasureResult(result);
            iterm->SetMeasureTime(dateTime);  
        }
    }
}


}
