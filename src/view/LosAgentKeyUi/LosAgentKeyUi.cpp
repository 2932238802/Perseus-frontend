// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentKeyUi.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/style/LosAgentKeyUi_style.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QTimer>
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
        setWindowTitle(QStringLiteral("Add Agent"));
        setModal(true);
        setFixedWidth(380);

        auto *root = new QVBoxLayout(this);
        root->setContentsMargins(20, 20, 20, 20);
        root->setSpacing(14);

        auto *title = new QLabel(QStringLiteral("Add Agent"), this);
        title->setObjectName(QStringLiteral("agentKeyTitle"));
        title->setAlignment(Qt::AlignCenter);
        root->addWidget(title);

        // 表单
        auto *form = new QFormLayout();
        form->setSpacing(10);
        form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

        L_providerEdit = new QLineEdit(this);
        L_providerEdit->setPlaceholderText(QStringLiteral("for example deepseek"));

        L_baseUrlEdit = new QLineEdit(this);
        L_baseUrlEdit->setPlaceholderText(QStringLiteral("such https://api.deepseek.com/v1 ?"));

        L_apiKeyEdit = new QLineEdit(this);
        L_apiKeyEdit->setPlaceholderText(QStringLiteral("sk-xxxxxx ?"));
        L_apiKeyEdit->setEchoMode(QLineEdit::Password);

        L_modelsCombo = new QComboBox(this);
        L_modelsCombo->setEditable(false);
        L_modelsCombo->setInsertPolicy(QComboBox::NoInsert);
        L_modelsCombo->addItem(QStringLiteral("点击右侧获取"));

        L_fetchBtn = new QPushButton(QStringLiteral("get models"), this);
        L_fetchBtn->setObjectName(QStringLiteral("agentKeyGhostBtn"));
        L_fetchBtn->setCursor(Qt::PointingHandCursor);

        auto *modelRow = new QHBoxLayout();
        modelRow->setSpacing(8);
        modelRow->addWidget(L_modelsCombo, 1);
        modelRow->addWidget(L_fetchBtn, 0);

        form->addRow(QStringLiteral("厂商名称"), L_providerEdit);
        form->addRow(QStringLiteral("接口地址"), L_baseUrlEdit);
        form->addRow(QStringLiteral("密钥"), L_apiKeyEdit);
        form->addRow(QStringLiteral("模型"), modelRow);
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
        connect(L_fetchBtn, &QPushButton::clicked, this, &LosAgentKeyUi::onFetchModelsClicked);
        connect(L_cancelBtn, &QPushButton::clicked, this, &LosAgentKeyUi::reject);
        connect(&router, &LosCore::LosRouter::_cmd_agent_listModels_response, this, &LosAgentKeyUi::onModelsReceived);
        connect(&router, &LosCore::LosRouter::_cmd_agent_addProvider_response, this, &LosAgentKeyUi::onProviderAdded);
        // 未登录 / token 失效: 关闭本弹窗 (登录窗由别处接 _cmd_needAuth 弹出)
        connect(&router, &LosCore::LosRouter::_cmd_needAuth, this, &LosAgentKeyUi::reject);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
    }



    /**
     * @brief onFetchModelsClicked
     * - 校验地址/密钥非空 -> 经 LosRouter 抛给网络层拉取模型列表
     */
    void LosAgentKeyUi::onFetchModelsClicked()
    {
        const QString baseUrl = L_baseUrlEdit->text().trimmed();
        const QString apiKey  = L_apiKeyEdit->text().trimmed();
        if (baseUrl.isEmpty() || apiKey.isEmpty())
        {
            showTip(QStringLiteral("请先填写接口地址与密钥"), true);
            return;
        }
        L_fetchBtn->setEnabled(false);
        showTip(QStringLiteral("拉取模型中..."), false);
        emit LosCore::LosRouter::instance()._cmd_agent_listModels_request(baseUrl, apiKey);
    }



    /**
     * @brief onModelsReceived
     * - 网络层返回模型列表后回填下拉框
     */
    void LosAgentKeyUi::onModelsReceived(bool ok, const QStringList &models, const QString &msg)
    {
        L_fetchBtn->setEnabled(true);
        if (!ok)
        {
            showTip(msg.isEmpty() ? QStringLiteral("拉取模型失败") : msg, true);
            return;
        }
        if (models.isEmpty())
        {
            showTip(QStringLiteral("该厂商未返回任何模型"), true);
            return;
        }
        L_modelsCombo->clear();
        L_modelsCombo->addItems(models);
        L_modelsCombo->setCurrentIndex(0);
        showTip(QStringLiteral("已获取 %1 个模型").arg(models.size()), false);
    }



    /**
     * @brief onSaveClicked
     * - 校验非空 -> 收集下拉框全部模型 -> 经 LosRouter 抛给网络层
     */
    void LosAgentKeyUi::onSaveClicked()
    {
        const QString provider = L_providerEdit->text().trimmed();
        const QString baseUrl  = L_baseUrlEdit->text().trimmed();
        const QString apiKey   = L_apiKeyEdit->text().trimmed();
        if (provider.isEmpty() || baseUrl.isEmpty() || apiKey.isEmpty())
        {
            showTip(QStringLiteral("请填写完整信息"), true);
            return;
        }
        QStringList models;
        for (int i = 0; i < L_modelsCombo->count(); ++i)
        {
            const QString t = L_modelsCombo->itemText(i).trimmed();
            if (!t.isEmpty() && !models.contains(t))
                models.append(t);
        }
        if (models.isEmpty())
        {
            const QString cur = L_modelsCombo->currentText().trimmed();
            if (!cur.isEmpty())
                models.append(cur);
        }
        if (models.isEmpty())
        {
            showTip(QStringLiteral("请先获取或手动填写模型"), true);
            return;
        }
        emit LosCore::LosRouter::instance()._cmd_agent_addProvider_request(provider, baseUrl, apiKey, models);
        L_saveBtn->setEnabled(false);
        showTip(QStringLiteral("提交中..."), false);
    }



    /**
     * @brief onProviderAdded
     * - 网络层返回添加结果: 成功则提示并延时关闭弹窗, 失败则恢复按钮并红字提示
     */
    void LosAgentKeyUi::onProviderAdded(bool success, const QString &message)
    {
        L_saveBtn->setEnabled(true);
        if (!success)
        {
            showTip(message.isEmpty() ? QStringLiteral("添加失败") : message, true);
            return;
        }
        showTip(QStringLiteral("添加成功"), false);
        QTimer::singleShot(600, this, &QDialog::accept);
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
