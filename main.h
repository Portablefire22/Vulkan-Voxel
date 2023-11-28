//
// Created by blakey on 26/11/23.
//

#ifndef MAIN_H
#define MAIN_H
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <map>
#include <optional>
#include <vector>
#include <bits/types/siginfo_t.h>
#include <limits>
#include <numeric>
#include <algorithm>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <set>
#include <fstream>
#include <array>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>



#include "Camera.h"
#include "vk_engine.h"
#include "vk_mesh.h"
#include "block/Block.h"

namespace entryPoint {
    inline VulkanEngine engine;
    int main();
}



#endif //MAIN_H
