#include "SingalWindow.h"

namespace UI
{

SingalWindow::SingalWindow(QWidget *parent) :
        QWidget(parent)
{
    listView = new QListView();
    listView->setFixedHeight(400);
    listView->setFixedWidth(101);

    QVBoxLayout *listViewLayout = new QVBoxLayout();
    listViewLayout->addWidget(listView);
    listViewLayout->setContentsMargins(0, 11, 0, 0);
    listViewLayout->addStretch();

    stackedWidget = new QStackedWidget();
    stackedWidget->setFixedHeight(450);
    stackedWidget->setFixedWidth(650);
    stackedWidget->setObjectName(QStringLiteral("stackedWidget"));

    QHBoxLayout *stackedWidgetLayout = new QHBoxLayout();
    stackedWidgetLayout->addWidget(stackedWidget);

    stackedWidgetLayout->addStretch();
    stackedWidgetLayout->setContentsMargins(10, 0, 0, 0);

    QVBoxLayout *vstackedWidgetLayout = new QVBoxLayout();
    vstackedWidgetLayout->addLayout(stackedWidgetLayout);
    vstackedWidgetLayout->addStretch();
    vstackedWidgetLayout->setContentsMargins(0, 3, 0, 0);

    QStringList num;
    num<<"基本信号";
    model = new QStringListModel(num);
    listView->setGridSize(QSize(50,40));
    listView->setModel(model);

    QFont listFont = listView->font();
    listFont.setPointSize(12);
    listView->setFont(listFont);

    basicSignal = new SignalWidget();
    stackedWidget->addWidget(basicSignal);

    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

    QHBoxLayout *mainlayout = new QHBoxLayout();

    mainlayout->addLayout(listViewLayout);
    mainlayout->addLayout(vstackedWidgetLayout);
    mainlayout->setContentsMargins(10, 5, 0, 0);

    stackedWidget->setCurrentWidget(basicSignal);
     this->setLayout(mainlayout);
}

void SingalWindow::itemClicked(QModelIndex index)
{
    QStringList strList;
    strList.append("基本信号");

    if(strList.at(0) == index.data().toString())
    {
        stackedWidget->setCurrentWidget(basicSignal);
    }

    else
    {
    }
}

SingalWindow::~SingalWindow()
{
    if (basicSignal)
        delete basicSignal;
}

}
