#pragma once

#include "Renderer.hpp"
#include <memory>

namespace Luna::FE
{
  class RendererVulkan : public Renderer
  {
    /** The name of the validation layer. This new validation layer replaces the
     * five legacy validation layers. */
    static const std::string kValidationLayerPropName;

    /** The vulkan instance. */
    VkInstance fInstance;

    /** The physical device used for rendering. */
    VkPhysicalDevice fPhysicalDevice;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    bool supportValidationLayers();

    void createInstance(bool enValLayer);
    void selectPhysicalDevice(const std::string & name);
    void createLogicalDevice();




  public:
    RendererVulkan(const std::string & name = "", bool enValLayer = false);

    virtual ~RendererVulkan() override;

    static std::vector<std::string> getExtensions();
  };
}
