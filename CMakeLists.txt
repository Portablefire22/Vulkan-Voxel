cmake_minimum_required(VERSION 3.27)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(source "${CMAKE_SOURCE_DIR}")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

# GLSLC command to compile shaders to SPIR-V
find_program(GLSLC glslc)
set(shader_path ${CMAKE_HOME_DIRECTORY}/shaders/)
file(GLOB shaders RELATIVE ${CMAKE_SOURCE_DIR} "${shader_path}*.vert" "${shader_path}*.frag")
foreach(shader ${shaders})
    set(input_glsl "${CMAKE_HOME_DIRECTORY}/${shader}")
    set(output_spv "${input_glsl}.spv")
    execute_process(COMMAND "glslc" "${input_glsl}" "-o" "${output_spv}")
endforeach()

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

link_libraries(glfw)
link_libraries(vulkan)
add_executable(VulkanTest main.cpp)

