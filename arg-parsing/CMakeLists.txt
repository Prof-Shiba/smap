add_library(arg-parser STATIC
  arg-parser.c
)

target_include_directories(arg-parser
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
