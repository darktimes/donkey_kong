#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <array>
#include <set>

#include "math.hpp"
#include "renderer.hpp"

namespace Rendering {


  struct QueueFamilyIndices {
      int graphicsFamily = -1;
      int presentFamily = -1;

      bool isComplete();
  };

  struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
  };

  struct UniformBufferObject {
      Math::mat4 model;
      Math::mat4 view;
      Math::mat4 proj;
  };

  struct Vertex {
      Math::vec2f pos;
      Math::vec3f color;
      Math::vec2f texCoord;

      static VkVertexInputBindingDescription getBindingDescription();

      static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
  };

  class VulkanRenderer: public Renderer {
    public:
      void init() override;
      void draw() override;
      void terminate() override;
      void createWindow(Math::vec2ui size, std::string) override;
      void generateTexture(const std::string&, RawTexture*) override;
      void generateFontChar(unsigned char c, Math::vec2f size,
       Math::vec2f bearing, unsigned advance, std::vector<unsigned char>*) override;
      void drawSprite(SpriteView*) override;
      void drawText(TextView*) override;
      VulkanRenderer();


      VkInstance instance;
      VkDebugReportCallbackEXT callback;
      VkSurfaceKHR surface;

      VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
      VkDevice device;

      VkQueue graphicsQueue;
      VkQueue presentQueue;

      VkSwapchainKHR swapChain;
      std::vector<VkImage> swapChainImages;
      VkFormat swapChainImageFormat;
      VkExtent2D swapChainExtent;
      std::vector<VkImageView> swapChainImageViews;
      std::vector<VkFramebuffer> swapChainFramebuffers;

      VkRenderPass renderPass;
      VkDescriptorSetLayout descriptorSetLayout;
      VkPipelineLayout pipelineLayout;
      VkPipeline graphicsPipeline;

      VkCommandPool commandPool;

      VkImage textureImage;
      VkDeviceMemory textureImageMemory;
      VkImageView textureImageView;
      VkSampler textureSampler;

      VkBuffer vertexBuffer;
      VkDeviceMemory vertexBufferMemory;
      VkBuffer indexBuffer;
      VkDeviceMemory indexBufferMemory;

      VkBuffer uniformBuffer;
      VkDeviceMemory uniformBufferMemory;

      VkDescriptorPool descriptorPool;
      VkDescriptorSet descriptorSet;

      std::vector<VkCommandBuffer> commandBuffers;

      VkSemaphore imageAvailableSemaphore;
      VkSemaphore renderFinishedSemaphore;

      void createInstance();
      void setupDebugCallback();
      void createSurface();
      void pickPhysicalDevice();
      void createLogicalDevice();
      void createSwapChain();
      void createImageViews();
      void createRenderPass();
      void createDescriptorSetLayout();
      void createGraphicsPipeline();
      void createFramebuffers();
      void createCommandPool();
      void createTextureImage();
      void createTextureImageView();
      void createTextureSampler();
      void createVertexBuffer();
      void createIndexBuffer();
      void createUniformBuffer();
      void createDescriptorPool();
      void createDescriptorSet();
      void createCommandBuffers();
      void createSemaphores();

      void updateUniformBuffer();
      void drawFrame();
      void cleanupSwapChain();
      std::vector<const char*> getRequiredExtensions();
      bool checkValidationLayerSupport();
      void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
      void recreateSwapChain();
      bool isDeviceSuitable(VkPhysicalDevice device);
      QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
      VkImageView createImageView(VkImage image, VkFormat format);
      VkShaderModule createShaderModule(const std::vector<char>& code);
      static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
      void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
      void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
      uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
      void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      VkCommandBuffer beginSingleTimeCommands();
      void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  };
};
