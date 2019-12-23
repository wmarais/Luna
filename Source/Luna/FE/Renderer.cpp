#include "../../../Include/Luna/FE/Renderer.hpp"
using namespace Luna::FE;

/******************************************************************************/
Renderer::Renderer()
{

}

/******************************************************************************/
Renderer::~Renderer()
{

}

///******************************************************************************/
//bool Renderer::hasValidationLayers()
//{
////  /* Get the number of layer properties. */
////  uint32_t layerCount;
////  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

////  /* Read the layer properties. */
////  std::vector<VkLayerProperties> availableLayers(layerCount);
////  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

////  for (const auto& layerProperties : availableLayers)
////  {
////    if (kValidationLayerPropName == layerProperties.layerName)
////      return true;
////  }
//  return false;
//}

///******************************************************************************/
//void Renderer::createInstance()
//{
////  if (fEnableValidationLayer && !hasValidationLayers())
////  {
////    //throw std::runtime_error("validation layers requested, but not available!");
////  }

////  VkApplicationInfo appInfo = {};
////  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
////  appInfo.pApplicationName = "Luna Front End";
////  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
////  appInfo.pEngineName = "No Engine";
////  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
////  appInfo.apiVersion = VK_API_VERSION_1_0;

////  VkInstanceCreateInfo createInfo = {};
////  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
////  createInfo.pApplicationInfo = &appInfo;

//////  auto extensions = getRequiredExtensions();
//////  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//////  createInfo.ppEnabledExtensionNames = extensions.data();

//////VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
//////if (enableValidationLayers) {
//////createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//////createInfo.ppEnabledLayerNames = validationLayers.data();

//////populateDebugMessengerCreateInfo(debugCreateInfo);
//////createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
//////} else {
//////createInfo.enabledLayerCount = 0;

//////createInfo.pNext = nullptr;
//////}

//////if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//////throw std::runtime_error("failed to create instance!");
//////}
//}

///******************************************************************************/
//void Renderer::selectPhysicalDevice()
//{
////  uint32_t deviceCount = 0;
////  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

////  if (deviceCount == 0) {
////      throw std::runtime_error("failed to find GPUs with Vulkan support!");
////  }

////  std::vector<VkPhysicalDevice> devices(deviceCount);
////  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

////  for (const auto& device : devices) {
////      if (isDeviceSuitable(device)) {
////          fPhysicalDevice = device;
////          break;
////      }
////  }

////  if (fPhysicalDevice == VK_NULL_HANDLE) {
////      throw std::runtime_error("failed to find a suitable GPU!");
////  }
//}
