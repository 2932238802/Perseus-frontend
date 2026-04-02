#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/util/CheckLang.h"
#include "core/LosLsp/LosLspCMake/LosLspCMake.h"
#include "core/LosLsp/LosLspClangd/LosLspClangd.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosLsp/LosLspRust/LosLspRust.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosCmakeRunner/LosCmakeRunner.h"
#include "core/LosToolChainManager/LosToolChainManager.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QFileInfo>
#include <QObject>
#include <qlist.h>
#include <qtmetamacros.h>

namespace LosCore
{
class LosLspManager : public QObject
{
    Q_OBJECT
  public:
    explicit LosLspManager(QObject *parent = nullptr);
    ~LosLspManager() override;


  public:  // tool
  private: // init
    void initConnect();

  private: // tool
    void openFile(const QString &file_path, const QString &file_context);
    void changeFile(const QString &file_path, const QString &file_context);
    void requestCompletion(const QString &file_path, int line, int col);
    void toDefineRequest(int line, int col, const QString &file_path);
    void didChangeWatchedFiles(const QString &file_path, int type);
    void onSemantic(const QString &);
    QString getLangId(LosCommon::LosToolChain_Constants::LosLanguage);
    LosLspClient *getClient(const QString &);

  private slots:
    void onLspReady(LosCommon::LosToolChain_Constants::LosTool, const QString &, const QStringList &);

  private:
    // 一个语言对应一个解释器
    // LosTool 指定工具
    QMap<LosCommon::LosToolChain_Constants::LosTool, LosLspClient *> LOS_clients;
};

} // namespace LosCore
