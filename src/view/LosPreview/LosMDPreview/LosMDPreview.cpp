// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include <QDesktopServices>
#include <QVBoxLayout>

#include "LosMDPreview.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/LosPreview/LosPreview.h"
#include "view/style/LosMDPreview_style.h"

namespace LosView
{
    /**
     * @brief Construct a new Los M D Preview:: Los M D Preview object
     *
     * @param parent
     */
    LosMDPreview::LosMDPreview(QWidget *parent) : LosPreview(parent)
    {
        L_browser = new QTextBrowser(this);
        L_browser->setOpenLinks(false);
        connect(L_browser, &QTextBrowser::anchorClicked, this, &LosMDPreview::onAnchorClicked);
        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(L_browser);
        const QString theme = LosCore::LosThemeManager::instance().currentTheme();
        L_browser->setStyleSheet(LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::LosMDPreview_styleTemplate(), theme));
    }



    /**
     * @brief onAnchorClicked 处理预览中的链接点击
     * - 外部链接(http/https)用系统默认浏览器打开
     * - 其余(相对路径/锚点)预览场景下忽略, 避免 QTextBrowser 内部导航崩溃
     * @param url 被点击的链接
     */
    void LosMDPreview::onAnchorClicked(const QUrl &url)
    {
        const QString scheme = url.scheme();
        if (scheme == QStringLiteral("http") || scheme == QStringLiteral("https"))
        {
            QDesktopServices::openUrl(url);
        }
    }



    /**
     * @brief 主题 和 字体 发生改变地时候 发生地 变化
     */
    void LosMDPreview::applyStyleChange()
    {
        const auto &theme = LosCore::LosThemeManager::instance().currentTheme();
        L_browser->setStyleSheet(LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::LosMDPreview_styleTemplate(), theme));
        L_browser->document()->setDefaultStyleSheet(
            LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::LosMDPreview_docCssTemplate(), theme));
    }



    /**
     * @brief 建立 连接 修改 字体 和 样式地时候
     *
     */
    void LosMDPreview::initConnect() noexcept
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_fontChanged, this, &LosMDPreview::applyStyleChange);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, &LosMDPreview::applyStyleChange);
    }


    /**
     * @brief 直接渲染
     *
     * @param content
     */


    void LosMDPreview::render(const QString &content)
    {
        // 文档级 CSS 必须在 setMarkdown 之前设置, 否则不作用于本次渲染
        const QString theme = LosCore::LosThemeManager::instance().currentTheme();
        L_browser->document()->setDefaultStyleSheet(
            LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::LosMDPreview_docCssTemplate(), theme));
        L_browser->setMarkdown(content);
        L_content = content;
    }
} // namespace LosView