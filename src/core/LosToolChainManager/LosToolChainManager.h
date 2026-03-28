#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/util/GetSettingKey.h"

#include <QHash>
#include <QJsonArray>
#include <QJsonParseError>
#include <QObject>
#include <qjsonobject.h>
#include <qtmetamacros.h>

namespace LosCore
{
/**
  LSP,       // 语言服务器
  Compiler,  // 编译器
  Formatter, // 格式化工具
  Linter,    // 静态分析
  Debugger   // 调试器

*/
class LosToolChainManager : public QObject
{
    Q_OBJECT

  public:
    explicit LosToolChainManager(QObject *parent = nullptr);
    ~LosToolChainManager() = default;

  public slots:
    void onCheckToolchain(LosCommon::LosToolChain_Constants::LosLanguage);
    void onCheckLspTool(LosCommon::LosToolChain_Constants::LosLanguage);
    void onCheckBuildTool(LosCommon::LosToolChain_Constants::LosLanguage language);

  private:
    void initConfig();
    void initConnect();
    bool validateExecutable(const QString &lastSavePath, const LosCommon::LosToolChain_Constants::ToolChainConfig &);

  private:
    LosCommon::LosToolChain_Constants::ToolChainConfig parseToolNode(const QJsonObject &, const QString &,
                                                                     LosCommon::LosToolChain_Constants::LosLanguage);

    LosCommon::LosToolChain_Constants::LosLanguage stringToLanguage(const QString &str);
    LosCommon::LosToolChain_Constants::ToolCategory stringToCategory(const QString &str);

  private:
    QHash<LosCommon::LosToolChain_Constants::LosLanguage,
          QHash<QString, LosCommon::LosToolChain_Constants::ToolChainConfig>>
        LOS_availableTools;

    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> LOS_LSP;       // clangd
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> LOS_Formatter; // clang-format
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> LOS_Compiler;  // g++, cargo, python
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> LOS_Linter;
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> LOS_Debugger;
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> LOS_BuildTool; // cmake
    QHash<QString, QString> L_activeToolPath;
};
} // namespace LosCore
