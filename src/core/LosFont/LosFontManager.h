// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <qtmetamacros.h>

namespace LosCore
{
    class LosFontManager : public QObject
    {
        Q_OBJECT
      public: // 单例模式
        static LosFontManager &instance();

      public: // api
        QStringList availableFonts() const;
        QString currentFontFamily() const;
        bool setFontFamily(const QString &family, bool persist = true);
        QString qssFontDeclaration() const;

      private:
        explicit LosFontManager(QObject *parent = nullptr);
        ~LosFontManager() override = default;
        Q_DISABLE_COPY(LosFontManager)

      private:
        QString L_curFontFamily;
    };
} /* namespace LosCore */
