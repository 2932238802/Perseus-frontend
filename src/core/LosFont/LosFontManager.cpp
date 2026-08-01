// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosFontManager.h"

#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <algorithm>

namespace LosCore
{
    /**
     * @brief 单例模式
     * 线程安全的
     * @return LosFontManager&
     */
    LosFontManager &LosFontManager::instance()
    {
        static LosFontManager s;
        return s;
    }



    /**
     * @brief Construct a new Los Font Manager:: Los Font Manager object
     *
     * @param parent
     */
    LosFontManager::LosFontManager(QObject *parent) : QObject(parent) {}



    /**
     * @brief 列举本机可用的所有字体族 (按本地化规则排序)
     *
     * @return QStringList
     */
    QStringList LosFontManager::availableFonts() const
    {
        QStringList families = QFontDatabase::families();
        std::sort(families.begin(), families.end(), [](const QString &a, const QString &b) { return QString::localeAwareCompare(a, b) < 0; });
        return families;
    }



    /**
     * @brief 当前字体族, 空串表示使用系统默认
     *
     * @return QString
     */
    QString LosFontManager::currentFontFamily() const
    {
        return L_curFontFamily;
    }



    /**
     * @brief 切换全局字体
     *
     * @param family 空串 = 恢复系统默认
     * @param persist
     * @return true
     * @return false
     */
    bool LosFontManager::setFontFamily(const QString &family, bool persist)
    {
        if (!family.isEmpty() && !QFontDatabase::families().contains(family))
        {
            WAR("font not found: " + family, "LosFontManager");
            return false;
        }
        L_curFontFamily = family;
        QFont font      = QApplication::font();
        if (!family.isEmpty())
        {
            font.setFamily(family);
        }
        QApplication::setFont(font);
        emit LosCore::LosRouter::instance()._cmd_fontChanged(family);
        if (persist)
        {
            // 持久化由 LosSession
            // 在主窗口关闭时统一处理
        }
        SUC("font switched: " + (family.isEmpty() ? QStringLiteral("(system default)") : family), "LosFontManager");
        return true;
    }



    /**
     * @brief 注入 QSS @fontFamily@ 占位符的字体声明
     * - 未选择时返回默认回退列表
     * - 与 perseus_style.qss 原行为一致
     * - 用户选择时返回带引号的单个字体族
     *
     * @return QString
     */
    QString LosFontManager::qssFontDeclaration() const
    {
        if (L_curFontFamily.isEmpty())
        {
            return QStringLiteral("\"Microsoft YaHei\", \"Segoe UI\", sans-serif");
        }
        QString escaped = L_curFontFamily;
        escaped.replace('\\', "\\\\").replace('"', "\\\"");
        return QStringLiteral("\"%1\"").arg(escaped);
    }
} /* namespace LosCore */
