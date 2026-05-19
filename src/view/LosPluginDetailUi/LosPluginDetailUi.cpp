#include "LosPluginDetailUi.h"
#include "ui_LosPluginDetailUi.h"


namespace LosView
{
    /**
     * @brief Construct a new Los Plugin Detail Ui:: Los Plugin Detail Ui object
     *
     * @param parent
     */
    LosPluginDetailUi::LosPluginDetailUi(QWidget *parent) : QWidget(parent), ui(new Ui::LosPluginDetailUi)
    {
        ui->setupUi(this);
        ui->label_icon->hide();
        initConnect();
    }
    LosPluginDetailUi::~LosPluginDetailUi()
    {
        delete ui;
    }


    void LosPluginDetailUi::setPluginInfo(const LosCommon::LosNet_Constants::PluginInfo &info)
    {
        LOS_info = info;
        ui->label_name->setText(info.L_name);
        ui->label_version->setText(info.L_version);

        /*
         * - 检测是否已经安装
         * - 默认设置 基础加载样式
         * - 调用单例 fetchPluginReadme
         * - setHtml 样式更为美观
         *
         * 1.080 修改
         * - 统一使用 md
         * - 依然存在乱码的问题
         */
        QString extDir     = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.perseus/extensions/";
        QString pluginPath = extDir + info.L_id;
        QFileInfo checkInstall(pluginPath);
        if (checkInstall.exists())
        {
            ui->btn_install->setText("Uninstall");
            ui->btn_install->setEnabled(true);
            ui->btn_install->setStyleSheet("background-color: #44475a; color: #f8f8f2; border: 1px solid #6272a4;");
        }
        else
        {
            ui->btn_install->setText("Install");
            ui->btn_install->setEnabled(true);
            ui->btn_install->setStyleSheet("");
        }
        ui->text_description->setMarkdown("> Loading description from server...");
        QPointer<LosPluginDetailUi> safeThis(this);
        LosCore::LosNet::instance().fetchPluginReadme(info.L_readmeUrl,
                                                      [safeThis, this](const QString &htmlContent)
                                                      {
                                                          if (safeThis)
                                                          {
                                                              QFont font = ui->text_description->font();
                                                              font.setFamily("Microsoft YaHei");
                                                              ui->text_description->setFont(font);
                                                              safeThis->ui->text_description->setMarkdown(htmlContent);
                                                          }
                                                      });
    }



    void LosPluginDetailUi::onDownloadBtnClicked()
    {
        /*
         * extDir
         * - 保存的位置
         * L_id
         * - 默认是 文件的名字
         *
         * 如果是卸载操作
         * - removeRecursively 递归删除
         */
        QString extDir =
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + LosCommon::LosPluginDetailUi_Constants::PLUGIN_EXTENSION_SAVE_PATH;
        if (ui->btn_install->text() == "Uninstall")
        {
            QString pluginPath = extDir + LOS_info.L_id;
            QDir dir(pluginPath);
            if (dir.exists())
            {
                dir.removeRecursively();
            }
            ui->btn_install->setText("Install");
            ui->btn_install->setStyleSheet("");
            return;
        }

        /*
         *
         */
        ui->btn_install->setEnabled(false);
        ui->btn_install->setText("Downloading...");
        QString savePath = extDir + LOS_info.L_id + ".zip";
        LosCore::LosNet::instance().downloadPlugin(LOS_info.L_downloadUrl, savePath);
    }



    void LosPluginDetailUi::onDownloadFinished(const QString &zip_path)
    {
        /*
         * 打印 下载的zip 信息
         * - 可删
         */
        INF(zip_path, "LosPluginDetailUi");
        if (!zip_path.contains(LOS_info.L_id))
            return;

        /*
         * 设置下载的ui
         * 当前的 tab ui可能会被删除
         * - 所以使用  QPointer 进行保护
         * - unzip Linux
         * - Expand-Archive windows
         * - 运行成功之后 删掉 zip文件 QFile::remove(zip_path);
         */
        ui->btn_install->setText("Extracting...");
        QFileInfo fileInfo(zip_path);
        QString targetDir = fileInfo.absolutePath();
        QProcess *process = new QProcess(this);
        QPointer<LosPluginDetailUi> safeThis(this);
        connect(process, &QProcess::finished, this,
                [=](int exitCode, QProcess::ExitStatus status)
                {
                    QString errorMsg = process->readAllStandardError();
                    if (!errorMsg.isEmpty())
                    {
                        ERR("error: " + errorMsg, "LosPluginDetailUi");
                    }
                    if (safeThis)
                    {
                        if (exitCode == 0 && status == QProcess::NormalExit)
                        {
                            safeThis->ui->btn_install->setText("Uninstall");
                            safeThis->ui->btn_install->setEnabled(true);
                            safeThis->ui->btn_install->setStyleSheet("background-color: #2a2d2e; color: #d4d4d4; border: 1px solid #333333;");
                            QFile::remove(zip_path);
                            emit LosCore::LosRouter::instance()._cmd_net_PluginPath(QDir(fileInfo.absolutePath()).filePath(fileInfo.baseName()));
                        }
                        else
                        {
                            safeThis->ui->btn_install->setText("Install Failed");
                            safeThis->ui->btn_install->setEnabled(true);
                        }
                    }
                    else
                    {
                        if (exitCode == 0)
                            QFile::remove(zip_path);
                    }
                    process->deleteLater();
                });
#ifdef Q_OS_WIN
        QStringList args;
        args << "-nologo" << "-noprofile" << "-command"
             << QString("Expand-Archive -Path '%1' -DestinationPath '%2' -Force").arg(zip_path).arg(targetDir);
        process->start("powershell", args);
#else
        QStringList args;
        args << "-O" << "UTF-8" << "-o" << zip_path << "-d" << targetDir;
        process->start("unzip", args);
#endif
    }



    /*
     * 初始化连接
     */
    void LosPluginDetailUi::initConnect()
    {
        /*
         * 点击 下载的按钮 会 触发  onDownloadBtnClicked 这个函数
         * - 更改样式
         * - 下载这个zip 到本地
         * - 然后 更新一下 指令集
         *   -  指令集 就是 ctrl + shift + p 弹出的窗口
         *
         * _cmd_net_downloadFinished 下载完成的信号
         * - 下载完成之后 提取对应的zip文件
         * - 解压到 .perseus 的extension 文件夹里面
         */
        connect(ui->btn_install, &QPushButton::clicked, this, &LosPluginDetailUi::onDownloadBtnClicked);
        connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_net_downloadFinished, this, &LosPluginDetailUi::onDownloadFinished);
    }

} /* namespace LosView */
