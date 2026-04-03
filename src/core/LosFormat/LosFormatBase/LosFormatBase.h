#pragma once
#include <qobject.h>
#include <qprocess.h>
#include <qtmetamacros.h>

namespace LosCore
{


class LosFormatBase : public QObject
{
    Q_OBJECT
  public:
    explicit LosFormatBase(QObject *parent = nullptr);
    ~LosFormatBase() = default;

  public: // tool
    virtual bool format(QString *out, const QString &file_path, const QString &raw_content) = 0;

  protected:
    QProcess *L_process;
};

} // namespace LosCore