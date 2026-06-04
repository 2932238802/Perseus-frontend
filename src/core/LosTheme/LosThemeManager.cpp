// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosThemeManager.h"

#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QResource>
#include <QStringList>
#include <QTextStream>


namespace LosCore
{

    /**
     * @brief 单例模式
     *
     * @return LosThemeManager&
     */
    LosThemeManager &LosThemeManager::instance()
    {
        static LosThemeManager s;
        return s;
    }



    /**
     * @brief Construct a new Los Theme Manager:: Los Theme Manager object
     *
     * @param parent
     */
    LosThemeManager::LosThemeManager(QObject *parent) : QObject(parent) {}


    /**
     * @brief 列举 :/highlight/themes/ 下所有 .json 主题
     *
     * @return QStringList
     */
    QStringList LosThemeManager::availableThemes() const
    {
        QStringList result;
        QDir dir(QStringLiteral(":/highlight/themes/"));
        if (!dir.exists())
        {
            WAR("themes dir not found in qrc", "LosThemeManager");
            return result;
        }
        const QFileInfoList list = dir.entryInfoList(QStringList() << "*.json", QDir::Files, QDir::Name);
        for (const auto &info : list)
        {
            result.append(info.baseName());
        }
        return result;
    }



    /**
     * @brief  currentTheme
     *
     * @return QString
     */
    QString LosThemeManager::currentTheme() const
    {
        return L_curTheme;
    }



    /**
     * @brief currentDisplayName
     *
     * @return QString
     */
    QString LosThemeManager::currentDisplayName() const
    {
        const QJsonObject obj = themeJson(L_curTheme);
        const QString display = obj.value(QStringLiteral("displayName")).toString();
        return display.isEmpty() ? L_curTheme : display;
    }



    /**
     * @brief 切主题
     *
     * @param themeName
     * @param persist
     * @return true
     * @return false
     */
    bool LosThemeManager::setTheme(const QString &themeName, bool persist)
    {
        const QStringList all = availableThemes();
        if (!all.contains(themeName))
        {
            WAR("theme not found: " + themeName, "LosThemeManager");
            return false;
        }
        L_curTheme = themeName;
        emit LosCore::LosRouter::instance()._cmd_themeChanged(themeName);
        if (persist)
        {
            // 持久化由 LosSession 在主窗口关闭时统一处理
        }
        SUC("theme switched: " + themeName, "LosThemeManager");
        return true;
    }


    /**
     * @brief 读 :/highlight/themes/<name>.json
     *
     * @param themeName
     * @return QJsonObject
     */
    QJsonObject LosThemeManager::themeJson(const QString &themeName) const
    {
        const QString path = QStringLiteral(":/highlight/themes/%1.json").arg(themeName);
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
        {
            ERR("open theme failed: " + path, "LosThemeManager");
            return QJsonObject();
        }
        QJsonParseError err;
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject())
        {
            ERR("parse theme failed: " + err.errorString(), "LosThemeManager");
            return QJsonObject();
        }
        return doc.object();
    }



    /**
     * @brief 提取 ui 节为 key/value map ui 字段可能缺失, 缺失时返回空 map
     *
     * @param themeName
     * @return QHash<QString, QString>
     */
    QHash<QString, QString> LosThemeManager::uiTokens(const QString &themeName) const
    {
        QHash<QString, QString> tokens;
        const QJsonObject root = themeJson(themeName);
        const QJsonObject ui   = root.value(QStringLiteral("ui")).toObject();
        for (auto it = ui.begin(); it != ui.end(); ++it)
        {
            tokens.insert(it.key(), it.value().toString());
        }
        return tokens;
    }



    /**
     * @brief  主 QSS = 模板 (perseus_style.qss) + 替换占位符
     *
     * @param themeName
     * @return QString
     */
    QString LosThemeManager::buildMainQss(const QString &themeName) const
    {
        const QString tmpl = readResourceText(QStringLiteral(":/style/perseus_style.qss"));
        if (tmpl.isEmpty())
        {
            ERR("perseus_style.qss empty", "LosThemeManager");
            return QString();
        }
        return applyTokens(tmpl, uiTokens(themeName));
    }



    /**
     * @brief 浮窗 QSS: 调用方传模板字符串 (Los*_style.h 提供)
     *
     * @param templateText
     * @param themeName
     * @return QString
     */
    QString LosThemeManager::buildExtraQss(const QString &templateText, const QString &themeName) const
    {
        return applyTokens(templateText, uiTokens(themeName));
    }



    /**
     * @brief templateText 中的 @key@ 全部替换为 tokens[key]
     *
     * @param templateText
     * @param tokens
     * @return QString
     */
    QString LosThemeManager::applyTokens(const QString &templateText, const QHash<QString, QString> &tokens) const
    {
        QString out = templateText;
        for (auto it = tokens.cbegin(); it != tokens.cend(); ++it)
        {
            const QString placeholder = QStringLiteral("@%1@").arg(it.key());
            out.replace(placeholder, it.value());
        }
        return out;
    }



    /**
     * @brief readResourceText
     *
     * @param qrcPath
     * @return QString
     */
    QString LosThemeManager::readResourceText(const QString &qrcPath) const
    {
        QFile file(qrcPath);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            return QString();
        }
        QTextStream s(&file);
        return s.readAll();
    }
} /* namespace LosCore */
