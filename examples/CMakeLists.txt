cmake_minimum_required(VERSION 3.10)
project(epaser_examples VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 添加选项用于指定Qt版本
option(USE_QT6 "Use Qt6 instead of Qt5" OFF)

#导入Qt相关依赖包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# 添加可执行文件
add_executable(gui_example
    ./common/QFileHelper.h
    ./common/QFileHelper.cpp
    MainWindow.ui
    MainWindow.cpp
    MainWindow.h
    main.cpp
)

# 添加包含路径
target_include_directories(gui_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/../parser_qt
        ${CMAKE_CURRENT_SOURCE_DIR}/../parser_std
)

# 设置链接
if(USE_QT6)
    target_link_libraries(gui_example
        PRIVATE
            eparserstd
            eparserqt
            muparser
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Widgets
    )
else()
    # Qt5 链接，使用原来正确的方式
    target_link_libraries(gui_example
        PRIVATE
            eparserstd
            eparserqt
            muparser
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Widgets
    )
endif()
