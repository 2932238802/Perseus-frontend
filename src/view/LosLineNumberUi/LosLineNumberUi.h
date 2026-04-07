#pragma once

#include "view/LosEditorUi/LosEditorUi.h"
#include <qevent.h>

namespace LosView
{
class LosLineNumberUi : public QWidget
{
  public:
    explicit LosLineNumberUi(LosEditorUi *line);
    ~LosLineNumberUi() = default;

  public: // tool
    QSize sizeHint() const override;

  protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

  private:
    LosEditorUi *LOS_editor;
};

} // namespace LosView