

#pragma once
#include "core/run/LosAbstractRunner/LosAbstractRunner.h"
#include "models/LosFilePath/LosFilePath.h"
#include "core/log/LosLog/LosLog.h"

#include <QObject>
#include <QProcess>
#include <qobject.h>
#include <qprocess.h>

namespace LosCore {
class LosSingleCppRunner : public LosAbstractRunner {
  Q_OBJECT
public: // const
  explicit LosSingleCppRunner(const QString &path, QObject *parent = nullptr);
  ~LosSingleCppRunner() override;

public: // tool
  void start() override;
  void stop() override;

private: // init
  void initConnect();

private:
  LosModel::LosFilePath LOS_filePath;
  QString L_exePath;
  QProcess *L_gxxPro;
  QProcess *L_runPro;
};
} // namespace LosCore