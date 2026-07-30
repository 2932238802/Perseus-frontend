// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "core/LosFormat/LosFormatBase/LosFormatBase.h"

#include <QMap>
#include <QObject>

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