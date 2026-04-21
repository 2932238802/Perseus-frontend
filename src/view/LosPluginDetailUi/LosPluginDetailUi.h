#pragma once
#include "common/constants/ConstantsStr/LosPluginDetailUiStr.h"
#include "core/LosNet/LosNet.h"
#include "core/LosRouter/LosRouter.h"

#include <QDir>
#include <QPointer>
#include <QProcess>
#include <QWidget>
#include <qpushbutton.h>
#include <qstandardpaths.h>
#include <qtmetamacros.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LosPluginDetailUi;
}
QT_END_NAMESPACE

namespace LosView
{
    class LosPluginDetailUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosPluginDetailUi(QWidget *parent = nullptr);
        ~LosPluginDetailUi();

      public: /* tool */
        void setPluginInfo(const LosCommon::LosNet_Constants::PluginInfo &info);

      private slots:
        void onDownloadBtnClicked();
        void onDownloadFinished(const QString &zipPath);

      private: /* init */
        void initConnect();

      private:
        Ui::LosPluginDetailUi *ui;
        LosCommon::LosNet_Constants::PluginInfo LOS_info;
    };

} /* namespace LosView */
