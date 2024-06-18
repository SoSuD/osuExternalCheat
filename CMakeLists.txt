cmake_minimum_required(VERSION 3.10)
project(ProjectAIP)

set(CMAKE_CXX_STANDARD 17)

# Указываем пути к заголовочным файлам
include_directories(func/include)

# Добавляем исходные файлы для основного приложения
file(GLOB_RECURSE SOURCE_FILES "func/src/*.cpp")



# Исполняемый файл для основного приложения
add_executable(MainApp main.cpp ${SOURCE_FILES})
