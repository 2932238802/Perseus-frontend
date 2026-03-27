#include "LosFileTreeUi.h"
#include "core/LosRouter/LosRouter.h"
#include "core/log/LosLog/LosLog.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include <qabstractitemmodel.h>
#include <qabstractitemview.h>
#include <qaction.h>
#include <qdir.h>
#include <qevent.h>
#include <qfileinfo.h>
#include <qguiapplication.h>
#include <qlineedit.h>
#include <qmimedata.h>



namespace LosView
{
LosFileTreeUi::LosFileTreeUi(QWidget *parent) : QTreeView{parent}
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &LosFileTreeUi::onCustomContextMenu);

    initStyle();
    initConnect();

    setHeaderHidden(true);
    setAnimated(true);
    setIndentation(15);
}

/**
更新视图
*/
void LosFileTreeUi::updateExplorer(LosModel::LosFileTreeModel *model)
{
    if (model)
        setModel(model);
}


void LosFileTreeUi::initConnect() {}

void LosFileTreeUi::initStyle() {}


/**
- 双击 单位
*/
void LosFileTreeUi::onCustomContextMenu(const QPoint &pos)
{
    // 判断一下 合不合法 然后 路径获取一下 这边 其实可以拆分一下
    auto *treeModel = qobject_cast<LosModel::LosFileTreeModel *>(this->model());
    if (!treeModel)
        return;
    QModelIndex index               = this->indexAt(pos);
    bool isClickingValidItem        = index.isValid();
    LosModel::LosFileNode *treeNode = treeModel->nodeFromIndex(index);
    if (!treeNode)
        return;
    QString clickedPath = treeNode->getFilePath();
    bool isFolder       = (treeNode->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER);
    QString targetDir;
    if (isClickingValidItem)
    {
        targetDir = isFolder ? clickedPath : QFileInfo(clickedPath).absolutePath();
    }
    else
    {
        targetDir = clickedPath;
    }

    // 这边是 画出来
    QMenu menu(this);
    QAction *newFileAct   = menu.addAction("New File");
    QAction *newFolderAct = menu.addAction("New Folder");
    menu.addSeparator();
    QAction *copyAct = nullptr;
    QAction *delAct  = nullptr;


    if (isClickingValidItem)
    {
        copyAct = menu.addAction("Copy");
    }
    QAction *pasteAct         = menu.addAction("Paste");
    const QMimeData *clipData = QGuiApplication::clipboard()->mimeData();
    if (!clipData->hasUrls())
    {
        pasteAct->setEnabled(false);
    }

    if (isClickingValidItem)
    {
        menu.addSeparator();
        delAct = menu.addAction("Delete");
    }

    QAction *selectedAction = menu.exec(this->viewport()->mapToGlobal(pos));
    if (!selectedAction)
        return;

    if (selectedAction == newFileAct)
    {
        bool ok;
        QString fileName = QInputDialog::getText(this, "New File", "please input", QLineEdit::Normal, "", &ok);
        if (ok && !fileName.trimmed().isEmpty())
        {
            QString newFilePath = QDir(targetDir).filePath(fileName.trimmed());
            if (QFile::exists(newFilePath))
            {
                QMessageBox::warning(this, "Error", "file already exists");
            }
            else
            {
                QFile file(newFilePath);
                if (file.open(QIODevice::WriteOnly))
                {
                    file.close();
                    INF("created new file: " + newFilePath, "LosFileTreeUi");
                    emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
                }
                else
                {
                    ERR("failed to create file: " + newFilePath, "LosFileTreeUi");
                }
            }
        }
    }
    else if (selectedAction == newFolderAct)
    {
        bool ok;
        QString folderName =
            QInputDialog::getText(this, "New Folder", "Enter folder name:", QLineEdit::Normal, "", &ok);
        if (ok && !folderName.trimmed().isEmpty())
        {
            QString newFolderPath = QDir(targetDir).filePath(folderName.trimmed());
            if (QDir().exists(newFolderPath))
            {
                QMessageBox::warning(this, "Error", "Folder already exists!");
            }
            else
            {
                if (QDir().mkpath(newFolderPath))
                {
                    INF("created new folder: " + newFolderPath, "LosFileTreeUi");
                    emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
                }
                else
                {
                    ERR("failed to create folder: " + newFolderPath, "LosFileTreeUi");
                }
            }
        }
    }
    else if (selectedAction == copyAct)
    {
        QMimeData *mimeData = new QMimeData();
        QList<QUrl> urls;
        urls.append(QUrl::fromLocalFile(clickedPath));
        mimeData->setUrls(urls);
        QGuiApplication::clipboard()->setMimeData(mimeData);
    }
    else if (selectedAction == pasteAct)
    {
        QList<QUrl> urls     = clipData->urls();
        bool pastedSomething = false;

        for (const QUrl &url : urls)
        {
            if (url.isLocalFile())
            {
                QString srcPath = url.toLocalFile();
                QFileInfo srcInfo(srcPath);
                QString desPath = QDir(targetDir).filePath(srcInfo.fileName());
                if (QFile::exists(desPath))
                {
                    QMessageBox::warning(this, "Skip", "file already exists: " + srcInfo.fileName());
                    continue;
                }
                copyFileOrFolder(srcPath, desPath);
                pastedSomething = true;
            }
        }

        if (pastedSomething)
        {
            emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
        }
    }
    else if (selectedAction == delAct)
    {
        auto reply = QMessageBox::warning(this, "Delete", "delete:\n" + clickedPath + " ?",
                                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            deleteFileOrFolder(clickedPath);
            emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
        }
    }
}



