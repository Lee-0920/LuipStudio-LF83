#include "ToolButton.h"

namespace UI
{

ToolButton::ToolButton(QString pic_name, QWidget *parent) :
        QToolButton(parent)
{
    //设置文本颜色
    QPalette textPalette = palette();
    textPalette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
    setPalette(textPalette);

    //设置文本粗体
    QFont &textFont = const_cast<QFont &>(font());
    textFont.setPointSize(12);
//    textFont.setWeight(QFont::Bold);

    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    //设置图标
    QPixmap pixmap(pic_name);
    setIcon(pixmap);
    setIconSize(pixmap.size());

    //设置大小
    setFixedSize(pixmap.width() + 32, pixmap.height() + 25);
    setAutoRaise(true);
//    setObjectName("transparentToolButton");

    m_mouseOver = false;
    m_mousePress = false;
}

ToolButton::~ToolButton()
{

}

void ToolButton::enterEvent(QEvent *)
{
    #ifdef _CS_X86_WINDOWS
        m_mouseOver = true;
    #endif
}

void ToolButton::leaveEvent(QEvent *)
{
    #ifdef _CS_X86_WINDOWS
        m_mouseOver = false;
    #endif
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

void ToolButton::SetMousePress(bool mousePress)
{
    this->m_mousePress = mousePress;
    update();
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    if (m_mouseOver)
    {
        //绘制鼠标移到按钮上的按钮效果
        painterInfo(0, 100, 150);
  //      painterInfo(255,255,255);
    }
    else
    {
        if (m_mousePress)
        {
            painterInfo(0, 100, 150);
        }
    }

    QToolButton::paintEvent(event);
}

void ToolButton::painterInfo(int topColor, int middleColor, int bottomColor)
{
    QPainter painter(this);
    QPen pen(Qt::NoBrush, 1);
    painter.setPen(pen);
    QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
    linear.setColorAt(0, QColor(230, 230, 230, topColor));
    linear.setColorAt(0.5, QColor(230, 230, 230, middleColor));
    linear.setColorAt(1, QColor(230, 230, 230, bottomColor));
    painter.setBrush(linear);
    painter.drawRect(rect());
}

}
