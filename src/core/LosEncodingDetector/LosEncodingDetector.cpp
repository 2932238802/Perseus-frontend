// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosEncodingDetector.h"
#include "common/constants/ConstantsStr/LosEncodingDetectorStr.h"
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
    QString LosEncodingDetector::detectFromFile(const QString &file_path)
    {
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly))
        {
            return LosCommon::LosEncodingDetector_Constants::ENC_UNKNOWN;
        }
        return detectFromBytes(file.read(LosCommon::LosEncodingDetector_Constants::SAMPLE_BYTES));
    }



    QString LosEncodingDetector::detectFromBytes(const QByteArray &bytes)
    {
        using namespace LosCommon::LosEncodingDetector_Constants;

        static const QByteArray BOM_UTF32_LE = QByteArray::fromHex(BOM_HEX_UTF32_LE);
        static const QByteArray BOM_UTF32_BE = QByteArray::fromHex(BOM_HEX_UTF32_BE);
        static const QByteArray BOM_UTF16_LE = QByteArray::fromHex(BOM_HEX_UTF16_LE);
        static const QByteArray BOM_UTF16_BE = QByteArray::fromHex(BOM_HEX_UTF16_BE);
        static const QByteArray BOM_UTF8     = QByteArray::fromHex(BOM_HEX_UTF8);
        if (bytes.startsWith(BOM_UTF32_LE))
        {
            return ENC_UTF32_LE;
        }
        if (bytes.startsWith(BOM_UTF32_BE))
        {
            return ENC_UTF32_BE;
        }
        if (bytes.startsWith(BOM_UTF16_LE))
        {
            return ENC_UTF16_LE;
        }
        if (bytes.startsWith(BOM_UTF16_BE))
        {
            return ENC_UTF16_BE;
        }
        if (bytes.startsWith(BOM_UTF8))
        {
            return ENC_UTF8_BOM;
        }
        if (isPureAscii(bytes))
        {
            return ENC_UTF8;
        }
        QStringDecoder dec(QStringConverter::Utf8);
        (void)dec.decode(bytes);
        if (!dec.hasError())
        {
            return ENC_UTF8;
        }
        if (tryCodec(bytes, ENC_GBK, CJK_MIN_RATIO))
        {
            return ENC_GBK;
        }
        if (tryCodec(bytes, ENC_GB18030, CJK_MIN_RATIO))
        {
            return ENC_GB18030;
        }
        return ENC_UNKNOWN;
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
        using namespace LosCommon::LosEncodingDetector_Constants;
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
            if (u >= CJK_RANGE_MIN && u <= CJK_RANGE_MAX)
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