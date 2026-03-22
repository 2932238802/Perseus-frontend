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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Perseus
{
public:
    QWidget *maincontainer;
    QVBoxLayout *verticalLayout;
    QWidget *toolbar_widget;
    QHBoxLayout *toolbarLayout;
    QPushButton *icon_btn;
    QPushButton *files_btn;
    QSpacerItem *horizontalSpacer;
    QSplitter *main_splitter;
    QTreeView *explorer_treeview;
    QTabWidget *editor_tabwidget;

    void setupUi(QMainWindow *Perseus)
    {
        if (Perseus->objectName().isEmpty())
            Perseus->setObjectName("Perseus");
        Perseus->resize(1200, 750);
        maincontainer = new QWidget(Perseus);
        maincontainer->setObjectName("maincontainer");
        verticalLayout = new QVBoxLayout(maincontainer);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        toolbar_widget = new QWidget(maincontainer);
        toolbar_widget->setObjectName("toolbar_widget");
        toolbar_widget->setMinimumSize(QSize(0, 40));
        toolbar_widget->setMaximumSize(QSize(16777215, 40));
        toolbarLayout = new QHBoxLayout(toolbar_widget);
        toolbarLayout->setSpacing(5);
        toolbarLayout->setObjectName("toolbarLayout");
        toolbarLayout->setContentsMargins(3, 0, 0, 0);
        icon_btn = new QPushButton(toolbar_widget);
        icon_btn->setObjectName("icon_btn");
        icon_btn->setMinimumSize(QSize(30, 30));
        icon_btn->setMaximumSize(QSize(30, 30));

        toolbarLayout->addWidget(icon_btn);

        files_btn = new QPushButton(toolbar_widget);
        files_btn->setObjectName("files_btn");
        files_btn->setMinimumSize(QSize(50, 30));
        files_btn->setMaximumSize(QSize(50, 30));

        toolbarLayout->addWidget(files_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(toolbar_widget);

        main_splitter = new QSplitter(maincontainer);
        main_splitter->setObjectName("main_splitter");
        main_splitter->setFrameShape(QFrame::NoFrame);
        main_splitter->setOrientation(Qt::Horizontal);
        explorer_treeview = new QTreeView(main_splitter);
        explorer_treeview->setObjectName("explorer_treeview");
        explorer_treeview->setMinimumSize(QSize(200, 0));
        main_splitter->addWidget(explorer_treeview);
        editor_tabwidget = new QTabWidget(main_splitter);
        editor_tabwidget->setObjectName("editor_tabwidget");
        editor_tabwidget->setDocumentMode(true);
        editor_tabwidget->setTabsClosable(true);
        editor_tabwidget->setMovable(true);
        main_splitter->addWidget(editor_tabwidget);

        verticalLayout->addWidget(main_splitter);

        Perseus->setCentralWidget(maincontainer);

        retranslateUi(Perseus);

        QMetaObject::connectSlotsByName(Perseus);
    } // setupUi

    void retranslateUi(QMainWindow *Perseus)
    {
        Perseus->setWindowTitle(QCoreApplication::translate("Perseus", "Perseus Editor", nullptr));
        icon_btn->setText(QCoreApplication::translate("Perseus", "P", nullptr));
        files_btn->setText(QCoreApplication::translate("Perseus", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Perseus: public Ui_Perseus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERSEUS_H
