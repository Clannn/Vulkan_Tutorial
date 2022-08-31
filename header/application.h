#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Clan
{
	void DestroyDebugUtilsMessengerEXT(VkInstance instance,
									   VkDebugUtilsMessengerEXT debugMessenger,
									   const VkAllocationCallbacks* pAllocator);

	class HelloTriangleApplication {
	public:
		void run();

	private:
		struct QueueFamilyIndices;

		void initWindow();

		void initVulkan();

		void mainLoop();

		void cleanup();

		void createInstance();

		bool checkValidationLayerSupport();

		std::vector<const char*> getRequiredExtensions();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		void setupDebugMessenger();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void pickPhysicalDevice();

		bool isDeviceSuitable(const VkPhysicalDevice& physicalDevice);

		QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& physicalDevice);

		void createLogicalDevice();

		void createSurface();

		bool checkDeviceExtensions(const VkPhysicalDevice& physicalDevice);

	private:
		static constexpr uint32_t WINDOW_WIDTH = 800;
		static constexpr uint32_t WINDOW_HEIGHT = 600;
#ifdef NDEBUG
		static constexpr bool enableValidationLayers = false;
#else
		static constexpr bool enableValidationLayers = true;
#endif // NDEBUG
		static const std::vector<const char*> validationLayers;
		static const std::vector<const char*> deviceExtensions;

		GLFWwindow* window;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSurfaceKHR surface;
	};
}