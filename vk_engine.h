//
// Created by blakey on 27/11/23.
//

#ifndef VK_ENGINE_H
#define VK_ENGINE_H
#include "vk_types.h"
#include "vk_mesh.h"
#define GLFW_INCLUDE_VULKAN
#include <deque>
#include <functional>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

#include "VkBootstrap.h"
#include "player/Player.h"

#include <unordered_map>

class vk_engine {

};

struct Texture {
    AllocatedImage image;
    VkImageView imageView;
};

struct DeletionQueue {
    std::deque<std::function<void()>> deletors;

    void push_function(std::function<void()>&& function) {
        deletors.push_back(function);
    }

    void flush() {
        for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
            (*it)();
        }
        deletors.clear();
    }
};

struct MeshPushConstants {
    glm::vec4 data;
    glm::mat4 renderMatrix;
};

struct Material {
    VkDescriptorSet textureSet{VK_NULL_HANDLE}; // set default to null
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
};

struct RenderObject {
    Mesh* mesh;
    Material* material;
    glm::mat4 transformMatrix;
    char* name;
    glm::vec4 colour;
};

struct FrameData {
    VkSemaphore _presentSemaphore, _renderSemaphore;
    VkFence _renderFence;

    VkCommandPool _commandPool;
    VkCommandBuffer _mainCommandBuffer;

    AllocatedBuffer cameraBuffer;
    VkDescriptorSet globalDescriptor;

    AllocatedBuffer objectBuffer;
    VkDescriptorSet objectDescriptor;
};

struct GPUObjectData {
    glm::mat4 modelMatrix;
    glm::vec4 modelColour;
};

struct GPUSceneData { // For GPU just try to stick to vec4 and mat4 for simplicity
    glm::vec4 fogColour;
    glm::vec4 fogDistances;
    glm::vec4 ambientColor;
    glm::vec4 sunlightDirection;
    glm::vec4 sunlightColour;
};

struct UploadContext {
    VkFence _uploadFence;
    VkCommandPool _commandPool;
    VkCommandBuffer _commandBuffer;
};

constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine {
    public:

    void initImgui();


    VkDescriptorSetLayout _singleTextureSetLayout;

    std::unordered_map<std::string, Texture> _loadedTextures;
    void loadImages();

    UploadContext _uploadContext;
    void immediateSubmit(std::function<void(VkCommandBuffer cmd)> && function);




    VkDescriptorSetLayout _globalSetLayout;
    VkDescriptorSetLayout _objectSetLayout;

    GPUSceneData _sceneParameters;
    AllocatedBuffer _sceneParameterBuffer;
    size_t padUniformBufferSize(size_t originalSize);

    VkPhysicalDeviceProperties _gpuProperties;
    VkPhysicalDeviceFeatures _gpuFeatures;

    VkDescriptorPool _descriptorPool;

    FrameData _frames[FRAME_OVERLAP];
    FrameData& getCurrentFrame();

    std::vector<RenderObject> _renderables;
    std::unordered_map<std::string, Material> _materials;
    std::unordered_map<std::string, Mesh> _meshes;

    Material* createMaterial(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);
    Material* getMaterial(const std::string& name);
    Mesh* getMesh(const std::string& name);

    AllocatedBuffer createBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
    void initDescriptors();

    DeletionQueue _mainDeletionQueue;
    bool _isInitialised{ false };
    int _frameNumber {0};

    bool freeMouse;

    void recreateSwapChain(); // Recreate the swapchain to deal with window resizing


    VkExtent2D _windowExtent{ 1700 , 900 };

    struct SDL_Window* _window{ nullptr };

    //initializes everything in the engine
    void init();

    //shuts down the engine
    void cleanup();

    //draw loop
    void draw();

    //run main loop
    void run();

    VkInstance _instance; // Vulkan library handle
    VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
    VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
    VkDevice _device; // Vulkan device for commands
    VkSurfaceKHR _surface; // Vulkan window surface
    GLFWwindow* window;

    Player::Player PlayerEntity;

    // Swapchain
    VkSwapchainKHR _swapchain;
    VkFormat _swapchainImageFormat;
    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;

    // Commands
    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;

    VkCommandPool _commandPool;
    VkCommandBuffer _mainCommandBuffer;

    // Renderpass
    VkRenderPass _renderPass;

    std::vector<VkFramebuffer> _framebuffers;

    VkSemaphore _presentSemaphore, _renderSemaphore;
    VkFence _renderFence;

    VkPipelineLayout _trianglePipelineLayout;
    VkPipeline _trianglePipeline;

    VmaAllocator _allocator;

    VkPipeline _meshPipeline;
    Mesh _triangleMesh;

    VkPipelineLayout _meshPipelineLayout;

    VkImageView _depthImageView;
    AllocatedImage _depthImage;

    VkFormat _depthFormat;

    double deltaTime;
    Uint64 lastFrameTime;
    Uint64 nowFrameTime = 0;

    void uploadMesh(Mesh& mesh);


    private:

    void initVulkan();
    void initSwapchain();
    void initCommands();
    void initDefaultRenderpass();
    void initFramebuffers();
    void initSyncStructures();
    bool loadShaderModule(const char* filePath, VkShaderModule* outShaderModule);
    void initPipelines();
    void loadMeshes();

    void initScene();
    void drawObjects(VkCommandBuffer cmd, RenderObject* first, int count);
};

class PipelineBuilder {
public:
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkViewport _viewport;
    VkRect2D _scissor;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colourBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;

    VkPipeline buildPipeline(VkDevice device, VkRenderPass pass);

    VkPipelineDepthStencilStateCreateInfo _depthStencil;





};



#endif //VK_ENGINE_H
