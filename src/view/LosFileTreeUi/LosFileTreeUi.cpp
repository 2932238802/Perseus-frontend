#include "LosFileTreeUi.h"


namespace LosView
{
    LosFileTreeUi::LosFileTreeUi(QWidget *parent) : QTreeView{parent}
    {
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        initStyle();
        initConnect();
        setHeaderHidden(true);
        setAnimated(true);
        setIndentation(15);
    }



    /*
     * 更新视图
     */
    bool LosFileTreeUi::updateExplorer(LosModel::LosFileTreeModel *newModel)
    {
        if (!newModel)
            return false;

        QSet<QString> expandedPaths;
        getExpandedPaths(&expandedPaths);

        QString selectedPath;
        auto *oldModel = qobject_cast<LosModel::LosFileTreeModel *>(this->model());
        if (oldModel && selectionModel() && !selectionModel()->selectedIndexes().isEmpty())
        {
            QModelIndex selectedIndex   = selectionModel()->selectedIndexes().first();
            LosModel::LosFileNode *node = oldModel->nodeFromIndex(selectedIndex);
            if (node)
                selectedPath = node->getFile().getFilePath();
        }
        setModel(newModel);
        restoreExpandedState(expandedPaths, selectedPath);
        return true;
    }



    /*
     * - customContextMenuRequested 右键
     */
    void LosFileTreeUi::initConnect()
    {
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);
        connect(this, &QTreeView::customContextMenuRequested, this, &LosFileTreeUi::onCustomContextMenu);
    }



    /*
     * - 初始化
     */
    void LosFileTreeUi::initStyle() {}



    /*
     * - 双击 单位
     */
    void LosFileTreeUi::onCustomContextMenu(const QPoint &pos)
    {
        /*
         * 判断一下 合不合法 然后 路径获取一下 这边 其实可以拆分一下
         */
        auto *treeModel = qobject_cast<LosModel::LosFileTreeModel *>(this->model());
        if (!treeModel)
            return;
        QModelIndex index               = this->indexAt(pos);
        bool isClickingValidItem        = index.isValid();
        LosModel::LosFileNode *treeNode = treeModel->nodeFromIndex(index);
        if (!treeNode)
            return;
        QString clickedPath = treeNode->getFile().getFilePath();
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

        /*
         * 这边是 画出来
         * New File
         * New Folder
         * ----
         * Copy
         * Paste
         * Rename
         * ----
         * delete
         */

        QMenu menu(this);
        QAction *newFileAct   = menu.addAction("New File");
        QAction *newFolderAct = menu.addAction("New Folder");
        menu.addSeparator();
        QAction *copyAct       = nullptr;
        QAction *delAct        = nullptr;
        QAction *renameAct     = nullptr;
        QAction *openLocalDir  = nullptr;
        QAction *copyLocalPath = nullptr;
        if (isClickingValidItem)
        {
            copyAct       = menu.addAction("Copy");
            renameAct     = menu.addAction("Rename");
            openLocalDir  = menu.addAction("Open Local Path");
            copyLocalPath = menu.addAction("Copy Local Path");
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
                LosCore::LosFileSystem::instance().createDir(newFolderPath);
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
        else if (selectedAction == renameAct)
        {
            edit(index);
        }
        else if (selectedAction == openLocalDir)
        {
            auto type = LosCore::LosPlatform::getOs();
            if (type == LosCommon::LosPlatform_Constants::OsType::LINUX_W)
            {
                QProcess process;
                process.start("wslpath", QStringList() << "-w" << targetDir);
                process.waitForFinished();
                QString winPath = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
                if (!winPath.isEmpty())
                {
                    QProcess::startDetached("explorer.exe", QStringList() << winPath);
                }
                else
                {
                    QProcess::startDetached("bash", QStringList()
                                                        << "-c" << QString("explorer.exe '%1'").arg(targetDir));
                }
            }
            else if (type == LosCommon::LosPlatform_Constants::OsType::MACOS)
            {
                QProcess::startDetached("open", QStringList() << "-R" << clickedPath);
            }
            else
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile(targetDir));
            }
        }
        else if (selectedAction == copyLocalPath)
        {
            QGuiApplication::clipboard()->setText(clickedPath);
        }
    }



    /*
     * - 增加 delete 按键 效果
     */
    void LosFileTreeUi::keyPressEvent(QKeyEvent *key)
    {
        if (key->key() == Qt::Key_Delete)
        {
            QModelIndexList selecteds = selectionModel()->selectedIndexes();
            if (selecteds.empty())
                return;
            QModelIndex index = selecteds.first();
            auto *treeModel   = qobject_cast<LosModel::LosFileTreeModel *>(model());
            if (!treeModel)
                return;
            LosModel::LosFileNode *node = treeModel->nodeFromIndex(index);
            if (!node)
                return;
            QString clickedPath = node->getFile().getFilePath();
            auto reply          = QMessageBox::warning(this, "Delete", "delete:\n" + clickedPath + " ?",
                                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                deleteFileOrFolder(clickedPath);
                emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
            }
        }
        else if (key->key() == Qt::Key_F2)
        {
            QModelIndexList selecteds = selectionModel()->selectedIndexes();
            if (selecteds.empty())
                return;
            QModelIndex index = selecteds.first();
            edit(index);
            return;
        }
        QTreeView::keyPressEvent(key);
    }



    /*
     * - 复制文件
     */
    void LosFileTreeUi::copyFileOrFolder(const QString &src_path, const QString &des_path)
    {
        LosModel::LosFilePath file(src_path);
        if (file.isFile())
        {
            /*
             * 如果是文件
             */
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



    /*
     * - 删除文件夹
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



    /*
     * - 展开到 指定的文件
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
            return false;
        }
        scrollTo(index, QAbstractItemView::EnsureVisible);
        return true;
    }



    /*
     * - 恢复展开
     */
    bool LosFileTreeUi::restoreExpandedState(const QSet<QString> &expand_paths, const QString &selected_path)
    {
        auto *treeModel = qobject_cast<LosModel::LosFileTreeModel *>(model());
        if (!treeModel)
            return false;

        std::function<void(const QModelIndex &)> traverse = [&](const QModelIndex &parent)
        {
            int sons = treeModel->rowCount(parent);

            for (int i = 0; i < sons; i++)
            {
                QModelIndex son = treeModel->index(i, 0, parent);

                /*
                 * 获取 内部 filenode
                 */
                LosModel::LosFileNode *node = treeModel->nodeFromIndex(son);
                if (!node)
                    return;

                QString path = node->getFile().getFilePath();
                if (expand_paths.contains(path))
                {
                    expand(son);
                    traverse(son);
                }

                if (path == selected_path)
                {
                    /*
                     * 知识点 补充：
                     * QItemSelectionModel
                     * selectionModel 这个 东西 返回的就是 QItemSelectionModel
                     */
                    selectionModel()->select(son, QItemSelectionModel::Select | QItemSelectionModel::Rows);
                }
            }
        };

        traverse(QModelIndex());
        return true;
    }



    /*
     * - 获取 展开的路径
     */
    bool LosFileTreeUi::getExpandedPaths(QSet<QString> *get_files)
    {
        auto *treeModel = qobject_cast<LosModel::LosFileTreeModel *>(model());
        if (!treeModel)
            return false;

        std::function<void(const QModelIndex &)> traverse = [&](const QModelIndex &parent)
        {
            int sons = treeModel->rowCount(parent);
            for (int i = 0; i < sons; i++)
            {
                QModelIndex child = treeModel->index(i, 0, parent);
                if (isExpanded(child))
                {
                    LosModel::LosFileNode *node = treeModel->nodeFromIndex(child);
                    if (!node)
                        return;
                    get_files->insert(node->getFile().getFilePath());
                    traverse(child);
                }
            }
        };
        traverse(QModelIndex()); /* 空 就是 从根开始 */
        return true;
    }



    /*
     * - 找到 对应的 index
     */
    QModelIndex LosFileTreeUi::findAndExpand(LosModel::LosFileNode *node, const QString &path,
                                             const QModelIndex &parent_index)
    {
        if (nullptr == node)
            return QModelIndex();

        if (node->getFile().getFilePath() == path)
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
            QModelIndex childIndex       = model()->index(i, 0, parent_index);
            QModelIndex rst              = findAndExpand(child, path, childIndex);
            {
                if (rst.isValid())
                    return rst;
            }
        }
        return QModelIndex();
    }


} /* namespace LosView */
