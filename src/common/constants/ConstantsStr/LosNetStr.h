// Copyright (c) 2026 LosAngelous (shengjie.lin)



namespace LosCommon
{

    namespace LosNet_Constants
    {
        inline constexpr const char *BASE_URL = "http://localhost:3000/";

        namespace API
        {
            inline constexpr const char *PLUGIN_API           = "LosAngelous/api/plugins";
            inline constexpr const char *REGISTER_API         = "LosAngelous/api/register";
            inline constexpr const char *LOGIN_API            = "LosAngelous/api/login";
            inline constexpr const char *AUTOLOGIN_API        = "LosAngelous/api/me";
            inline constexpr const char *AGENT_CHAT_API       = "LosAngelous/api/agent/chat";
            inline constexpr const char *AGENT_ADD_API        = "LosAngelous/api/agent/add";
            inline constexpr const char *AGENT_LIST_MODELS    = "LosAngelous/api/agent/list_models";
            inline constexpr const char *AGENT_LIST_PROVIDERS = "LosAngelous/api/agent/list_providers";
            inline constexpr const char *AGENT_DELETE_API     = "LosAngelous/api/agent/delete";
            inline constexpr const char *ROOT_API             = "";
        } /* namespace API */

        namespace HEADER_TYPE
        {
            inline constexpr const char *JSON_TYPE = "application/json";
        } /* namespace HEADER_TYPE */

    } /* namespace LosNet_Constants */
} // namespace LosCommon