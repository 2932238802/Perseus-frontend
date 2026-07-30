// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "core/LosFormat/LosFormatBase/LosFormatBase.h"

namespace LosCore
{
    class LosFNeocmakelsp : public LosFormatBase
    {
      public:
        LosFNeocmakelsp(QObject *parent = nullptr);
        ~LosFNeocmakelsp() = default;

      public:
        bool format(QString *out, const QString &file_path, const QString &raw_content) override;

      private:
    };
} /* namespace LosCore */