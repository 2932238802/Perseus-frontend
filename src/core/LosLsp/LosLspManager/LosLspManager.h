
#pragma once

#include "common/constants/ConstantsClass.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"

namespace LosCore {
class LosLspManager : public QObject {
  Q_OBJECT
public: // const
  explicit LosLspManager(QObject *parent = nullptr);
  ~LosLspManager();

public: // tool
  void start();
  void openFile(const QString &file_path, const QString &file_context);
  void changeFile(const QString &file_path, const QString &file_context);
  void requestCompletion(const QString &file_path, int line, int col);
  void toDefineRequest(int line,int col,const QString& file_path);

signals:
  void _completion(const QStringList &words);
  void _diagnostics(const QString &file_path,
                    const QList<LosCommon::LosDiagnostic> &diags);
  void _definitionResult(const QString &file_name, int line);

private:
  void initConnect();

private:
  LosLspClient *LOS_client;
};
} // namespace LosCore