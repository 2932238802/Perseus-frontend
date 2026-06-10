// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosSettingsUi.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"

#include "ui_LosSettingsUi.h"

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QStringList>



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
        initThemePage();
        initFormatPage();
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
        connect(ui->combo_theme, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LosSettingsUi::onThemeComboChanged);
        connect(ui->btn_format_reload, &QPushButton::clicked, this, &LosSettingsUi::onFormatReload);
        connect(ui->btn_format_save, &QPushButton::clicked, this, &LosSettingsUi::onFormatSave);
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



    /**
     * @brief initThemePage
     * - 填充主题下拉, 选中当前主题, 渲染初始预览
     */
    void LosSettingsUi::initThemePage()
    {
        if (!ui->combo_theme)
        {
            return;
        }
        ui->combo_theme->blockSignals(true);
        ui->combo_theme->clear();
        const QStringList all = LosCore::LosThemeManager::instance().availableThemes();
        for (const auto &name : all)
        {
            const QJsonObject obj = LosCore::LosThemeManager::instance().themeJson(name);
            const QString display = obj.value(QStringLiteral("displayName")).toString(name);
            ui->combo_theme->addItem(display, name);
        }
        const QString cur  = LosCore::LosThemeManager::instance().currentTheme();
        const int curIndex = ui->combo_theme->findData(cur);
        if (curIndex >= 0)
        {
            ui->combo_theme->setCurrentIndex(curIndex);
        }
        ui->combo_theme->blockSignals(false);
    }



    /**
     * @brief onThemeComboChanged
     * - 立即生效: 通过 ThemeManager 切换主题
     */
    void LosSettingsUi::onThemeComboChanged(int index)
    {
        if (index < 0 || !ui->combo_theme)
        {
            return;
        }
        const QString themeName = ui->combo_theme->itemData(index).toString();
        if (themeName.isEmpty())
        {
            return;
        }
        LosCore::LosThemeManager::instance().setTheme(themeName);
    }



    /**
     * @brief clangFormatPath
     * - 项目根目录下的 .clang-format 路径
     */
    static QString clangFormatPath()
    {
        return QDir::currentPath() + QStringLiteral("/.clang-format");
    }



    /**
     * @brief initFormatPage
     * - 打开设置时加载 .clang-format 内容到编辑区
     */
    void LosSettingsUi::initFormatPage()
    {
        onFormatReload();
    }



    /**
     * @brief onFormatReload
     * - 从磁盘重新读取 .clang-format, 覆盖编辑区(放弃未保存修改)
     */
    void LosSettingsUi::onFormatReload()
    {
        if (!ui->edit_clang_format)
        {
            return;
        }
        QFile file(clangFormatPath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui->edit_clang_format->setPlainText(QStringLiteral("# .clang-format not found in project root.\n"
                                                               "# Type your config here and click Save to create it.\n"));
            return;
        }
        ui->edit_clang_format->setPlainText(QString::fromUtf8(file.readAll()));
        file.close();
    }



    /**
     * @brief onFormatSave
     * - 把编辑区内容写回 .clang-format
     */
    void LosSettingsUi::onFormatSave()
    {
        if (!ui->edit_clang_format)
        {
            return;
        }
        QFile file(clangFormatPath());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            ERR("cannot write .clang-format: " + clangFormatPath(), "LosSettingsUi");
            return;
        }
        file.write(ui->edit_clang_format->toPlainText().toUtf8());
        file.close();
        SUC("saved .clang-format", "LosSettingsUi");
    }

} /* namespace LosView */