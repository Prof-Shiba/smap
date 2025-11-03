add_library(shiba-getopt STATIC
  shiba-getopt.c
)

target_include_directories(shiba-getopt
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
