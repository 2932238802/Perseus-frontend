#pragma once
#include <QString>

namespace LosCommon
{
    namespace LosTerminal_Constants
    {
        inline const QString CLOSEBTN_STYLE = R"(
        QPushButton 
        { 
            color: red; 
            border: none; 
            font-weight: bold; 
            padding: 2px; 
        }
        QPushButton:hover 
        { 
            background-color: #ffcccc; 
        }
        )";
    } // namespace LosTerminal_Constants
} // namespace LosCommon