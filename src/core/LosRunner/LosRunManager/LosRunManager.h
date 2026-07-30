// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"

#include <QMap>
#include <QObject>
#include <qtmetamacros.h>

namespace LosCore
{

    class LosRunManager : public QObject
    {
        Q_OBJECT

      public:
        explicit LosRunManager(QObject *parent = nullptr);
        ~LosRunManager() override;

      public: /* tool */
        void execute(const QString &file_path, bool is_project = false);
        void stop();

      private slots:
        void onToolChainReady(LosCommon::LosToolChain_Constants::LosLanguage, LosCommon::LosToolChain_Constants::LosTool tool, const QString &);
        void onBuildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath, const QStringList &args);

      private: /* init */
        void initConnect();

      private: /* param */
        QString L_mainEntryFilePath = "";
        QMap<LosCommon::LosToolChain_Constants::LosTool, LosAbstractRunner *> LOS_runners;
    };
} /* namespace LosCore */