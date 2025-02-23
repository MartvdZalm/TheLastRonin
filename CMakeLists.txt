cmake_minimum_required(VERSION 3.1)
project(TheLastRonin)

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

message("Configuring glad")
include_directories(libs/glad/include)
add_subdirectory(libs/glad)

message("Configuring glm")
include_directories(libs/glm/)
add_subdirectory(libs/glm)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCE src/*.cpp)

add_executable(TheLastRonin ${SOURCE})

target_link_libraries(TheLastRonin OpenGL::GL glad glfw)
