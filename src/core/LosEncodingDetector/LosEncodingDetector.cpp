#include "LosEncodingDetector.h"
#include <QByteArray>
#include <QFile>
#include <QString>
#include <QStringConverter>
#include <QStringDecoder>
#include <QTextCodec>
#include <QtGlobal>
#include <qglobal.h>
#include <qtextcodec.h>

namespace LosCore
{
    /**
     * @brief
     *
     * @param file_path
     * @return QString
     */
    QString LosEncodingDetector::detectFromFile(const QString &file_path)
    {
        constexpr qint64 SAMPLE_BYTES = 8 * 1024;
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly))
        {
            return QStringLiteral("Unknown");
        }
        return detectFromBytes(file.read(SAMPLE_BYTES));
    }



    /**
     * @brief
     *
     * @param file_data
     * @return QString
     */
    QString LosEncodingDetector::detectFromBytes(const QByteArray &bytes)
    {
        static const QByteArray BOM_UTF32_LE = QByteArray::fromHex("FFFE0000");
        static const QByteArray BOM_UTF32_BE = QByteArray::fromHex("0000FEFF");
        static const QByteArray BOM_UTF16_LE = QByteArray::fromHex("FFFE");
        static const QByteArray BOM_UTF16_BE = QByteArray::fromHex("FEFF");
        static const QByteArray BOM_UTF8     = QByteArray::fromHex("EFBBBF");
        if (bytes.startsWith(BOM_UTF32_LE))
        {
            return QStringLiteral("UTF-32 LE");
        }
        if (bytes.startsWith(BOM_UTF32_BE))
        {
            return QStringLiteral("UTF-32 BE");
        }
        if (bytes.startsWith(BOM_UTF16_LE))
        {
            return QStringLiteral("UTF-16 LE");
        }
        if (bytes.startsWith(BOM_UTF16_BE))
        {
            return QStringLiteral("UTF-16 BE");
        }
        if (bytes.startsWith(BOM_UTF8))
        {
            return QStringLiteral("UTF-8 with BOM");
        }
        if (isPureAscii(bytes))
        {
            return QStringLiteral("UTF-8");
        }
        QStringDecoder dec(QStringConverter::Utf8);
        (void)dec.decode(bytes);
        if (!dec.hasError())
        {
            return QStringLiteral("UTF-8");
        }
        if (tryCodec(bytes, "GBK", 0.30))
        {
            return QStringLiteral("GBK");
        }
        if (tryCodec(bytes, "GB18030", 0.30))
        {
            return QStringLiteral("GB18030");
        }
        return QStringLiteral("Unknown");
    }



    /**
     * @brief tryCodeC
     *
     * @param bytes
     * @param codecName
     * @param minCjkRatio
     * @return true
     * @return false
     */
    bool LosEncodingDetector::tryCodec(const QByteArray &bytes, const char *codecName, double minCjkRatio)
    {
        QTextCodec *codec = QTextCodec::codecForName(codecName);
        if (!codec)
        {
            return false;
        }
        QTextCodec::ConverterState state;
        QString rst = codec->toUnicode(bytes.constData(), bytes.size(), &state);
        if (state.invalidChars > 0)
        {
            return false;
        }
        int total = rst.size();
        if (total == 0)
        {
            return false;
        }
        int cjkCount = 0;
        for (QChar c : rst) 
        {
            ushort u = c.unicode();
            // 0x4E00 9FFF 常见中文字符
            if (u >= 0x4E00 && u <= 0x9FFF)
            {
                cjkCount++;
            }
        }
        double ratio = static_cast<double>(cjkCount) / total;
        return ratio >= minCjkRatio;
    }



    /**
     * @brief
     *
     * @param bytes
     * @return true
     * @return false
     */
    bool LosEncodingDetector::isPureAscii(const QByteArray &bytes)
    {
        for (unsigned char c : bytes)
        {
            if (c >= 0x80)
                return false;
        }
        return true;
    }
} // namespace LosCore