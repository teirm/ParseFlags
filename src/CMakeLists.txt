
add_library(parseflagscpp cpp/parse_flags.cpp)
add_library(parseflags    c/parse_flags.c)

target_include_directories(parseflagscpp PUBLIC ${CMAKE_CURRENT_DIR})
target_include_directories(parseflags PUBLIC ${CMAKE_CURRENT_DIR})

