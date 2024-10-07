//
// Created by blakey on 27/11/23.
//

#ifndef VK_INITIALISERS_H
#define VK_INITIALISERS_H

#include <vector>

#include "vk_types.h"

namespace  vkInit {
    VkCommandPoolCreateInfo commandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

    VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags = 0);
    VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags = 0);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo();
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo(VkPrimitiveTopology topology);
    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VkPolygonMode polygonMode);
    VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo();
    VkPipelineColorBlendAttachmentState colourBlendAttachmentState();
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo();

    VkImageCreateInfo imageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo imageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

    VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
    VkRenderPassBeginInfo renderPassBeginInfo(VkRenderPass renderPass, VkExtent2D windowExtent, VkFramebuffer framebuffers);


    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding);
    VkWriteDescriptorSet writeDescriptorBuffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding);

    VkCommandBufferBeginInfo commandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
    VkSubmitInfo submitInfo(VkCommandBuffer* cmd);

    VkSamplerCreateInfo samplerCreateInfo(VkFilter filters, VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
    VkWriteDescriptorSet writeDescriptorImage(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorImageInfo* imageInfo, uint32_t binding);

};



#endif //VK_INITIALISERS_H
