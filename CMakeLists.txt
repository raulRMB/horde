cmake_minimum_required(VERSION 3.22)
project(horde)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_TESTING TRUE)

add_subdirectory(libs/enet)
add_subdirectory(libs/entt)
add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/bgfx)
add_subdirectory(libs/tinygltf)

file(GLOB_RECURSE GAME_SOURCES game/*.cpp)
file(GLOB_RECURSE GAME_HEADERS game/*.h)
file(GLOB_RECURSE FLATBUFFER libs/serialization/*.h)
file(GLOB_RECURSE CLI11 libs/CLI11/*.hpp)

if(ENABLE_TESTING)
    file(GLOB_RECURSE TESTS test/*)
    add_executable(horde_tests ${TESTS} libs/doctest/doctest.h ${CORE_SOURCES} ${FLATBUFFER} ${GAME_SOURCES} ${GAME_HEADERS})
    target_include_directories(horde_tests PUBLIC libs/enet/include glm libs/glfw/include libs/entt/single_include game/ libs/serialization libs/doctest)
    target_link_libraries(horde_tests enet glm glfw)
    target_compile_definitions(horde_tests PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
endif()

add_executable(horde main.cpp ${CLI11} ${CORE_SOURCES} ${FLATBUFFER} ${GAME_SOURCES} ${GAME_HEADERS})

target_include_directories(horde PUBLIC libs/enet/include glm libs/glfw/include libs/entt/single_include game/ libs/serialization libs/bgfx/bgfx/include libs/bgfx/bx/include libs/bimg/bx/include libs/CLI11 libs/tinygltf)
target_link_libraries(horde enet glm glfw bgfx tinygltf)

