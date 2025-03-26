#ifndef SWAGKANT_H
#define SWAGKANT_H
//#define NDEBUG

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <optional>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>

#include "SwagDebug.hpp"

const uint16_t WIDTH = 800;
const uint16_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifndef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

/// <summary>
/// Les main app B)
/// </summary>
class SwagkantApp {
public:
	void run(const char* title);

private:
	GLFWwindow* window = nullptr;
	VkInstance instance = nullptr;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkDebugUtilsMessengerEXT debugMessenger;

	void initWindow(const char* title);
	void initVulkan();
	void mainLoop();
	void cleanup();

	void createInstance();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();

	void pickPhysicalDevice();
	uint32_t ratePhysicalDevice(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void createLogicalDevice();
	void createSurface();
};

#endif // !SWAGKANT_H