/**
- 复制文件
*/
void LosFileTreeUi::copyFileOrFolder(const QString &src_path, const QString &des_path)
{
    LosModel::LosFilePath file(src_path);
    if (file.isFile())
    {
        // 如果是文件
        QFile::copy(src_path, des_path);
    }
    else if (file.isDir())
    {
        QDir dir(src_path);
        dir.mkpath(des_path);

        QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::Hidden);
        for (const QFileInfo &info : list)
        {
            copyFileOrFolder(info.absoluteFilePath(), des_path + QDir::separator() + info.fileName());
        }
    }
}



/**
- 删除文件夹
*/
void LosFileTreeUi::deleteFileOrFolder(const QString &file_path)
{
    LosModel::LosFilePath fileInfo(file_path);
    if (fileInfo.isDir())
    {
        QDir(file_path).removeRecursively();
    }
    else
    {
        QFile::remove(file_path);
    }
}


/**
- 展开到 指定的文件
*/
bool LosFileTreeUi::expandToFile(const QString &file_path)
{
    auto *treeModel = qobject_cast<LosModel::LosFileTreeModel *>(model());
    if (nullptr == treeModel)
    {
        return false;
    }
    LosModel::LosFileNode *rootNode = treeModel->getRoot();
    QModelIndex index               = findAndExpand(rootNode, file_path, QModelIndex());
    if (!index.isValid())
    {
        ERR("the specified file was not found", "LosFileTreeUi");
        return false;
    }
    scrollTo(index, QAbstractItemView::EnsureVisible);
    return true;
}


/**
- 找到 对应的 index
*/
QModelIndex LosFileTreeUi::findAndExpand(LosModel::LosFileNode *node, const QString &path,
                                         const QModelIndex &parent_index)
{
    if (nullptr == node)
        return QModelIndex();

    if (node->getFilePath() == path)
    {
        QModelIndex cur = parent_index;
        while (cur.isValid())
        {
            expand(cur);
            cur = cur.parent();
        }
        return parent_index;
    };

    for (int i = 0; i < node->getChildCount(); i++)
    {
        LosModel::LosFileNode *child = node->getChild(i);
        QModelIndex childIndex       = model()->index(i, 0, childIndex);
        QModelIndex rst              = findAndExpand(child, path, parent_index);
        {
            if (rst.isValid())
                return rst;
        }
    }
    return QModelIndex();
}


} // namespace LosView
