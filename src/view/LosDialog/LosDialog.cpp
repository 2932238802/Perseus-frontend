// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosDialog.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/style/LosDialog_style.h"

#include <QFileDialog>

namespace LosView
{
    namespace
    {
        /**
         * @brief 给 QFileDialog 套上主题样式并 exec, 返回选中路径 (取消返回空字符串)
         * 
         * @param dlg 
         * @return QString 
         */
        QString applyAndExec(QFileDialog &dlg)
        {
            const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(
                LosStyle::LosDialog_styleTemplate(),
                LosCore::LosThemeManager::instance().currentTheme());
            dlg.setStyleSheet(qss);
            if (dlg.exec() == QDialog::Accepted)
            {
                QStringList files = dlg.selectedFiles();
                if (!files.isEmpty())
                {
                    return files.first();
                }
            }
            return QString();
        }
    } // anonymous namespace



    /**
     * @brief pickFile
     * 
     * @param parent 
     * @param title 
     * @param defaultDir 
     * @return QString 
     */
    QString LosDialog::pickFile(QWidget *parent, const QString &title, const QString &defaultDir)
    {
        QFileDialog dlg(parent, title, defaultDir);
        dlg.setFileMode(QFileDialog::ExistingFile);
        dlg.setAcceptMode(QFileDialog::AcceptOpen);
        return applyAndExec(dlg);
    }



    /**
     * @brief pickDir
     * 
     * @param parent 
     * @param title 
     * @param defaultDir 
     * @return QString 
     */
    QString LosDialog::pickDir(QWidget *parent, const QString &title, const QString &defaultDir)
    {
        QFileDialog dlg(parent, title, defaultDir);
        dlg.setFileMode(QFileDialog::Directory);
        dlg.setOption(QFileDialog::ShowDirsOnly, true);
        dlg.setOption(QFileDialog::DontResolveSymlinks, true);
        return applyAndExec(dlg);
    }
} /* namespace LosView */