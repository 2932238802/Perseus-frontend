// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

namespace LosCommon
{
    namespace Perseus_Constants
    {
        enum BottomTabWidget
        {
            OUTPUT   = 0,
            ISSUES   = 1,
            TERMINAL = 2
        };

        inline constexpr int WAIT_FOR_SESSION_TIME_MS = 300;
        inline constexpr int ZOOM_MAX                 = 28;
        // inline const int ZOOM_MIN                 = 8;
        inline constexpr int ZOOM_MIN   = 8;
        inline constexpr int ZOOM_DELTA = 2;

    } /* namespace Perseus_Constants */
} // namespace LosCommon