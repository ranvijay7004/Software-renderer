cmake_minimum_required(VERSION 3.20)
project(SoftwareRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL2_ROOT "C:/dev/SDL2/x86_64-w64-mingw32")

add_executable(renderer src/main.cpp)

target_include_directories(renderer PRIVATE ${SDL2_ROOT}/include/SDL2)
target_link_directories(renderer PRIVATE ${SDL2_ROOT}/lib)
target_link_libraries(renderer PRIVATE ${SDL2_ROOT}/lib/libSDL2.dll.a)
target_link_options(renderer PRIVATE -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive)
