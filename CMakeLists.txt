cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(smap C)

option(DEBUG "Build the project in debug mode" OFF)
if (DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
  message(STATUS "Debug build enabled")
endif()

add_executable(${PROJECT_NAME} main.c)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wfatal-errors)
endif()

add_subdirectory(libs/shiba-core)
add_subdirectory(libs/shiba-network)
add_subdirectory(libs/shiba-threads)
add_subdirectory(libs/shiba-getopt)
add_subdirectory(arg-parsing)
add_subdirectory(smap-options)
add_subdirectory(port-engine)

target_link_libraries(${PROJECT_NAME} PRIVATE
  shiba-core
  shiba-network
  shiba-threads
  arg-parser
  smap-options
  shiba-getopt
  port-engine
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()
