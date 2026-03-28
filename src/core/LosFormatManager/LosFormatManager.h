#pragma once
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosLog/LosLog.h"
#include "models/LosFileContext/LosFileContext.h"

#include <QObject>
#include <QProcess>
#include <qstringview.h>

namespace LosCore
{


class LosFormatManager : public QObject
{
    Q_OBJECT

  private:
    explicit LosFormatManager(QObject *parent = nullptr);
    ~LosFormatManager() = default;

  public:
    static LosFormatManager &instance();
    bool format(QString *out, const QString &file_path, const QString &raw_content);
};


}; // namespace LosCore