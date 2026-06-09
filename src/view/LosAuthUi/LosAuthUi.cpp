// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAuthUi.h"
#include "common/constants/ConstantsClass/LosAuthUiClass.h"
#include "common/constants/ConstantsNum/LosAuthUiNum.h"
#include "common/constants/ConstantsStr/LosAuthUiStr.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/style/LosAuthUi_style.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace LosView
{
    namespace AuthC = LosCommon::LosAuthUi_Constants;

    /**
     * @brief 构造函数
     * - 构建控件 -> 着色 -> 连接信号
     */
    LosAuthUi::LosAuthUi(QWidget *parent) : QDialog(parent)
    {
        initUi();
        initStyle();
        initConnect();
    }



    /**
     * @brief initUi
     * - 纯代码构建: 标题 + 提示 + QStackedWidget(登录页/注册页)
     */
    void LosAuthUi::initUi()
    {
        setObjectName(AuthC::OBJ_ROOT);
        setWindowTitle(AuthC::WINDOW_TITLE);
        setModal(true);
        setFixedSize(AuthC::WINDOW_WIDTH, AuthC::WINDOW_HEIGHT);

        auto *root = new QVBoxLayout(this);
        root->setContentsMargins(AuthC::ROOT_MARGIN_L, AuthC::ROOT_MARGIN_T, AuthC::ROOT_MARGIN_R, AuthC::ROOT_MARGIN_B);
        root->setSpacing(AuthC::ROOT_SPACING);

        // 标题
        auto *title = new QLabel(AuthC::MAIN_TITLE, this);
        title->setObjectName(AuthC::OBJ_TITLE);
        title->setAlignment(Qt::AlignCenter);
        root->addWidget(title);

        // 页面栈
        L_stack = new QStackedWidget(this);
        L_stack->addWidget(buildLoginPage());
        L_stack->addWidget(buildRegisterPage());
        root->addWidget(L_stack, 1);

        // 提示行
        L_tipLabel = new QLabel(QString(), this);
        L_tipLabel->setObjectName(AuthC::OBJ_TIP);
        L_tipLabel->setAlignment(Qt::AlignCenter);
        L_tipLabel->setWordWrap(true);
        root->addWidget(L_tipLabel);
    }



    /**
     * @brief buildLoginPage
     */
    QWidget *LosAuthUi::buildLoginPage()
    {
        auto *page = new QWidget(this);
        auto *lay  = new QVBoxLayout(page);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->setSpacing(AuthC::PAGE_SPACING);

        L_loginUser = new QLineEdit(page);
        L_loginUser->setPlaceholderText(AuthC::PH_USERNAME);
        L_loginUser->setClearButtonEnabled(true);

        L_loginPwd = new QLineEdit(page);
        L_loginPwd->setPlaceholderText(AuthC::PH_PASSWORD);
        L_loginPwd->setEchoMode(QLineEdit::Password);

        L_loginBtn = new QPushButton(AuthC::BTN_LOGIN, page);
        L_loginBtn->setObjectName(AuthC::OBJ_PRIMARY_BTN);
        L_loginBtn->setCursor(Qt::PointingHandCursor);

        L_toRegisterBtn = new QPushButton(AuthC::BTN_TO_REGISTER, page);
        L_toRegisterBtn->setObjectName(AuthC::OBJ_LINK_BTN);
        L_toRegisterBtn->setCursor(Qt::PointingHandCursor);
        L_toRegisterBtn->setFlat(true);

        lay->addWidget(L_loginUser);
        lay->addWidget(L_loginPwd);
        lay->addSpacing(AuthC::FIELD_GAP);
        lay->addWidget(L_loginBtn);
        lay->addWidget(L_toRegisterBtn, 0, Qt::AlignCenter);
        lay->addStretch(1);
        return page;
    }


    /**
     * @brief buildRegisterPage
     * - 用户名 + 密码 + 确认密码 + [注册] + "已有账号? 去登录"
     */
    QWidget *LosAuthUi::buildRegisterPage()
    {
        auto *page = new QWidget(this);
        auto *lay  = new QVBoxLayout(page);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->setSpacing(AuthC::PAGE_SPACING);

        L_regUser = new QLineEdit(page);
        L_regUser->setPlaceholderText(AuthC::PH_USERNAME);
        L_regUser->setClearButtonEnabled(true);

        L_regPwd = new QLineEdit(page);
        L_regPwd->setPlaceholderText(AuthC::PH_PASSWORD);
        L_regPwd->setEchoMode(QLineEdit::Password);

        L_regPwdConfirm = new QLineEdit(page);
        L_regPwdConfirm->setPlaceholderText(AuthC::PH_PWD_CONFIRM);
        L_regPwdConfirm->setEchoMode(QLineEdit::Password);

        L_registerBtn = new QPushButton(AuthC::BTN_REGISTER, page);
        L_registerBtn->setObjectName(AuthC::OBJ_PRIMARY_BTN);
        L_registerBtn->setCursor(Qt::PointingHandCursor);

        L_toLoginBtn = new QPushButton(AuthC::BTN_TO_LOGIN, page);
        L_toLoginBtn->setObjectName(AuthC::OBJ_LINK_BTN);
        L_toLoginBtn->setCursor(Qt::PointingHandCursor);
        L_toLoginBtn->setFlat(true);

        lay->addWidget(L_regUser);
        lay->addWidget(L_regPwd);
        lay->addWidget(L_regPwdConfirm);
        lay->addSpacing(AuthC::FIELD_GAP);
        lay->addWidget(L_registerBtn);
        lay->addWidget(L_toLoginBtn, 0, Qt::AlignCenter);
        lay->addStretch(1);
        return page;
    }



    /**
     * @brief initStyle
     * - 去掉右上角问号
     * - 用当前主题着色
     */
    void LosAuthUi::initStyle()
    {
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        applyTheme(LosCore::LosThemeManager::instance().currentTheme());
    }



    /**
     * @brief applyTheme
     * - 把 LosAuthUi_style.h 的模板里的 @token@ 替换为当前主题色后应用
     */
    void LosAuthUi::applyTheme(const QString &themeName)
    {
        const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::losAuth_getStyleTemplate(), themeName);
        setStyleSheet(qss);
    }



    /**
     * @brief initConnect
     * - 页面切换 / 提交 / 主题换肤 / 网络回包
     */
    void LosAuthUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();

        // 页面切换
        connect(L_toRegisterBtn, &QPushButton::clicked, this,
                [this]()
                {
                    L_tipLabel->clear();
                    L_stack->setCurrentIndex(AuthC::PAGE_REGISTER);
                });
        connect(L_toLoginBtn, &QPushButton::clicked, this,
                [this]()
                {
                    L_tipLabel->clear();
                    L_stack->setCurrentIndex(AuthC::PAGE_LOGIN);
                });
        connect(L_loginBtn, &QPushButton::clicked, this, &LosAuthUi::onLoginSubmit);
        connect(L_registerBtn, &QPushButton::clicked, this, &LosAuthUi::onRegisterSubmit);
        connect(L_loginPwd, &QLineEdit::returnPressed, this, &LosAuthUi::onLoginSubmit);
        connect(L_regPwdConfirm, &QLineEdit::returnPressed, this, &LosAuthUi::onRegisterSubmit);
        connect(&router, &LosCore::LosRouter::_cmd_auth_response, this, &LosAuthUi::onAuthResult);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
    }



    /**
     * @brief onLoginSubmit
     * - 校验非空
     * 经 LosRouter 把登录请求抛给网络层
     */
    void LosAuthUi::onLoginSubmit()
    {
        const QString user = L_loginUser->text().trimmed();
        const QString pwd  = L_loginPwd->text();
        if (user.isEmpty() || pwd.isEmpty())
        {
            showTip(AuthC::TIP_EMPTY, true);
            return;
        }
        showTip(AuthC::TIP_LOGGING_IN, false);
        emit LosCore::LosRouter::instance()._cmd_auth_login_request(user, pwd);
    }



    /**
     * @brief onRegisterSubmit
     * - 校验非空 + 两次密码一致
     * 经 LosRouter 把注册请求抛给网络层
     */
    void LosAuthUi::onRegisterSubmit()
    {
        const QString user    = L_regUser->text().trimmed();
        const QString pwd     = L_regPwd->text();
        const QString confirm = L_regPwdConfirm->text();
        if (user.isEmpty() || pwd.isEmpty())
        {
            showTip(AuthC::TIP_EMPTY, true);
            return;
        }
        if (pwd != confirm)
        {
            showTip(AuthC::TIP_PWD_MISMATCH, true);
            return;
        }
        showTip(AuthC::TIP_REGISTERING, false);
        emit LosCore::LosRouter::instance()._cmd_auth_register_request(user, pwd);
    }



    /**
     * @brief onAuthResult
     * - 网络层回包:
     *   - 成功: 清空输入 -> 通知主窗已登录(切图标) -> 关闭对话框
     *   - 失败: 红字提示, 不关窗
     */
    void LosAuthUi::onAuthResult(bool success, const QString &message)
    {
        showTip(message, !success);
        if (success)
        {
            clearInputs();
            emit LosCore::LosRouter::instance()._cmd_auth_loginStateChanged(true);
            accept();
            // 关闭对话框 (QDialog::accept)
        }
    }



    /**
     * @brief showTip
     * - 成功用 success 色,
     * - 失败用 danger 色 取自当前主题 token
     */
    void LosAuthUi::showTip(const QString &text, bool isError)
    {
        const auto tokens     = LosCore::LosThemeManager::instance().uiTokens(LosCore::LosThemeManager::instance().currentTheme());
        const QString success = tokens.value(AuthC::TOKEN_KEY_SUCCESS, AuthC::FALLBACK_SUCCESS);
        const QString danger  = tokens.value(AuthC::TOKEN_KEY_DANGER, AuthC::FALLBACK_DANGER);
        L_tipLabel->setStyleSheet(AuthC::TIP_QSS_TEMPLATE.arg(isError ? danger : success));
        L_tipLabel->setText(text);
    }



    /**
     * @brief clearInputs
     * - 清空所有输入框 成功后调用
     */
    void LosAuthUi::clearInputs()
    {
        L_loginUser->clear();
        L_loginPwd->clear();
        L_regUser->clear();
        L_regPwd->clear();
        L_regPwdConfirm->clear();
    }
}; /* namespace LosView */
