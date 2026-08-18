// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

/*
 * LosDialog 样式模板
 * - 与 perseus_style.qss 共用同一套 @token@ 占位符
 * - 运行时由 LosCore::LosThemeManager::buildExtraQss(模板, 主题名) 替换为当前主题色
 * - 抽离到独立头文件, 保持 逻辑(.cpp) 与 样式 分离 (参照 LosToolMissUI_style.h)
 *
 * 用于美化"选择本地一个文件"等 QDialog / QFileDialog 窗口.
 *
 * 可用 token:
 *   @background@  @panelBg@  @borderStrong@  @selection@  @foreground@
 *   @primary@  @primaryHover@  @primaryPressed@  @highlightFg@
 *   @danger@  @success@  @muted@
 *   @mutedAlpha50@  @primaryAlpha70@  @primaryAlpha75@
 */
namespace LosStyle
{
    inline const QString LosDialog_styleTemplate()
    {
        return QStringLiteral(R"(
            /* ── 对话框根容器 ── */
            QDialog {
                background-color: @background@;
                color: @foreground@;
                border: none;
            }

            QFileDialog {
                background-color: @background@;
                color: @foreground@;
            }

            /* ── 标签 ── */
            QLabel {
                color: @foreground@;
                background-color: transparent;
            }

            /* ── 文件名输入框 ── */
            QLineEdit {
                background-color: @panelBg@;
                color: @foreground@;
                border: 1px solid @borderStrong@;
                border-radius: 8px;
                padding: 8px 12px;
                selection-background-color: @primaryAlpha70@;
                selection-color: @highlightFg@;
            }
            QLineEdit:focus {
                border: 1px solid @primary@;
                background-color: @background@;
            }

            /* ── 文件类型下拉框 ── */
            QComboBox {
                background-color: @panelBg@;
                color: @foreground@;
                border: 1px solid @borderStrong@;
                border-radius: 8px;
                padding: 7px 12px;
                min-height: 20px;
            }
            QComboBox:hover {
                border: 1px solid @primary@;
            }
            QComboBox:focus {
                border: 1px solid @primary@;
            }
            QComboBox::drop-down {
                border: none;
                width: 28px;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 5px solid transparent;
                border-right: 5px solid transparent;
                border-top: 6px solid @primary@;
                margin-right: 10px;
            }
            QComboBox QAbstractItemView {
                background-color: @panelBg@;
                color: @foreground@;
                border: 1px solid @borderStrong@;
                border-radius: 6px;
                padding: 4px;
                selection-background-color: @primaryAlpha70@;
                selection-color: @highlightFg@;
                outline: none;
            }

            /* ── 通用按钮 ── */
            QPushButton {
                background-color: @panelBg@;
                color: @foreground@;
                border: 1px solid @borderStrong@;
                border-radius: 8px;
                padding: 8px 18px;
                min-height: 24px;
            }
            QPushButton:hover {
                background-color: @primaryAlpha70@;
                border: 1px solid @primary@;
                color: @highlightFg@;
            }
            QPushButton:pressed {
                background-color: @primaryPressed@;
                border: 1px solid @primaryPressed@;
                color: @background@;
            }
            QPushButton:disabled {
                background-color: @panelBg@;
                color: @mutedAlpha50@;
                border: 1px solid @borderStrong@;
            }

            /* ── 主按钮 (Open / OK) ── */
            QPushButton#primaryBtn {
                background-color: @primary@;
                color: @highlightFg@;
                border: 1px solid @primary@;
                border-radius: 8px;
                padding: 9px 24px;
                font-weight: bold;
                min-height: 26px;
            }
            QPushButton#primaryBtn:hover {
                background-color: @primaryHover@;
                border: 1px solid @primaryHover@;
            }
            QPushButton#primaryBtn:pressed {
                background-color: @primaryPressed@;
                border: 1px solid @primaryPressed@;
                color: @foreground@;
            }

            /* ── 文件列表 / 树形视图 ── */
            QListView, QTreeView {
                background-color: @panelBg@;
                color: @foreground@;
                border: 1px solid @borderStrong@;
                border-radius: 8px;
                padding: 2px;
                alternate-background-color: @mutedAlpha50@;
                outline: none;
            }
            QListView::item, QTreeView::item {
                border-radius: 6px;
                padding: 5px 8px;
                margin: 1px 2px;
                border: none;
            }
            QListView::item:hover, QTreeView::item:hover {
                background-color: @primaryAlpha70@;
                color: @highlightFg@;
            }
            QListView::item:selected, QTreeView::item:selected {
                background-color: @primary@;
                color: @highlightFg@;
                font-weight: bold;
            }
            QListView::item:selected:!active, QTreeView::item:selected:!active {
                background-color: @primaryAlpha70@;
                color: @foreground@;
            }

            /* ── 表头 ── */
            QHeaderView::section {
                background-color: @background@;
                color: @foreground@;
                border: none;
                border-bottom: 1px solid @borderStrong@;
                padding: 7px 10px;
                font-weight: bold;
                font-size: 12px;
            }
            QHeaderView::section:hover {
                background-color: @selection@;
            }

            /* ── 垂直滚动条 ── */
            QScrollBar:vertical {
                background-color: transparent;
                width: 10px;
                border: none;
                margin: 3px 1px;
            }
            QScrollBar::handle:vertical {
                background-color: @borderStrong@;
                border-radius: 5px;
                min-height: 36px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: @primary@;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
            QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                height: 0px;
                background-color: transparent;
            }

            /* ── 水平滚动条 ── */
            QScrollBar:horizontal {
                background-color: transparent;
                height: 10px;
                border: none;
                margin: 1px 3px;
            }
            QScrollBar::handle:horizontal {
                background-color: @borderStrong@;
                border-radius: 5px;
                min-width: 36px;
            }
            QScrollBar::handle:horizontal:hover {
                background-color: @primary@;
            }
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
            QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                width: 0px;
                background-color: transparent;
            }

            /* ── 侧边栏 (QFileDialog 的快捷导航) ── */
            QFileDialog QListView#sidebar {
                background-color: @background@;
                border: none;
                border-right: 1px solid @borderStrong@;
                padding: 6px 2px;
                border-radius: 0px;
            }
            QFileDialog QListView#sidebar::item {
                border-radius: 6px;
                padding: 6px 10px;
                margin: 1px 4px;
            }
            QFileDialog QListView#sidebar::item:hover {
                background-color: @primaryAlpha70@;
                color: @highlightFg@;
            }
            QFileDialog QListView#sidebar::item:selected {
                background-color: @primary@;
                color: @highlightFg@;
            }

            /* ── 工具栏按钮 (后退/前进/上级) ── */
            QFileDialog QToolButton {
                background-color: transparent;
                color: @foreground@;
                border: 1px solid transparent;
                border-radius: 6px;
                padding: 5px 8px;
            }
            QFileDialog QToolButton:hover {
                background-color: @primaryAlpha70@;
                border: 1px solid @primary@;
                color: @highlightFg@;
            }
            QFileDialog QToolButton:pressed {
                background-color: @primaryPressed@;
                border: 1px solid @primaryPressed@;
            }

            /* ── 对话框按钮盒 (Open / Cancel 区域) ── */
            QDialogButtonBox {
                background-color: transparent;
            }
            QDialogButtonBox QPushButton {
                min-width: 80px;
            }
        )");
    }
} /* namespace LosStyle */