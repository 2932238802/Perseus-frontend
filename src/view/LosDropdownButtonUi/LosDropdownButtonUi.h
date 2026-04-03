#pragma once


#include <QObject>
#include <functional>
#include <qaction.h>
#include <qmenu.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qwidget.h>


namespace LosView
{
class LosDropdownButtonUi : public QPushButton
{
    Q_OBJECT
  public:
    explicit LosDropdownButtonUi(QWidget *parent = nullptr);
    explicit LosDropdownButtonUi(const QString &, QWidget *parent = nullptr);

    // 一个 按钮名字像 一个 按钮 效果
    QAction *addOption(const QString &, std::function<void()>);

    // 这个 用来 增加 一个 下划线
    void addSeparator();

    QMenu *getMenu() const;

  private: // init
    void initMenu();

  private: // params
    QMenu *L_menu;
};
} // namespace LosView