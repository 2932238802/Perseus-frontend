#pragma once
#include "core/LosLog/LosLog.h"
#include <QObject>
#include <QString>
#include <functional>
#include <qnamespace.h>
#include <qobject.h>
#include <qshortcut.h>
#include <qwidget.h>

namespace LosCore
{
    class LosShortcutManager : public QObject
    {
        Q_OBJECT
      public:
        static LosShortcutManager &instance();
        void reg(const QString &key, QWidget *parent, const std::function<void()> &action, const QString &des = "");

      private:
        ~LosShortcutManager() = default;
        explicit LosShortcutManager(QObject *parent = nullptr);

      private:
        QMap<QString, QShortcut *> L_strToCuts;
        QMap<QString, QString> L_strToDes;
    };
} /* namespace LosCore */