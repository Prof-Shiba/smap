add_library(scan-engine STATIC
  scan-engine.c
  scan-engine-connect.c
)

target_include_directories(scan-engine
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
