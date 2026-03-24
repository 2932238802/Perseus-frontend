
#include "LosSingleCppRunner.h"
#include "common/constants/ConstantsStr.h"
#include "core/run/LosAbstractRunner/LosAbstractRunner.h"
#include <qglobal.h>
#include <qobject.h>
#include <qprocess.h>
#include <qtmetamacros.h>

namespace LosCore {

/**
默认构造和初始化
*/
LosSingleCppRunner::LosSingleCppRunner(const QString &file_path,
                                       QObject *parent)
    : LosAbstractRunner{parent} {
  LOS_filePath.loadFile(file_path);
  L_gxxPro = new QProcess(this);
  L_runPro = new QProcess(this);

#ifdef Q_OS_WIN
  L_exePath = LOS_filePath.getAbsolutePath() + "/" +
              LOS_filePath.getBaseFileName() + LosCommon::WIN_EXE;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
  L_exePath = LOS_filePath.getAbsolutePath() + "/" +
              LOS_filePath.getBaseFileName() + LosCommon::LINUX_EXE;
#else
  L_exePath = LOS_filePath.getAbsolutePath() + "/" +
              LOS_filePath.getBaseFileName() + LosCommon::LINUX_EXE;
#endif
  initConnect();
}

/**
析构
强行停止
*/
LosSingleCppRunner::~LosSingleCppRunner() { stop(); }

/**
停止
*/
void LosSingleCppRunner::stop() {
  if (L_gxxPro->state() != QProcess::NotRunning) {
    L_gxxPro->kill();
  }
  if (L_runPro->state() != QProcess::NotRunning) {
    L_runPro->kill();
  }
}

/*
运行
*/
void LosSingleCppRunner::start() {
  if (!LOS_filePath.isExist()) {
    emit _appendErr(u8"The file to be executed does not exist...");
    return;
  }

  if (!LOS_filePath.isFile()) {
    emit _appendErr(u8"unknown content");
    return;
  }

  L_gxxPro->setWorkingDirectory(LOS_filePath.getAbsolutePath());
  QStringList args;
  args << LOS_filePath << LosCommon::CXX_17 << LosCommon::CMD_OBJECT
       << L_exePath;
  L_gxxPro->start(LosCommon::GXX, args);
}

/**
- 建立联系
*/
void LosSingleCppRunner::initConnect() {
  connect(L_gxxPro, &QProcess::readyReadStandardError, this, [=]() {
    emit _appendLog(QString::fromLocal8Bit(L_gxxPro->readAllStandardError()));
  });

  connect(L_gxxPro, &QProcess::finished, this,
          [=](int exit_code, QProcess::ExitStatus exitStatus) {
            if (exit_code == 0) {
              emit _appendLog(u8"editing successful!");

              L_runPro->setWorkingDirectory(LOS_filePath.getAbsolutePath());
              L_runPro->start(L_exePath);
            } else {
              emit _appendErr(u8"compilation failed...");
            }
          });
  connect(L_gxxPro, &QProcess::errorOccurred, this,
          [=](QProcess::ProcessError error) {
            emit _appendErr(u8"The compiler failed to start (possibly due to "
                            u8"g++ not being installed or an invalid path)");
          });

  connect(L_runPro, &QProcess::readyReadStandardOutput, this, [=]() {
    emit _appendLog(
        QString(R"(
==============================  out put 
      %1
==============================  out end
      )")
            .arg(QString::fromLocal8Bit(L_runPro->readAllStandardOutput())));
  });

  connect(L_runPro, &QProcess::readyReadStandardError, this, [=]() {
    emit _appendErr(QString::fromLocal8Bit(L_runPro->readAllStandardError()));
  });

  connect(L_runPro, &QProcess::finished, this,
          [=](int exitCode, QProcess::ExitStatus exitStatus) {
            emit _appendLog(u8"\n process terminated (exit code: " +
                            QString::number(exitCode) + ")");
          });

  connect(L_runPro, &QProcess::errorOccurred, this,
          [=](QProcess::ProcessError error) {
            emit _appendErr(u8"The compiled program failed to start");
          });
}

} // namespace LosCore