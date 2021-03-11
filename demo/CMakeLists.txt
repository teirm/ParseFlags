add_executable(c_demo test_main.c)
add_executable(cpp_demo test_main.cpp)

target_link_libraries(c_demo LINK_PUBLIC parseflags)
target_link_libraries(cpp_demo LINK_PUBLIC parseflagscpp)
