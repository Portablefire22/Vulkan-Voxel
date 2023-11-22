cmake_minimum_required(VERSION 3.27)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")


find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

link_libraries(glfw)
link_libraries(vulkan)
add_executable(VulkanTest main.cpp)

