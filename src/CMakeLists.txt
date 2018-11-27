
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")


add_executable(fbs_reader "main.cpp" 
    ${sources} ${headers}
    )

install(TARGETS fbs_reader DESTINATION /usr/local/bin/)

