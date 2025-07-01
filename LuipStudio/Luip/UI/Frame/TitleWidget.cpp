#include "Log.h"
#include <QSignalMapper>
#include "TitleWidget.h"
#include "UI/Frame/ChangePasswordDialog.h"
#include "System/LockScreen/ScreenProtection.h"
#include "LuaEngine/LuaEngine.h"

using namespace System;
using namespace System::Screen;
using namespace Lua;

namespace UI
{
TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
{
#ifdef _CS_ARM_LINUX
    Screenprotection::Instance()->Register(this);
#endif
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    this->SetupUIWidget();
    this->TranslateLanguage();
}

void TitleWidget::SetupUIWidget()
{
#ifdef _CS_ARM_LINUX
    m_closeButton = new QPushButton();
    m_closeButton->setFixedSize(1,1);
    titleLayout = new QHBoxLayout();
    titleLayout->addWidget(m_closeButton, 0, Qt::AlignTop);
#endif

#ifdef _CS_X86_WINDOWS
    m_minButton = new PushButton();
    m_closeButton = new PushButton();

    m_minButton->setPicName(QString(":/sysButton/min"));
    m_closeButton->setPicName(QString(":/sysButton/close"));

    connect(m_minButton, SIGNAL(clicked()), this, SIGNAL(ShowMin()));
    connect(m_closeButton, SIGNAL(clicked()), this, SIGNAL(CloseWidget()));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addStretch();
    titleLayout->addWidget(m_minButton, 0, Qt::AlignTop);
    titleLayout->addWidget(m_closeButton, 0, Qt::AlignTop);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 5, 0);
#endif

    QStringList stringList;

    stringList<<":/toolWidget/home"<<":/toolWidget/data"<<":/toolWidget/signal"
               <<":/toolWidget/setting"<<":/toolWidget/mainTain"<<":/toolWidget/information";

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for(int i=0; i<stringList.size(); i++)
    {
        ToolButton *toolButton = new ToolButton(stringList.at(i));
        m_buttonList.append(toolButton);
        connect(toolButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(toolButton, QString::number(i, 10));

        buttonLayout->addWidget(toolButton, 0, Qt::AlignBottom);
    }
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(SlotTurnPage(QString)));
    buttonLayout->addStretch();
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(15, 0, 15, 0);

    if (LoginDialog::userType == General)
    {
        m_contextMenuButton = new ToolButton(":/toolWidget/generalUser");
    }
    else if (LoginDialog::userType == Maintain)
    {
        m_contextMenuButton = new ToolButton(":/toolWidget/maintainUser");
    }
    else if (LoginDialog::userType == Administrator)
    {
        m_contextMenuButton = new ToolButton(":/toolWidget/administrorUser");
    }
    else if (LoginDialog::userType == Super)
    {
        m_contextMenuButton = new ToolButton(":/toolWidget/administrorUser");
    }

    connect(m_contextMenuButton,SIGNAL(clicked()),this,SLOT(SlotCreatContextMenu()));

    // 用户菜单栏
    m_menuList<<"切换用户"<<"修改密码"<<"注销";

    contextMenu = new QMenu(m_contextMenuButton);

    for(int i=0; i<m_menuList.size(); i++)
    {
        QAction * action;
        action = contextMenu->addAction(m_menuList.at(i));
        m_actionList.append(action);
    }
    contextMenu->setStyleSheet("QMenu::item{ padding:15px;font:12pt;padding-right:24px}");
    QObject::connect(contextMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotMenuTriggered(QAction*)));

    m_contextMenuButton->setMenu(contextMenu);

    //布局
    QHBoxLayout *contextMenuButtonLayout = new QHBoxLayout();
    contextMenuButtonLayout->addStretch();
    contextMenuButtonLayout->addWidget(m_contextMenuButton);
    contextMenuButtonLayout->setContentsMargins(0,0,15,0);

    QHBoxLayout *buttonContextLayout = new QHBoxLayout();
    buttonContextLayout->addLayout(buttonLayout);
    buttonContextLayout->addStretch();
    buttonContextLayout->addLayout(contextMenuButtonLayout);

//    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(buttonContextLayout);
    mainLayout->setSpacing(0);

#ifdef _CS_ARM_LINUX
    mainLayout->setContentsMargins(0, 20, 0, 0);
#endif

#ifdef _CS_X86_WINDOWS
    mainLayout->setContentsMargins(0, 0, 0, 0);
#endif

    setFixedHeight(100);

    ChangeBottomStatus();

    m_isMove = false;

    connect(this, SIGNAL(ScreenAlreadyLock()), this, SLOT(slotUserCancellation()));
}

void TitleWidget::OnLockScreen()
{
    emit ScreenAlreadyLock();
}

void TitleWidget::TranslateLanguage()
{
    m_buttonList.at(0)->setText(tr("主界面"));
    m_buttonList.at(1)->setText(tr("数据"));
    m_buttonList.at(2)->setText(tr("信号"));
    m_buttonList.at(3)->setText(tr("设置"));
    m_buttonList.at(4)->setText(tr("运维"));
    m_buttonList.at(5)->setText(tr("系统"));
}

void TitleWidget::SlotTurnPage(QString currentPage)
{
    bool ok;
    int currentIndex = currentPage.toInt(&ok, 10);

    for(int i = 0; i < m_buttonList.count(); i++)
    {
        ToolButton *toolButton = m_buttonList.at(i);
        if(currentIndex == i)
        {
            toolButton->SetMousePress(true);
        }
        else
        {
            toolButton->SetMousePress(false);
        }
    }

    emit TurnPage(currentIndex);
}

