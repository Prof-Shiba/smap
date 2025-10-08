cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(smap C)
add_executable(${PROJECT_NAME} main.c)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wfatal-errors)
endif()

add_subdirectory(libs/shiba-core)
target_link_libraries(${PROJECT_NAME} PRIVATE shiba-core)
