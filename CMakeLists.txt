cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.3\\mingw_64\\")

find_package(Qt6Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(dusttracker
    src/main.cpp
    src/TrackerWindow.cpp
    src/PackManager.cpp
    src/LogicEngine.cpp
    src/headers/TrackerWindow.h
)
target_include_directories(dusttracker PRIVATE src)
target_link_libraries(dusttracker Qt6::Widgets)
