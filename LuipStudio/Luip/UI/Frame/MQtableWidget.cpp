#include "MQtableWidget.h"
MQtableWidget::MQtableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    this->horizontalHeader()->setSectionsClickable(false); //设置表头不可点击（默认点击后进行排序）
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setHighlightSections(false); // 点击表时不对表头行光亮（获取焦点）
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(255, 255, 245);}");
    QFont dataFont = this->font();
    dataFont.setPointSize(13);
    this->setFont(dataFont);
    this->verticalHeader()->setVisible(false); // 隐藏列表头
    this->setFrameShape(QFrame::StyledPanel); //设置无边框
    this->setShowGrid(true); //设置不显示格子线
    this->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

    //设置水平滚动条样式
    this->horizontalScrollBar()->setStyleSheet
            ("QScrollBar{background:transparent; height:1px;}"
             "QScrollBar::handle{background:lightgray; border:1px solid transparent; border-radius:1px;}"
             "QScrollBar::handle:hover{background:gray;}"
             "QScrollBar::sub-line{background:transparent;}"
             "QScrollBar::add-line{background:transparent;}");
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);// 设置不可拖拽
    this->horizontalScrollBar()->setVisible(false);
    this->horizontalScrollBar()->setDisabled(true);
}

void MQtableWidget::setColumn(QStringList columnName)
{
    QFont headerFont;
    headerFont.setPointSize(10);
    for(int i = 0;i< columnName.size();i++)
    {
        QTableWidgetItem *column = new QTableWidgetItem(columnName.at(i));
        column->setFont(headerFont);
        this->setHorizontalHeaderItem(i, column);
    }
}
