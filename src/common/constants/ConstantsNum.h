
#pragma once
namespace LosCommon
{



// 最多 能看到 的 补全 字段
inline const int MAX_VISIBLE_COMPLETION = 10;


// 格式化
namespace LosFormatManager_Constants
{
inline const int WAITFORSTARTED_TIME_MS  = 1000;
inline const int WAITFORFINISHED_TIME_MS = 3000;
} // namespace LosFormatManager_Constants


namespace Perseus_Constants
{
enum BottomTabWidget
{
    OUTPUT   = 0,
    PROBLEMS = 1,
    TERMINAL = 2
};

inline const int WAIT_FOR_SESSION_TIME_MS = 1000;
inline const int ZOOM_MAX                 = 48;
inline const int ZOOM_MIN                 = 8;


} // namespace Perseus_Constants

} // namespace LosCommon