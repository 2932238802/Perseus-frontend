// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "view/LosFileTreeDelegate/LosFileTreeDelegate.h"
#include "common/constants/ConstantsClass/LosFileTreeModelClass.h" 
#include "common/constants/ConstantsStr/LosFileTreeModelStr.h"   
#include <QPainter>

namespace LosView
{
    LosFileTreeDelegate::LosFileTreeDelegate(QObject *parent) : QStyledItemDelegate(parent) {}
 
 

    /**
     * @brief 重写左侧文件树的 git 状态书写
     * 
     * @param painter 
     * @param option 
     * @param index 
     */
    void LosFileTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
    {
        QStyledItemDelegate::paint(painter, option, index);
        QString letter = index.data(LosCommon::LosFileTreeModel_Constants::FileRole::GITSTATUS_ROLE).toString();
        if (letter.isEmpty())
            return;  
        QColor color;
        if (letter == "M")
            color = QColor(LosCommon::LosFileTreeModel_Constants::MODIFY_COLOR);
        else if (letter == "U")
            color = QColor(LosCommon::LosFileTreeModel_Constants::WT_NEW_COLOR);
        else if (letter == "A")
            color = QColor(LosCommon::LosFileTreeModel_Constants::INDEX_NEW_COLOR);
        else
            color = option.palette.text().color();
        painter->save();
        painter->setPen(color);
        QRect r = option.rect;
        r.setRight(r.right() - 8);
        painter->drawText(r, Qt::AlignRight | Qt::AlignVCenter, letter);
        painter->restore();
    }
} // namespace LosView