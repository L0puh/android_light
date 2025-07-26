cmake_minimum_required(VERSION 3.5)

add_library(native-lib SHARED myapp.cpp) 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_include_directories(native-lib PRIVATE
    /usr/lib/jvm/java-24-jdk/include
    /usr/lib/jvm/java-24-jdk/include/linux
)
target_link_libraries( native-lib )
