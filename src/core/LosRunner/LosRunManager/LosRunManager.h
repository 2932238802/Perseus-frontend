#pragma once

#include "common/constants/ConstantsClass.h"
#include "common/util/CheckLang.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "core/LosRunner/LosSingleCppRunner/LosSingleCppRunner.h"
#include "models/LosFilePath/LosFilePath.h"
#include <qsharedpointer.h>

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

  public: // tool
    void execute(const QString &file_path, bool is_project = false);
    void stop();

  private slots:
    void onToolChainReady(LosCommon::LosToolChain_Constants::LosLanguage, const QString &);

  private: // init
    void initConnect();

  private: // tool
    LosAbstractRunner *getCurRunner(const QString &file_path);

  private: // param
    QString L_mainEntryFilePath = "";
    QMap<LosCommon::LosToolChain_Constants::LosLanguage, LosAbstractRunner *> LOS_runners;
};
} // namespace LosCore