cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project(LearnOpenGL)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)

# find the required packages
find_package(glfw3 3.3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

if (WIN32)
    set(LIBS glfw opengl32)
elseif (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    # note that the order is important for setting the libs
    # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread ${ASSIMP_LIBRARY})
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
    INCLUDE_DIRECTORIES(/System/Library/Frameworks)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
    FIND_LIBRARY(GLFW3_LIBRARY glfw)
    SET(APPLE_LIBS ${OpenGL_LIBRARY})
    SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY})
    set(LIBS ${LIBS} ${APPLE_LIBS})
endif()

add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

set(CHAPTERS
    window
    triangle
    shader
)

foreach(CHAPTER ${CHAPTERS})
    file(GLOB SOURCE
         "src/${CHAPTER}/*.cpp"
    )
    set(NAME "${CHAPTER}")
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})
endforeach(CHAPTER)

include_directories(${CMAKE_SOURCE_DIR}/include)



