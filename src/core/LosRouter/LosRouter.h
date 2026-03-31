

#pragma once

#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>

#include "common/constants/ConstantsClass.h"

namespace LosCore
{
class LosRouter : public QObject
{
    Q_OBJECT
  public:
    static LosRouter &instance();

  private:
    explicit LosRouter(QObject *parent = nullptr);
    ~LosRouter() = default;


  signals:
    // 文本定义
    void _cmd_whereDefine(int line, int col, const QString &file_path);

    // 打开文件 给 lsp 的请求
    void _cmd_lsp_request_openFile(const QString &file_path, const QString &file_content);

    // 文本 修改 之后 lsp的请求
    void _cmd_lsp_request_textChanged(const QString &file_path, const QString &file_content);

    // 语言补全
    void _cmd_lsp_request_completeion(const QString &file_path, int line, int col);

    // 悬停
    void _cmd_lsp_request_hover(const QString &file_path, int line, int col);

    // 文件树的修改
    void _cmd_lsp_msg_didChangeWatchedFiles(const QString &,
                                            LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type);

    // 补全的结果 返回
    void _cmd_lsp_result_completion(const QStringList &words);

    // 诊断的结构 返回
    void _cmd_lsp_result_diagnostics(const QString &file_path,

                                     // 文本定义的位置的返回 是lsp 返回
                                     const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &diags);
    void _cmd_lsp_result_definition(const QString &file_name, int line);

    void _cmd_lsp_result_hover(const QString &md_content);

    // 从诊断的结果 ctrl 点击 跳转到指定位置
    void _cmd_gotoFile(const QString &file_path, int line);

    // 格式 话 文本的 请求
    void _cmd_codeFormat();

    // 文本 变脏的信号 请求 ui处理
    void _cmd_fileDirty(const QString &file_path, bool dirty);

    // 左侧树 建立 完毕 之后
    void _cmd_fileTreeDone(); // filetree 建立完毕 发送信号 就是左侧的文件树

    // 对左侧 文件树的各种操作 复制 删除 粘贴 等等
    void _cmd_fileSystemChanged();

    // 启动 语言分析其
    void _cmd_changeLang(const QString &file_path);

    // 启动ui 督促 下载
    void _cmd_toolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &);

    // lsp
    void _cmd_lspReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                       const QStringList &args);

    //
    void _cmd_buildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                             const QStringList &args);

    void _cmd_toolChainReady(LosCommon::LosToolChain_Constants::LosLanguage lang,
                             LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath);

    void _cmd_checkLanguageToolchain(LosCommon::LosToolChain_Constants::LosLanguage lang,
                                     LosCommon::LosToolChain_Constants::LosTool tool);
    void _cmd_checkSingleTool(LosCommon::LosToolChain_Constants::LosTool tool);

    // 用户设定 信号 设置 要打开 设置窗口
    void _cmd_manuallySet(const LosCommon::LosToolChain_Constants::ToolChainConfig &config);
};
} // namespace LosCore