// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>
#include <qtmetamacros.h>

class QJsonObject;

namespace LosCore
{
    class LosThemeManager : public QObject
    {
        Q_OBJECT
      public:
        static LosThemeManager &instance();

      public: /* api */
        QStringList availableThemes() const;
        QString currentTheme() const;
        QString currentDisplayName() const;
        bool setTheme(const QString &themeName, bool persist = true);
        QJsonObject themeJson(const QString &themeName) const;

        QString buildMainQss(const QString &themeName) const;
        QString buildExtraQss(const QString &templateText, const QString &themeName) const;

        // 当前主题的 ui token map (key -> color string)
        QHash<QString, QString> uiTokens(const QString &themeName) const;

      private:
        explicit LosThemeManager(QObject *parent = nullptr);
        ~LosThemeManager() override;

      private:
        class Impl;
        std::unique_ptr<Impl> L_themeManagerImpl;
    };
} /* namespace LosCore */
