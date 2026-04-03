set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_executable(LosTest LosTest.cpp)

target_link_libraries(LosTest
    PRIVATE
        PerseusCore
        Qt${QT_VERSION_MAJOR}::Test
)

add_test(
    NAME LosTest
    COMMAND LosTest
)