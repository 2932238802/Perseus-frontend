
#pragma once
namespace LosCommon
{

enum BottomTabWidget
{
    OUTPUT   = 0,
    PROBLEMS = 1,
    TERMINAL = 2
};

inline const int MAX_VISIBLE_COMPLETION = 10;


namespace LosFormatManager_Constants
{
inline const int WAITFORSTARTED_TIME_MS  = 1000;
inline const int WAITFORFINISHED_TIME_MS = 3000;
} // namespace LosFormatManager_Constants

} // namespace LosCommon