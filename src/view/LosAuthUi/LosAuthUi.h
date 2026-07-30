// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <qtmetamacros.h>

namespace LosView
{
    /*
     * LosAuthUi
     * - 登录 / 注册 对话框
     * - 由活动栏底部 act_auth_btn 点击 -> LosRouter::_cmd_authBtnClick -> 主窗 exec() 弹出
     * - 内部用 QStackedWidget 切换 "登录页 / 注册页"
     * - 表单提交后经 LosRouter 把 (username, password) 抛给网络层(LosNet), 由后端校验
     * - 网络层回包后经 LosRouter::_cmd_auth_result 回到本窗, 在 L_tipLabel 上提示
     */
    class LosAuthUi : public QDialog
    {
        Q_OBJECT
      public: // construct
        explicit LosAuthUi(QWidget *parent = nullptr);
        ~LosAuthUi() = default;

      private: // init
        void initUi();
        void initStyle();
        void initConnect();

      private: // tool
        QWidget *buildLoginPage();
        QWidget *buildRegisterPage();
        void applyTheme(const QString &themeName);
        void showTip(const QString &text, bool isError);
        void clearInputs();

      private slots: // chs
        void onLoginSubmit();
        void onRegisterSubmit();
        void onAuthResult(bool success, const QString &message);

      private: // widgets
        QStackedWidget *L_stack = nullptr;
        QLabel *L_tipLabel      = nullptr;

        // 登录页
        QLineEdit *L_loginUser       = nullptr;
        QLineEdit *L_loginPwd        = nullptr;
        QPushButton *L_loginBtn      = nullptr;
        QPushButton *L_toRegisterBtn = nullptr;

        // 注册页
        QLineEdit *L_regUser       = nullptr;
        QLineEdit *L_regPwd        = nullptr;
        QLineEdit *L_regPwdConfirm = nullptr;
        QPushButton *L_registerBtn = nullptr;
        QPushButton *L_toLoginBtn  = nullptr;
    };
}; /* namespace LosView */
