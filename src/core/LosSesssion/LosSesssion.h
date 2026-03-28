#pragma once
#include "common/constants/ConstantsClass.h"
#include "core/LosLog/LosLog.h"
#include <QDir>
#include <QStandardPaths>
#include <QString>

namespace LosCore
{

class LosSession
{
  public:
    LosSession()  = default;
    ~LosSession() = default;

  public: // tool
    static LosSession &instance();
    bool loadConfig(LosCommon::LosSession_Constants::Config *conf);   // 加载本地信息
    bool saveConfig(const LosCommon::LosSession_Constants::Config &); // 保存本地信息
  private:                                                            // tool
    QString getDefaultConfigAbsoluteFilePath();                       // 获取config 文件夹
};

} // namespace LosCore