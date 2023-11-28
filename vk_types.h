//
// Created by blakey on 27/11/23.
//
#pragma once

#ifndef VK_TYPES_H
#define VK_TYPES_H
#include <vk_mem_alloc.h>

struct AllocatedBuffer {
    VkBuffer _buffer;
    VmaAllocation _allocation;
};

struct AllocatedImage {
    VkImage _image;
    VmaAllocation _allocation;
};
#endif //VK_TYPES_H
