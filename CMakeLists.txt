cmake_minimum_required(VERSION 3.14)
project(example)

# Set project-wide version
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(SOURCE_FILES
    src/main.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
include(FetchContent)

## Resolve dependencies ##

# Fetch raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

# Fetch raylib_cpp
FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG v4.5.0
)
FetchContent_MakeAvailable(raylib_cpp)

# Fetch raygui
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 3.6
)
FetchContent_MakeAvailable(raygui)

# Fetch imnotgui - my own gui library
#FetchContent_Declare(
#    imnotgui
#    GIT_REPOSITORY https://github.com/Gabriel-Dropout/imnotgui-raylib-cpp.git
#    GIT_TAG 0.1.0
#)
#FetchContent_MakeAvailable(imnotgui)


# Add your project executable or library target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link against raylib and raylib_cpp
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${raygui_SOURCE_DIR}/src")  # raylib doesn't have CMakelist.txt, so we have to manually add the include directory
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")

# asset folder
file(COPY assets DESTINATION .)