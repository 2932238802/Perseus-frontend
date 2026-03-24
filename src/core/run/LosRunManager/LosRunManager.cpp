
#include "LosRunManager.h"
#include "core/run/LosSingleCppRunner/LosSingleCppRunner.h"
#include "models/LosFilePath/LosFilePath.h"

namespace LosCore {
/**
构造函数

*/
LosRunManager::LosRunManager(QObject *parent) : QObject{parent} {
  LOS_runner = nullptr;
}
LosRunManager::~LosRunManager() { stop(); }

/**
执行函数
*/
void LosRunManager::execute(const QString &file_path) {
  stop();

  LosModel::LosFilePath path(file_path);
  if (!path.isExist()) {
    emit _appendErr("error, path does not exist!" + file_path);
    return;
  }

  if (path.isFile()) {
    if (file_path.endsWith(".cpp") || file_path.endsWith(".cc")) {
      LOS_runner = new LosSingleCppRunner(file_path, this);
    } else {
      emit _appendErr("compiling this type of file is not currently supported");
    }
  } else if (path.isDir()) {
    emit _appendErr(
        u8"folder-level overall builds (CMake) are not yet implemented!");
    return;
  }

  if (LOS_runner) {
    connect(LOS_runner, &LosSingleCppRunner::_appendErr, this,
            &LosRunManager::_appendErr);
    connect(LOS_runner, &LosSingleCppRunner::_appendLog, this,
            &LosRunManager::_appendLog);
    connect(LOS_runner, &LosSingleCppRunner::_buildOver, this,
            &LosRunManager::_buildOver);
    LOS_runner->start();
  }
}

/**
暂停
*/
void LosRunManager::stop() {
  if (LOS_runner) {
    LOS_runner->stop();
    LOS_runner->deleteLater();
    LOS_runner = nullptr;
  }
}

} // namespace LosCore