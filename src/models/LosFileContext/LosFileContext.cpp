#include "LosFileContext.h"

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



    std::optional<QString> LosFileContext::load(const QString &file_path)
    {
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            WAR("the file is empty", "LosFileContext");
            return std::nullopt;
        }
        INF("load ... :" + file_path, "LosFileContext");
        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);
        L_content = in.readAll();
        file.close();
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