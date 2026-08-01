// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "core/LosFormat/LosFormatBase/LosFormatBase.h"

namespace LosCore
{

    class LosFClangFormat : public LosFormatBase
    {
        Q_OBJECT
      public:
        LosFClangFormat(QObject *parent = nullptr);
        ~LosFClangFormat() = default;

      public:
        bool format(QString *out, const QString &file_path, const QString &raw_content) override;

      public slots:
        void setClangFormat(const QString &str) noexcept;

      private:
        void initConnect() noexcept;

      private:
        QString L_formatStyle;
    };
} /* namespace LosCore */