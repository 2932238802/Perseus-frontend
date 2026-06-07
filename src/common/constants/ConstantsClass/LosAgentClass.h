// Copyright (c) 2026 LosAngelous (shengjie.lin)
#include <QString>

namespace LosCommon
{
    namespace LosAgent_Constants
    {
        struct LosModelConfig
        {
            QString L_name;                 // 用户起的显示名    我的DeepSeek"
            QString L_baseUrl;              // API 基地址       如 "https://api.deepseek.com"
            QString L_apiKey;               // 用户填的密钥
            QString L_model;                // 模型名           如 "deepseek-chat"
        };
    } // namespace LosAgent_Constants
} // namespace LosCommon
