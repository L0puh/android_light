cmake_minimum_required(VERSION 3.31)
project(editor)
cmake_policy(SET CMP0072 NEW)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs)
find_package(OpenGL REQUIRED)

set(IMGUI_LIB imgui)
set(OPENGL_LIB OpenGL::GL)

set(IMGUI_INCLUDE ${CMAKE_CURRENT_SOURE_DIR}/libs/imgui)
set(INCLUDE_DIR include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) 
add_executable(editor ${SOURCES})

target_link_libraries(editor PRIVATE
    ${IMGUI_LIB}
    ${GLFW_LIB}
    ${GLAD_LIB}
    ${OPENGL_LIB}
)

target_include_directories(editor PRIVATE
   ${INCLUDE_DIR}
   ${IMGUI_INCLUDE_DIR}

)
