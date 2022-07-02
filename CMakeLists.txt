cmake_minimum_required(VERSION 3.1)

project(WinToast)

add_library(${PROJECT_NAME} INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/wintoastlib.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/wintoastlib.h)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src)
