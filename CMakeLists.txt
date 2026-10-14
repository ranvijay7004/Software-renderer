cmake_minimum_required(VERSION 3.20)
project(SoftwareRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(renderer src/main.cpp)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

target_link_libraries(renderer PRIVATE
  sfml-graphics
  sfml-window
  sfml-system)
add_custom_command(TARGET renderer POST_BUILD COMMAND 
  ${CMAKE_COMMAND} -E copy_directory 
  ${CMAKE_SOURCE_DIR}/models 
  $<TARGET_FILE_DIR:renderer>/models)
