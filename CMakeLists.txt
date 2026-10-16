cmake_minimum_required(VERSION 3.16)
project(SoftwareRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build in Release mode unless told otherwise. An unoptimised build is several times slower.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# SFML 2.6.2: by default CMake downloads and builds it (nothing to install by hand).
# Pass -DUSE_SYSTEM_SFML=ON to use an SFML 2.6.x that is already installed instead.
option(USE_SYSTEM_SFML "Use an installed SFML 2.6.x instead of downloading SFML 2.6.2" OFF)

if(USE_SYSTEM_SFML)
    find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
else()
    include(FetchContent)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)           # static: no SFML DLLs to ship
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)            # the renderer needs no audio
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)          # ... and no networking
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)                    # lets new CMake versions accept SFML's older CMake files
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(SFML)
endif()

add_executable(renderer src/main.cpp)
target_link_libraries(renderer PRIVATE sfml-graphics sfml-window sfml-system)

# MinGW (Windows): link the compiler's runtime into the .exe so it runs without extra DLLs.
if(MINGW)
    target_link_options(renderer PRIVATE -static)
endif()

# Put assets/ (models, textures, font) next to the executable, so it runs from either the repo root or the build folder.
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    add_custom_command(TARGET renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:renderer>/assets)
endif()
