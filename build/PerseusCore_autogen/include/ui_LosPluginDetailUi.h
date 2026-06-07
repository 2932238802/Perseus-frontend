/********************************************************************************
** Form generated from reading UI file 'LosPluginDetailUi.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOSPLUGINDETAILUI_H
#define UI_LOSPLUGINDETAILUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LosPluginDetailUi
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *headerLayout;
    QLabel *label_icon;
    QVBoxLayout *titleLayout;
    QLabel *label_name;
    QLabel *label_version;
    QHBoxLayout *buttonLayout;
    QPushButton *btn_install;
    QSpacerItem *horizontalSpacer_btn;
    QTextBrowser *text_description;

    void setupUi(QWidget *LosPluginDetailUi)
    {
        if (LosPluginDetailUi->objectName().isEmpty())
            LosPluginDetailUi->setObjectName("LosPluginDetailUi");
        LosPluginDetailUi->resize(800, 600);
        verticalLayout = new QVBoxLayout(LosPluginDetailUi);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 25, 30, 20);
        headerLayout = new QHBoxLayout();
        headerLayout->setSpacing(20);
        headerLayout->setObjectName("headerLayout");
        label_icon = new QLabel(LosPluginDetailUi);
        label_icon->setObjectName("label_icon");
        label_icon->setMinimumSize(QSize(72, 72));
        label_icon->setMaximumSize(QSize(72, 72));
        label_icon->setScaledContents(true);

        headerLayout->addWidget(label_icon, 0, Qt::AlignTop);

        titleLayout = new QVBoxLayout();
        titleLayout->setSpacing(6);
        titleLayout->setObjectName("titleLayout");
        label_name = new QLabel(LosPluginDetailUi);
        label_name->setObjectName("label_name");

        titleLayout->addWidget(label_name);

        label_version = new QLabel(LosPluginDetailUi);
        label_version->setObjectName("label_version");

        titleLayout->addWidget(label_version);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        buttonLayout->setContentsMargins(-1, 8, -1, -1);
        btn_install = new QPushButton(LosPluginDetailUi);
        btn_install->setObjectName("btn_install");
        btn_install->setCursor(QCursor(Qt::PointingHandCursor));

        buttonLayout->addWidget(btn_install);

        horizontalSpacer_btn = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer_btn);


        titleLayout->addLayout(buttonLayout);


        headerLayout->addLayout(titleLayout);


        verticalLayout->addLayout(headerLayout);

        text_description = new QTextBrowser(LosPluginDetailUi);
        text_description->setObjectName("text_description");
        text_description->setFrameShape(QFrame::NoFrame);
        text_description->setOpenExternalLinks(true);

        verticalLayout->addWidget(text_description);


        retranslateUi(LosPluginDetailUi);

        QMetaObject::connectSlotsByName(LosPluginDetailUi);
    } // setupUi

    void retranslateUi(QWidget *LosPluginDetailUi)
    {
        label_icon->setText(QString());
        label_name->setText(QCoreApplication::translate("LosPluginDetailUi", "Plugin Name", nullptr));
        label_version->setText(QCoreApplication::translate("LosPluginDetailUi", "Version 1.0.0", nullptr));
        btn_install->setText(QCoreApplication::translate("LosPluginDetailUi", "Install", nullptr));
        (void)LosPluginDetailUi;
    } // retranslateUi

};

namespace Ui {
    class LosPluginDetailUi: public Ui_LosPluginDetailUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOSPLUGINDETAILUI_H
