cmake_minimum_required(VERSION 3.27)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(source "${CMAKE_SOURCE_DIR}")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

link_libraries(vulkan)

set(PROJECT_SOURCES main.cpp world/chunk.cpp block/Block.cpp
        vk_mesh.cpp
        vk_engine.cpp
        VkBootstrap.cpp)
set(PROJECT_HEADERS main.h stb_image.h Camera.h world/chunk.h block/Block.h
        vk_types.h
        vk_mesh.h
        VkBootstrap.h
        VkBootstrapDispatch.h
        vk_engine.h
        vk_initialisers.cpp
        vk_initialisers.h
        player/Player.cpp
        player/Player.h
        RenderUtils/RenderBlock.cpp
        RenderUtils/RenderBlock.h
        vk_textures.cpp
        vk_textures.h)

add_executable(VulkanTest ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(VulkanTest ${SDL2_LIBRARIES})