
#include "core/LosShortcutManager/LosShortcutManager.h"
#include <qnamespace.h>
#include <qobject.h>
#include <qshortcut.h>


namespace LosCore
{

LosShortcutManager::LosShortcutManager(QObject *parent) : QObject(parent) {}

LosShortcutManager &LosShortcutManager::instance()
{
    static LosShortcutManager m;
    return m;
}



/**
- 注册快捷键
*/
void LosShortcutManager::reg(const QString &key, QWidget *parent, const std::function<void()> &action,
                             const QString &des)
{
    if (L_strToCuts.contains(key))
    {
        WAR("this shortcut key has already been occupied", "LosShortcutManager");
        return;
    }
    QShortcut *cur = new QShortcut(parent);
    cur->setContext(Qt::WindowShortcut);
    connect(cur, &QShortcut::activated, this, action);
    L_strToCuts.insert(key, cur);
    L_strToDes.insert(key, des);
    SUC("Shortcut registered successfully: " + key + " -> " + des, "LosShortcutManager");
}



} // namespace LosCore