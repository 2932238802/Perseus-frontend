

#include "LosCommandArgsUi.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFilePath/LosFilePath.h"
#include "ui_LosCommandArgsUi.h"
#include <qfiledialog.h>
#include <qglobal.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qlineedit.h>
#include <qmessagebox.h>

namespace LosView
{
    /*
     * init
     * 无边框
     */
    LosCommandArgsUi::LosCommandArgsUi(QWidget *parent) : QDialog(parent), ui(new Ui::LosCommandArgsUi)
    {
        ui->setupUi(this);
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        initStyle();
        initConnect();
    }
    LosCommandArgsUi::~LosCommandArgsUi()
    {
        delete ui;
    }



    /*
     * 设置弹窗的内容
     */
    void LosCommandArgsUi::onExeScript(const QString &script_path, const QStringList &params)
    {
        L_scriptPath = script_path;
        LosModel::LosFilePath labelName(script_path);
        ui->label_title->setText(labelName.getBaseFileName());
        QLayoutItem *item = nullptr;
        while ((item = ui->L_dynamicParamsLayout->takeAt(0)) != nullptr)
        {
            if (item->widget())
            {
                delete item->widget();
            }
            delete item;
        }
        L_paramsInputs.clear();
        if (params.isEmpty())
        {
            QLabel *lable = new QLabel("no need param...", this);
            lable->setStyleSheet("color: #888888;");
            ui->L_dynamicParamsLayout->addWidget(lable);
        }
        else
        {
            for (auto param : params)
            {
                QLabel *lable         = new QLabel(param, this);
                QLineEdit *paramInput = new QLineEdit(this);
                paramInput->setPlaceholderText("enter param: " + param);
                ui->L_dynamicParamsLayout->addWidget(lable);
                ui->L_dynamicParamsLayout->addWidget(paramInput);
                L_paramsInputs.append(paramInput);
            }
        }
        ui->L_dynamicParamsLayout->addStretch();

        if (parentWidget())
        {
            int x = parentWidget()->geometry().x() + (parentWidget()->width() - width()) / 2;
            int y = parentWidget()->geometry().y() + (parentWidget()->height() - height()) / 2;
            move(x, y);
        }
        this->exec();
    }



    /*
     * 点击 运行按钮
     */
    void LosCommandArgsUi::onRunBtnClicked()
    {
        QString dir = ui->L_dirEdit->text();
        if (dir.isEmpty())
        {
            QMessageBox::warning(this, "Err", "working dir is empty!");
            return;
        }

        QDir dirE(dir);
        if (!dirE.exists())
        {
            QMessageBox::critical(
                this, "Path Error",
                QString("The directory does not exist:\n%1\n\nPlease select a valid directory.").arg(dir));
            return;
        }

        QStringList args;
        for (auto *lineEdit : L_paramsInputs)
        {
            QString input = lineEdit->text().trimmed();
            if (input.isEmpty())
            {
                QMessageBox::warning(this, "warning", "must input all the params!");
                return;
            }
            args.append(input);
        }

        SUC(L_scriptPath, "LosCommandArgsUi");

        this->accept();
        emit LosCore::LosRouter::instance()._cmd_runScript(L_scriptPath, dir, args);
    }



    /*
     *
     */
    void LosCommandArgsUi::onBrowseBtnClicked()
    {
        QString selectDir = QFileDialog::getExistingDirectory(
            this, "select working dir", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!selectDir.isEmpty())
        {
            ui->L_dirEdit->setText(selectDir);
        }
    }



    /*
     * init 初始化 样式
     */
    void LosCommandArgsUi::initStyle()
    {
        this->setStyleSheet(LosStyle::LosCommandArgsUi_style());
    }


    /*
     *
     */
    void LosCommandArgsUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_command_exeScript, this, &LosCommandArgsUi::onExeScript);
        connect(ui->btn_browse, &QPushButton::clicked, this, &LosCommandArgsUi::onBrowseBtnClicked);
        connect(ui->btn_run, &QPushButton::clicked, this, &LosCommandArgsUi::onRunBtnClicked);
        connect(ui->btn_cancel, &QPushButton::clicked, this, &QDialog::reject); /* Cancel直接关闭 */
    }

} /* namespace LosView */