

#include "core/lsp/LosLspManager/LosLspManager.h"
#include "core/lsp/LosLspClient/LosLspClient.h"
#include <qglobal.h>

namespace LosCore {

/**
默认构造
*/
LosLspManager::LosLspManager(QObject *parent) : QObject(parent) {
  LOS_client = new LosLspClient(this);
  initConnect();
}

/**
析构
*/
LosLspManager::~LosLspManager() {}

/**
开启服务
*/
void LosLspManager::start() { LOS_client->start(); }

/**
打开文件
*/
void LosLspManager::openFile(const QString &file_path,
                             const QString &file_context) {
  LOS_client->didOpen(file_path, file_context);
}

/**
改变文件信号
*/
void LosLspManager::changeFile(const QString &file_path,
                               const QString &file_context) {
  LOS_client->didChange(file_path, file_context);
}

/**
请求 完成
*/
void LosLspManager::requestCompletion(const QString &file_path, int line,
                                      int col) {
  LOS_client->sendRequestForCompletion(file_path, line, col);
}

void LosLspManager::toDefineRequest(int line, int col,
                                    const QString &file_path) {
  LOS_client->defineRequest(line, col, file_path);
}

/**
初始化链接
*/
void LosLspManager::initConnect() {
  connect(LOS_client, &LosLspClient::_completion, this,
          &LosLspManager::_completion);
  connect(LOS_client, &LosLspClient::_diagnostics, this,
          &LosLspManager::_diagnostics);
  connect(LOS_client, &LosLspClient::_definitionResult, this,
          &LosLspManager::_definitionResult);
}

} // namespace LosCore