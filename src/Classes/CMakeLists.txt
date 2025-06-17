cmake_minimum_required(VERSION 3.13)
add_subdirectory(Geometry)
target_sources(${PROJECT_NAME}
                PUBLIC
                Shader.cpp
                Camera.cpp)