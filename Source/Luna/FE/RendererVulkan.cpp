#include "../../../Include/Luna/FE/RendererVulkan.hpp"

using namespace Luna::FE;

#define LUNA_CHECK_VULKAN_ERROR(statement) \
  if(statement != VK_SUCCESS) \
    throw "Failed to call: " #statement ""


//#define LUNA_LOG_ERROR(msg)   std::cout << "ERROR - " << msg << std::endl
#define LUNA_FATAL_ERROR(msg)  std::cerr << "FATAL - " << msg << std::endl
//#define LUNA_LOG_INFO(msg)    std::cout << "INFO - " << msg << std::endl

const std::string RendererVulkan::kValidationLayerPropName =
        "VK_LAYER_KHRONOS_validation";

/******************************************************************************/
RendererVulkan::RendererVulkan(const std::string & name, bool enValLayer) :
  fInstance(nullptr), fPhysicalDevice(nullptr)
{
  /* Create the Vulkan instance. */
  createInstance(enValLayer);

  /* Select the physical device to use. */
  selectPhysicalDevice(name);

  /* Create the logical device to use. */
  createLogicalDevice();
}

/******************************************************************************/
RendererVulkan::~RendererVulkan()
{
  /* Destroy the device. */
  if(fPhysicalDevice)


  /* Destroy the Vulkan Instance. */
  if(fInstance)
  {
    vkDestroyInstance(fInstance, nullptr);
  }
}

/******************************************************************************/
VKAPI_ATTR VkBool32 VKAPI_CALL RendererVulkan::debugCallback(
         VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
         VkDebugUtilsMessageTypeFlagsEXT messageType,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
         void* pUserData)
{
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

/******************************************************************************/
bool RendererVulkan::supportValidationLayers()
{
  /* Get the number of global layer properties. */
  uint32_t count;
  LUNA_CHECK_VULKAN_ERROR(vkEnumerateInstanceLayerProperties(&count, nullptr));

  /* Read the layer properties. */
  std::vector<VkLayerProperties> availableLayers(count);
  LUNA_CHECK_VULKAN_ERROR(vkEnumerateInstanceLayerProperties(&count,
                          availableLayers.data()));

  /* Iterate through all the properties and see if the validation layer is
   * supported. */
  for (const auto& layerProperties : availableLayers)
  {
    if (kValidationLayerPropName == layerProperties.layerName)
      return true;
  }
  return false;
}

/******************************************************************************/
void RendererVulkan::createInstance(bool enValLayer)
{
  /* Check if a validation layer is required and supported. It is annoying if
   * validation layers are not supported, but not fatal, so we are not going
   * to throw and exception. */
  if(enValLayer && !supportValidationLayers())
  {
    LUNA_LOG_ERROR("Validation layer not supported, ignoring request.");
    enValLayer = false;
  }

  /* Setup the application information. */
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Luna Front End";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  /* Setup the instance creation information. */
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  /* No extension to load at this time. */
  createInfo.enabledExtensionCount = 0;

  /* Setup the debug interface. */
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
  if(enValLayer)
  {
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames =
      reinterpret_cast<const char * const *>(kValidationLayerPropName.c_str());

    debugCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    debugCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debugCreateInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debugCreateInfo.pfnUserCallback = debugCallback;

    createInfo.pNext =
        static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
  }
  else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  /* Create the Vulkan Instance. */
  LUNA_CHECK_VULKAN_ERROR(vkCreateInstance(&createInfo, nullptr, &fInstance));
}

/******************************************************************************/
std::vector<std::string> RendererVulkan::getExtensions()
{
  /* The number of extensions. */
  uint32_t count = 0;

  /* Get the number of extensions. */
  LUNA_CHECK_VULKAN_ERROR(vkEnumerateInstanceExtensionProperties(nullptr,
                          &count, nullptr));

  /* Allocate memory to store the extensions. */
  std::vector<VkExtensionProperties> extensions(count);

  /* Read the extensions. */
  LUNA_CHECK_VULKAN_ERROR(vkEnumerateInstanceExtensionProperties(nullptr,
                          &count, extensions.data()));

  /* The list of extension strings. */
  std::vector<std::string> extnesionStrs;

  /* Build the extensions list. */
  for(auto & extension : extensions)
  {
    /* Just copy the name / string of the extension. */
    extnesionStrs.push_back(extension.extensionName);
  }

  /* Return just the extension strings. */
  return extnesionStrs;
}

/******************************************************************************/
void RendererVulkan::selectPhysicalDevice(const std::string & name)
{
  /* Get the number of physical devices. */
  uint32_t count = 0;
  vkEnumeratePhysicalDevices(fInstance, &count, nullptr);

  /* Check if any GPUs with Vulkan support is found. */
  if (count == 0)
  {
    LUNA_FATAL_ERROR("No GPU with Vulkan support found.");
  }

  /* Get the list of devices. */
  std::vector<VkPhysicalDevice> devices(count);
  vkEnumeratePhysicalDevices(fInstance, &count, devices.data());

  /* The preffered device. */
  VkPhysicalDevice prefDevice = nullptr;

  /* Print the names of all the detected GPUs. */
  for(const auto & device : devices)
  {
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(device, &properties);

    LUNA_LOG_INFO(" Detected GPU: " << properties.deviceName);

    /* Check if the device match the specified one. */
    if(name == properties.deviceName)
    {
      prefDevice = device;
    }
  }

  /* Check if the selected device was found. It is not fatal if the preffered
   * device was not found. */
  if(!name.empty() && prefDevice == nullptr)
  {
    LUNA_LOG_ERROR("Failed to find the specified GPU: " << name);
  }

  /* For now, select the first physical device until we write a selection
   * criteria. */
  fPhysicalDevice = devices[0];
}

/******************************************************************************/
void RendererVulkan::createLogicalDevice()
{

}
