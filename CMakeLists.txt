cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dynamic Qt path detection
if(WIN32)
    file(GLOB QT_VERSIONS "C:/Qt/6.*/mingw_64")
    if(QT_VERSIONS)
        list(SORT QT_VERSIONS)
        list(REVERSE QT_VERSIONS)
        list(GET QT_VERSIONS 0 QT_PATH)
        set(CMAKE_PREFIX_PATH ${QT_PATH})
        message(STATUS "Detected Qt: ${CMAKE_PREFIX_PATH}")
    endif()
elseif(UNIX AND NOT APPLE)
    find_program(QMAKE_EXECUTABLE NAMES qmake6 qmake)
    if(QMAKE_EXECUTABLE)
        execute_process(COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_PREFIX
            OUTPUT_VARIABLE QT_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(CMAKE_PREFIX_PATH ${QT_PATH})
        message(STATUS "Detected Qt: ${CMAKE_PREFIX_PATH}")
    endif()
elseif(APPLE)
    file(GLOB QT_VERSIONS "/usr/local/Cellar/qt/6.*" "$ENV{HOME}/Qt/6.*/clang_64")
    if(QT_VERSIONS)
        list(SORT QT_VERSIONS)
        list(REVERSE QT_VERSIONS)
        list(GET QT_VERSIONS 0 QT_PATH)
        set(CMAKE_PREFIX_PATH ${QT_PATH})
        message(STATUS "Detected Qt: ${CMAKE_PREFIX_PATH}")
    endif()
endif()

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

# Deploy Qt runtime libraries
if(WIN32)
    add_custom_command(TARGET dusttracker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE_DIR:dusttracker>
    )

    # Copy platform plugin
    add_custom_command(TARGET dusttracker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:dusttracker>/platforms
    )
    add_custom_command(TARGET dusttracker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/plugins/platforms/qwindows.dll
        $<TARGET_FILE_DIR:dusttracker>/platforms/
    )
endif()
