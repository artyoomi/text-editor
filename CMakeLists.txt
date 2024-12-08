cmake_minimum_required(VERSION 3.20)

project(text_editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# for debug
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
else()
    message(STATUS "Building in Release mode")
endif()

# Ensure static linking for GCC/MinGW
if(GCC)
    message(STATUS "Building in GCC environment")
    # Force static linking
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Set compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
include(FetchContent)

# Check if GLFW is already installed
find_package(glfw3 QUIET)

if ((NOT glfw3_FOUND) AND MINGW)
    message(WARNING "GLFW not found. Fetching and building GLFW locally. Its better to install GLFW system-wide first.")
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)
else()
    message(STATUS "GLFW found.")
endif()

# Conditionally add ImGui
FetchContent_Declare(
    imgui_external
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.4
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(imgui_external)

add_library(imgui
${imgui_external_SOURCE_DIR}/imgui.cpp
${imgui_external_SOURCE_DIR}/imgui_demo.cpp
${imgui_external_SOURCE_DIR}/imgui_draw.cpp
${imgui_external_SOURCE_DIR}/imgui_tables.cpp
${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

if (glfw3_FOUND)
    target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR})
else()
    target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR} ${glfw_SOURCE_DIR}/include)
endif()
include(FetchContent)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Libraries
# Rope DS
add_library(Rope STATIC lib/Rope/RNode.cpp lib/Rope/Rope.cpp)

# Stack DS
add_library(Stack STATIC lib/Stack/Stack.cpp)
target_link_libraries(Stack op)

add_library(op STATIC lib/op.cpp)

add_library(History STATIC lib/History.cpp)
target_link_libraries(History Stack Rope)

find_package(OpenGL REQUIRED)

add_library(GUI STATIC lib/GUI.cpp)
target_link_libraries(GUI OpenGL::GL glfw imgui Rope History)

add_executable(${CMAKE_PROJECT_NAME} lib/main.cpp)
# target_link_libraries(${CMAKE_PROJECT_NAME} Rope)

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} GUI)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# enable_testing()

# add_executable(Rope_test tests/Rope_test.cpp)
# target_link_libraries(Rope_test Rope gtest gtest_main)

# add_executable(Stack_test tests/Stack_test.cpp)
# target_link_libraries(Stack_test Stack gtest gtest_main)

#include(GoogleTest)
#gtest_discover_tests(Rope_test)