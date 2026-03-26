#pragma once

#include "core/LosConfig/LosConfig/LosConfig.h"
#include <qobject.h>
namespace LosCore
{

class LosConfigManager : public QObject
{
    Q_OBJECT
  public:                                       // tool
    void analyse(const QString &absolute_path); // absolute_path 项目的路径 也就是文件夹路径
    void create(const QString &file_path);
    bool isInFiles(const QString& file_name);

  public:
    explicit LosConfigManager(QObject *parent = nullptr);
    ~LosConfigManager() = default;

  private:
    void initConnect();

  private:
    LosConfig *LOS_config = nullptr;
};
} // namespace LosCore