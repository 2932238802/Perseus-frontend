// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>
#include <QWidget>

/*
 * LosDialog
 * - 封装 QFileDialog 的主题化静态工具类
 * - 选择一个文件 (pickFile) 或选择一个目录 (pickDir)
 * - 自动应用 LosDialog_style.h 的当前主题样式
 * - 供 Perseus::onFilesBtnClicked / onFileChooseBtnClicked / onDirChooseBtnClick 等处使用
 *
 * 用法:
 *   QString filePath = LosView::LosDialog::pickFile(this, "Select a file");
 *   QString dirPath  = LosView::LosDialog::pickDir(this, "Select a directory");
 */
namespace LosView
{
    class LosDialog
    {
      public:
        LosDialog()  = default;
        ~LosDialog() = default;

        /**
         * @brief 选择一个文件, 返回绝对路径 (取消时返回空字符串)
         * 
         * @param parent 
         * @param title 
         * @param defaultDir 
         * @return QString 
         */
        static QString pickFile(QWidget *parent = nullptr,
                                const QString &title = QStringLiteral("Select a file"),
                                const QString &defaultDir = QString());
        
        /**
         * @brief // 选择一个目录, 返回绝对路径 (取消时返回空字符串)
         * 
         * @param parent 
         * @param title 
         * @param defaultDir 
         * @return QString 
         */
        static QString pickDir(QWidget *parent = nullptr,
                               const QString &title = QStringLiteral("Select a directory"),
                               const QString &defaultDir = QString());
    };
} /* namespace LosView */