// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosCommon
{
    namespace LosAuthUi_Constants
    {
        // 窗口
        inline const QString WINDOW_TITLE = QStringLiteral("Perseus 账户");
        inline const QString MAIN_TITLE   = QStringLiteral("欢迎使用 Perseus");

        // 占位符
        inline const QString PH_USERNAME    = QStringLiteral("用户名");
        inline const QString PH_PASSWORD    = QStringLiteral("密码");
        inline const QString PH_PWD_CONFIRM = QStringLiteral("确认密码");

        // 按钮文案
        inline const QString BTN_LOGIN       = QStringLiteral("登 录");
        inline const QString BTN_REGISTER    = QStringLiteral("注 册");
        inline const QString BTN_TO_REGISTER = QStringLiteral("没有账号? 去注册");
        inline const QString BTN_TO_LOGIN    = QStringLiteral("已有账号? 去登录");

        // 提示文案
        inline const QString TIP_EMPTY        = QStringLiteral("用户名和密码不能为空");
        inline const QString TIP_PWD_MISMATCH = QStringLiteral("两次输入的密码不一致");
        inline const QString TIP_LOGGING_IN   = QStringLiteral("登录中...");
        inline const QString TIP_REGISTERING  = QStringLiteral("注册中...");

        // 对象名 (qss 选择器用)
        inline constexpr const char *OBJ_ROOT        = "LosAuthUi";
        inline constexpr const char *OBJ_TITLE       = "authTitle";
        inline constexpr const char *OBJ_TIP         = "authTip";
        inline constexpr const char *OBJ_PRIMARY_BTN = "authPrimaryBtn";
        inline constexpr const char *OBJ_LINK_BTN    = "authLinkBtn";

        // 主题 token key + fallback 色 (showTip 提示文字着色用)
        inline const QString TOKEN_KEY_SUCCESS = QStringLiteral("success");
        inline const QString TOKEN_KEY_DANGER  = QStringLiteral("danger");
        inline const QString FALLBACK_SUCCESS  = QStringLiteral("#4caf50");
        inline const QString FALLBACK_DANGER   = QStringLiteral("#e53935");
        inline const QString TIP_QSS_TEMPLATE  = QStringLiteral("#authTip { color: %1; }");

    } /* namespace LosAuthUi_Constants */
} // namespace LosCommon
