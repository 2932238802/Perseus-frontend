#include "models/LosFileTreeModel/LosFileTreeModel.h"


namespace LosModel
{

LosFileTreeModel::LosFileTreeModel(LosFileNode *rootNode, QObject *parent)
    : QAbstractItemModel{parent}, LOS_rootNode(rootNode){};

LosFileTreeModel::~LosFileTreeModel()
{
    if (LOS_rootNode)
    {
        delete LOS_rootNode;
        LOS_rootNode = nullptr;
    }
};


/**
获取身份证
*/
QModelIndex LosFileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    LosFileNode *parentNode = nullptr;
    if (!parent.isValid())
    {
        parentNode = LOS_rootNode;
    }
    else
    {
        parentNode = static_cast<LosFileNode *>(parent.internalPointer());
    }

    LosFileNode *childNode = parentNode->getChild(row);

    if (childNode)
    {
        return createIndex(row, column, childNode);
    }

    return QModelIndex();
}



/**
找爸爸
*/
QModelIndex LosFileTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }

    LosFileNode *childNode  = static_cast<LosFileNode *>(child.internalPointer());
    LosFileNode *parentNode = childNode->getParent();

    if (!parentNode || parentNode == LOS_rootNode)
    {
        return QModelIndex();
    }

    return createIndex(parentNode->row(), 0, parentNode);
}



/**
判断孩子
*/
bool LosFileTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        LosFileNode *node = static_cast<LosFileNode *>(parent.internalPointer());
        if (node && node->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER)
        {
            return true;
        }
    }
    return QAbstractItemModel::hasChildren(parent);
}



/**
设置节点的交互标志 允许被选中和点击
*/
Qt::ItemFlags LosFileTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}



/**
问行数
*/
int LosFileTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    LosFileNode *parentNode = nullptr;
    if (!parent.isValid())
    {
        parentNode = LOS_rootNode;
    }
    else
    {
        parentNode = static_cast<LosFileNode *>(parent.internalPointer());
    }
    return parentNode ? parentNode->getChildCount() : 0;
}



/**
问列数
*/
int LosFileTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}



/**
要数据
*/
QVariant LosFileTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    LosFileNode *node = static_cast<LosFileNode *>(index.internalPointer());
    switch (role)
    {
    case Qt::DisplayRole:
    {
        return node->getFile().getFileName();
    }
    case Qt::DecorationRole:
    {
        static QFileIconProvider iconProvider;
        if (node->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER)
        {
            return iconProvider.icon(QFileIconProvider::Folder);
        }
        else
        {
            auto file      = node->getFile();
            QString suffix = file.getSuffix();
            if (suffix == "cpp" || suffix == "cc" || suffix == "cxx")
            {
                return QIcon(":/icons/cpp_white.png");
            }
            else if (suffix == "h" || suffix == "hpp")
            {
                return QIcon(":/icons/h_white.png");
            }
            else if (suffix == "md")
            {
                return QIcon(":/icons/md_white.png");
            }
            else if (suffix == "txt")
            {
                return QIcon(":/icons/txt_white.png");
            }
            else
            {
                return iconProvider.icon(QFileIconProvider::File);
            }
        }
    }
    default:
    {
        return QVariant();
    }
    }
}



/**
获取 内部 节点
*/
LosFileNode *LosFileTreeModel::nodeFromIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return LOS_rootNode;
    return static_cast<LosFileNode *>(index.internalPointer());
}



/**
- 获取 根节点
*/
LosModel::LosFileNode *LosFileTreeModel::getRoot() const
{
    return LOS_rootNode;
}

} // namespace LosModel