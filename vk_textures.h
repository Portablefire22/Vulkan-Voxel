//
// Created by blakey on 01/12/23.
//

#ifndef VK_TEXTURES_H
#define VK_TEXTURES_H
#include "vk_engine.h"

namespace vkUtil {

    bool loadImageFromFile(VulkanEngine& engine, const char* file, AllocatedImage& outImage);

} // vkUtil

#endif //VK_TEXTURES_H
