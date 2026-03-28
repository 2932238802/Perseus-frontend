#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/util/CheckLang.h"

#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosToolChainManager/LosToolChainManager.h"


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


  public: // tool
    void start(const QString &);

  private: // init
    void initConnect();

  private: // tool
    void openFile(const QString &file_path, const QString &file_context);
    void changeFile(const QString &file_path, const QString &file_context);
    void requestCompletion(const QString &file_path, int line, int col);
    void toDefineRequest(int line, int col, const QString &file_path);
    void didChangeWatchedFiles(const QString &file_path, int type);
    QString getLangId(LosCommon::LosToolChain_Constants::LosLanguage);
    LosLspClient *getClient(const QString &);

  private slots:
    void onLspReady(LosCommon::LosToolChain_Constants::LosLanguage, const QString &, const QStringList &);

  private:
    // 一个语言对应一个解释器
    QMap<LosCommon::LosToolChain_Constants::LosLanguage, LosLspClient *> LOS_clients;
};

} // namespace LosCore
