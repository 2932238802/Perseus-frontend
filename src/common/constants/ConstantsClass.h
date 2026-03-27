#pragma once
#include <QObject>
#include <QString>
#include <functional>
#include <qglobal.h>

namespace LosCore
{
class LosConfig;
}

namespace LosCommon
{


/**
LosLsp 就是 识别 一些语法 和 补全
*/
namespace LosLsp_Constants
{
enum DiagnosticSeverity
{
    Error = 1,
    Warning,
    Information,
    Hint
};

/**
didChangeWatchedFiles 字段
changes 字段的
type 整形
*/
enum LspJson_didChangeWatchedFiles_changes_type
{
    Created = 1,
    Changed = 2
};


struct LosDiagnostic
{
    int startLine;
    int startChar;
    int endLine;
    int endChar;
    DiagnosticSeverity ds = DiagnosticSeverity::Warning;
    QString message;
};
} // namespace LosLsp_Constants



/**
- 识别 CMAKE 等的常量
*/
namespace LosConfig_Constants
{
struct ConfigItem
{
    QString L_name;
    QStringList L_filesToHotUpdate;
    std::function<bool(const QString &)> L_sniffer;
    std::function<LosCore::LosConfig *(QObject *)> L_creator;
};
} // namespace LosConfig_Constants


namespace LosSession_Constants
{
struct Config
{
    QStringList L_curFilePaths{};
    QString L_curProDir = "";
};

} // namespace LosSession_Constants

} // namespace LosCommon