#include "core/LosConfig/LosConfig/LosConfig.h"
#include "core/LosConfig/LosConfigFactory/LosConfigFactory.h"
#include <QUrl>
#include <qtmetamacros.h>

namespace LosCore
{
class LosConfigCMake : public LosConfig
{
    Q_OBJECT
  public:
    explicit LosConfigCMake(QObject *parent = nullptr);

    ~LosConfigCMake() override = default;

  public: // tool
    void analyse(const QString &) override;
    bool isInFiles(const QString &) override;

    static bool can(const QString &file_path);
    static QStringList files;
};
} // namespace LosCore