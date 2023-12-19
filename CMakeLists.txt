cmake_minimum_required(VERSION 3.22)
project(horde)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/enet)
add_subdirectory(libs/entt)
add_subdirectory(libs/glm)
add_subdirectory(libs/raylib)
add_subdirectory(libs/imgui)

file(GLOB_RECURSE GAME_SOURCES game/*.cpp)
file(GLOB_RECURSE GAME_HEADERS game/*.h)
file(GLOB_RECURSE FLATBUFFER libs/serialization/*.h)

add_executable(horde main.cpp ${CORE_SOURCES} ${FLATBUFFER} ${GAME_SOURCES} ${GAME_HEADERS})

target_include_directories(horde PUBLIC libs/enet/include glm raylib/src libs/entt/single_include game/ libs/serialization libs/imgui)
target_link_libraries(horde enet glm raylib imgui)

