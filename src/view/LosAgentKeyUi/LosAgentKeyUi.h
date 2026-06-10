// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "core/LosRouter/LosRouter.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qtmetamacros.h>

namespace LosView
{
    /*
     * LosAgentKeyUi
     * - 添加 AI 配置对话框 (由 LosAgentUi 顶部 "+" 按钮弹出)
     * - 纯代码构建 UI (参照 LosAuthUi)
     * - 表单: 厂商名 / 接口地址 / 密钥 / 模型(逗号分隔)
     * - 提交 -> 经 LosRouter 抛给网络层, 由后端 /agent/add 落库
     */
    class LosAgentKeyUi : public QDialog
    {
        Q_OBJECT
      public: // construct
        explicit LosAgentKeyUi(QWidget *parent = nullptr);
        ~LosAgentKeyUi() = default;

      private slots: // chs
        void onSaveClicked();
        void applyTheme(const QString &themeName);

      private: // init
        void initUi();
        void initStyle();
        void initConnect();

      private: // tool
        void showTip(const QString &text, bool isError);

      private: // widgets
        QLabel *L_tip            = nullptr;
        QLineEdit *L_providerEdit = nullptr;
        QLineEdit *L_baseUrlEdit  = nullptr;
        QLineEdit *L_apiKeyEdit   = nullptr;
        QLineEdit *L_modelsEdit   = nullptr;
        QPushButton *L_saveBtn   = nullptr;
        QPushButton *L_cancelBtn = nullptr;
    };

} /* namespace LosView */
