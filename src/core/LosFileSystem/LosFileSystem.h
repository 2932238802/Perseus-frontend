// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QObject>
#include <qtmetamacros.h>
namespace LosCore
{
    class LosFileSystem : public QObject
    {
        Q_OBJECT
      public:
        static LosFileSystem &instance();
        bool createDir(const QString &);
        bool createFile(const QString &file_path, const QByteArray & = {});
        bool removeFile(const QString &file_path);
        bool removeDir(const QString &dir_path);
        bool renameFile(const QString &src, const QString &dst);
    };

} /* namespace LosCore */