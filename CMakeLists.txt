# Copyright (c) 2026 LosAngelous (shengjie.lin)

cmake_minimum_required(VERSION 3.18)
# 可以直接 再后面 加入 C 支持 C语言的编译
project(Perseus VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/view/)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    Network
    Concurrent
    WebEngineWidgets
    WebChannel
    Test
    Core5Compat
)

file(GLOB_RECURSE CORE_SOURCES
    "src/common/*.cpp" "src/common/*.h"
    "src/core/*.cpp" "src/core/*.h"
    "src/models/*.cpp" "src/models/*.h"
    "src/view/*.ui" "src/view/*.cpp" "src/view/*.h"
    "src/*.h" "src/*.cpp"

)

add_library(PerseusCore STATIC ${CORE_SOURCES})
add_library(tree_sitter STATIC
    third_party/tree-sitter/lib/src/lib.c
)
add_library(tree_sitter_cpp STATIC
    third_party/tree-sitter-cpp/src/parser.c
    third_party/tree-sitter-cpp/src/scanner.c
)


# libgit2
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CLI OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(USE_SSH OFF CACHE STRING "" FORCE)
set(USE_HTTPS ON CACHE STRING "" FORCE)
set(USE_NSEC OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/libgit2)


target_link_libraries(PerseusCore PUBLIC libgit2package)
target_link_libraries(PerseusCore PUBLIC tree_sitter tree_sitter_cpp)
target_link_libraries(PerseusCore PUBLIC stdc++exp)


target_include_directories(PerseusCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libgit2/include
)
target_include_directories(tree_sitter
    PUBLIC third_party/tree-sitter/lib/include
    PRIVATE third_party/tree-sitter/lib/src
)
target_include_directories(tree_sitter_cpp
    PUBLIC
    third_party/tree-sitter-cpp/bindings/c
    PRIVATE third_party/tree-sitter-cpp/src
)

set_target_properties(tree_sitter PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS ON
    POSITION_INDEPENDENT_CODE ON
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
set_target_properties(tree_sitter_cpp PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS ON
    POSITION_INDEPENDENT_CODE ON
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)

target_include_directories(PerseusCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(PerseusCore PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    Qt${QT_VERSION_MAJOR}::WebChannel
    Qt${QT_VERSION_MAJOR}::Test
    Qt6::Core5Compat
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Perseus MANUAL_FINALIZATION "src/main.cpp" "resources/resources.qrc")
else()
    if(ANDROID)
        add_library(Perseus SHARED "src/main.cpp")
    else()
        add_executable(Perseus "src/main.cpp")
    endif()
endif()
target_link_libraries(Perseus PRIVATE PerseusCore)


enable_testing()
add_subdirectory(test)


if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Perseus)
endif()

set_target_properties(Perseus PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Perseus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Perseus)
endif()
