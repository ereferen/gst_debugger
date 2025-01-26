cmake_minimum_required(VERSION 3.10)
project(GStreamerHelloWorld)

# C++14を有効化
set(CMAKE_CXX_STANDARD 14)

# GStreamerのパッケージを探す
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# インクルードディレクトリとリンクライブラリを設定
include_directories( ${GSTREAMER_INCLUDE_DIRS}
                     ${GLIB_INCLUDE_DIRS}
                     src/bus_handler
                     src/input_controller)

link_directories(${GSTREAMER_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS})

add_definitions(${GSTREAMER_CFLAGS_OTHER} ${GLIB_CFLAGS_OTHER})

# 実行ファイルを作成
add_executable( hello_world 
                src/main.cpp 
                src/bus_handler/bus_handler.cpp
                src/input_controller/input_controller.cpp)

target_link_libraries( hello_world 
                       ${GSTREAMER_LIBRARIES}
                       ${GLIB_LIBRARIES})
