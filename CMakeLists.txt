CMAKE_MINIMUM_REQUIRED(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(RazeRenderer)

add_executable(raze)

target_sources(raze PRIVATE
    src/main.cpp
    src/interval.cpp
    src/camera.cpp
    src/color.cpp
    src/random.cpp
    src/renderer.cpp
)

target_include_directories(raze PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
)