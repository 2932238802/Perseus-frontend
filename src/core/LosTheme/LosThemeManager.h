// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QStringList>
#include <qtmetamacros.h>

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
        ~LosThemeManager() override = default;
        Q_DISABLE_COPY(LosThemeManager)

      private: /* tool */
        QString applyTokens(const QString &templateText, const QHash<QString, QString> &tokens) const;
        QString readResourceText(const QString &qrcPath) const;

      private:
        QString L_curTheme = QStringLiteral("dracula");
    };
} /* namespace LosCore */
