cmake_minimum_required(VERSION 3.13)

project(NovelRTTest VERSION 0.0.1)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

include_directories(../include)

add_executable(NovelRTSamples src/main.cpp)

add_dependencies(NovelRTSamples libNovelRT)

target_link_libraries(NovelRTSamples LINK_PUBLIC libNovelRT)
