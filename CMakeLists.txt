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

# Where SFML comes from (this project needs SFML 2.6.x, not SFML 3):
#   AUTO (default)  use an installed SFML 2.6.x if there is one (Linux only), otherwise download 2.6.2
#   ON              always use the installed SFML 2.6.x (stops with an error if there is none)
#   OFF             always download and build SFML 2.6.2
set(USE_SYSTEM_SFML "AUTO" CACHE STRING "SFML source: AUTO, ON (installed 2.6.x) or OFF (download 2.6.2)")
set_property(CACHE USE_SYSTEM_SFML PROPERTY STRINGS AUTO ON OFF)

set(RENDERER_USE_SYSTEM_SFML FALSE)
if(USE_SYSTEM_SFML STREQUAL "AUTO")
    if(UNIX AND NOT APPLE)
        find_package(SFML 2.6 COMPONENTS graphics window system QUIET)
        if(SFML_FOUND)
            set(RENDERER_USE_SYSTEM_SFML TRUE)
        endif()
    endif()
elseif(USE_SYSTEM_SFML)
    set(RENDERER_USE_SYSTEM_SFML TRUE)
endif()

if(RENDERER_USE_SYSTEM_SFML)
    message(STATUS "SFML: using the SFML 2.6 installed on this machine")
    find_package(SFML 2.6 COMPONENTS graphics window system QUIET)
    if(NOT SFML_FOUND)
        message(FATAL_ERROR "No SFML 2.6.x was found on this machine. Install SFML 2.6, or configure with -DUSE_SYSTEM_SFML=OFF to download SFML 2.6.2 instead. (SFML 3 does not work with this project.)")
    endif()
else()
    message(STATUS "SFML: downloading and building SFML 2.6.2 (first build only, takes a few minutes, needs internet)")
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)                              # show the download progress
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
