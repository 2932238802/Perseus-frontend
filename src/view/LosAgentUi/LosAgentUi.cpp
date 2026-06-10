// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentUi.h"
#include "./ui_LosAgentUi.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/LosAgentKeyUi/LosAgentKeyUi.h"
#include "view/style/LosAgent_style.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>

namespace LosView
{
    /**
     * @brief 构造函数
     * - 构建控件 -> 着色 -> 连接信号
     */
    LosAgentUi::LosAgentUi(QWidget *parent) : QWidget(parent), ui(new Ui::LosAgentUi)
    {
        initUi();
        initStyle();
        initConnect();
    }
    LosAgentUi::~LosAgentUi()
    {
        delete ui;
    }



    /**
     * @brief   装载 .ui 表单
     */
    void LosAgentUi::initUi()
    {
        ui->setupUi(this);
        loadProviders();
    }



    /**
     * @brief loadProviders
     * - 填充厂商下拉 (第一版: 示例预设, 后续改为从后端 / 本地配置读取)
     * - 选中厂商后联动刷新模型下拉
     */
    void LosAgentUi::loadProviders()
    {
        ui->provider_combo->clear();
        ui->provider_combo->addItem(QStringLiteral("deepseek"));
        ui->provider_combo->addItem(QStringLiteral("openai"));
        ui->provider_combo->addItem(QStringLiteral("moonshot"));
        // 初始化模型下拉
        onProviderChanged(ui->provider_combo->currentIndex());
    }



    /**
     * @brief onProviderChanged
     * - 厂商切换 -> 刷新模型下拉 (第一版: 示例预设)
     */
    void LosAgentUi::onProviderChanged(int index)
    {
        Q_UNUSED(index);
        const QString provider = ui->provider_combo->currentText();
        ui->model_combo->clear();
        if (provider == QStringLiteral("deepseek"))
        {
            ui->model_combo->addItem(QStringLiteral("deepseek-chat"));
            ui->model_combo->addItem(QStringLiteral("deepseek-reasoner"));
        }
        else if (provider == QStringLiteral("openai"))
        {
            ui->model_combo->addItem(QStringLiteral("gpt-4o"));
            ui->model_combo->addItem(QStringLiteral("gpt-4o-mini"));
        }
        else if (provider == QStringLiteral("moonshot"))
        {
            ui->model_combo->addItem(QStringLiteral("moonshot-v1-8k"));
            ui->model_combo->addItem(QStringLiteral("moonshot-v1-32k"));
        }
    }



    /**
     * @brief onAddClicked
     * - 弹出添加 AI 配置对话框
     */
    void LosAgentUi::onAddClicked()
    {
        LosAgentKeyUi dialog(this);
        dialog.exec();
        // 保存成功后可在此刷新厂商下拉 (后续接入)
    }



    /**
     * @brief initStyle
     * - 用当前主题着色
     */
    void LosAgentUi::initStyle()
    {
        applyTheme(LosCore::LosThemeManager::instance().currentTheme());
    }



    /**
     * @brief applyTheme
     * - 主题换肤回调 (与 LosAuthUi 约定一致)
     * - 把 LosAgent_style.h 模板里的 @token@ 替换为当前主题色后应用
     */
    void LosAgentUi::applyTheme(const QString &themeName)
    {
        const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::losAgent_getStyleTemplate(), themeName);
        setStyleSheet(qss);
    }



    /**
     * @brief initConnect
     * - 发送按钮 / 回车提交 / Agent 回包 / 主题换肤
     */
    void LosAgentUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(ui->send_btn, &QPushButton::clicked, this, &LosAgentUi::onSendClicked);
        connect(ui->input_edit, &QLineEdit::returnPressed, this, &LosAgentUi::onSendClicked);
        connect(ui->add_btn, &QPushButton::clicked, this, &LosAgentUi::onAddClicked);
        connect(ui->provider_combo, &QComboBox::currentIndexChanged, this, &LosAgentUi::onProviderChanged);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
    }



    /**
     * @brief onSendClicked
     * - 立刻显示用户气泡 -> 把消息抛给 core (LosAgentManager)
     */
    void LosAgentUi::onSendClicked()
    {
        const QString text = ui->input_edit->text().trimmed();
        if (text.isEmpty())
            return;
        addBubble(Role::User, text);
        emit LosCore::LosRouter::instance()._cmd_agent_sendMessage(text);
        ui->input_edit->clear();
    }



    /**
     * @brief onAgentReply
     */
    void LosAgentUi::onAgentReply(const QString &message)
    {
        addBubble(Role::Agent, message);
    }



    /**
     * @brief
     *
     */
    void LosAgentUi::onAgentError(const QString &message)
    {
        addBubble(Role::Agent, message);
    }



    /**
     * @brief addBubble
     * - 向消息列表追加一个气泡
     * - User 靠右, Agent 靠左; 配色由 objectName 在 LosAgent_style.h 控制
     *
     * @param role     消息角色
     * @param content  消息内容
     */
    void LosAgentUi::addBubble(Role role, const QString &content)
    {
        const bool isUser    = (role == Role::User);
        const int viewW      = ui->chat_view->viewport()->width();
        const int maxBubbleW = qMax(120, static_cast<int>(viewW * 0.75));
        const int padH       = 6;
        const int padV       = 4;
        QLabel *bubble       = new QLabel(content);
        bubble->setObjectName(isUser ? QStringLiteral("agentBubbleUser") : QStringLiteral("agentBubbleAgent"));
        bubble->setWordWrap(true);
        bubble->setTextInteractionFlags(Qt::TextSelectableByMouse);
        bubble->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        bubble->setContentsMargins(padH, padV, padH, padV);
        bubble->setMaximumWidth(maxBubbleW);
        const int textW = maxBubbleW - padH * 2;
        int textH       = bubble->heightForWidth(textW);
        if (textH <= 0)
            textH = bubble->fontMetrics().height();
        const int bubbleH = textH + padV * 2;
        QWidget *holder   = new QWidget();
        QHBoxLayout *lay  = new QHBoxLayout(holder);
        lay->setContentsMargins(8, 4, 8, 4);
        if (isUser)
        {
            lay->addStretch(1);
            lay->addWidget(bubble);
        }
        else
        {
            lay->addWidget(bubble);
            lay->addStretch(1);
        }
        const int itemH       = bubbleH + 8 + 6;
        QListWidgetItem *item = new QListWidgetItem(ui->chat_view);
        item->setSizeHint(QSize(viewW, itemH));
        ui->chat_view->setItemWidget(item, holder);
        ui->chat_view->scrollToBottom();
    }
} /* namespace LosView */
