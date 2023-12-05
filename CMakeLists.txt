cmake_minimum_required(VERSION 3.27)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(source "${CMAKE_SOURCE_DIR}")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
link_libraries(noise)

include_directories(${SDL2_INCLUDE_DIRS})

link_libraries(vulkan)

set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/imgui")

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES})

set(PROJECT_SOURCES main.cpp world/Chunk.cpp block/Block.cpp
        vk_mesh.cpp
        vk_engine.cpp
        VkBootstrap.cpp
        vk_initialisers.cpp
        Entity/Entity.cpp
        player/Player.cpp
        RenderUtils/RenderBlock.cpp
        DebugUtils/ImGuiHandler.cpp
        world/World.cpp
        vk_textures.cpp)

set(PROJECT_HEADERS main.h stb_image.h Camera.h world/Chunk.h block/Block.h
        vk_types.h
        vk_mesh.h
        VkBootstrap.h
        VkBootstrapDispatch.h
        vk_engine.h
        vk_initialisers.h
        player/Player.h
        RenderUtils/RenderBlock.h
        vk_textures.h

        world/World.h

        DebugUtils/ImGuiHandler.h

        Entity/Entity.h
        PerlinNoise.hpp)

add_executable(VulkanTest ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(VulkanTest ${SDL2_LIBRARIES} ImGui)