/**
 * @brief 发送创建上下文菜单信号
 */
void TitleWidget::SlotCreatContextMenu()
{
    QPoint g_point = m_contextMenuButton->mapToGlobal(QPoint(0,0));
    contextMenu->move(g_point.x(),g_point.y()+80);

    if (LoginDialog::userType == Super || LoginDialog::userType == Administrator || LoginDialog::userType == Maintain)
    {
        m_actionList.at(1)->setVisible(true);
    }
    else
    {
        m_actionList.at(1)->setVisible(false);
    }

    m_contextMenuButton->SetMousePress(false);

    contextMenu->show();
}

void TitleWidget::slotMenuTriggered(QAction *action)
{
    QString text = action->text();

    if (text == m_menuList.at(0))
    {
        ChangeUser();
    }
    else if (text == m_menuList.at(1))
    {
        ModifyPassword();
    }
    else if (text == m_menuList.at(2))
    {
        UserCancellation();
    }
}

void TitleWidget::slotUserCancellation()
{
    UserCancellation();
}

void TitleWidget::ChangeUser()
{
    RoleType oldUserType= LoginDialog::userType;

    LoginDialog changeloginDialog;
    if(changeloginDialog.exec()== LoginDialog::Accepted)
    {
        this->ChangeContextMenu();

        ChangeBottomStatus();

        UserChangeInfo(oldUserType);

        emit ChangeUserInterface();
        emit m_buttonList.at(0)->clicked();
    }
}

void TitleWidget::ModifyPassword()
{
    ChangePasswordDialog changePasswordDialog;
    changePasswordDialog.exec();
}

void TitleWidget::UserCancellation()
{
    RoleType oldUserType= LoginDialog::userType;

    LoginDialog::userType = General;
    this->ChangeContextMenu();

    ChangeBottomStatus();
    UserChangeInfo(oldUserType);

    emit ChangeUserInterface();
    emit m_buttonList.at(0)->clicked();
}

void TitleWidget::ChangeBottomStatus()
{
    if (LoginDialog::userType == General)
    {
        m_buttonList.at(2)->hide();
        LuaEngine* luaEngine = LuaEngine::Instance();
        lua_State* state = luaEngine->GetThreadState();
        bool ret;
        luaEngine->GetLuaValue(state, "config.system.displayMeaParamInGeneral", ret);
        if (ret)
        {
            m_buttonList.at(3)->show();
        }
        else
        {
            m_buttonList.at(3)->hide();
        }
        m_buttonList.at(4)->hide();
        m_buttonList.at(5)->show();
    }
    else if (LoginDialog::userType == Maintain)
    {
        m_buttonList.at(2)->hide();
        m_buttonList.at(3)->show();
        m_buttonList.at(4)->show();
        m_buttonList.at(5)->show();
    }
    else if (LoginDialog::userType == Administrator)
    {
        m_buttonList.at(2)->show();
        m_buttonList.at(3)->show();
        m_buttonList.at(4)->show();
        m_buttonList.at(5)->show();
    }
    else if (LoginDialog::userType == Super)
    {
        m_buttonList.at(2)->show();
        m_buttonList.at(3)->show();
        m_buttonList.at(4)->show();
        m_buttonList.at(5)->show();
    }
}

void TitleWidget::UserChangeInfo(RoleType  old)
{
    if (old == General)
    {
        if (LoginDialog::userType == Maintain)
        {
            logger->info("运维员登录");
        }
        else if (LoginDialog::userType == Administrator)
        {
            logger->info("管理员登录");
        }
        else if (LoginDialog::userType == Super)
        {
            logger->info("工程师登录");
        }
    }
    else if (old == Maintain)
    {
        if (LoginDialog::userType == General)
        {
            logger->info("普通用户登录");
        }
        else if (LoginDialog::userType == Administrator)
        {
            logger->info("管理员登录");
        }
        else if (LoginDialog::userType == Super)
        {
            logger->info("工程师登录");
        }
    }
    else if (old == Administrator)
    {
        if (LoginDialog::userType == General)
        {
            logger->info("普通用户登录");
        }
        else if (LoginDialog::userType == Maintain)
        {
            logger->info("运维员登录");
        }
        else if (LoginDialog::userType == Super)
        {
            logger->info("工程师登录");
        }
    }
    else if (old == Super)
    {
        if (LoginDialog::userType == General)
        {
            logger->info("普通用户登录");
        }
        else if (LoginDialog::userType == Maintain)
        {
            logger->info("运维员登录");
        }
        else if (LoginDialog::userType == Administrator)
        {
            logger->info("管理员登录");
        }
    }
}

void TitleWidget::ChangeContextMenu()
{
    if (LoginDialog::userType == General)
    {
        QPixmap pixmap(":/toolWidget/generalUser");
        m_contextMenuButton->setIcon(pixmap);
        m_contextMenuButton->setIconSize(pixmap.size());
    }
    else if (LoginDialog::userType == Maintain)
    {
        QPixmap pixmap(":/toolWidget/maintainUser");
        m_contextMenuButton->setIcon(pixmap);
        m_contextMenuButton->setIconSize(pixmap.size());
    }
    else if (LoginDialog::userType == Administrator)
    {
        QPixmap pixmap(":/toolWidget/administrorUser");
        m_contextMenuButton->setIcon(pixmap);
        m_contextMenuButton->setIconSize(pixmap.size());
    }
    else if (LoginDialog::userType == Super)
    {
        QPixmap pixmap(":/toolWidget/administrorUser");
        m_contextMenuButton->setIcon(pixmap);
        m_contextMenuButton->setIconSize(pixmap.size());
    }
}

}
