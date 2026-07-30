// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <qtmetamacros.h>

namespace LosView
{
    class LosAgentKeyUi : public QDialog
    {
        Q_OBJECT
      public: // construct
        explicit LosAgentKeyUi(QWidget *parent = nullptr);
        ~LosAgentKeyUi() = default;

      private slots: // chs
        void onSaveClicked();
        void onFetchModelsClicked();
        void onModelsReceived(bool ok, const QStringList &models, const QString &msg);
        void onProviderAdded(bool success, const QString &message);
        void applyTheme(const QString &themeName);

      private: // init
        void initUi();
        void initStyle();
        void initConnect();

      private: // tool
        void showTip(const QString &text, bool isError);

      private: // widgets
        QLabel *L_tip             = nullptr;
        QLineEdit *L_providerEdit = nullptr;
        QLineEdit *L_baseUrlEdit  = nullptr;
        QLineEdit *L_apiKeyEdit   = nullptr;
        QComboBox *L_modelsCombo  = nullptr;
        QPushButton *L_fetchBtn   = nullptr;
        QPushButton *L_saveBtn    = nullptr;
        QPushButton *L_cancelBtn  = nullptr;
    };

} /* namespace LosView */
