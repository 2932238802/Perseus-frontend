#include "LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"

namespace LosModel
{

LosFileNode::LosFileNode(const QString &filePath, LosFileNode *parent) : LOS_parent(nullptr), LOS_filePath(filePath)
{
    if (parent)
        parent->appendChild(this);
}
LosFileNode::~LosFileNode()
{
    clear();
}



/**
- QDir::fromNativeSeparators 标准化
*/
LosFileNode *LosFileNode::create(const QString &filePath, LosFileNode *parent)
{
    LosFileNode *node = new LosFileNode(QDir::fromNativeSeparators(filePath), parent);
    return node;
}



/**
增加子元素
*/
void LosFileNode::appendChild(LosFileNode *node)
{
    if (node)
    {
        LOS_childrens.append(node);
        node->setParentNode(this);
    }
}



/**
清理
*/
void LosFileNode::clear()
{
    qDeleteAll(LOS_childrens);
    LOS_childrens.clear();
}



/**
设置 父亲 节点
*/
void LosFileNode::setParentNode(LosFileNode *node)
{
    LOS_parent = node;
}



/**
build

*/
void LosFileNode::build(LosFileNode *root, const QString &cur, std::function<void()> on_finished)
{

    QFuture<void> funture = QtConcurrent::run([root, cur]() { buildImpl(root, cur); });
    auto *watch           = new QFutureWatcher<void>();
    // Qt 默认这个 Lambda 表达式会在发出信号的那个线程中直接执行
    QObject::connect(watch, &QFutureWatcher<void>::finished,
                     [watch, on_finished]()
                     {
                         if (on_finished)
                         {
                             on_finished();
                         }
                         watch->deleteLater();
                     });
    watch->setFuture(funture);
}



/**
- 构建的 工具接口
*/
void LosFileNode::buildImpl(LosFileNode *root, const QString &cur)
{
    QDir dir(cur);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);

    QString canonicalPath = QFileInfo(cur).canonicalFilePath();
    QFileInfoList fileList(dir.entryInfoList());
    for (const auto &info : fileList)
    {
        QString abs        = info.absoluteFilePath();
        LosFileNode *child = LosFileNode::create(abs, root);
        if (info.isDir())
        {
            buildImpl(child, abs);
        }
    }
}


/**
获取元素
*/
int LosFileNode::getChildCount() const
{
    return LOS_childrens.size();
}



/**
获取子
*/
LosFileNode *LosFileNode::getChild(int index) const
{
    if (index >= 0 && index < LOS_childrens.size())
    {
        return LOS_childrens[index];
    }
    return nullptr;
}



/**
获取
*/
LosFileNode *LosFileNode::getParent() const
{
    return LOS_parent;
}



/**
获取完整的路径
*/
LosModel::LosFilePath &LosFileNode::getFile()
{
    return LOS_filePath;
}



/**
获取对应在 父亲那里的位置
*/
int LosFileNode::row() const
{
    if (LOS_parent)
        return LOS_parent->LOS_childrens.indexOf(const_cast<LosFileNode *>(this));
    else
    {
        return 0;
    }
}



LosCommon::LOS_ENUM_FileType LosFileNode::getFileType() const
{
    return LOS_filePath.getFileType();
}

}; // namespace LosModel