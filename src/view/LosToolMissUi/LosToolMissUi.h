#pragma once

#include "common/constants/ConstantsClass.h"
#include "core/LosRouter/LosRouter.h"
#include "view/style/LosToolMissUI_style.h"
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qtmetamacros.h>
#include <qwidget.h>

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