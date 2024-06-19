cmake_minimum_required(VERSION 3.10)

# Укажите имя вашего проекта
project(MyProject)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(googletest)


include_directories(func/include)
file(GLOB_RECURSE SOURCE_FILES "func/src/*.cpp")

add_executable(MainApp main.cpp ${SOURCE_FILES})

add_executable(TestApp test.cpp ${SOURCE_FILES})

# Добавьте поддиректорию с Google Test




target_link_libraries(TestApp gtest gtest_main)
target_include_directories(TestApp PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(TestApp)