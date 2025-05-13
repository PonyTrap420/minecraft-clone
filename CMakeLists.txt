cmake_minimum_required(VERSION 3.12)
project(minecraft-clone C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(src/include)

add_subdirectory(lib/glfw)

add_executable(${PROJECT_NAME}
    src/main.c
)

target_link_libraries(${PROJECT_NAME}
    glfw
    opengl32
    user32
    gdi32
    shell32
)