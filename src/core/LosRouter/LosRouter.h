

#pragma once

#include <QObject>
#include <qglobal.h>
#include <qjsonarray.h>
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

        // 语言补全的星球
        void _cmd_lsp_request_completeion(const QString &file_path, int line, int col);

        // 悬停 请求
        void _cmd_lsp_request_hover(const QString &file_path, int line, int col);

        // 语义高亮的请求
        void _cmd_lsp_request_semantic(const QString &);

        // 文件树的修改
        void
        _cmd_lsp_msg_didChangeWatchedFiles(const QString &,
                                           LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type);

        // 补全的结果 返回
        void _cmd_lsp_result_completion(const QStringList &words);

        // 诊断的结构 返回
        void _cmd_lsp_result_diagnostics(const QString &file_path,

                                         // 文本定义的位置的返回 是lsp 返回
                                         const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &diags);

        void _cmd_lsp_result_definition(const QString &file_name, int line);

        void _cmd_lsp_result_hover(const QString &md_content);

        // 语义高亮
        void _cmd_lsp_result_semanticTokens(const QJsonArray &arr);

        void _cmd_lsp_result_semanticLegend(const QStringList &, const QStringList &);


        // 从诊断的结果 ctrl 点击 跳转到指定位置
        void _cmd_gotoFile(const QString &file_path, int line);

        // 对左侧 文件树的各种操作 复制 删除 粘贴 等等
        void _cmd_fileSystemChanged();

        // lsp
        void _cmd_lspReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                           const QStringList &args);



        // ----------------------------------------------------------------------------



        // 格式 话 文本的 请求
        void _cmd_codeFormat();

        // 文本 变脏的信号 请求 ui处理
        void _cmd_fileDirty(const QString &file_path, bool dirty);

        // 左侧树 建立 完毕 之后
        void _cmd_fileTreeDone(); // filetree 建立完毕 发送信号 就是左侧的文件树



        // ----------------------------------------------------------------------------



        // 启动 语言分析其
        void _cmd_changeLang(const QString &file_path);

        // 启动ui 督促 下载
        void _cmd_toolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &);

        // 构建工具完备
        void _cmd_buildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                                 const QStringList &args);

        // 单个工具完备
        void _cmd_toolChainReady(LosCommon::LosToolChain_Constants::LosLanguage lang,
                                 LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath);

        void _cmd_checkLanguageToolchain(LosCommon::LosToolChain_Constants::LosLanguage lang,
                                         LosCommon::LosToolChain_Constants::LosTool tool);
        void _cmd_checkSingleTool(LosCommon::LosToolChain_Constants::LosTool tool);



        // ----------------------------------------------------------------------------



        // 用户设定 信号 设置 要打开 设置窗口
        void _cmd_manuallySet(const LosCommon::LosToolChain_Constants::ToolChainConfig &config);



        // ----------------------------------------------------------------------------

        void _cmd_net_pluginReply(const QList<LosCommon::LosNet_Constants::PluginInfo> &);
        void _cmd_openPluginDetail(const LosCommon::LosNet_Constants::PluginInfo &);
        void _cmd_net_downloadFinished(const QString &downpath);
        /*
         * 启动 命令行 参数的 ui 的
         */
        void _cmd_command_exeScript(const QString &script_path, const QStringList &params);

        /*
         * 
         */
        void _cmd_net_PluginPath(const QString &plugin_path);
        /*
         * 第一个参数 就是 在哪个 工作目录下面运行
         * 第二个参数 就是 脚本在哪
         * 第三个参数 就是 脚本参数
         */
        void _cmd_runScript(const QString &working_dir, const QString &script_path, const QStringList &args);
    };
} // namespace LosCore