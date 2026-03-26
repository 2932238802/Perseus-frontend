

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

QString LosFileContext::load(const QString &file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "wrong in open:" + file_path;
    }
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    L_content = in.readAll();
    file.close();
    L_isDirty  = false;
    L_isLoaded = true;
    return L_content;
}

bool LosFileContext::save(const QString &content, const QString &file_path)
{
    if (file_path.isEmpty())
        return false;
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << content;
    file.close();
    L_isDirty = false;
    return true;
}

bool LosFileContext::isDirty() const
{
    return L_isDirty;
}

bool LosFileContext::isLoaded() const
{
    return L_isLoaded;
}



/**
set
*/
void LosFileContext::setDirty(bool dirty)
{
    L_isDirty = dirty;
}



/**

*/
bool LosFileContext::isEmpty() const
{
    return L_content.isEmpty();
}



/**
获取内容
*/
const QString& LosFileContext::getContent() const
{
    return L_content;
}


}; // namespace LosModel