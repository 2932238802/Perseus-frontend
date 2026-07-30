// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosFileContext.h"
#include "common/constants/ConstantsStr/LosEncodingDetectorStr.h"
#include "core/LosEncodingDetector/LosEncodingDetector.h"

#include <QStringDecoder>
#include <QtCore>
#include <cstddef>
#include <qglobal.h>
#include <qstringconverter_base.h>
#include <qtextcodec.h>

namespace LosModel
{

    LosFileContext::LosFileContext(const QString &file_path)
    {
        load(file_path);
    }



    LosFileContext::LosFileContext(const LosModel::LosFilePath &path)
    {
        load(path.getFilePath());
    }



    LosFileContext::LosFileContext() {}



    LosFileContext *LosFileContext::create()
    {
        LosFileContext *text = new LosFileContext();
        return text;
    }



    /**
     * @brief 加载文件
     *
     * @param file_path
     * @return std::optional<QString>
     */
    std::optional<QString> LosFileContext::load(const QString &file_path)
    {
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            WAR("the file is empty", "LosFileContext");
            return std::nullopt;
        }
        INF("load ... :" + file_path, "LosFileContext");
        // 返回二进制
        const QByteArray rawData = file.readAll();
        file.close(); // 及时 关闭
        QString codingWays(LosCore::LosEncodingDetector::detectFromBytes(rawData));
        // ENC_UTF8_BOM
        if (codingWays == LosCommon::LosEncodingDetector_Constants::ENC_UTF8_BOM)
        {
            // UTF-8 with BOM
            QByteArray data(rawData);
            data.remove(0, 3);
            // 初始化 方式 QStringConverter
            QStringDecoder decoder(QStringConverter::Utf8);
            L_content = decoder.decode(data);
            if (decoder.hasError())
            {
                ERR("decode error!", "LosFileContext::load");
            }
        }
        else if (codingWays == LosCommon::LosEncodingDetector_Constants::ENC_UTF8)
        {
            QStringDecoder decoder(QStringConverter::Utf8);
            L_content = decoder.decode(rawData);

            if (decoder.hasError())
            {
                WAR("invalid UTF-8 file: " + file_path, "LosFileContext");
                return std::nullopt;
            }
        }
        else if (codingWays == LosCommon::LosEncodingDetector_Constants::ENC_GBK ||
                 codingWays == LosCommon::LosEncodingDetector_Constants::ENC_GB18030)
        {
            // GBK 编码
            QTextCodec *codec = QTextCodec::codecForName(codingWays.toLatin1());
            if (nullptr == codec)
            {
                WAR("unsupported encoding: " + codingWays, "LosFileContext::load");
                return std::nullopt;
            }

            // QStringConverterBase::State
            // QStringConverterBase 是 QStringConverterBase 的派生类
            QTextCodec::ConverterState state;
            L_content = codec->toUnicode(rawData.data(), rawData.length(), &state);
            if (state.invalidChars > 0)
            {
                WAR("invalid encoded file: " + file_path, "LosFileContext");
                return std::nullopt;
            }
        }
        else if (codingWays == QStringLiteral("UTF-16 LE") || codingWays == QStringLiteral("UTF-16 BE"))
        {
            QTextCodec *codec = QTextCodec::codecForName(codingWays.toLatin1());
            if (codec == nullptr)
            {
                WAR("unsupported encoding: " + codingWays, "LosFileContext");
                return std::nullopt;
            }
            L_content = codec->toUnicode(rawData);
        }
        else
        {
            // 无法确定编码时
            // 优先按 UTF-8 尝试
            QStringDecoder decoder(QStringConverter::Utf8);
            L_content = decoder.decode(rawData);
            if (decoder.hasError())
            {
                WAR("unknown file encoding: " + file_path, "LosFileContext");
                return std::nullopt;
            }
        }
        L_isLoaded = true;
        return L_content;
    }



    /**
     * @brief save
     *
     * @param content
     * @param file_path
     * @return true
     * @return false
     */
    bool LosFileContext::save(const QString &content, const QString &file_path)
    {
        if (file_path.isEmpty())
            return false;
        QFile file(file_path);
        if (!file.open(QIODevice::WriteOnly))
        {
            return false;
        }
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << content;
        file.close();
        return true;
    }



    bool LosFileContext::isLoaded() const
    {
        return L_isLoaded;
    }



    /*
     * 空的
     */
    bool LosFileContext::isEmpty() const
    {
        return L_content.isEmpty();
    }



    /*
     * 获取内容
     */
    const QString &LosFileContext::getContent() const
    {
        return L_content;
    }


}; /* namespace LosModel */