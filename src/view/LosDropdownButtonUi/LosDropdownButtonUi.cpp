

#include "LosDropdownButtonUi.h"
#include <qaction.h>
#include <qmenu.h>
#include <qpushbutton.h>
namespace LosView
{


    /*
     * - 初始化
     */
    LosDropdownButtonUi::LosDropdownButtonUi(QWidget *parent) : QPushButton(parent), L_menu(nullptr)
    {
        initMenu();
    }



    /*
     * - 初始化
     */
    LosDropdownButtonUi::LosDropdownButtonUi(const QString &, QWidget *parent) : QPushButton(parent), L_menu(nullptr)
    {
        initMenu();
    }



    QAction *LosDropdownButtonUi::addOption(const QString &text, std::function<void()> f)
    {
        QAction *action = L_menu->addAction(text);
        connect(action, &QAction::triggered, this, f);
        return action;
    }



    void LosDropdownButtonUi::addSeparator()
    {
        L_menu->addSeparator();
    }



    QMenu *LosDropdownButtonUi::getMenu() const
    {
        return L_menu;
    }



    void LosDropdownButtonUi::initMenu()
    {
        L_menu = new QMenu(this);
        this->setMenu(L_menu);
    }


} /* namespace LosView */