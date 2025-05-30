cmake_minimum_required(VERSION 3.12)
project(minecraft-clone C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SRC_FILES
    src/*.c
)

file(GLOB LIB_FILES
    lib/*/*.c
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/util/config.h
)

# Include directories
include_directories(
    lib/noise
    lib/parson
    lib/miniz
    lib/stb
    lib/cglm/include
    src/include
    ${CMAKE_CURRENT_BINARY_DIR}/src/util
)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)

add_compile_options(-Wall -Wextra -Werror -Wreturn-type -Wno-type-limits)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${LIB_FILES}
)

target_link_libraries(${PROJECT_NAME}
    libglew_static
    glfw
    opengl32
)
