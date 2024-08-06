cmake_minimum_required(VERSION 3.26.4)
project(td_game)
add_executable(td_game src/main.cpp)

set(SFML_DIR "/usr/lib64/cmake/SFML")
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)