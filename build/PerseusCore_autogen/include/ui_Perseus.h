/********************************************************************************
** Form generated from reading UI file 'Perseus.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERSEUS_H
#define UI_PERSEUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <view/LosIssuesUi/LosIssuesUi.h>
#include "view/LosFileTreeUi/LosFileTreeUi.h"
#include "view/LosPlugUi/LosPlugUi.h"
#include "view/LosStateBarUi/LosStateBarUi.h"
#include "view/LosTerminalTabsUi/LosTerminalTabsUi.h"
#include "view/LosToolBarUi/LosToolBarUi.h"

QT_BEGIN_NAMESPACE

class Ui_Perseus
{
public:
    QWidget *maincontainer;
    QVBoxLayout *verticalLayout;
    LosView::LosToolBarUi *toolbar_widget;
    QHBoxLayout *body_layout;
    QWidget *activity_bar_widget;
    QVBoxLayout *activity_bar_layout;
    QPushButton *act_explorer_btn;
    QPushButton *act_extensions_btn;
    QSpacerItem *verticalSpacer;
    QSplitter *main_splitter;
    QStackedWidget *left_panel_stack;
    QWidget *page_explorer;
    QVBoxLayout *page_exp_layout;
    LosView::LosFileTreeUi *explorer_treeview;
    LosView::LosPlugUi *page_plugins;
    QSplitter *right_splitter;
    QTabWidget *editor_tabwidget;
    QTabWidget *bottom_tabwidget;
    QWidget *tab_output;
    QVBoxLayout *verticalLayout_output;
    QPlainTextEdit *output_plaintextedit;
    LosView::LosIssuesUi *tab_problems;
    QWidget *tab_terminal;
    QVBoxLayout *verticalLayout_2;
    LosView::LosTerminalTabsUi *terminal_tabs;
    LosView::LosStateBarUi *statusbar_widget;

    void setupUi(QMainWindow *Perseus)
    {
        if (Perseus->objectName().isEmpty())
            Perseus->setObjectName("Perseus");
        Perseus->resize(1200, 939);
        maincontainer = new QWidget(Perseus);
        maincontainer->setObjectName("maincontainer");
        verticalLayout = new QVBoxLayout(maincontainer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        toolbar_widget = new LosView::LosToolBarUi(maincontainer);
        toolbar_widget->setObjectName("toolbar_widget");
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolbar_widget->sizePolicy().hasHeightForWidth());
        toolbar_widget->setSizePolicy(sizePolicy);
        toolbar_widget->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(toolbar_widget);

        body_layout = new QHBoxLayout();
        body_layout->setSpacing(0);
        body_layout->setObjectName("body_layout");
        activity_bar_widget = new QWidget(maincontainer);
        activity_bar_widget->setObjectName("activity_bar_widget");
        activity_bar_widget->setMinimumSize(QSize(48, 0));
        activity_bar_widget->setMaximumSize(QSize(48, 16777215));
        activity_bar_layout = new QVBoxLayout(activity_bar_widget);
        activity_bar_layout->setSpacing(15);
        activity_bar_layout->setObjectName("activity_bar_layout");
        activity_bar_layout->setContentsMargins(0, 10, 0, 10);
        act_explorer_btn = new QPushButton(activity_bar_widget);
        act_explorer_btn->setObjectName("act_explorer_btn");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(act_explorer_btn->sizePolicy().hasHeightForWidth());
        act_explorer_btn->setSizePolicy(sizePolicy1);
        act_explorer_btn->setMinimumSize(QSize(48, 48));
        act_explorer_btn->setMaximumSize(QSize(48, 48));
        act_explorer_btn->setCheckable(true);
        act_explorer_btn->setChecked(true);
        act_explorer_btn->setAutoExclusive(true);
        act_explorer_btn->setFlat(true);

        activity_bar_layout->addWidget(act_explorer_btn);

        act_extensions_btn = new QPushButton(activity_bar_widget);
        act_extensions_btn->setObjectName("act_extensions_btn");
        sizePolicy1.setHeightForWidth(act_extensions_btn->sizePolicy().hasHeightForWidth());
        act_extensions_btn->setSizePolicy(sizePolicy1);
        act_extensions_btn->setMinimumSize(QSize(48, 48));
        act_extensions_btn->setMaximumSize(QSize(48, 48));
        act_extensions_btn->setCheckable(true);
        act_extensions_btn->setChecked(false);
        act_extensions_btn->setAutoExclusive(true);
        act_extensions_btn->setFlat(true);

        activity_bar_layout->addWidget(act_extensions_btn);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        activity_bar_layout->addItem(verticalSpacer);


        body_layout->addWidget(activity_bar_widget);

        main_splitter = new QSplitter(maincontainer);
        main_splitter->setObjectName("main_splitter");
        main_splitter->setFrameShape(QFrame::NoFrame);
        main_splitter->setOrientation(Qt::Horizontal);
        left_panel_stack = new QStackedWidget(main_splitter);
        left_panel_stack->setObjectName("left_panel_stack");
        page_explorer = new QWidget();
        page_explorer->setObjectName("page_explorer");
        page_exp_layout = new QVBoxLayout(page_explorer);
        page_exp_layout->setSpacing(0);
        page_exp_layout->setObjectName("page_exp_layout");
        page_exp_layout->setContentsMargins(0, 0, 0, 0);
        explorer_treeview = new LosView::LosFileTreeUi(page_explorer);
        explorer_treeview->setObjectName("explorer_treeview");
        explorer_treeview->setMinimumSize(QSize(200, 0));

        page_exp_layout->addWidget(explorer_treeview);

        left_panel_stack->addWidget(page_explorer);
        page_plugins = new LosView::LosPlugUi();
        page_plugins->setObjectName("page_plugins");
        left_panel_stack->addWidget(page_plugins);
        main_splitter->addWidget(left_panel_stack);
        right_splitter = new QSplitter(main_splitter);
        right_splitter->setObjectName("right_splitter");
        right_splitter->setOrientation(Qt::Vertical);
        right_splitter->setHandleWidth(1);
        editor_tabwidget = new QTabWidget(right_splitter);
        editor_tabwidget->setObjectName("editor_tabwidget");
        editor_tabwidget->setMinimumSize(QSize(0, 100));
        editor_tabwidget->setDocumentMode(true);
        editor_tabwidget->setTabsClosable(true);
        editor_tabwidget->setMovable(true);
        right_splitter->addWidget(editor_tabwidget);
        bottom_tabwidget = new QTabWidget(right_splitter);
        bottom_tabwidget->setObjectName("bottom_tabwidget");
        bottom_tabwidget->setDocumentMode(true);
        tab_output = new QWidget();
        tab_output->setObjectName("tab_output");
        verticalLayout_output = new QVBoxLayout(tab_output);
        verticalLayout_output->setSpacing(0);
        verticalLayout_output->setObjectName("verticalLayout_output");
        verticalLayout_output->setContentsMargins(0, 0, 0, 0);
        output_plaintextedit = new QPlainTextEdit(tab_output);
        output_plaintextedit->setObjectName("output_plaintextedit");
        output_plaintextedit->setFrameShape(QFrame::NoFrame);
        output_plaintextedit->setReadOnly(true);

        verticalLayout_output->addWidget(output_plaintextedit);

        bottom_tabwidget->addTab(tab_output, QString());
        tab_problems = new LosView::LosIssuesUi();
        tab_problems->setObjectName("tab_problems");
        bottom_tabwidget->addTab(tab_problems, QString());
        tab_terminal = new QWidget();
        tab_terminal->setObjectName("tab_terminal");
        verticalLayout_2 = new QVBoxLayout(tab_terminal);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 5, 0, 0);
        terminal_tabs = new LosView::LosTerminalTabsUi(tab_terminal);
        terminal_tabs->setObjectName("terminal_tabs");

        verticalLayout_2->addWidget(terminal_tabs);

        bottom_tabwidget->addTab(tab_terminal, QString());
        right_splitter->addWidget(bottom_tabwidget);
        main_splitter->addWidget(right_splitter);

        body_layout->addWidget(main_splitter);


        verticalLayout->addLayout(body_layout);

        statusbar_widget = new LosView::LosStateBarUi(maincontainer);
        statusbar_widget->setObjectName("statusbar_widget");
        sizePolicy.setHeightForWidth(statusbar_widget->sizePolicy().hasHeightForWidth());
        statusbar_widget->setSizePolicy(sizePolicy);
        statusbar_widget->setMinimumSize(QSize(0, 26));

        verticalLayout->addWidget(statusbar_widget);

        Perseus->setCentralWidget(maincontainer);

        retranslateUi(Perseus);

        left_panel_stack->setCurrentIndex(0);
        bottom_tabwidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Perseus);
    } // setupUi

    void retranslateUi(QMainWindow *Perseus)
    {
        Perseus->setWindowTitle(QCoreApplication::translate("Perseus", "Perseus Editor", nullptr));
        act_explorer_btn->setText(QString());
        act_extensions_btn->setText(QString());
        bottom_tabwidget->setTabText(bottom_tabwidget->indexOf(tab_output), QString());
        bottom_tabwidget->setTabText(bottom_tabwidget->indexOf(tab_problems), QString());
        bottom_tabwidget->setTabText(bottom_tabwidget->indexOf(tab_terminal), QCoreApplication::translate("Perseus", "Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Perseus: public Ui_Perseus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERSEUS_H
