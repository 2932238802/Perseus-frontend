// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QMap>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <qtmetamacros.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LosAgentUi;
}
class QTextBrowser;
class QListWidgetItem;
QT_END_NAMESPACE

namespace LosView
{
    class LosAgentUi : public QWidget
    {
        Q_OBJECT
      public: // construct
        explicit LosAgentUi(QWidget *parent = nullptr);
        ~LosAgentUi();

      private:
        enum class Role
        {
            User,
            Agent
        };
      private slots: // chs
        void onSendClicked();
        void onAddClicked();
        void onProviderChanged(int index);
        void onProvidersReceived(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg);
        void onProviderAdded(bool success, const QString &message);
        void onAgentReply(const QString &message);
        void onAgentError(const QString &message);
        void applyTheme(const QString &themeName);
        void onReplyChunk(const QString &data);
        void onReplyDone();

      private: // init
        void initUi();
        void initStyle();
        void initConnect();

      private: // tool
        // 追加一个气泡
        // Agent 气泡返回其 QTextBrowser* (供流式追加) 
        // User 气泡返回 nullptr
        QTextBrowser *addBubble(Role role, const QString &content);
        // 重新计算流式气泡的高度并同步 QListWidgetItem 行高 (内容变长时调用)
        void relayoutStreamingBubble();
        void loadProviders();

      private: // widgets
        Ui::LosAgentUi *ui;

      private:                                       // data
        QMap<QString, QStringList> L_providerModels; // 厂商名 -> 模型列表 (来自后端 list_providers)

      private: // streaming state (流式回复状态)
        QTextBrowser *L_streamingBubble  = nullptr;  // 当前正在接收的 Agent 气泡; nullptr 表示当前无流
        QListWidgetItem *L_streamingItem = nullptr;  // 该气泡对应的列表项 (用于动态更新行高)
        QString L_streamingBuffer;                   // 累积收到的全部文字 (用于整段重渲染 Markdown)
    };

} /* namespace LosView */
