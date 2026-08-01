// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosSettingsUi.h"
#include "common/constants/ConstantsStr/LosStateStr.h"
#include "core/LosFont/LosFontManager.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include "core/LosTheme/LosThemeManager.h"

#include "ui_LosSettingsUi.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QPushButton>
#include <QScreen>
#include <QShowEvent>
#include <QStringList>
#include <QStyle>
#include <QWindow>



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
        initFontPage();
        initFormatPage();
        initConnect();

        ui->category_list->setCurrentRow(0);
        ui->pages_stack->setCurrentIndex(0);
        updateCategoryListWidth();
    }
    LosSettingsUi::~LosSettingsUi()
    {
        delete ui;
    }



    /**
     * @brief
     *
     * @param event
     */
    void LosSettingsUi::showEvent(QShowEvent *event) {}



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
        connect(ui->category_list, &QListWidget::currentRowChanged, this,
                [this](int row)
                {
                    if (ui->pages_stack->widget(row) == ui->page_format)
                    {
                        onFormatReload();
                    }
                });
        QPushButton *applyBtn = ui->buttonBox->button(QDialogButtonBox::Apply);
        if (applyBtn)
        {
            connect(applyBtn, &QPushButton::clicked, this, &LosSettingsUi::onSaveSettings);
        }
        connect(ui->btn_install_cmake, &QPushButton::clicked, this, &LosSettingsUi::onCMakeInstallBtnClicked);
        connect(&router, &LosCore::LosRouter::_cmd_findExePathAndSetSettingUi, this, &LosSettingsUi::onFindExePath);
        connect(ui->combo_theme, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LosSettingsUi::onThemeComboChanged);
        connect(ui->combo_font, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LosSettingsUi::onFontComboChanged);
        connect(ui->btn_format_reload, &QPushButton::clicked, this, &LosSettingsUi::onFormatReload);
        connect(ui->btn_format_save, &QPushButton::clicked, this, &LosSettingsUi::onFormatSave);
    }



    /**
     * @brief
     */
    void LosSettingsUi::updateCategoryListWidth() {}



    /**
     * @brief
     *
     */
    void LosSettingsUi::centerOnParent() // 居中 界面
    {
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
     * - 填充主题下拉
     * 选中当前主题
     * 渲染初始预览
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
     * @brief initFontPage
     * - 枚举本机字体填充下拉  选中当前字体 首项为 "System Default"
     */
    void LosSettingsUi::initFontPage()
    {
        if (!ui->combo_font)
        {
            return;
        }
        ui->combo_font->blockSignals(true);
        ui->combo_font->clear();
        ui->combo_font->addItem(QStringLiteral("System Default"), QString());
        const QStringList all = LosCore::LosFontManager::instance().availableFonts();
        for (const auto &family : all)
        {
            ui->combo_font->addItem(family, family);
            ui->combo_font->setItemData(ui->combo_font->count() - 1, QFont(family), Qt::FontRole);
        }
        const QString cur  = LosCore::LosFontManager::instance().currentFontFamily();
        const int curIndex = ui->combo_font->findData(cur);
        if (curIndex >= 0)
        {
            ui->combo_font->setCurrentIndex(curIndex);
        }
        ui->combo_font->blockSignals(false);
    }



    /**
     * @brief onFontComboChanged
     * - 立即生效:
     * - 通过 FontManager 切换全局字体
     */
    void LosSettingsUi::onFontComboChanged(int index)
    {
        if (index < 0 || !ui->combo_font)
        {
            return;
        }
        const QString family = ui->combo_font->itemData(index).toString();
        LosCore::LosFontManager::instance().setFontFamily(family);
    }



    /**
     * @brief initFormatPage
     * - 打开设置时加载 .clang-format 内容到编辑区
     * TODO: 这里可能要删除 因为 初始化顺序的问题 几乎没有什么用
     */
    void LosSettingsUi::initFormatPage()
    {
        onFormatReload();
    }



    /**
     * @brief onFormatReload
     * 从 用户设置里面 进行读取
     */
    void LosSettingsUi::onFormatReload()
    {
        if (!ui->edit_clang_format)
        {
            return;
        }
        QString format{LosCore::LosState::instance().get<QString>(LosCommon::LosState_Constants::SG_STR::CLANG_FORMAT)};
        ui->edit_clang_format->setPlainText(format);
        emit LosCore::LosRouter::instance()._cmd_clangFormatSet(format); // 更新运行 器 里面的缓存
    }



    /**
     * @brief onFormatSave
     */
    void LosSettingsUi::onFormatSave()
    {
        if (!ui->edit_clang_format)
        {
            return;
        }
        QString format{ui->edit_clang_format->toPlainText()};
        LosCore::LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::CLANG_FORMAT, format);
        emit LosCore::LosRouter::instance()._cmd_clangFormatSet(format);
    }

} /* namespace LosView */