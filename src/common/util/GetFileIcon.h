// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QHash>
#include <QIcon>
#include <QString>

namespace LosCommon
{
    const QHash<QString, QString> kIconByName = {
        {"CMakeLists.txt", ":/icons/cmake_white.png"},
    };
    const QHash<QString, QString> kIconWhiteThemeBySuffix = {
        {"cpp", ":/icons/cpp_white.png"}, {"cc", ":/icons/cpp_white.png"},    {"cxx", ":/icons/cpp_white.png"},   {"h", ":/icons/h_white.png"},
        {"hpp", ":/icons/h_white.png"},   {"md", ":/icons/md_white.png"},     {"txt", ":/icons/txt_white.png"},   {"json", ":/icons/json_white.png"},
        {"rs", ":/icons/rust_white.png"}, {"py", ":/icons/python_white.png"}, {"html", ":/icons/html_white.png"}, {"out", ":/icons/exe_white.png"},
        {"exe", ":/icons/exe_white.png"}, {"c",":/icons/c_white.png"}
    };

    inline QIcon GetFileIcon(const QString &name_or_suffix)
    {
        static QHash<QString, QIcon> cache;
        auto it = cache.constFind(name_or_suffix);
        if (it != cache.cend())
            return it.value();
        QIcon icon(name_or_suffix);
        cache.insert(name_or_suffix, icon);
        return icon;
    }
} // namespace LosCommon