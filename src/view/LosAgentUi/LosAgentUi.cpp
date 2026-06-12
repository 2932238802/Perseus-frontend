// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentUi.h"
#include "./ui_LosAgentUi.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/LosAgentKeyUi/LosAgentKeyUi.h"
#include "view/style/LosAgent_style.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QtMath>
#include <QTextBrowser>
#include <QTextDocument>

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
    }



    /**
     * @brief loadProviders
     * - 向后端请求当前用户的厂商/模型配置 (list_providers)
     * - 真正的填充在 onProvidersReceived 中完成
     */
    void LosAgentUi::loadProviders()
    {
        emit LosCore::LosRouter::instance()._cmd_agent_listProviders_request();
    }



    /**
     * @brief onProvidersReceived
     * - 收到后端配置 -> 缓存映射 -> 填充厂商下拉 -> 联动模型下拉
     */
    void LosAgentUi::onProvidersReceived(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg)
    {
        Q_UNUSED(msg);
        if (!ok)
            return;
        L_providerModels = providerModels;
        ui->provider_combo->blockSignals(true);
        ui->provider_combo->clear();
        for (auto it = L_providerModels.constBegin(); it != L_providerModels.constEnd(); ++it)
            ui->provider_combo->addItem(it.key());
        ui->provider_combo->blockSignals(false);
        onProviderChanged(ui->provider_combo->currentIndex());
    }



    /**
     * @brief onProviderChanged
     * - 厂商切换 -> 从缓存映射取出该厂商模型, 刷新模型下拉
     */
    void LosAgentUi::onProviderChanged(int index)
    {
        Q_UNUSED(index);
        const QString provider = ui->provider_combo->currentText();
        ui->model_combo->clear();
        if (provider.isEmpty())
            return;
        const QStringList models = L_providerModels.value(provider);
        for (const QString &m : models)
            ui->model_combo->addItem(m);
    }



    /**
     * @brief onAddClicked
     * - 弹出添加 AI 配置对话框
     */
    void LosAgentUi::onAddClicked()
    {
        LosAgentKeyUi dialog(this);
        dialog.exec();
    }



    /**
     * @brief onProviderAdded
     * - 新增厂商配置成功后 -> 重新拉取一次, 刷新下拉
     */
    void LosAgentUi::onProviderAdded(bool success, const QString &message)
    {
        Q_UNUSED(message);
        if (success)
            loadProviders();
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
        connect(ui->refresh_btn, &QPushButton::clicked, this, &LosAgentUi::loadProviders);
        connect(ui->provider_combo, &QComboBox::currentIndexChanged, this, &LosAgentUi::onProviderChanged);
        connect(&router, &LosCore::LosRouter::_cmd_agent_listProviders_response, this, &LosAgentUi::onProvidersReceived);
        connect(&router, &LosCore::LosRouter::_cmd_agent_addProvider_response, this, &LosAgentUi::onProviderAdded);
        connect(&router, &LosCore::LosRouter::_cmd_auth_loginStateChanged, this,
                [this](bool loggedIn)
                {
                    if (loggedIn)
                        loadProviders();
                });
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
        connect(&router, &LosCore::LosRouter::_cmd_agent_reply, this,
                [this](bool ok, const QString &msg)
                {
                    if (ok)
                    {
                        onAgentReply(msg);
                    }
                    else
                    {
                        onAgentError(msg);
                    }
                });
        connect(&router, &LosCore::LosRouter::_cmd_agent_replyChunk, this, &LosAgentUi::onReplyChunk);
        connect(&router, &LosCore::LosRouter::_cmd_agent_replyDone, this, &LosAgentUi::onReplyDone);
        loadProviders();
    }



    /**
     * @brief onSendClicked
     * - 立刻显示用户气泡 -> 把消息抛给 core (LosAgentManager)
     * - 随后建立一个空的 Agent 气泡作为流式容器, 后续 chunk 不断往里追加
     */
    void LosAgentUi::onSendClicked()
    {
        const QString text = ui->input_edit->text().trimmed();
        if (text.isEmpty())
            return;
        addBubble(Role::User, text);
        emit LosCore::LosRouter::instance()._cmd_agent_sendMessage(text, ui->provider_combo->currentText(), ui->model_combo->currentText());
        ui->input_edit->clear();
        L_streamingBuffer.clear();
        addBubble(Role::Agent, QString()); // 内部会把 L_streamingBubble / L_streamingItem 指向它
    }



    /**
     * @brief onAgentReply
     */
    void LosAgentUi::onAgentReply(const QString &message)
    {
        INF("LosAgentUi", message);
        addBubble(Role::Agent, message);
    }



    /**
     * @brief
     */
    void LosAgentUi::onAgentError(const QString &message)
    {
        ERR("onAgentError", message);
        addBubble(Role::Agent, message);
    }



    /**
     * @brief onReplyChunk
     * - 收到流式回复的一个增量片段
     * - 累积到 buffer, 整段重渲染当前 Agent 气泡的 Markdown, 并重算行高 (打字机效果)
     *
     * @param data 本次到达的文字片段
     */
    void LosAgentUi::onReplyChunk(const QString &data)
    {
        if (L_streamingBubble == nullptr) // 没有正在接收的气泡, 忽略 (防御)
            return;
        L_streamingBuffer += data;
        L_streamingBubble->document()->setMarkdown(L_streamingBuffer); 
        relayoutStreamingBubble();                                     
    }



    /**
     * @brief onReplyDone
     * - 流式回复结束: 给当前气泡"封口", 清理流式状态, 为下一轮做准备
     * - 幂等: replyDone 可能被触发两次 ([DONE] + finished), 第二次进来直接返回
     */
    void LosAgentUi::onReplyDone()
    {
        if (L_streamingBubble == nullptr) // 已经收过尾, 直接返回 (幂等保护)
            return;
        // 收到的全是空内容 (如出错没产生任何片段) 时给个占位提示
        if (L_streamingBuffer.isEmpty())
            L_streamingBubble->document()->setMarkdown(QStringLiteral("_(无回复)_"));
        relayoutStreamingBubble();

        // 封口: 解除对当前气泡的引用, 下一轮 onSendClicked 会重新建立
        L_streamingBubble = nullptr;
        L_streamingItem   = nullptr;
        L_streamingBuffer.clear();
    }



    /**
     * @brief addBubble
     * - 向消息列表追加一个气泡
     * - User 靠右, Agent 靠左; 配色由 objectName 在 LosAgent_style.h 控制
     * - Agent 气泡返回内部 QTextBrowser* (供流式逐片追加), User 气泡返回 nullptr
     *
     * @param role     消息角色
     * @param content  消息内容
     * @return QTextBrowser* Agent 气泡的浏览器指针; User 气泡为 nullptr
     */
    QTextBrowser *LosAgentUi::addBubble(Role role, const QString &content)
    {
        const bool isUser    = (role == Role::User);
        const int viewW      = ui->chat_view->viewport()->width();
        const int maxBubbleW = qMax(120, static_cast<int>(viewW * 0.75));
        const int padV       = 4;
        // 横向余量必须与 LosAgent_style.h 中 #agentBubbleAgent 的 QSS 保持一致:
        //   padding: 4px 10px  -> 左右各 10px;  border: 1px -> 左右各 1px
        // 否则 document 的 textWidth 会比 QTextBrowser viewport 实际可视宽度大,
        // 导致内容横向溢出被裁切 (即"内容被遮挡, 需右移光标框选才看得全")。
        const int cssPadH    = 10; // QSS 左右内边距
        const int cssBorder  = 1;  // QSS 左右边框
        const int chromeH    = (cssPadH + cssBorder) * 2; // 单侧之和 *2 = 横向总占用
        const int textW      = maxBubbleW - chromeH;

        QWidget *bubble        = nullptr; // 统一用基类指针, 便于后面放进布局
        QTextBrowser *browser  = nullptr; // 仅 Agent 气泡使用, 用于返回
        int bubbleH            = 0;

        if (isUser)
        {
            // 用户气泡: 纯文本 QLabel (无需 Markdown)
            // 内边距由 QSS #agentBubbleUser (padding: 8px 12px) 提供, 这里不再叠加 setContentsMargins,
            // 避免双重内边距导致测量宽度与实际显示宽度不一致。
            QLabel *label = new QLabel(content);
            label->setObjectName(QStringLiteral("agentBubbleUser"));
            label->setWordWrap(true);
            label->setTextInteractionFlags(Qt::TextSelectableByMouse);
            label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
            label->setMaximumWidth(maxBubbleW);
            int textH = label->heightForWidth(textW);
            if (textH <= 0)
                textH = label->fontMetrics().height();
            bubbleH = textH + padV * 2;
            bubble  = label;
        }
        else
        {
            // Agent 气泡: QTextBrowser 渲染 Markdown
            browser = new QTextBrowser();
            browser->setObjectName(QStringLiteral("agentBubbleAgent"));
            browser->setFrameShape(QFrame::NoFrame);
            browser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            browser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            browser->setOpenExternalLinks(true);
            browser->setTextInteractionFlags(Qt::TextBrowserInteraction);
            browser->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
            browser->document()->setDocumentMargin(0);  // 内边距统一交给 QSS, 文档自身不再留白
            browser->document()->setMarkdown(content);   // Qt6 原生 Markdown 渲染
            browser->document()->setTextWidth(textW);     // 先按最大可用宽度排版

            // 让气泡贴合内容: 取文档真正需要的宽度 (idealWidth), 但不超过 textW。
            // 之后必须把 widget 的固定宽度设为 "排版宽 + QSS横向余量", 使
            // document textWidth == viewport 可视宽度, 内容才不会横向溢出被裁切。
            const int idealW   = qCeil(browser->document()->idealWidth());
            const int finalTxtW = qBound(1, idealW, textW);
            browser->document()->setTextWidth(finalTxtW); // 用贴合后的宽度重新排版
            const int bubbleW   = finalTxtW + chromeH;
            browser->setFixedWidth(bubbleW);

            const qreal docH = browser->document()->size().height();
            bubbleH          = qMax(static_cast<int>(docH) + padV * 2, browser->fontMetrics().height() + padV * 2);
            browser->setFixedHeight(bubbleH);
            bubble = browser;
        }

        QWidget *holder  = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout(holder);
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

        // Agent 气泡: 记录浏览器与列表项, 供流式逐片追加时重算行高
        if (!isUser)
        {
            L_streamingBubble = browser;
            L_streamingItem   = item;
        }
        return browser; // User 气泡返回 nullptr
    }



    /**
     * @brief relayoutStreamingBubble
     * - 流式内容变长后, 重新测量气泡高度并同步 QListWidgetItem 行高
     * - 否则气泡高度被创建时 setFixedHeight 钉死, 新增文字会被裁掉看不见
     */
    void LosAgentUi::relayoutStreamingBubble()
    {
        if (L_streamingBubble == nullptr || L_streamingItem == nullptr)
            return;
        const int viewW      = ui->chat_view->viewport()->width();
        const int maxBubbleW = qMax(120, static_cast<int>(viewW * 0.75));
        const int padV       = 4;
        // 与 addBubble 中保持一致的横向余量 (QSS padding 10px + border 1px)
        const int cssPadH    = 10;
        const int cssBorder  = 1;
        const int chromeH    = (cssPadH + cssBorder) * 2;
        const int textW      = maxBubbleW - chromeH;

        // 先按最大宽排版, 取内容真正需要的宽度, 再据此固定 widget 宽度,
        // 保证 document textWidth == viewport 可视宽度, 流式追加的内容不会被横向裁切。
        L_streamingBubble->document()->setTextWidth(textW);
        const int idealW    = qCeil(L_streamingBubble->document()->idealWidth());
        const int finalTxtW = qBound(1, idealW, textW);
        L_streamingBubble->document()->setTextWidth(finalTxtW);
        L_streamingBubble->setFixedWidth(finalTxtW + chromeH);

        const qreal docH = L_streamingBubble->document()->size().height();
        const int bubbleH =
            qMax(static_cast<int>(docH) + padV * 2, L_streamingBubble->fontMetrics().height() + padV * 2);
        L_streamingBubble->setFixedHeight(bubbleH);
        const int itemH = bubbleH + 8 + 6;
        L_streamingItem->setSizeHint(QSize(viewW, itemH));
        ui->chat_view->scrollToBottom();
    }
} /* namespace LosView */
