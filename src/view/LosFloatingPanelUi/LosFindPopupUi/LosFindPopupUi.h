// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QTextDocument>
#include <QWidget>

class QCheckBox;
class QKeyEvent;

namespace LosView
{
    /**
     * @brief 文本 查找 / 替换 的类
     */
    class LosFindPopupUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosFindPopupUi(QWidget *parent = nullptr);
        ~LosFindPopupUi() = default;

      public: // get
        QString getInput() const;
        QLineEdit *getEdit() const;
        QString getReplaceInput() const;
        QLineEdit *getReplaceEdit() const;
        QTextDocument::FindFlags getFlags() const;
        bool isRegex() const;

      public: // set
        void setMatchInfo(int current, int total);
        void setReplaceVisible(bool visible);

      signals:
        void searchOptionsChanged();
        void resizeRequested();

      protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

      private:
        QLineEdit *L_lineEdit;
        QLineEdit *L_replaceEdit;
        QLabel *L_matchLabel;
        QWidget *L_replaceRow;
        QCheckBox *L_caseBox;
        QCheckBox *L_wordBox;
        QCheckBox *L_regexBox;
    };
} /* namespace LosView */
