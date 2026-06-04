// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QByteArray>
#include <QString>

namespace LosCore
{
    class LosEncodingDetector
    {
      public: // public tool
        static QString detectFromFile(const QString &file_path);
        static QString detectFromBytes(const QByteArray &bytes);

      private: // private tool
        static bool tryCodec(const QByteArray &bytes, const char *codecName, double minCjkRatio = 0.30);
        static bool isPureAscii(const QByteArray &bytes);
    };
} // namespace LosCore