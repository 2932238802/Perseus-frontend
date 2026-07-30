// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "core/LosRouter/LosRouter.h"

#include <QDialog>
#include <qtmetamacros.h>

namespace LosView
{
    class LosToolMissUi : public QDialog
    {
        Q_OBJECT
      private:
        void initStyle(const LosCommon::LosToolChain_Constants::ToolChainConfig &config);

      public:
        explicit LosToolMissUi(const LosCommon::LosToolChain_Constants::ToolChainConfig &, QWidget *parent = nullptr);
        ~LosToolMissUi() = default;
    };
} /* namespace LosView */