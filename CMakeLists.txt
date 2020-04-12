cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project(LearnOpenGL)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)

add_executable(window
        src/window/createwindow.cpp)
add_executable(triangle
        src/triangle/triangle.cpp)

# find the required packages
find_package(glfw3 3.3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBS glfw opengl32)
set(LIBS ${LIBS} GLAD)

add_library(GLAD "src/glad.c")

target_link_libraries(window ${LIBS})
target_link_libraries(triangle ${LIBS})



