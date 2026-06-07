/********************************************************************************
** Form generated from reading UI file 'LosPlugUi.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOSPLUGUI_H
#define UI_LOSPLUGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LosPlugUi
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLineEdit *searchLineEdit;
    QListWidget *extensionsList;

    void setupUi(QWidget *LosPlugUi)
    {
        if (LosPlugUi->objectName().isEmpty())
            LosPlugUi->setObjectName("LosPlugUi");
        LosPlugUi->resize(596, 600);
        verticalLayout = new QVBoxLayout(LosPlugUi);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 15, 10, 10);
        titleLabel = new QLabel(LosPlugUi);
        titleLabel->setObjectName("titleLabel");
        QFont font;
        font.setFamilies({QString::fromUtf8("JetBrains Mono")});
        font.setPointSize(10);
        font.setBold(true);
        titleLabel->setFont(font);

        verticalLayout->addWidget(titleLabel);

        searchLineEdit = new QLineEdit(LosPlugUi);
        searchLineEdit->setObjectName("searchLineEdit");

        verticalLayout->addWidget(searchLineEdit);

        extensionsList = new QListWidget(LosPlugUi);
        extensionsList->setObjectName("extensionsList");
        extensionsList->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(extensionsList);


        retranslateUi(LosPlugUi);

        QMetaObject::connectSlotsByName(LosPlugUi);
    } // setupUi

    void retranslateUi(QWidget *LosPlugUi)
    {
        LosPlugUi->setWindowTitle(QCoreApplication::translate("LosPlugUi", "Extensions", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("LosPlugUi", "color: #cccccc;", nullptr));
        titleLabel->setText(QCoreApplication::translate("LosPlugUi", "EXTENSIONS", nullptr));
        searchLineEdit->setStyleSheet(QCoreApplication::translate("LosPlugUi", "\n"
"        QLineEdit {\n"
"            background-color: #2a2d2e;\n"
"            color: #ffffff;\n"
"            border: 1px solid #333333;\n"
"            border-radius: 4px;\n"
"            padding: 4px 8px;\n"
"            font-family: \"Microsoft YaHei\", sans-serif;\n"
"            font-size: 13px;\n"
"        }\n"
"        QLineEdit:focus {\n"
"            border: 1px solid #007acc;\n"
"        }\n"
"      ", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("LosPlugUi", "Search extensions in Marketplace...", nullptr));
        extensionsList->setStyleSheet(QCoreApplication::translate("LosPlugUi", "\n"
"        QListWidget {\n"
"            background-color: transparent;\n"
"            color: #d4d4d4;\n"
"            font-family: \"Microsoft YaHei\", sans-serif;\n"
"            outline: none;\n"
"        }\n"
"        QListWidget::item {\n"
"            padding: 8px;\n"
"            border-radius: 4px;\n"
"            border-bottom: 1px solid #222222;\n"
"        }\n"
"        QListWidget::item:hover {\n"
"            background-color: #2a2d2e;\n"
"        }\n"
"        QListWidget::item:selected {\n"
"            background-color: #37373d;\n"
"            color: #ffffff;\n"
"        }\n"
"      ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LosPlugUi: public Ui_LosPlugUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOSPLUGUI_H
