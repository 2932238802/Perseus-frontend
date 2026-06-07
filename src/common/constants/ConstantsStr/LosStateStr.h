// Copyright (c) 2026 LosAngelous (shengjie.lin)

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
            PROJECT_DIR,         // 工作目录
            SCRIPTS_INSTALL_DIR, // 脚本安装路径
            PYTHON_EXE_PATH,     // python 安装路径
            RUSTC_EXE_PATH,
            CMAKE_EXE_PATH,
            GXX_EXE_PATH
        };
    } /* namespace LosState_Constants */
} // namespace LosCommon