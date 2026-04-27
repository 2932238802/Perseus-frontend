
#pragma once
#include<QString>

namespace LosCommon{
    /**
     * @brief Get the Leading White Space object
     * 
     * @param text 
     * @return QString 
     */
    inline QString GetLeadingWhiteSpace(const QString&text)
    {
        int i =0 ;
        while(i < text.size() && (text.at(i) == ' ' || text.at(i) == '\t'))
        {
            ++i;
        }
        return text.left(i);
    }
}