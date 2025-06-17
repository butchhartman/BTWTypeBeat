cmake_minimum_required(VERSION 3.13)
add_subdirectory(Classes)
add_subdirectory(Filesystem)
target_sources(${PROJECT_NAME}
                PUBLIC
                main.cpp)
