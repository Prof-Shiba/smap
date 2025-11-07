add_library(port-engine STATIC
  port-list.c
)

target_include_directories(port-engine
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
