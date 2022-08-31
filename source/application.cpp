#include "application.h"
#include "macro.h"

#include <iostream>
#include <optional>
#include <unordered_set>
#include <string>

namespace Clan
{
	struct HelloTriangleApplication::QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() &&
				   presentFamily.has_value();
		}
	};

	const std::vector<const char*> HelloTriangleApplication::validationLayers = {
		"VK_LAYER_KHRONOS_validation",
	};
	const std::vector<const char*> HelloTriangleApplication::deviceExtensions = {
		"VK_KHR_swapchain",
	};

	void HelloTriangleApplication::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	void HelloTriangleApplication::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void HelloTriangleApplication::initVulkan() {
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
	}

	void HelloTriangleApplication::mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void HelloTriangleApplication::cleanup() {
		vkDestroyDevice(device, nullptr);
		vkDestroySurfaceKHR(instance, surface, nullptr);
		if (enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}


	void HelloTriangleApplication::createInstance() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
		for (const auto& property : extensionProperties) {
			std::cout << property.extensionName << '\n';
		}

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		//specify the desired global extensions
		std::vector<const char*> extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = (uint32_t)extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		//specify the desired validation layers
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			ASSERT(checkValidationLayerSupport());
			createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();
			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		ASSERT(vkCreateInstance(&createInfo, nullptr, &instance) == VK_SUCCESS);
	}
	//-------------------------------------------------------------------------------------------------
	bool HelloTriangleApplication::checkValidationLayerSupport()
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layerProperties(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());
		//check all needed validation layers in it.
		for (auto& layer : validationLayers) {
			bool flag = false;
			for (auto& property : layerProperties) {
				if (strcmp(layer, property.layerName) == 0) {
					flag = true;
					break;
				}
			}
			if (!flag) return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------
	std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
	{
		//glfw extensions
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		//debug callback
		if (enableValidationLayers) {
			extensions.push_back("VK_EXT_debug_utils");
		}
		return extensions;
	}
	//-------------------------------------------------------------------------------------------------
	VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
	//------------------------------------------------------------------------------------------------
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
										  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
										  const VkAllocationCallbacks* pAllocator,
										  VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
	//------------------------------------------------------------------------------------------------
	void DestroyDebugUtilsMessengerEXT(VkInstance instance,
									   VkDebugUtilsMessengerEXT debugMessenger,
									   const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
	//------------------------------------------------------------------------------------------------
	void HelloTriangleApplication::setupDebugMessenger()
	{
		if (!enableValidationLayers) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		populateDebugMessengerCreateInfo(createInfo);

		ASSERT(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) == VK_SUCCESS);
	}
	//------------------------------------------------------------------------------------------------
	void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = this; // Optional
	}
	//-----------------------------------------------------------------------------------------------
	void HelloTriangleApplication::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		ASSERT(deviceCount > 0);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
		}
		ASSERT(physicalDevice != VK_NULL_HANDLE);
	}
	//-----------------------------------------------------------------------------------------------
	bool HelloTriangleApplication::isDeviceSuitable(const VkPhysicalDevice& physicalDevice)
	{
		//VkPhysicalDeviceProperties devicePropertyies;
		//vkGetPhysicalDeviceProperties(physicalDevice, &devicePropertyies);
		//VkPhysicalDeviceFeatures deviceFeatures;
		//vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		return indices.isComplete() && checkDeviceExtensions(physicalDevice);
	}
	//-----------------------------------------------------------------------------------------------
	HelloTriangleApplication::QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(const VkPhysicalDevice& physicalDevice)
	{
		QueueFamilyIndices indices;
		uint32_t familiesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familiesCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(familiesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familiesCount, queueFamilies.data());
		for (uint32_t i = 0; i < familiesCount; i++) {
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			if (presentSupport) indices.presentFamily = i;
			if (indices.isComplete()) break;
		}
		return indices;
	}
	//-----------------------------------------------------------------------------------------------
	void HelloTriangleApplication::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		std::unordered_set<uint32_t> queueFamilyIndices = {
			indices.graphicsFamily.value(),
			indices.presentFamily.value(),
		};
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(queueFamilyIndices.size());
		float queuePriority = 1.0f;
		for (uint32_t i = 0; uint32_t queueFamily: queueFamilyIndices) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos[i++] = queueCreateInfo;
		}

		//I think I've done this when I pick physical device
		//May TODO: VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		//May TODO: createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) == VK_SUCCESS);

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}
	//-----------------------------------------------------------------------------------------------
	void HelloTriangleApplication::createSurface()
	{
		ASSERT(glfwCreateWindowSurface(instance, window, nullptr, &surface) == VK_SUCCESS);
	}
	//-----------------------------------------------------------------------------------------------
	bool HelloTriangleApplication::checkDeviceExtensions(const VkPhysicalDevice& physicalDevice)
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, supportedExtensions.data());
		std::unordered_set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for (const auto& extention : supportedExtensions) {
			requiredExtensions.erase(extention.extensionName);
		}
		return requiredExtensions.empty();
	}
}