#pragma once

#include "core/LosFormat/LosFormatBase/LosFormatBase.h"
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr.h"
#include "common/util/CheckLang.h"
#include <QTemporaryFile>
#include <QStringList>

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
} // namespace LosCore