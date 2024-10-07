cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)
project(VulkanTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(source "${CMAKE_SOURCE_DIR}")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

#find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
add_subdirectory(deps/vk-bootstrap)

include_directories(/Users/blakey/VulkanSDK/1.3.290.0/macOS/include)
add_subdirectory(deps/vk-bootstrap)

set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build Noise Tool" FORCE)
add_subdirectory(deps/FastNoise2)
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/deps/imgui")

find_package(Vulkan COMPONENTS MoltenVK)

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES})


set(PROJECT_SOURCES main.cpp world/Chunk.cpp block/Block.cpp
        vulkan/vk_mesh.cpp
        vulkan/vk_engine.cpp
        vulkan/vk_initialisers.cpp
        Entity/Entity.cpp
        player/Player.cpp
        RenderUtils/RenderBlock.cpp
        DebugUtils/ImGuiHandler.cpp
        threadpool/ThreadPool.cpp
        threadpool/QueueSafe.cpp
        threadpool/ChunkPool.cpp
        threadpool/MapSafe.cpp
        world/World.cpp
        vulkan/vk_textures.cpp)

set(PROJECT_HEADERS main.h stb_image.h Camera.h world/Chunk.h block/Block.h
        vulkan/vk_types.h
        vulkan/vk_mesh.h
        vulkan/vk_engine.h
        vulkan/vk_initialisers.h
        player/Player.h
        RenderUtils/RenderBlock.h
        vulkan/vk_textures.h

        world/World.h

        DebugUtils/ImGuiHandler.h

        threadpool/ThreadPool.hpp
        threadpool/QueueSafe.hpp
        threadpool/ChunkPool.hpp
        threadpool/MapSafe.hpp

        Entity/Entity.h
        world/Region.cpp
        world/Region.h
        world/Region.cpp)

add_executable(VulkanTest ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(VulkanTest ${SDL2_LIBRARIES} ImGui  FastNoise)
target_link_libraries(VulkanTest vk-bootstrap::vk-bootstrap)
target_link_libraries(VulkanTest Vulkan::Vulkan)
