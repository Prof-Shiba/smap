add_library(shiba-threads STATIC
  shiba-threads.c
)

target_include_directories(shiba-threads
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
