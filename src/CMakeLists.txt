#configure directories

add_library(parseflagscpp cpp/parse_flags.cpp)
add_library(parseflags    c/parse_flags.c)

target_include_directories(parseflagscpp
    SYSTEM PUBLIC
   "${PROJECT_SOURCE_DIR}/include")
target_include_directories(parseflags 
    SYSTEM PUBLIC 
   "${PROJECT_SOURCE_DIR}/include")

