// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentKeyUi.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/style/LosAgentKeyUi_style.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace LosView
{
    /**
     * @brief 构造函数
     * - 构建控件 -> 着色 -> 连接信号
     */
    LosAgentKeyUi::LosAgentKeyUi(QWidget *parent) : QDialog(parent)
    {
        initUi();
        initStyle();
        initConnect();
    }



    /**
     * @brief initUi
     * - 纯代码构建: 标题 + 表单(厂商/地址/密钥/模型) + 提示 + 按钮
     */
    void LosAgentKeyUi::initUi()
    {
        setObjectName(QStringLiteral("LosAgentKeyUi"));
        setWindowTitle(QStringLiteral("添加 AI 配置"));
        setModal(true);
        setFixedWidth(380);

        auto *root = new QVBoxLayout(this);
        root->setContentsMargins(20, 20, 20, 20);
        root->setSpacing(14);

        // 标题
        auto *title = new QLabel(QStringLiteral("添加 AI 配置"), this);
        title->setObjectName(QStringLiteral("agentKeyTitle"));
        title->setAlignment(Qt::AlignCenter);
        root->addWidget(title);

        // 表单
        auto *form = new QFormLayout();
        form->setSpacing(10);
        form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

        L_providerEdit = new QLineEdit(this);
        L_providerEdit->setPlaceholderText(QStringLiteral("例如 deepseek"));

        L_baseUrlEdit = new QLineEdit(this);
        L_baseUrlEdit->setPlaceholderText(QStringLiteral("https://api.deepseek.com/v1"));

        L_apiKeyEdit = new QLineEdit(this);
        L_apiKeyEdit->setPlaceholderText(QStringLiteral("sk-xxxxxx"));
        L_apiKeyEdit->setEchoMode(QLineEdit::Password);

        L_modelsEdit = new QLineEdit(this);
        L_modelsEdit->setPlaceholderText(QStringLiteral("多个模型用逗号分隔"));

        form->addRow(QStringLiteral("厂商名称"), L_providerEdit);
        form->addRow(QStringLiteral("接口地址"), L_baseUrlEdit);
        form->addRow(QStringLiteral("密钥"), L_apiKeyEdit);
        form->addRow(QStringLiteral("模型"), L_modelsEdit);
        root->addLayout(form);

        // 提示行
        L_tip = new QLabel(QString(), this);
        L_tip->setObjectName(QStringLiteral("agentKeyTip"));
        L_tip->setAlignment(Qt::AlignCenter);
        L_tip->setWordWrap(true);
        root->addWidget(L_tip);

        // 按钮行
        auto *btnRow = new QHBoxLayout();
        btnRow->setSpacing(10);
        L_cancelBtn = new QPushButton(QStringLiteral("取消"), this);
        L_cancelBtn->setObjectName(QStringLiteral("agentKeyGhostBtn"));
        L_cancelBtn->setCursor(Qt::PointingHandCursor);
        L_saveBtn = new QPushButton(QStringLiteral("保存"), this);
        L_saveBtn->setObjectName(QStringLiteral("agentKeyPrimaryBtn"));
        L_saveBtn->setCursor(Qt::PointingHandCursor);
        btnRow->addStretch(1);
        btnRow->addWidget(L_cancelBtn);
        btnRow->addWidget(L_saveBtn);
        root->addLayout(btnRow);
    }



    /**
     * @brief initStyle
     * - 去掉右上角问号 + 用当前主题着色
     */
    void LosAgentKeyUi::initStyle()
    {
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        applyTheme(LosCore::LosThemeManager::instance().currentTheme());
    }



    /**
     * @brief applyTheme
     * - 主题换肤回调 (与 LosAuthUi 约定一致)
     */
    void LosAgentKeyUi::applyTheme(const QString &themeName)
    {
        const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::losAgentKey_getStyleTemplate(), themeName);
        setStyleSheet(qss);
    }



    /**
     * @brief initConnect
     * - 保存 / 取消 / 主题换肤
     */
    void LosAgentKeyUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(L_saveBtn, &QPushButton::clicked, this, &LosAgentKeyUi::onSaveClicked);
        connect(L_cancelBtn, &QPushButton::clicked, this, &LosAgentKeyUi::reject);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
    }



    /**
     * @brief onSaveClicked
     * - 校验非空 -> 拆分模型 -> 经 LosRouter 抛给网络层
     */
    void LosAgentKeyUi::onSaveClicked()
    {
        const QString provider = L_providerEdit->text().trimmed();
        const QString baseUrl  = L_baseUrlEdit->text().trimmed();
        const QString apiKey   = L_apiKeyEdit->text().trimmed();
        const QString modelsTx = L_modelsEdit->text().trimmed();

        if (provider.isEmpty() || baseUrl.isEmpty() || apiKey.isEmpty() || modelsTx.isEmpty())
        {
            showTip(QStringLiteral("请填写完整信息"), true);
            return;
        }

        // 模型: 逗号分隔 -> 去空白 -> 过滤空项
        QStringList models;
        for (const QString &m : modelsTx.split(',', Qt::SkipEmptyParts))
        {
            const QString t = m.trimmed();
            if (!t.isEmpty())
                models.append(t);
        }
        if (models.isEmpty())
        {
            showTip(QStringLiteral("请至少填写一个模型"), true);
            return;
        }

        // TODO: 待 LosRouter 增加 _cmd_agent_addProvider_request 信号后放开
        // emit LosCore::LosRouter::instance()._cmd_agent_addProvider_request(provider, baseUrl, apiKey, models);

        showTip(QStringLiteral("提交中..."), false);
    }



    /**
     * @brief showTip
     */
    void LosAgentKeyUi::showTip(const QString &text, bool isError)
    {
        const QString color = isError ? QStringLiteral("#e06c75") : QStringLiteral("#98c379");
        L_tip->setStyleSheet(QStringLiteral("color:%1;").arg(color));
        L_tip->setText(text);
    }

} /* namespace LosView */
