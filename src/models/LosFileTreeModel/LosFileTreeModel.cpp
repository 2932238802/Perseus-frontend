// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "common/constants/ConstantsClass/LosFileTreeModelClass.h"
#include "common/constants/ConstantsClass/LosFileType.h"
#include "common/constants/ConstantsClass/LosGitManagerClass.h"
#include "common/constants/ConstantsStr/LosFileTreeModelStr.h"
#include "common/util/GetFileIcon.h"
#include "core/LosFileSystem/LosFileSystem.h"
#include "core/LosGitManager/LosGitManager.h"
#include "core/LosRouter/LosRouter.h"
#include "git2.h"
#include "models/LosFileNode/LosFileNode.h"
#include <QDir>
#include <QFile>
#include <QFileIconProvider>
#include <QFileInfo>
#include <qbrush.h>
#include <qicon.h>
#include <qnamespace.h>
#include <qvariant.h>


namespace LosModel
{
    /**
     * @brief Construct a new Los File Tree Model:: Los File Tree Model object
     *
     * @param rootNode
     * @param parent
     */
    LosFileTreeModel::LosFileTreeModel(LosFileNode *rootNode, QObject *parent) : QAbstractItemModel{parent}, LOS_rootNode(rootNode)
    {
        initConnect();
    };
    LosFileTreeModel::~LosFileTreeModel()
    {
        if (LOS_rootNode)
        {
            delete LOS_rootNode;
            LOS_rootNode = nullptr;
        }
    };



    /**
     * @brief index 获取身份证
     *
     * @param row
     * @param column
     * @param parent
     * @return QModelIndex
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
     * @brief parent
     * 找爸爸
     * @param child
     * @return QModelIndex
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
     * @brief hasChildren
     *
     * @param parent
     * @return true
     * @return false
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
     * @brief flags
     *
     * @param index
     * @return Qt::ItemFlags
     */
    Qt::ItemFlags LosFileTreeModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }



    /**
     * @brief rowCount
     *
     * @param parent
     * @return int
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
     * @brief columnCount
     *
     * @param parent
     * @return int
     */
    int LosFileTreeModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 1;
    }



    /**
     * @brief data
     *
     * @param index
     * @param role
     * @return QVariant
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
        case Qt::EditRole:
        case Qt::DisplayRole:
        {
            return node->getFile().getFileName();
        }
        case Qt::DecorationRole:
        {
            if (node->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER)
                return LosCommon::GetFileIcon(":/icons/folder_white.png");
            const auto &file     = node->getFile();
            const QString &name  = file.getFileName();
            const QString suffix = file.getSuffix().toLower();
            if (auto it = LosCommon::kIconByName.constFind(name); it != LosCommon::kIconByName.cend())
                return LosCommon::GetFileIcon(it.value());
            if (auto it = LosCommon::kIconBySuffix.constFind(suffix); it != LosCommon::kIconBySuffix.cend())
                return LosCommon::GetFileIcon(it.value());
            static QFileIconProvider provider;
            return provider.icon(QFileIconProvider::File);
        }
        case Qt::ForegroundRole: // 这个是显示字体颜色
        {
            if (node->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER)
                return QVariant();
            QString absPath    = node->getFile().getAbsoluteFilePath();
            unsigned int flags = LosCore::LosGitManager::instance().statusOfFile(absPath);
            if (flags & GIT_STATUS_WT_MODIFIED || flags & GIT_STATUS_INDEX_MODIFIED)
                return QBrush(QColor(LosCommon::LosFileTreeModel_Constants::MODIFY_COLOR)); // 修改
            if (flags & GIT_STATUS_WT_NEW)
                return QBrush(QColor(LosCommon::LosFileTreeModel_Constants::WT_NEW_COLOR)); // 未跟踪新文件
            if (flags & GIT_STATUS_INDEX_NEW)
                return QBrush(QColor(LosCommon::LosFileTreeModel_Constants::INDEX_NEW_COLOR)); // 已暂存新增
            return QVariant();
        }
        case (LosCommon::LosFileTreeModel_Constants::FileRole::GITSTATUS_ROLE):
        {
            if (node->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER)
            {
                QString dirPath = node->getFile().getAbsoluteFilePath();
                if (LosCore::LosGitManager::instance().folderHasBeenChanged(dirPath))
                    return QStringLiteral("●");
                return QVariant();
            }
            QString path       = node->getFile().getAbsoluteFilePath();
            unsigned int flags = LosCore::LosGitManager::instance().statusOfFile(path);
            if (flags & GIT_STATUS_WT_MODIFIED || flags & GIT_STATUS_INDEX_MODIFIED)
                return QStringLiteral("M");
            if (flags & GIT_STATUS_WT_NEW)
                return QStringLiteral("U");
            if (flags & GIT_STATUS_INDEX_NEW)
                return QStringLiteral("A");
            return QVariant();
        }
        default:
        {
            return QVariant();
        }
        }
    }



    /**
     * @brief setData
     *
     * @param index
     * @param value
     * @param role
     * @return true
     * @return false
     */
    bool LosFileTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!index.isValid() || role != Qt::EditRole)
        {
            return false;
        }
        LosFileNode *node = nodeFromIndex(index);
        if (!node)
            return false;
        QString newName = value.toString().trimmed();
        if (newName.isEmpty())
            return false;

        QString oldPath = node->getFile().getFilePath();
        QFileInfo oldInfo(oldPath);
        QString parentDir = oldInfo.absolutePath();
        QString newPath   = QDir(parentDir).filePath(newName);

        if (QFile::exists(newPath) || QDir(newPath).exists())
        {
            return false;
        }
        if (!LosCore::LosFileSystem::instance().renameFile(oldPath, newPath))
            return false;
        node->getFile().setFilePath(newPath);

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        emit LosCore::LosRouter::instance()._cmd_fileRenamed(oldPath, newPath);
        emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
        return true;
    }



    /**
     * @brief nodeFromIndex
     *
     * @param index
     * @return LosFileNode*
     */
    LosFileNode *LosFileTreeModel::nodeFromIndex(const QModelIndex &index)
    {
        if (!index.isValid())
            return LOS_rootNode;
        return static_cast<LosFileNode *>(index.internalPointer());
    }



    /**
     * @brief getRoot 获取根节点
     *
     * @return LosModel::LosFileNode*
     */
    LosModel::LosFileNode *LosFileTreeModel::getRoot() const
    {
        return LOS_rootNode;
    }


    void LosFileTreeModel::initConnect()
    {
        connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_gitStatusUpdated, this, [this]() { emit layoutChanged(); });
    }

} /* namespace LosModel */