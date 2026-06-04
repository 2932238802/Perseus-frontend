// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosSettingsUi.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "core/LosRouter/LosRouter.h"

#include "ui_LosSettingsUi.h"



namespace LosView
{
    /**
     * @brief 构造函数
     *
     * @param in parent
     * @return * construct
     */
    LosSettingsUi::LosSettingsUi(QWidget *parent) : QDialog(parent), ui(new Ui::LosSettingsUi)
    {
        ui->setupUi(this);
        initStyle();
        initConnect();
    }
    LosSettingsUi::~LosSettingsUi()
    {
        delete ui;
    }



    /**
     * @brief initStyle
     *
     * 设置样式
     * - windowFlags
     *   - 获取当前 窗口已经设置的所有样式
     * - WindowContextHelpButtonHint
     *   - 右上角的 问号
     *   - ~ 就是按位取反
     */
    void LosSettingsUi::initStyle()
    {
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    }



    /**
     * @brief initConnect
     * 初始化 连接
     */
    void LosSettingsUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() { accept(); });
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [this]() { reject(); });
        connect(ui->category_list, &QListWidget::currentRowChanged, ui->pages_stack, &QStackedWidget::setCurrentIndex);
        QPushButton *applyBtn = ui->buttonBox->button(QDialogButtonBox::Apply);
        if (applyBtn)
        {
            connect(applyBtn, &QPushButton::clicked, this, &LosSettingsUi::onSaveSettings);
        }
        connect(ui->btn_install_cmake, &QPushButton::clicked, this, &LosSettingsUi::onCMakeInstallBtnClicked);
        connect(&router, &LosCore::LosRouter::_cmd_findExePathAndSetSettingUi, this, &LosSettingsUi::onFindExePath);
    }



    /**
     * @brief Set the Installed object
     *
     * @param tool
     */
    void LosSettingsUi::setInstalled(LosCommon::LosToolChain_Constants::LosTool tool)
    {
        using LosTool                  = LosCommon::LosToolChain_Constants::LosTool;
        auto setInstallButtonInstalled = [](QPushButton *button)
        {
            if (!button)
                return;
            button->setText("Installed");
            button->setEnabled(false);
            button->setCursor(Qt::ArrowCursor);
            button->setProperty("installed", true);
            button->style()->unpolish(button);
            button->style()->polish(button);
            button->update();
        };

        switch (tool)
        {
        case LosTool::CMAKE:
            setInstallButtonInstalled(ui->btn_install_cmake);
            break;
        case LosTool::NINJA:
            setInstallButtonInstalled(ui->btn_install_ninja);
            break;
        case LosTool::GIT:
            setInstallButtonInstalled(ui->btn_install_git);
            break;
        case LosTool::G_PLUS_PLUS:
            setInstallButtonInstalled(ui->btn_install_gxx);
            break;
        case LosTool::CLANGD:
            setInstallButtonInstalled(ui->btn_install_clangd);
            break;
        case LosTool::CLANG_FORMAT:
            setInstallButtonInstalled(ui->btn_install_clang_format);
            break;
        case LosTool::NEOCMAKELSP:
            setInstallButtonInstalled(ui->btn_install_neocmakelsp);
            break;
        case LosTool::CARGO:
            setInstallButtonInstalled(ui->btn_install_cargo);
            break;
        case LosTool::RUSTC:
            setInstallButtonInstalled(ui->btn_install_rustc);
            break;
        case LosTool::RUST_ANALYZER:
            setInstallButtonInstalled(ui->btn_install_rustc);
            break;
        case LosTool::PYTHON:
            setInstallButtonInstalled(ui->btn_install_python);
            break;
        case LosTool::PYRIGHT:
            setInstallButtonInstalled(ui->btn_install_pyright);
            break;
        default:
            break;
        }
    }



    /**
     * @brief onSaveSettings
     *
     * 保存 设置
     */
    void LosSettingsUi::onSaveSettings()
    {
        INF("save ...", "LosSettingsUi");
    }



    /**
     * @brief 设置 exe 路径
     * @param file_path
     * @param tool
     */
    void LosSettingsUi::onFindExePath(const QString &file_path, LosCommon::LosToolChain_Constants::LosTool tool)
    {
        SUC("file_path: " + file_path, "LosSettingsUi");
        setInstalled(tool);
        switch (tool)
        {
        case (LosCommon::LosToolChain_Constants::LosTool::RUSTC):
        {
            ui->edit_rustc_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::CMAKE):
        {
            ui->edit_cmake_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::CARGO):
        {
            ui->edit_cargo_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::PYTHON):
        {
            ui->edit_python_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::PYRIGHT):
        {
            ui->edit_pyright_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS):
        {
            ui->edit_gxx_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::CLANG_FORMAT):
        {
            ui->edit_clang_format_path->setText(file_path);
            break;
        }
        case (LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP):
        {
            ui->edit_neocmakelsp_path->setText(file_path);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::NINJA:
        {
            ui->edit_ninja_path->setText(file_path);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::GIT:
        {
            ui->edit_git_path->setText(file_path);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::CLANGD:
        {
            ui->edit_clangd_path->setText(file_path);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::RUST_ANALYZER:
        {
            ui->edit_rust_analyzer_path->setText(file_path);
            break;
        }
        default:
        {
            break;
        }
        }
    }



    /**
     * @brief
     */
    void LosSettingsUi::onCMakeInstallBtnClicked() {}

} /* namespace LosView */