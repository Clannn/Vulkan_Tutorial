#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <string>

namespace Clan
{
	struct Vertex;
	struct UniformBufferObject;

	void DestroyDebugUtilsMessengerEXT(VkInstance instance,
									   VkDebugUtilsMessengerEXT debugMessenger,
									   const VkAllocationCallbacks* pAllocator);

	class HelloTriangleApplication {
	public:
		HelloTriangleApplication() = default;
		HelloTriangleApplication(const HelloTriangleApplication&) = delete;
		HelloTriangleApplication& operator=(const HelloTriangleApplication&) = delete;
		void run();
		~HelloTriangleApplication() = default;

	private:
		struct QueueFamilyIndices;
		struct SwapChainSupportDetails;

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);


		static std::vector<char> readBinaryFile(const std::string& filename);

		void initWindow();

		void initVulkan();

		void mainLoop();

		void cleanup();

		void createInstance();

		bool checkValidationLayerSupport();

		std::vector<const char*> getRequiredExtensions();

		void setupDebugMessenger();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void pickPhysicalDevice();

		bool isDeviceSuitable(const VkPhysicalDevice& physicalDevice);

		QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& physicalDevice);

		void createLogicalDevice();

		void createSurface();

		bool checkDeviceExtensions(const VkPhysicalDevice& physicalDevice);

		SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& physicalDevice);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void createSwapChain();

		void createImageViews();

		void createGraphicsPipeline();

		VkShaderModule createShaderModule(const std::vector<char>& bytecode);
		
		void createRenderPass();

		void createFramebuffers();

		void createCommandPool();

		void createCommandBuffers();

		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		void drawFrame();

		void createSyncObjects();

		void recreateSwapChain();

		void cleanupSwapChain();

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		void createVertIDBuffer();

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	private:
		static constexpr uint32_t WINDOW_WIDTH = 800;
		static constexpr uint32_t WINDOW_HEIGHT = 600;
		static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
#ifdef NDEBUG
		static constexpr bool enableValidationLayers = false;
#else
		static constexpr bool enableValidationLayers = true;
#endif // NDEBUG
		static const std::vector<const char*> validationLayers;
		static const std::vector<const char*> deviceExtensions;

		GLFWwindow* window{};

		VkInstance instance{};
		VkDebugUtilsMessengerEXT debugMessenger{};
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device{};
		VkQueue graphicsQueue{};
		VkQueue presentQueue{};
		VkSurfaceKHR surface{};
		VkSwapchainKHR swapChain{};
		std::vector<VkImage> swapChainImages{};
		VkFormat swapChainImageFormat{};
		VkExtent2D swapChainExtent{};
		std::vector<VkImageView> swapChainImageViews{};
		VkRenderPass renderPass{};
		VkPipelineLayout pipelineLayout{};
		VkPipeline graphicsPipeline{};
		std::vector<VkFramebuffer> swapChainFramebuffers{};
		VkCommandPool commandPool{};
		std::vector<VkCommandBuffer> commandBuffers{};
		std::vector<VkSemaphore> imageAvailableSemaphores{};
		std::vector<VkSemaphore> renderFinishedSemaphores{};
		std::vector<VkFence> inFlightFences{};
		uint32_t currentFrame{0};
		VkBuffer VertIDBuffer;
		VkDeviceMemory VertIDBufferMemory;
	};
}