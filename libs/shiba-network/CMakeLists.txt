add_library(shiba-network STATIC
  shiba-network.c
)

target_include_directories(shiba-network
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
