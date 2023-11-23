cmake_minimum_required(VERSION 3.22)
project(horde)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/enet)
add_subdirectory(libs/entt)
add_subdirectory(libs/glm)
add_subdirectory(libs/raylib)

file(GLOB_RECURSE CORE_SOURCES core/*.cpp)
file(GLOB_RECURSE CORE_HEADERS core/*.h)

file(GLOB_RECURSE GAME_SOURCES game/*.cpp)
file(GLOB_RECURSE GAME_HEADERS game/*.h)

add_executable(horde main.cpp ${CORE_SOURCES} ${CORE_HEADERS} ${GAME_SOURCES} ${GAME_HEADERS})

target_include_directories(horde PUBLIC libs/enet/include glm raylib/src libs/entt/single_include core/ game/)
target_link_libraries(horde enet glm raylib)

