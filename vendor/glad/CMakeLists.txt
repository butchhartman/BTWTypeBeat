cmake_minimum_required(VERSION 3.13)
target_sources(${PROJECT_NAME}
                PUBLIC
                glad.c
                PUBLIC
                FILE_SET HEADERS
                FILES
                glad.h)
