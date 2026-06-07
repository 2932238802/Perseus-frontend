/********************************************************************************
** Form generated from reading UI file 'LosSettingsUi.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOSSETTINGSUI_H
#define UI_LOSSETTINGSUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LosSettingsUi
{
public:
    QHBoxLayout *main_layout;
    QListWidget *category_list;
    QVBoxLayout *right_layout;
    QStackedWidget *pages_stack;
    QWidget *page_editor;
    QVBoxLayout *verticalLayout_editor;
    QLabel *label_title_editor;
    QLabel *label_editor_placeholder;
    QSpacerItem *verticalSpacer_editor;
    QWidget *page_format;
    QVBoxLayout *verticalLayout_format;
    QLabel *label_title_format;
    QLabel *label_desc_format;
    QPlainTextEdit *edit_clang_format;
    QHBoxLayout *hbox_format_btns;
    QSpacerItem *hspacer_format_btns;
    QPushButton *btn_format_reload;
    QPushButton *btn_format_save;
    QWidget *page_terminal;
    QVBoxLayout *verticalLayout_terminal;
    QLabel *label_title_terminal;
    QLabel *label_terminal_placeholder;
    QSpacerItem *verticalSpacer_terminal;
    QWidget *page_build;
    QVBoxLayout *verticalLayout_build;
    QLabel *label_title_build;
    QScrollArea *scrollArea_build_tools;
    QWidget *scrollAreaWidgetContents_build_tools;
    QFormLayout *formLayout_build;
    QLabel *label_cmake;
    QHBoxLayout *horizontalLayout_cmake;
    QLineEdit *edit_cmake_path;
    QToolButton *btn_browse_cmake;
    QLabel *label_ninja;
    QHBoxLayout *horizontalLayout_ninja;
    QLineEdit *edit_ninja_path;
    QToolButton *btn_browse_ninja;
    QLabel *label_git;
    QHBoxLayout *horizontalLayout_git;
    QLineEdit *edit_git_path;
    QToolButton *btn_browse_git;
    QLabel *label_gxx;
    QHBoxLayout *horizontalLayout_gxx;
    QLineEdit *edit_gxx_path;
    QToolButton *btn_browse_gxx;
    QLabel *label_clangd;
    QHBoxLayout *horizontalLayout_clangd;
    QLineEdit *edit_clangd_path;
    QToolButton *btn_browse_clangd;
    QLabel *label_clang_format;
    QHBoxLayout *horizontalLayout_clang_format;
    QLineEdit *edit_clang_format_path;
    QToolButton *btn_browse_clang_format;
    QLabel *label_neocmakelsp;
    QHBoxLayout *horizontalLayout_neocmakelsp;
    QLineEdit *edit_neocmakelsp_path;
    QToolButton *btn_browse_neocmakelsp;
    QLabel *label_cargo;
    QHBoxLayout *horizontalLayout_cargo;
    QLineEdit *edit_cargo_path;
    QToolButton *btn_browse_cargo;
    QLabel *label_rustc;
    QHBoxLayout *horizontalLayout_rustc;
    QLineEdit *edit_rustc_path;
    QToolButton *btn_browse_rustc;
    QLabel *label_rust_analyzer;
    QHBoxLayout *horizontalLayout_rust_analyzer;
    QLineEdit *edit_rust_analyzer_path;
    QToolButton *btn_browse_rust_analyzer;
    QLabel *label_python;
    QHBoxLayout *horizontalLayout_python;
    QLineEdit *edit_python_path;
    QToolButton *btn_browse_python;
    QLabel *label_pyright;
    QHBoxLayout *horizontalLayout_pyright;
    QLineEdit *edit_pyright_path;
    QToolButton *btn_browse_pyright;
    QWidget *page_install;
    QVBoxLayout *verticalLayout_install;
    QLabel *label_title_install;
    QLabel *label_desc_install;
    QScrollArea *scrollArea_install_tools;
    QWidget *scrollAreaWidgetContents_install_tools;
    QVBoxLayout *verticalLayout_install_tools;
    QHBoxLayout *hLayout_inst_cmake;
    QLabel *lbl_inst_cmake;
    QSpacerItem *spacer_inst_cmake;
    QPushButton *btn_install_cmake;
    QHBoxLayout *hLayout_inst_ninja;
    QLabel *lbl_inst_ninja;
    QSpacerItem *spacer_inst_ninja;
    QPushButton *btn_install_ninja;
    QHBoxLayout *hLayout_inst_git;
    QLabel *lbl_inst_git;
    QSpacerItem *spacer_inst_git;
    QPushButton *btn_install_git;
    QHBoxLayout *hLayout_inst_gxx;
    QLabel *lbl_inst_gxx;
    QSpacerItem *spacer_inst_gxx;
    QPushButton *btn_install_gxx;
    QHBoxLayout *hLayout_inst_clangd;
    QLabel *lbl_inst_clangd;
    QSpacerItem *spacer_inst_clangd;
    QPushButton *btn_install_clangd;
    QHBoxLayout *hLayout_inst_clang_format;
    QLabel *lbl_inst_clang_format;
    QSpacerItem *spacer_inst_clang_format;
    QPushButton *btn_install_clang_format;
    QHBoxLayout *hLayout_inst_neocmakelsp;
    QLabel *lbl_inst_neocmakelsp;
    QSpacerItem *spacer_inst_neocmakelsp;
    QPushButton *btn_install_neocmakelsp;
    QHBoxLayout *hLayout_inst_cargo;
    QLabel *lbl_inst_cargo;
    QSpacerItem *spacer_inst_cargo;
    QPushButton *btn_install_cargo;
    QHBoxLayout *hLayout_inst_rustc;
    QLabel *lbl_inst_rustc;
    QSpacerItem *spacer_inst_rustc;
    QPushButton *btn_install_rustc;
    QHBoxLayout *hLayout_inst_rust_analyzer;
    QLabel *lbl_inst_rust_3;
    QSpacerItem *spacer_inst_rust_3;
    QPushButton *btn_install_rust_3;
    QHBoxLayout *hLayout_inst_python;
    QLabel *lbl_inst_python;
    QSpacerItem *spacer_inst_python;
    QPushButton *btn_install_python;
    QHBoxLayout *hLayout_inst_pyright;
    QLabel *lbl_inst_pyright;
    QSpacerItem *spacer_inst_pyright;
    QPushButton *btn_install_pyright;
    QSpacerItem *verticalSpacer_install_tools;
    QWidget *page_theme;
    QVBoxLayout *verticalLayout_theme;
    QLabel *label_title_theme;
    QLabel *label_desc_theme;
    QHBoxLayout *hbox_theme_select;
    QLabel *label_theme_select;
    QComboBox *combo_theme;
    QSpacerItem *hspacer_theme_select;
    QSpacerItem *verticalSpacer_theme;
    QHBoxLayout *bottom_layout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LosSettingsUi)
    {
        if (LosSettingsUi->objectName().isEmpty())
            LosSettingsUi->setObjectName("LosSettingsUi");
        LosSettingsUi->resize(820, 560);
        main_layout = new QHBoxLayout(LosSettingsUi);
        main_layout->setSpacing(0);
        main_layout->setObjectName("main_layout");
        main_layout->setContentsMargins(0, 0, 0, 0);
        category_list = new QListWidget(LosSettingsUi);
        new QListWidgetItem(category_list);
        new QListWidgetItem(category_list);
        new QListWidgetItem(category_list);
        new QListWidgetItem(category_list);
        new QListWidgetItem(category_list);
        new QListWidgetItem(category_list);
        category_list->setObjectName("category_list");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(category_list->sizePolicy().hasHeightForWidth());
        category_list->setSizePolicy(sizePolicy);
        category_list->setMinimumSize(QSize(160, 0));
        category_list->setMaximumSize(QSize(320, 16777215));
        category_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        category_list->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        main_layout->addWidget(category_list);

        right_layout = new QVBoxLayout();
        right_layout->setSpacing(0);
        right_layout->setObjectName("right_layout");
        right_layout->setContentsMargins(-1, -1, -1, 10);
        pages_stack = new QStackedWidget(LosSettingsUi);
        pages_stack->setObjectName("pages_stack");
        page_editor = new QWidget();
        page_editor->setObjectName("page_editor");
        verticalLayout_editor = new QVBoxLayout(page_editor);
        verticalLayout_editor->setObjectName("verticalLayout_editor");
        verticalLayout_editor->setContentsMargins(20, 20, 20, -1);
        label_title_editor = new QLabel(page_editor);
        label_title_editor->setObjectName("label_title_editor");

        verticalLayout_editor->addWidget(label_title_editor);

        label_editor_placeholder = new QLabel(page_editor);
        label_editor_placeholder->setObjectName("label_editor_placeholder");
        label_editor_placeholder->setWordWrap(true);

        verticalLayout_editor->addWidget(label_editor_placeholder);

        verticalSpacer_editor = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_editor->addItem(verticalSpacer_editor);

        pages_stack->addWidget(page_editor);
        page_format = new QWidget();
        page_format->setObjectName("page_format");
        verticalLayout_format = new QVBoxLayout(page_format);
        verticalLayout_format->setSpacing(15);
        verticalLayout_format->setObjectName("verticalLayout_format");
        verticalLayout_format->setContentsMargins(20, 20, 20, 20);
        label_title_format = new QLabel(page_format);
        label_title_format->setObjectName("label_title_format");

        verticalLayout_format->addWidget(label_title_format);

        label_desc_format = new QLabel(page_format);
        label_desc_format->setObjectName("label_desc_format");
        label_desc_format->setWordWrap(true);

        verticalLayout_format->addWidget(label_desc_format);

        edit_clang_format = new QPlainTextEdit(page_format);
        edit_clang_format->setObjectName("edit_clang_format");

        verticalLayout_format->addWidget(edit_clang_format);

        hbox_format_btns = new QHBoxLayout();
        hbox_format_btns->setObjectName("hbox_format_btns");
        hspacer_format_btns = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hbox_format_btns->addItem(hspacer_format_btns);

        btn_format_reload = new QPushButton(page_format);
        btn_format_reload->setObjectName("btn_format_reload");

        hbox_format_btns->addWidget(btn_format_reload);

        btn_format_save = new QPushButton(page_format);
        btn_format_save->setObjectName("btn_format_save");

        hbox_format_btns->addWidget(btn_format_save);


        verticalLayout_format->addLayout(hbox_format_btns);

        pages_stack->addWidget(page_format);
        page_terminal = new QWidget();
        page_terminal->setObjectName("page_terminal");
        verticalLayout_terminal = new QVBoxLayout(page_terminal);
        verticalLayout_terminal->setObjectName("verticalLayout_terminal");
        verticalLayout_terminal->setContentsMargins(20, 20, 20, -1);
        label_title_terminal = new QLabel(page_terminal);
        label_title_terminal->setObjectName("label_title_terminal");

        verticalLayout_terminal->addWidget(label_title_terminal);

        label_terminal_placeholder = new QLabel(page_terminal);
        label_terminal_placeholder->setObjectName("label_terminal_placeholder");
        label_terminal_placeholder->setWordWrap(true);

        verticalLayout_terminal->addWidget(label_terminal_placeholder);

        verticalSpacer_terminal = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_terminal->addItem(verticalSpacer_terminal);

        pages_stack->addWidget(page_terminal);
        page_build = new QWidget();
        page_build->setObjectName("page_build");
        verticalLayout_build = new QVBoxLayout(page_build);
        verticalLayout_build->setObjectName("verticalLayout_build");
        verticalLayout_build->setContentsMargins(20, 20, 20, -1);
        label_title_build = new QLabel(page_build);
        label_title_build->setObjectName("label_title_build");

        verticalLayout_build->addWidget(label_title_build);

        scrollArea_build_tools = new QScrollArea(page_build);
        scrollArea_build_tools->setObjectName("scrollArea_build_tools");
        scrollArea_build_tools->setWidgetResizable(true);
        scrollAreaWidgetContents_build_tools = new QWidget();
        scrollAreaWidgetContents_build_tools->setObjectName("scrollAreaWidgetContents_build_tools");
        scrollAreaWidgetContents_build_tools->setGeometry(QRect(0, 0, 602, 514));
        formLayout_build = new QFormLayout(scrollAreaWidgetContents_build_tools);
        formLayout_build->setObjectName("formLayout_build");
        formLayout_build->setHorizontalSpacing(14);
        formLayout_build->setVerticalSpacing(18);
        formLayout_build->setContentsMargins(14, 14, 14, 14);
        label_cmake = new QLabel(scrollAreaWidgetContents_build_tools);
        label_cmake->setObjectName("label_cmake");

        formLayout_build->setWidget(0, QFormLayout::LabelRole, label_cmake);

        horizontalLayout_cmake = new QHBoxLayout();
        horizontalLayout_cmake->setSpacing(8);
        horizontalLayout_cmake->setObjectName("horizontalLayout_cmake");
        edit_cmake_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_cmake_path->setObjectName("edit_cmake_path");

        horizontalLayout_cmake->addWidget(edit_cmake_path);

        btn_browse_cmake = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_cmake->setObjectName("btn_browse_cmake");

        horizontalLayout_cmake->addWidget(btn_browse_cmake);


        formLayout_build->setLayout(0, QFormLayout::FieldRole, horizontalLayout_cmake);

        label_ninja = new QLabel(scrollAreaWidgetContents_build_tools);
        label_ninja->setObjectName("label_ninja");

        formLayout_build->setWidget(1, QFormLayout::LabelRole, label_ninja);

        horizontalLayout_ninja = new QHBoxLayout();
        horizontalLayout_ninja->setSpacing(8);
        horizontalLayout_ninja->setObjectName("horizontalLayout_ninja");
        edit_ninja_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_ninja_path->setObjectName("edit_ninja_path");

        horizontalLayout_ninja->addWidget(edit_ninja_path);

        btn_browse_ninja = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_ninja->setObjectName("btn_browse_ninja");

        horizontalLayout_ninja->addWidget(btn_browse_ninja);


        formLayout_build->setLayout(1, QFormLayout::FieldRole, horizontalLayout_ninja);

        label_git = new QLabel(scrollAreaWidgetContents_build_tools);
        label_git->setObjectName("label_git");

        formLayout_build->setWidget(2, QFormLayout::LabelRole, label_git);

        horizontalLayout_git = new QHBoxLayout();
        horizontalLayout_git->setSpacing(8);
        horizontalLayout_git->setObjectName("horizontalLayout_git");
        edit_git_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_git_path->setObjectName("edit_git_path");

        horizontalLayout_git->addWidget(edit_git_path);

        btn_browse_git = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_git->setObjectName("btn_browse_git");

        horizontalLayout_git->addWidget(btn_browse_git);


        formLayout_build->setLayout(2, QFormLayout::FieldRole, horizontalLayout_git);

        label_gxx = new QLabel(scrollAreaWidgetContents_build_tools);
        label_gxx->setObjectName("label_gxx");

        formLayout_build->setWidget(3, QFormLayout::LabelRole, label_gxx);

        horizontalLayout_gxx = new QHBoxLayout();
        horizontalLayout_gxx->setSpacing(8);
        horizontalLayout_gxx->setObjectName("horizontalLayout_gxx");
        edit_gxx_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_gxx_path->setObjectName("edit_gxx_path");

        horizontalLayout_gxx->addWidget(edit_gxx_path);

        btn_browse_gxx = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_gxx->setObjectName("btn_browse_gxx");

        horizontalLayout_gxx->addWidget(btn_browse_gxx);


        formLayout_build->setLayout(3, QFormLayout::FieldRole, horizontalLayout_gxx);

        label_clangd = new QLabel(scrollAreaWidgetContents_build_tools);
        label_clangd->setObjectName("label_clangd");

        formLayout_build->setWidget(4, QFormLayout::LabelRole, label_clangd);

        horizontalLayout_clangd = new QHBoxLayout();
        horizontalLayout_clangd->setSpacing(8);
        horizontalLayout_clangd->setObjectName("horizontalLayout_clangd");
        edit_clangd_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_clangd_path->setObjectName("edit_clangd_path");

        horizontalLayout_clangd->addWidget(edit_clangd_path);

        btn_browse_clangd = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_clangd->setObjectName("btn_browse_clangd");

        horizontalLayout_clangd->addWidget(btn_browse_clangd);


        formLayout_build->setLayout(4, QFormLayout::FieldRole, horizontalLayout_clangd);

        label_clang_format = new QLabel(scrollAreaWidgetContents_build_tools);
        label_clang_format->setObjectName("label_clang_format");

        formLayout_build->setWidget(5, QFormLayout::LabelRole, label_clang_format);

        horizontalLayout_clang_format = new QHBoxLayout();
        horizontalLayout_clang_format->setSpacing(8);
        horizontalLayout_clang_format->setObjectName("horizontalLayout_clang_format");
        edit_clang_format_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_clang_format_path->setObjectName("edit_clang_format_path");

        horizontalLayout_clang_format->addWidget(edit_clang_format_path);

        btn_browse_clang_format = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_clang_format->setObjectName("btn_browse_clang_format");

        horizontalLayout_clang_format->addWidget(btn_browse_clang_format);


        formLayout_build->setLayout(5, QFormLayout::FieldRole, horizontalLayout_clang_format);

        label_neocmakelsp = new QLabel(scrollAreaWidgetContents_build_tools);
        label_neocmakelsp->setObjectName("label_neocmakelsp");

        formLayout_build->setWidget(6, QFormLayout::LabelRole, label_neocmakelsp);

        horizontalLayout_neocmakelsp = new QHBoxLayout();
        horizontalLayout_neocmakelsp->setSpacing(8);
        horizontalLayout_neocmakelsp->setObjectName("horizontalLayout_neocmakelsp");
        edit_neocmakelsp_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_neocmakelsp_path->setObjectName("edit_neocmakelsp_path");

        horizontalLayout_neocmakelsp->addWidget(edit_neocmakelsp_path);

        btn_browse_neocmakelsp = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_neocmakelsp->setObjectName("btn_browse_neocmakelsp");

        horizontalLayout_neocmakelsp->addWidget(btn_browse_neocmakelsp);


        formLayout_build->setLayout(6, QFormLayout::FieldRole, horizontalLayout_neocmakelsp);

        label_cargo = new QLabel(scrollAreaWidgetContents_build_tools);
        label_cargo->setObjectName("label_cargo");

        formLayout_build->setWidget(7, QFormLayout::LabelRole, label_cargo);

        horizontalLayout_cargo = new QHBoxLayout();
        horizontalLayout_cargo->setSpacing(8);
        horizontalLayout_cargo->setObjectName("horizontalLayout_cargo");
        edit_cargo_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_cargo_path->setObjectName("edit_cargo_path");

        horizontalLayout_cargo->addWidget(edit_cargo_path);

        btn_browse_cargo = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_cargo->setObjectName("btn_browse_cargo");

        horizontalLayout_cargo->addWidget(btn_browse_cargo);


        formLayout_build->setLayout(7, QFormLayout::FieldRole, horizontalLayout_cargo);

        label_rustc = new QLabel(scrollAreaWidgetContents_build_tools);
        label_rustc->setObjectName("label_rustc");

        formLayout_build->setWidget(8, QFormLayout::LabelRole, label_rustc);

        horizontalLayout_rustc = new QHBoxLayout();
        horizontalLayout_rustc->setSpacing(8);
        horizontalLayout_rustc->setObjectName("horizontalLayout_rustc");
        edit_rustc_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_rustc_path->setObjectName("edit_rustc_path");

        horizontalLayout_rustc->addWidget(edit_rustc_path);

        btn_browse_rustc = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_rustc->setObjectName("btn_browse_rustc");

        horizontalLayout_rustc->addWidget(btn_browse_rustc);


        formLayout_build->setLayout(8, QFormLayout::FieldRole, horizontalLayout_rustc);

        label_rust_analyzer = new QLabel(scrollAreaWidgetContents_build_tools);
        label_rust_analyzer->setObjectName("label_rust_analyzer");

        formLayout_build->setWidget(9, QFormLayout::LabelRole, label_rust_analyzer);

        horizontalLayout_rust_analyzer = new QHBoxLayout();
        horizontalLayout_rust_analyzer->setSpacing(8);
        horizontalLayout_rust_analyzer->setObjectName("horizontalLayout_rust_analyzer");
        edit_rust_analyzer_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_rust_analyzer_path->setObjectName("edit_rust_analyzer_path");

        horizontalLayout_rust_analyzer->addWidget(edit_rust_analyzer_path);

        btn_browse_rust_analyzer = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_rust_analyzer->setObjectName("btn_browse_rust_analyzer");

        horizontalLayout_rust_analyzer->addWidget(btn_browse_rust_analyzer);


        formLayout_build->setLayout(9, QFormLayout::FieldRole, horizontalLayout_rust_analyzer);

        label_python = new QLabel(scrollAreaWidgetContents_build_tools);
        label_python->setObjectName("label_python");

        formLayout_build->setWidget(10, QFormLayout::LabelRole, label_python);

        horizontalLayout_python = new QHBoxLayout();
        horizontalLayout_python->setSpacing(8);
        horizontalLayout_python->setObjectName("horizontalLayout_python");
        edit_python_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_python_path->setObjectName("edit_python_path");

        horizontalLayout_python->addWidget(edit_python_path);

        btn_browse_python = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_python->setObjectName("btn_browse_python");

        horizontalLayout_python->addWidget(btn_browse_python);


        formLayout_build->setLayout(10, QFormLayout::FieldRole, horizontalLayout_python);

        label_pyright = new QLabel(scrollAreaWidgetContents_build_tools);
        label_pyright->setObjectName("label_pyright");

        formLayout_build->setWidget(11, QFormLayout::LabelRole, label_pyright);

        horizontalLayout_pyright = new QHBoxLayout();
        horizontalLayout_pyright->setSpacing(8);
        horizontalLayout_pyright->setObjectName("horizontalLayout_pyright");
        edit_pyright_path = new QLineEdit(scrollAreaWidgetContents_build_tools);
        edit_pyright_path->setObjectName("edit_pyright_path");

        horizontalLayout_pyright->addWidget(edit_pyright_path);

        btn_browse_pyright = new QToolButton(scrollAreaWidgetContents_build_tools);
        btn_browse_pyright->setObjectName("btn_browse_pyright");

        horizontalLayout_pyright->addWidget(btn_browse_pyright);


        formLayout_build->setLayout(11, QFormLayout::FieldRole, horizontalLayout_pyright);

        scrollArea_build_tools->setWidget(scrollAreaWidgetContents_build_tools);

        verticalLayout_build->addWidget(scrollArea_build_tools);

        pages_stack->addWidget(page_build);
        page_install = new QWidget();
        page_install->setObjectName("page_install");
        verticalLayout_install = new QVBoxLayout(page_install);
        verticalLayout_install->setSpacing(15);
        verticalLayout_install->setObjectName("verticalLayout_install");
        verticalLayout_install->setContentsMargins(20, 20, 20, -1);
        label_title_install = new QLabel(page_install);
        label_title_install->setObjectName("label_title_install");

        verticalLayout_install->addWidget(label_title_install);

        label_desc_install = new QLabel(page_install);
        label_desc_install->setObjectName("label_desc_install");
        label_desc_install->setWordWrap(true);

        verticalLayout_install->addWidget(label_desc_install);

        scrollArea_install_tools = new QScrollArea(page_install);
        scrollArea_install_tools->setObjectName("scrollArea_install_tools");
        scrollArea_install_tools->setWidgetResizable(true);
        scrollAreaWidgetContents_install_tools = new QWidget();
        scrollAreaWidgetContents_install_tools->setObjectName("scrollAreaWidgetContents_install_tools");
        scrollAreaWidgetContents_install_tools->setGeometry(QRect(0, 0, 602, 484));
        verticalLayout_install_tools = new QVBoxLayout(scrollAreaWidgetContents_install_tools);
        verticalLayout_install_tools->setSpacing(14);
        verticalLayout_install_tools->setObjectName("verticalLayout_install_tools");
        verticalLayout_install_tools->setContentsMargins(14, 14, 14, 14);
        hLayout_inst_cmake = new QHBoxLayout();
        hLayout_inst_cmake->setObjectName("hLayout_inst_cmake");
        lbl_inst_cmake = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_cmake->setObjectName("lbl_inst_cmake");

        hLayout_inst_cmake->addWidget(lbl_inst_cmake);

        spacer_inst_cmake = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_cmake->addItem(spacer_inst_cmake);

        btn_install_cmake = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_cmake->setObjectName("btn_install_cmake");
        btn_install_cmake->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_cmake->addWidget(btn_install_cmake);


        verticalLayout_install_tools->addLayout(hLayout_inst_cmake);

        hLayout_inst_ninja = new QHBoxLayout();
        hLayout_inst_ninja->setObjectName("hLayout_inst_ninja");
        lbl_inst_ninja = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_ninja->setObjectName("lbl_inst_ninja");

        hLayout_inst_ninja->addWidget(lbl_inst_ninja);

        spacer_inst_ninja = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_ninja->addItem(spacer_inst_ninja);

        btn_install_ninja = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_ninja->setObjectName("btn_install_ninja");
        btn_install_ninja->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_ninja->addWidget(btn_install_ninja);


        verticalLayout_install_tools->addLayout(hLayout_inst_ninja);

        hLayout_inst_git = new QHBoxLayout();
        hLayout_inst_git->setObjectName("hLayout_inst_git");
        lbl_inst_git = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_git->setObjectName("lbl_inst_git");

        hLayout_inst_git->addWidget(lbl_inst_git);

        spacer_inst_git = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_git->addItem(spacer_inst_git);

        btn_install_git = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_git->setObjectName("btn_install_git");
        btn_install_git->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_git->addWidget(btn_install_git);


        verticalLayout_install_tools->addLayout(hLayout_inst_git);

        hLayout_inst_gxx = new QHBoxLayout();
        hLayout_inst_gxx->setObjectName("hLayout_inst_gxx");
        lbl_inst_gxx = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_gxx->setObjectName("lbl_inst_gxx");

        hLayout_inst_gxx->addWidget(lbl_inst_gxx);

        spacer_inst_gxx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_gxx->addItem(spacer_inst_gxx);

        btn_install_gxx = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_gxx->setObjectName("btn_install_gxx");
        btn_install_gxx->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_gxx->addWidget(btn_install_gxx);


        verticalLayout_install_tools->addLayout(hLayout_inst_gxx);

        hLayout_inst_clangd = new QHBoxLayout();
        hLayout_inst_clangd->setObjectName("hLayout_inst_clangd");
        lbl_inst_clangd = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_clangd->setObjectName("lbl_inst_clangd");

        hLayout_inst_clangd->addWidget(lbl_inst_clangd);

        spacer_inst_clangd = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_clangd->addItem(spacer_inst_clangd);

        btn_install_clangd = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_clangd->setObjectName("btn_install_clangd");
        btn_install_clangd->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_clangd->addWidget(btn_install_clangd);


        verticalLayout_install_tools->addLayout(hLayout_inst_clangd);

        hLayout_inst_clang_format = new QHBoxLayout();
        hLayout_inst_clang_format->setObjectName("hLayout_inst_clang_format");
        lbl_inst_clang_format = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_clang_format->setObjectName("lbl_inst_clang_format");

        hLayout_inst_clang_format->addWidget(lbl_inst_clang_format);

        spacer_inst_clang_format = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_clang_format->addItem(spacer_inst_clang_format);

        btn_install_clang_format = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_clang_format->setObjectName("btn_install_clang_format");
        btn_install_clang_format->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_clang_format->addWidget(btn_install_clang_format);


        verticalLayout_install_tools->addLayout(hLayout_inst_clang_format);

        hLayout_inst_neocmakelsp = new QHBoxLayout();
        hLayout_inst_neocmakelsp->setObjectName("hLayout_inst_neocmakelsp");
        lbl_inst_neocmakelsp = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_neocmakelsp->setObjectName("lbl_inst_neocmakelsp");

        hLayout_inst_neocmakelsp->addWidget(lbl_inst_neocmakelsp);

        spacer_inst_neocmakelsp = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_neocmakelsp->addItem(spacer_inst_neocmakelsp);

        btn_install_neocmakelsp = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_neocmakelsp->setObjectName("btn_install_neocmakelsp");
        btn_install_neocmakelsp->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_neocmakelsp->addWidget(btn_install_neocmakelsp);


        verticalLayout_install_tools->addLayout(hLayout_inst_neocmakelsp);

        hLayout_inst_cargo = new QHBoxLayout();
        hLayout_inst_cargo->setObjectName("hLayout_inst_cargo");
        lbl_inst_cargo = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_cargo->setObjectName("lbl_inst_cargo");

        hLayout_inst_cargo->addWidget(lbl_inst_cargo);

        spacer_inst_cargo = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_cargo->addItem(spacer_inst_cargo);

        btn_install_cargo = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_cargo->setObjectName("btn_install_cargo");
        btn_install_cargo->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_cargo->addWidget(btn_install_cargo);


        verticalLayout_install_tools->addLayout(hLayout_inst_cargo);

        hLayout_inst_rustc = new QHBoxLayout();
        hLayout_inst_rustc->setObjectName("hLayout_inst_rustc");
        lbl_inst_rustc = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_rustc->setObjectName("lbl_inst_rustc");

        hLayout_inst_rustc->addWidget(lbl_inst_rustc);

        spacer_inst_rustc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_rustc->addItem(spacer_inst_rustc);

        btn_install_rustc = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_rustc->setObjectName("btn_install_rustc");
        btn_install_rustc->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_rustc->addWidget(btn_install_rustc);


        verticalLayout_install_tools->addLayout(hLayout_inst_rustc);

        hLayout_inst_rust_analyzer = new QHBoxLayout();
        hLayout_inst_rust_analyzer->setObjectName("hLayout_inst_rust_analyzer");
        lbl_inst_rust_3 = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_rust_3->setObjectName("lbl_inst_rust_3");

        hLayout_inst_rust_analyzer->addWidget(lbl_inst_rust_3);

        spacer_inst_rust_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_rust_analyzer->addItem(spacer_inst_rust_3);

        btn_install_rust_3 = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_rust_3->setObjectName("btn_install_rust_3");
        btn_install_rust_3->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_rust_analyzer->addWidget(btn_install_rust_3);


        verticalLayout_install_tools->addLayout(hLayout_inst_rust_analyzer);

        hLayout_inst_python = new QHBoxLayout();
        hLayout_inst_python->setObjectName("hLayout_inst_python");
        lbl_inst_python = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_python->setObjectName("lbl_inst_python");

        hLayout_inst_python->addWidget(lbl_inst_python);

        spacer_inst_python = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_python->addItem(spacer_inst_python);

        btn_install_python = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_python->setObjectName("btn_install_python");
        btn_install_python->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_python->addWidget(btn_install_python);


        verticalLayout_install_tools->addLayout(hLayout_inst_python);

        hLayout_inst_pyright = new QHBoxLayout();
        hLayout_inst_pyright->setObjectName("hLayout_inst_pyright");
        lbl_inst_pyright = new QLabel(scrollAreaWidgetContents_install_tools);
        lbl_inst_pyright->setObjectName("lbl_inst_pyright");

        hLayout_inst_pyright->addWidget(lbl_inst_pyright);

        spacer_inst_pyright = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout_inst_pyright->addItem(spacer_inst_pyright);

        btn_install_pyright = new QPushButton(scrollAreaWidgetContents_install_tools);
        btn_install_pyright->setObjectName("btn_install_pyright");
        btn_install_pyright->setCursor(QCursor(Qt::PointingHandCursor));

        hLayout_inst_pyright->addWidget(btn_install_pyright);


        verticalLayout_install_tools->addLayout(hLayout_inst_pyright);

        verticalSpacer_install_tools = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_install_tools->addItem(verticalSpacer_install_tools);

        scrollArea_install_tools->setWidget(scrollAreaWidgetContents_install_tools);

        verticalLayout_install->addWidget(scrollArea_install_tools);

        pages_stack->addWidget(page_install);
        page_theme = new QWidget();
        page_theme->setObjectName("page_theme");
        verticalLayout_theme = new QVBoxLayout(page_theme);
        verticalLayout_theme->setSpacing(15);
        verticalLayout_theme->setObjectName("verticalLayout_theme");
        verticalLayout_theme->setContentsMargins(20, 20, 20, 20);
        label_title_theme = new QLabel(page_theme);
        label_title_theme->setObjectName("label_title_theme");

        verticalLayout_theme->addWidget(label_title_theme);

        label_desc_theme = new QLabel(page_theme);
        label_desc_theme->setObjectName("label_desc_theme");
        label_desc_theme->setWordWrap(true);

        verticalLayout_theme->addWidget(label_desc_theme);

        hbox_theme_select = new QHBoxLayout();
        hbox_theme_select->setObjectName("hbox_theme_select");
        label_theme_select = new QLabel(page_theme);
        label_theme_select->setObjectName("label_theme_select");
        label_theme_select->setMinimumSize(QSize(80, 0));

        hbox_theme_select->addWidget(label_theme_select);

        combo_theme = new QComboBox(page_theme);
        combo_theme->setObjectName("combo_theme");
        combo_theme->setMinimumSize(QSize(240, 0));

        hbox_theme_select->addWidget(combo_theme);

        hspacer_theme_select = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hbox_theme_select->addItem(hspacer_theme_select);


        verticalLayout_theme->addLayout(hbox_theme_select);

        verticalSpacer_theme = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_theme->addItem(verticalSpacer_theme);

        pages_stack->addWidget(page_theme);

        right_layout->addWidget(pages_stack);

        bottom_layout = new QHBoxLayout();
        bottom_layout->setObjectName("bottom_layout");
        bottom_layout->setContentsMargins(-1, 10, 15, -1);
        buttonBox = new QDialogButtonBox(LosSettingsUi);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        bottom_layout->addWidget(buttonBox);


        right_layout->addLayout(bottom_layout);


        main_layout->addLayout(right_layout);


        retranslateUi(LosSettingsUi);

        category_list->setCurrentRow(-1);
        pages_stack->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LosSettingsUi);
    } // setupUi

    void retranslateUi(QDialog *LosSettingsUi)
    {
        LosSettingsUi->setWindowTitle(QCoreApplication::translate("LosSettingsUi", "Perseus Settings", nullptr));

        const bool __sortingEnabled = category_list->isSortingEnabled();
        category_list->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = category_list->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("LosSettingsUi", "Editor", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = category_list->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("LosSettingsUi", "Format", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = category_list->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("LosSettingsUi", "Terminal", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = category_list->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("LosSettingsUi", "Build & Run", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = category_list->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("LosSettingsUi", "Install Tools", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = category_list->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("LosSettingsUi", "Theme", nullptr));
        category_list->setSortingEnabled(__sortingEnabled);

        label_title_editor->setText(QCoreApplication::translate("LosSettingsUi", "<h2>Editor</h2>", nullptr));
        label_editor_placeholder->setStyleSheet(QCoreApplication::translate("LosSettingsUi", "color: #888888;", nullptr));
        label_editor_placeholder->setText(QCoreApplication::translate("LosSettingsUi", "Editor settings will be added here.", nullptr));
        label_title_format->setText(QCoreApplication::translate("LosSettingsUi", "<h2>Format</h2>", nullptr));
        label_desc_format->setText(QCoreApplication::translate("LosSettingsUi", "Edit the project .clang-format configuration. Click Save to write it back.", nullptr));
        edit_clang_format->setStyleSheet(QCoreApplication::translate("LosSettingsUi", "QPlainTextEdit { font-family: 'JetBrains Mono', 'Consolas', monospace; padding: 8px; }", nullptr));
        btn_format_reload->setText(QCoreApplication::translate("LosSettingsUi", "Reload", nullptr));
        btn_format_save->setText(QCoreApplication::translate("LosSettingsUi", "Save", nullptr));
        label_title_terminal->setText(QCoreApplication::translate("LosSettingsUi", "<h2>Terminal</h2>", nullptr));
        label_terminal_placeholder->setStyleSheet(QCoreApplication::translate("LosSettingsUi", "color: #888888;", nullptr));
        label_terminal_placeholder->setText(QCoreApplication::translate("LosSettingsUi", "Terminal settings will be added here.", nullptr));
        label_title_build->setText(QCoreApplication::translate("LosSettingsUi", "<h2>Build &amp; Run Tools</h2>", nullptr));
        label_cmake->setText(QCoreApplication::translate("LosSettingsUi", "CMake Path:", nullptr));
        btn_browse_cmake->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_ninja->setText(QCoreApplication::translate("LosSettingsUi", "Ninja Path:", nullptr));
        btn_browse_ninja->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_git->setText(QCoreApplication::translate("LosSettingsUi", "Git Path:", nullptr));
        btn_browse_git->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_gxx->setText(QCoreApplication::translate("LosSettingsUi", "g++ Path:", nullptr));
        btn_browse_gxx->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_clangd->setText(QCoreApplication::translate("LosSettingsUi", "clangd Path:", nullptr));
        btn_browse_clangd->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_clang_format->setText(QCoreApplication::translate("LosSettingsUi", "clang-format Path:", nullptr));
        btn_browse_clang_format->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_neocmakelsp->setText(QCoreApplication::translate("LosSettingsUi", "neocmakelsp Path:", nullptr));
        btn_browse_neocmakelsp->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_cargo->setText(QCoreApplication::translate("LosSettingsUi", "Cargo Path:", nullptr));
        btn_browse_cargo->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_rustc->setText(QCoreApplication::translate("LosSettingsUi", "Rustc Path:", nullptr));
        btn_browse_rustc->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_rust_analyzer->setText(QCoreApplication::translate("LosSettingsUi", "rust-analyzer Path:", nullptr));
        btn_browse_rust_analyzer->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_python->setText(QCoreApplication::translate("LosSettingsUi", "Python Path:", nullptr));
        btn_browse_python->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_pyright->setText(QCoreApplication::translate("LosSettingsUi", "pyright Path:", nullptr));
        btn_browse_pyright->setText(QCoreApplication::translate("LosSettingsUi", "...", nullptr));
        label_title_install->setText(QCoreApplication::translate("LosSettingsUi", "<h2>Install Environment &amp; Tools</h2>", nullptr));
        label_desc_install->setStyleSheet(QCoreApplication::translate("LosSettingsUi", "color: #888888; margin-bottom: 10px;", nullptr));
        label_desc_install->setText(QCoreApplication::translate("LosSettingsUi", "Click 'Install' to automatically run setup scripts in the background.", nullptr));
        lbl_inst_cmake->setText(QCoreApplication::translate("LosSettingsUi", "CMake", nullptr));
        btn_install_cmake->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_ninja->setText(QCoreApplication::translate("LosSettingsUi", "Ninja Build", nullptr));
        btn_install_ninja->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_git->setText(QCoreApplication::translate("LosSettingsUi", "Git", nullptr));
        btn_install_git->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_gxx->setText(QCoreApplication::translate("LosSettingsUi", "g++", nullptr));
        btn_install_gxx->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_clangd->setText(QCoreApplication::translate("LosSettingsUi", "clangd", nullptr));
        btn_install_clangd->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_clang_format->setText(QCoreApplication::translate("LosSettingsUi", "clang-format", nullptr));
        btn_install_clang_format->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_neocmakelsp->setText(QCoreApplication::translate("LosSettingsUi", "neocmakelsp", nullptr));
        btn_install_neocmakelsp->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_cargo->setText(QCoreApplication::translate("LosSettingsUi", "Cargo", nullptr));
        btn_install_cargo->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_rustc->setText(QCoreApplication::translate("LosSettingsUi", "Rustc", nullptr));
        btn_install_rustc->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_rust_3->setText(QCoreApplication::translate("LosSettingsUi", "rust-analyzer", nullptr));
        btn_install_rust_3->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_python->setText(QCoreApplication::translate("LosSettingsUi", "Python 3", nullptr));
        btn_install_python->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        lbl_inst_pyright->setText(QCoreApplication::translate("LosSettingsUi", "Pyright", nullptr));
        btn_install_pyright->setText(QCoreApplication::translate("LosSettingsUi", "Install", nullptr));
        label_title_theme->setText(QCoreApplication::translate("LosSettingsUi", "<h2>Theme</h2>", nullptr));
        label_desc_theme->setText(QCoreApplication::translate("LosSettingsUi", "Choose a color theme for the editor and UI. The change applies immediately.", nullptr));
        label_theme_select->setText(QCoreApplication::translate("LosSettingsUi", "Theme:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LosSettingsUi: public Ui_LosSettingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOSSETTINGSUI_H
