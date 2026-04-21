#pragma once

#include "common/constants/ConstantsNum/LosFormatManagerNum.h"
#include "common/constants/ConstantsStr/LLVM_formatStyle.h"
#include "common/constants/ConstantsStr/LosFormatManagerStr.h"
#include "common/util/CheckLang.h"
#include "core/LosFormat/LosFormatBase/LosFormatBase.h"

namespace LosCore
{

    class LosFClangFormat : public LosFormatBase
    {
      public:
        LosFClangFormat(QObject *parent = nullptr);
        ~LosFClangFormat() = default;

      public:
        bool format(QString *out, const QString &file_path, const QString &raw_content) override;

      private:
    };
} /* namespace LosCore */