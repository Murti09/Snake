cmake_minimum_required(VERSION 3.15)
project(Snake VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

add_executable(snake src/main.cpp)
target_link_libraries(snake PRIVATE raylib)

# Windows braucht diese zusätzlichen System-Libraries für raylib
if (WIN32)
    target_link_libraries(snake PRIVATE opengl32 gdi32 winmm)
endif()