
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")


add_executable(fbs_reader 
    ${sources} ${headers}
)

install(TARGETS fbs_reader DESTINATION /usr/local/bin/)

