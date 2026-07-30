// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/LosLog/LosLog.h"
#include <qnamespace.h>

namespace LosCore
{
    /**
     * @brief Construct a new Los Shortcut Manager:: Los Shortcut Manager object
     * 
     * @param parent 
     */
    LosShortcutManager::LosShortcutManager(QObject *parent) : QObject(parent) {}
    LosShortcutManager &LosShortcutManager::instance()
    {
        static LosShortcutManager m;
        return m;
    }



    /**
     * @brief
     *
     * @param key
     * @param parent
     * @param action
     * @param des
     */
    void LosShortcutManager::reg(const QString &key, QWidget *parent, const std::function<void()> &action,
                                 const QString &des)
    {
        if (L_strToCuts.contains(key))
        {
            WAR("this shortcut key has already been occupied", "LosShortcutManager");
            return;
        }
        QShortcut *cur = new QShortcut(QKeySequence(key), parent);
        cur->setContext(Qt::WindowShortcut);
        connect(cur, &QShortcut::activated, this, action);
        L_strToCuts.insert(key, cur);
        L_strToDes.insert(key, des);
        SUC("shortcut registered successfully: " + key + " -> " + des, "LosShortcutManager");
    }


} /* namespace LosCore */