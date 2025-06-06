cmake_minimum_required(VERSION 3.13)
project(VoxelSurvivalGame
        VERSION 0.0.1
        DESCRIPTION "Minecraft-like survival game inspired by Better Than Wolves"
        LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(
    GWL
    GIT_REPOSITORY https://github.com/butchhartman/Game-Window-Library.git
    GIT_TAG v0.0.1
)

FetchContent_MakeAvailable(GWL)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} GWL)
target_include_directories(${PROJECT_NAME} PUBLIC include)
add_subdirectory(src)