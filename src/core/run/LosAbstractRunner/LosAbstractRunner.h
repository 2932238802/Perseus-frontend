
#pragma once

#include <QObject>
#include <qtmetamacros.h>
namespace LosCore {
class LosAbstractRunner : public QObject {
  Q_OBJECT
public:
  explicit LosAbstractRunner(QObject *parent = nullptr);
  virtual ~LosAbstractRunner() = default;
  virtual void start() = 0;
  virtual void stop() = 0;

signals:
  void _appendLog(const QString &log);
  void _appendErr(const QString &err);
  void _buildOver(bool is_ok);

private:
};
} // namespace LosCore