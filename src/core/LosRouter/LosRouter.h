// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosLspClass.h"
#include "common/constants/ConstantsClass/LosNetClass.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include <QObject>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qobject.h>
#include <qtmetamacros.h>

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
        void _cmd_openFile_suc(const QString &);


        // 文本 修改 之后 lsp的请求
        void _cmd_lsp_request_textChanged(const QString &file_path, const QString &file_content);

        // 语言补全的星球
        void _cmd_lsp_request_completeion(const QString &file_path, int line, int col);

        // 悬停 请求
        void _cmd_lsp_request_hover(const QString &file_path, int line, int col);

        // 语义高亮的请求
        void _cmd_lsp_request_semantic(const QString &);

        // 文件树的修改
        void _cmd_lsp_msg_didChangeWatchedFiles(const QString &, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type);
        // 补全的结果 返回
        void _cmd_lsp_result_completion(const QStringList &words);

        // 诊断的结构 返回
        // 文本定义的位置的返回 是lsp 返回
        void _cmd_lsp_result_diagnostics(const QString &file_path, const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &diags);
        void _cmd_lsp_result_definition(const QString &file_name, int line);
        void _cmd_lsp_result_hover(const QString absoluteFilePath, const QString &md_content);

        // 语义高亮
        void _cmd_lsp_result_semanticTokens(const QString &absolute_file_path, const QJsonArray &arr);

        void _cmd_lsp_result_semanticLegend(const QStringList &, const QStringList &);

        // 从诊断的结果 ctrl 点击 跳转到指定位置
        void _cmd_gotoFile(const QString &file_path, int line);

        // 对左侧 文件树的各种操作 复制 删除 粘贴 等等
        void _cmd_fileSystemChanged();

        // lsp
        void _cmd_lspReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath, const QStringList &args);

        // 切换标签页的预览状态
        void _cmd_togglePreview(const QString &absolute_file_path);


        // ----------------------------------------------------------------------------
        // 格式 话 文本的 请求
        void _cmd_codeFormat();

        // 文本 变脏的信号 请求 ui处理
        void _cmd_fileDirty(const QString &file_path, bool dirty);

        // 左侧树 建立 完毕 之后
        // filetree 建立完毕 发送信号 就是左侧的文件树
        void _cmd_fileTreeDone();

        // 修改 文件
        void _cmd_fileRenamed(const QString &oldPath, const QString &newPath);

        // 切换文件
        void _cmd_fileChanged(const QString &absolute_file_path);



        // ----------------------------------------------------------------------------
        // 启动ui 督促 下载
        void _cmd_toolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &);

        // 构建工具完备
        void _cmd_buildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath, const QStringList &args);

        // 单个工具完备
        void _cmd_toolChainReady(LosCommon::LosToolChain_Constants::LosLanguage lang, LosCommon::LosToolChain_Constants::LosTool tool,
                                 const QString &exePath);
        void _cmd_checkLanguageToolchain(LosCommon::LosToolChain_Constants::LosLanguage lang, LosCommon::LosToolChain_Constants::LosTool tool);
        void _cmd_checkSingleTool(LosCommon::LosToolChain_Constants::LosTool tool);
        void _cmd_findExePathAndSetSettingUi(const QString &file_path, LosCommon::LosToolChain_Constants::LosTool tool);


        // ----------------------------------------------------------------------------
        // 用户设定 信号 设置 要打开 设置窗口
        void _cmd_manuallySet(const LosCommon::LosToolChain_Constants::ToolChainConfig &config);



        // ----------------------------------------------------------------------------
        /*
         * _cmd_runScript
         * - 第一个参数 就是 在哪个 工作目录下面运行
         * - 第二个参数 就是 脚本在哪
         * - 第三个参数 就是 脚本参数
         *
         * _cmd_command_exeScript
         * - 启动命令行 参数的UI的
         */
        void _cmd_net_pluginReply(const QList<LosCommon::LosNet_Constants::PluginInfo> &);
        void _cmd_openPluginDetail(const LosCommon::LosNet_Constants::PluginInfo &);
        void _cmd_net_downloadFinished(const QString &downpath);
        void _cmd_command_exeScript(const QString &script_path, const QStringList &params);
        void _cmd_net_PluginPath(const QString &plugin_path);
        void _cmd_runScript(const QString &working_dir, const QString &script_path, const QStringList &args);



        // ----------------------------------------------------------------------------
        /*
         * _cmd_autoInstallTool
         * - 自动后台安装
         */
        void _cmd_autoInstallTool(const LosCommon::LosToolChain_Constants::ToolChainConfig &);



        // ----------------------------------------------------------------------------
        /**
         * @brief git的信号
         *
         */
        void _cmd_initGitProject(const QString &path);
        void _cmd_getBranchName(const QString &branch_name);
        void _cmd_gitStatusUpdated();


        // ----------------------------------------------------------------------------
        /**
         * @brief 工具栏 信号
         */
        void _cmd_chooseFileBtnClick();
        void _cmd_chooseDirBtnClick();
        void _cmd_settingBtnClick();
        void _cmd_runBtnClick();
        void _cmd_projectBtnToggled(bool checked);
        // - 0=Output, 1=Issues, 2=Terminal
        void _cmd_bottomTabVisibilityChanged(int tabIndex, bool visible);



        // ----------------------------------------------------------------------------
        // 网络部分
        void _cmd_authBtnClick();
        void _cmd_auth_login_request(const QString &username, const QString &password);
        void _cmd_auth_register_request(const QString &username, const QString &password);
        void _cmd_auth_autoLogin_request(const QString &token);
        void _cmd_auth_autoLogin_response(bool suc, const QString &message, const QString &username, const QString &user_id);
        void _cmd_auth_response(bool success, const QString &message);
        void _cmd_auth_loginStateChanged(bool loggedIn);
        void _cmd_needAuth(); // 打开登录的窗口 提示 需要登录 在部分 需要登录和注册的逻辑
        void _cmd_agent_sendMessage(const QString &, const QString &provider_name, const QString &model_name);
        void _cmd_agent_addProvider_request(const QString &provider, const QString &baseUrl, const QString &apiKey, const QStringList &models);
        void _cmd_agent_addProvider_response(bool success, const QString &message);
        void _cmd_agent_listModels_request(const QString &baseUrl, const QString &apiKey);
        void _cmd_agent_listModels_response(bool ok, const QStringList &models, const QString &msg);
        void _cmd_agent_listProviders_request();                                                                               // 厂商 - 模型
        void _cmd_agent_listProviders_response(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg); // 服务器的答复
        void _cmd_agent_reply(bool suc, const QString &reply);
        void _cmd_agent_replyDone();
        void _cmd_agent_replyChunk(const QString &data);
        void _cmd_agent_completion_request(const QString &file_path, const QString &prefix, const QString &suffix,
                                           LosCommon::LosToolChain_Constants::LosLanguage language);
        void _cmd_agent_completion_response(const QString &file_path, const QString &completion_text);


        // ----------------------------------------------------------------------------
        // 主题切换部分
        void _cmd_themeChanged(const QString &themeName);



        // ----------------------------------------------------------------------------
    };
} /* namespace LosCore */