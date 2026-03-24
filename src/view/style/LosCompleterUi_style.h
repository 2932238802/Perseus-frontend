

#pragma once
#include <QString>
namespace LosStyle {
inline const QString LosCompleterUi_getStyle() {
  return QString(R"(
    QListView {
      background-color: #1E1E1E;
      color: #D4D4D4;
      border: 1px solid #454545;
      font-family: 'Consolas', 'Courier New', monospace;
      font-size: 14px;
      padding: 2px;
    }
    QListView::item {
      padding: 4px;
    }
    QListView::item:selected {
      background-color: #062F4A; /* 选中时的酷炫深蓝色 */
      color: #FFFFFF;
      border: none;
    }
)");
}
} // namespace LosStyle