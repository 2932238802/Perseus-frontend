#pragma once

#include "core/run/LosAbstractRunner/LosAbstractRunner.h"
#include <QObject>
#include <qtmetamacros.h>
namespace LosCore {

class LosRunManager : public QObject {
  Q_OBJECT

public:
  explicit LosRunManager(QObject *parent = nullptr);
  ~LosRunManager() override;

public: // tool
  void execute(const QString &file_path);
  void stop();

private: // param
  LosAbstractRunner *LOS_runner = nullptr;
};
} // namespace LosCore