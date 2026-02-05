add_library(output STATIC
  scan_report.c
  print_time.c
  html.c
)

target_include_directories(output
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
