
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

include_directories(./)

add_executable(test_cpp "main.cpp" 
    ${sources} ${headers}
    )

