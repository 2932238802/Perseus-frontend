#pragma once
#include "core/LosLog/LosLog.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include <QDir>
#include <QObject>
#include <qprocess.h>

namespace LosCore
{

class LosCmakeRunner : public LosAbstractRunner
{
  public:
    explicit LosCmakeRunner(QObject *parent = nullptr);
    ~LosCmakeRunner() override;

  public: // tool
    void start(const QString &path = "cmake") override;
    void stop() override;

  public: // set
    void setCMakeExe(const QString &file_path);

  private:
    void initConnect();

  private: // tool
    std::optional<QString> findExePath(const QString &build_path, const QString &targer_name);

  private:
    QString L_cmakeExe;
    QProcess *L_cmake;
    QProcess *L_runner;
};

} // namespace LosCore