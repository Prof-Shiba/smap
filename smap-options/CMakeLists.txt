add_library(smap-options STATIC
  smap-options.c
)

target_include_directories(smap-options
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
