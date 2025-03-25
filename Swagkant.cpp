#include "Swagkant.hpp"

/// <summary>
/// Inits the window & vulkan, starts the main loop and finishes by cleaning up everything
/// </summary>
/// <param name="title">The window title</param>
void SwagkantApp::run(const char* title) {
	initWindow(title);
	initVulkan();
	mainLoop();
	cleanup();
}

/// <summary>
/// Creates the GLFW window
/// </summary>
void SwagkantApp::initWindow(const char* title) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
}

/// <summary>
/// Sets up vulkan by creating instances, seting up the debug messenger...
/// </summary>
void SwagkantApp::initVulkan() {
	createInstance();
	setupDebugMessenger(&debugMessenger, enableValidationLayers, instance);
	pickPhysicalDevice();
	createLogicalDevice();
}

/// <summary>
/// The programs main loop containing a while loop in which it polls events
/// </summary>
void SwagkantApp::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void SwagkantApp::cleanup() {
	if (enableValidationLayers) {
		destroyDebugMessenger(instance, debugMessenger, nullptr);
	}

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}

/// <summary>
/// Creates an instance which works as the connection between the application and the Vulkan library
/// </summary>
void SwagkantApp::createInstance() {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available");
	}

	// Creates the application info (name, versions, engine etc.)
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Godav swagkant";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Keine motor";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	/* Creates the createInfo variable which will hold the appInfo created above,
	* how many, along with which extensions and layers are enabled
	*/
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	auto extensions = getRequiredExtensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		// Populates the debug info, and binds it to the createInfo's instance creation process
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	// Actually creates the instance using the createInfo variable
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance!");
	}
}

/// <summary>
/// Gets the required extensions
/// </summary>
/// <returns>A cstring vector of the required extensions</returns>
std::vector<const char*> SwagkantApp::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (enableValidationLayers) { extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }

#ifndef NDEBUG
	std::cout << "Extension count: " << glfwExtensionCount << "\n";
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		std::cout << "Extension: " << glfwExtensions[i] << "\n";
	}
#endif // !NDEBUG

	return extensions;
}

/// <summary>
/// Check whether the given validation layers (see hpp file) are supported
/// </summary>
/// <returns>Whether the validation layers are supported</returns>
bool SwagkantApp::checkValidationLayerSupport() {
	// Gets the TOTAL layer count
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Gets all AVAILABLE layers
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Enumerates through the available layers and check if the wanted ones are in there
	for (const char* layerName : validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) { return false; }
	}
	return true;
}

void SwagkantApp::pickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	std::multimap<int, VkPhysicalDevice> candidates; // automatically sorts based on score
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		int score = ratePhysicalDevice(device);
		candidates.insert(std::make_pair(score, device));
	}

	// Check if the best candidate is suitable at all
	if (candidates.rbegin()->first > 0) {
		VkPhysicalDevice _device = candidates.rbegin()->second;
		physicalDevice = isDeviceSuitable(_device) ? _device : VK_NULL_HANDLE;
	}
	else {
		throw std::runtime_error("Failed to find a suitable GPU!");
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

uint32_t SwagkantApp::ratePhysicalDevice(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int score = 0;
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
		score += 1000; // Discrete GPUs have a significant performance advantage
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;
	if (!deviceFeatures.geometryShader) {
		return 0; // Application can't function without geometry shaders
	}

#ifndef NDEBUG
	std::cout << deviceProperties.deviceName << " >> " << score << '\n';
#endif

	return score;
}

bool SwagkantApp::isDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = findQueueFamilies(device);
	return indices.isComplete();
}

QueueFamilyIndices SwagkantApp::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

void SwagkantApp::createLogicalDevice() {

}