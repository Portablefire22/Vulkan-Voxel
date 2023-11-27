//
// Created by blakey on 27/11/23.
//
#pragma once
#ifndef VK_MESH_H
#define VK_MESH_H
#include "vk_types.h"
#include <vector>
#include <glm/vec3.hpp>

struct VertexInputDescription {
    std::vector<VkVertexInputBindingDescription> binding;
    std::vector<VkVertexInputAttributeDescription> attributes;

    VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 colour;
    glm::vec3 normal;

    static VertexInputDescription getVertexDescription();
};

struct Mesh {
    std::vector<Vertex> _vertices;
    AllocatedBuffer _vertexBuffer;
};
#endif //VK_MESH_H
