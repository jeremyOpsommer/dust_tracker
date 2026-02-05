cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.3\\mingw_64\\")

find_package(Qt6Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB UI_SOURCES "src/ui/*.cpp" "src/ui/*.h")
file(GLOB CORE_SOURCES "src/core/*.cpp" "src/core/*.h")
file(GLOB MANAGER_SOURCES "src/managers/*.cpp" "src/managers/*.h")

add_executable(dusttracker
        src/main.cpp
        ${UI_SOURCES}
        ${CORE_SOURCES}
        ${MANAGER_SOURCES}
)
target_include_directories(dusttracker PRIVATE src)
target_link_libraries(dusttracker Qt6::Widgets)
