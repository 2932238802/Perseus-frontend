/********************************************************************************
** Form generated from reading UI file 'LosCommandArgsUi.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOSCOMMANDARGSUI_H
#define UI_LOSCOMMANDARGSUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LosCommandArgsUi
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QLabel *label_dir;
    QHBoxLayout *horizontalLayout_dir;
    QLineEdit *L_dirEdit;
    QPushButton *btn_browse;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *L_dynamicParamsLayout;
    QHBoxLayout *horizontalLayout_bottom;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_cancel;
    QPushButton *btn_run;

    void setupUi(QDialog *LosCommandArgsUi)
    {
        if (LosCommandArgsUi->objectName().isEmpty())
            LosCommandArgsUi->setObjectName("LosCommandArgsUi");
        LosCommandArgsUi->resize(550, 450);
        verticalLayout = new QVBoxLayout(LosCommandArgsUi);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        label_title = new QLabel(LosCommandArgsUi);
        label_title->setObjectName("label_title");

        verticalLayout->addWidget(label_title);

        label_dir = new QLabel(LosCommandArgsUi);
        label_dir->setObjectName("label_dir");

        verticalLayout->addWidget(label_dir);

        horizontalLayout_dir = new QHBoxLayout();
        horizontalLayout_dir->setObjectName("horizontalLayout_dir");
        L_dirEdit = new QLineEdit(LosCommandArgsUi);
        L_dirEdit->setObjectName("L_dirEdit");

        horizontalLayout_dir->addWidget(L_dirEdit);

        btn_browse = new QPushButton(LosCommandArgsUi);
        btn_browse->setObjectName("btn_browse");
        btn_browse->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_dir->addWidget(btn_browse);


        verticalLayout->addLayout(horizontalLayout_dir);

        scrollArea = new QScrollArea(LosCommandArgsUi);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 510, 208));
        L_dynamicParamsLayout = new QVBoxLayout(scrollAreaWidgetContents);
        L_dynamicParamsLayout->setObjectName("L_dynamicParamsLayout");
        L_dynamicParamsLayout->setContentsMargins(0, 10, 0, 10);
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        horizontalLayout_bottom = new QHBoxLayout();
        horizontalLayout_bottom->setObjectName("horizontalLayout_bottom");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_bottom->addItem(horizontalSpacer);

        btn_cancel = new QPushButton(LosCommandArgsUi);
        btn_cancel->setObjectName("btn_cancel");
        btn_cancel->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_bottom->addWidget(btn_cancel);

        btn_run = new QPushButton(LosCommandArgsUi);
        btn_run->setObjectName("btn_run");
        btn_run->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_bottom->addWidget(btn_run);


        verticalLayout->addLayout(horizontalLayout_bottom);


        retranslateUi(LosCommandArgsUi);

        QMetaObject::connectSlotsByName(LosCommandArgsUi);
    } // setupUi

    void retranslateUi(QDialog *LosCommandArgsUi)
    {
        label_title->setText(QCoreApplication::translate("LosCommandArgsUi", "Run Command Configuration", nullptr));
        label_dir->setText(QCoreApplication::translate("LosCommandArgsUi", "Working Directory:", nullptr));
        btn_browse->setText(QCoreApplication::translate("LosCommandArgsUi", "Browse...", nullptr));
        btn_cancel->setText(QCoreApplication::translate("LosCommandArgsUi", "Cancel", nullptr));
        btn_run->setText(QCoreApplication::translate("LosCommandArgsUi", "Run", nullptr));
        (void)LosCommandArgsUi;
    } // retranslateUi

};

namespace Ui {
    class LosCommandArgsUi: public Ui_LosCommandArgsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOSCOMMANDARGSUI_H
