#pragma once

namespace LosCommon
{


    /**
     * @brief LosState_Constants
     * 全局的一些字段
     *
     * PYTHON_EXE_PATH
     * GXX_EXE_PATH
     * - 自已设定的 exe 解释器
     *
     * SCRIPTS_INSTALL_DIR
     * - 内置安装脚本所在的目录
     */
    namespace LosState_Constants
    {
        enum class SG_STR
        {
            PROJECT_DIR,
            SCRIPTS_INSTALL_DIR,
            PYTHON_EXE_PATH,
            GXX_EXE_PATH,
            RUSTC_EXE_PATH,
            CMAKE_EXE_PATH,
        };
    } /* namespace LosState_Constants */
} // namespace LosCommon