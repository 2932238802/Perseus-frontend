#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr.h"
#include "common/util/CheckLang.h"
#include "core/LosFormat/LosFClangFormat/LosFClangFormat.h"
#include "core/LosFormat/LosFNeocmakelsp/LosFNeocmakelsp.h"
#include "core/LosFormat/LosFormatBase/LosFormatBase.h"
#include "core/LosLog/LosLog.h"
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QObject>
#include <QProcess>
#include <qfileinfo.h>
#include <qmap.h>
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

      private:
        QMap<LosCommon::LosToolChain_Constants::LosLanguage, LosFormatBase *> L_formats;
    };


}; /* namespace LosCore */