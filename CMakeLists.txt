cmake_minimum_required(VERSION 3.13)
project(VoxelSurvivalGame
        VERSION 0.0.1
        DESCRIPTION "Minecraft-like survival game inspired by Better Than Wolves"
        LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(
    GWL
    GIT_REPOSITORY https://github.com/butchhartman/Game-Window-Library.git
    GIT_TAG v0.0.5
)

FetchContent_Declare(
    GML
    GIT_REPOSITORY https://github.com/butchhartman/GameMathLibrary.git
    GIT_TAG v0.0.1-alpha.8 
)

FetchContent_MakeAvailable(GWL GML)

link_libraries(GML)
add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} GWL)

target_include_directories(${PROJECT_NAME} PUBLIC include/Engine)

add_subdirectory(src)
add_subdirectory(vendor)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/src/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders)