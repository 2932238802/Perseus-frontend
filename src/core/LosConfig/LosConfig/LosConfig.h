
#pragma once

#include <QObject>
namespace LosCore
{
class LosConfig : public QObject
{
    Q_OBJECT
  public:
    explicit LosConfig(QObject *parent = nullptr);
    virtual ~LosConfig() = default;

  public:
    // 作为解析器
    virtual void analyse(const QString &) = 0;
    // 这个就是 判断 当前 有没有 核心的 文件 在 分析核心的文件 来判断是不是 该 程序
    virtual bool isInFiles(const QString &) = 0;
};
} // namespace LosCore