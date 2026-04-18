#pragma once

#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr/LLVM_formatStyle.h"
#include "common/constants/ConstantsStr/LosFormatManagerStr.h"
#include "core/LosFormat/LosFormatBase/LosFormatBase.h"

#include "common/util/CheckLang.h"

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