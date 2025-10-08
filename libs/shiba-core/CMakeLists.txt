add_library(shiba-core STATIC
  shiba-extra.c
  shiba-memory-debug.c
)

target_include_directories(shiba-core
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
