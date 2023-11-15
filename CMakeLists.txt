cmake_minimum_required(VERSION 3.27)
project(horde)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/enet)
add_subdirectory(libs/entt)
add_subdirectory(libs/glm)
add_subdirectory(libs/raylib)

file(GLOB_RECURSE SOURCES core/*.cpp)
file(GLOB_RECURSE HEADERS core/*.h)

file(GLOB_RECURSE SOURCES ${SOURCES} game/*.cpp)
file(GLOB_RECURSE HEADERS ${HEADERS} game/*.h)

add_executable(horde main.cpp ${SOURCES} ${HEADERS}
        core/Scene.cpp
        core/Scene.h)

target_include_directories(horde PUBLIC libs/enet/include glm raylib/src libs/entt/single_include)
target_link_libraries(horde enet glm raylib)

