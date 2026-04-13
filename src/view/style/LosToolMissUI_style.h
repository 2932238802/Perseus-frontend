

#include <QString>
namespace LosStyle
{
    inline QString LosToolMissUI_getStyle()
    {
        return QString(R"(
    QDialog {
        background-color: #2b2b2b;
        color: #ffffff;
    }
    QLabel {
        color: #cccccc;
    }
    QPushButton {
        background-color: #3c3c3c;
        border: 1px solid #555555;
        padding: 8px 16px;
        border-radius: 4px;
        color: #ffffff;
    }
    QPushButton:hover {
        background-color: #4a4a4a;
    }
    QPushButton:pressed {
        background-color: #2a2a2a;
    }
                
)");
    };
} /* namespace LosStyle */