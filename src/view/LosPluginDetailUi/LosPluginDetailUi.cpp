#include "LosPluginDetailUi.h"
#include "core/LosNet/LosNet.h"
#include "core/LosRouter/LosRouter.h"
#include "ui_LosPluginDetailUi.h"
#include <QDir>
#include <QPointer>
#include <QProcess>
#include <qpushbutton.h>
#include <qstandardpaths.h>

namespace LosView
{
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
         * 这里 检测 一下 是不是 已经安装了
         */
        QString extDir     = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.perseus/extensions/";
        QString pluginPath = extDir + info.L_id;
        QFileInfo checkInstall(pluginPath);
        if (checkInstall.exists())
        {
            ui->btn_install->setText("Uninstall");
            ui->btn_install->setEnabled(true);
            ui->btn_install->setStyleSheet("background-color: #2a2d2e; color: #d4d4d4; border: 1px solid #333333;");
        }
        else
        {
            ui->btn_install->setText("Install");
            ui->btn_install->setEnabled(true);
            ui->btn_install->setStyleSheet("");
        }
        ui->text_description->setHtml("<p style='color: #888888; font-family: \"Microsoft YaHei\";'>"
                                      "Loading description from server...</p>");
        QPointer<LosPluginDetailUi> safeThis(this);
        LosCore::LosNet::instance().fetchPluginReadme(info.L_readmeUrl,
                                                      [safeThis](const QString &htmlContent)
                                                      {
                                                          if (safeThis)
                                                          {
                                                              safeThis->ui->text_description->setHtml(htmlContent);
                                                          }
                                                      });
    }



    void LosPluginDetailUi::onDownloadBtnClicked()
    {
        QString extDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.perseus/extensions/";
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

        ui->btn_install->setEnabled(false);
        ui->btn_install->setText("Downloading...");
        QString savePath = extDir + LOS_info.L_id + ".zip";
        LosCore::LosNet::instance().downloadPlugin(LOS_info.L_downloadUrl, savePath);
    }



    void LosPluginDetailUi::onDownloadFinished(const QString &zip_path)
    {
        INF(zip_path, "LosPluginDetailUi");
        if (!zip_path.contains(LOS_info.L_id))
            return;

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
                            safeThis->ui->btn_install->setStyleSheet(
                                "background-color: #2a2d2e; color: #d4d4d4; border: 1px solid #333333;");
                            QFile::remove(zip_path);
                            emit LosCore::LosRouter::instance()._cmd_net_PluginPath(
                                QDir(fileInfo.absolutePath()).filePath(fileInfo.baseName()));
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
        args << "-o" << zip_path << "-d" << targetDir;
        process->start("unzip", args);
#endif
    }



    void LosPluginDetailUi::initConnect()
    {
        /*
         * 点击 下载的按钮 会 触发  onDownloadBtnClicked 这个函数
         * - 更改样式
         * - 下载这个zip 到本地
         * - 然后 更新一下 指令集
         *   -  指令集 就是 ctrl + shift + p 弹出的窗口
         */
        connect(ui->btn_install, &QPushButton::clicked, this, &LosPluginDetailUi::onDownloadBtnClicked);
        connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_net_downloadFinished, this,
                &LosPluginDetailUi::onDownloadFinished);
    }

} /* namespace LosView */
