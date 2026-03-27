
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
    virtual void analyse(const QString &)   = 0;
    virtual bool isInFiles(const QString &) = 0;
};
} // namespace LosCore