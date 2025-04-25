#include "SwagDebug.hpp"

#ifndef NDEBUG
void printDebugSection(const char* sectionName, bool sub) {
	if (sub) {
		std::cout << "\n -------------------------------\n\t" << sectionName << "\n -------------------------------\n";
	}
	else {
		std::cout << "\n\n ========================================\n\t" << sectionName << "\n ========================================\n";
	}
}
#endif // !NDEBUG

/// <summary>
/// Populates a debug messengers create info with severity, type and the debug callback
/// </summary>
/// <param name="createInfo">The debug messengers create info</param>
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

/// <summary>
/// Setup a debug messenger
/// </summary>
void setupDebugMessenger(VkDebugUtilsMessengerEXT* messenger, bool enableLayers, VkInstance instance) {
	if (!enableLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (createDebugMessenger(instance, &createInfo, nullptr, messenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to setup debug messenger");
	}
}

/// <summary>
/// Creates a Vulkan debug messenger for handling validation layer callbacks
/// </summary>
/// <param name="instance">The Vulkan instance to associate with the debug messenger</param>
/// <param name="pCreateInfo">Pointer to a structure containing debug callback configuration</param>
/// <param name="pAllocator">Pointer to memory allocation callbacks</param>
/// <param name="pDebugMessenger">Output parameter for the created debug messenger handle</param>
/// <returns>
/// VK_SUCCESS if successful,
/// VK_ERROR_EXTENSION_NOT_PRESENT if debug utils extension is unavailable,
/// Other Vulkan error codes if creation fails
/// </return>
VkResult createDebugMessenger(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger
) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

/// <summary>
/// Destroy a given debug messenger
/// </summary>
/// <param name="instance">the instance associated with the debug messenger</param>
/// <param name="debugMessenger">The debug messenger to destroy</param>
/// <param name="pAllocator">Pointer to memory allocation callbacks</param>
void destroyDebugMessenger(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator
) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

/// <summary>
/// Callback function for Vulkan validation layer messages
/// </summary>
/// <param name="messageSeverity">Bitmask specifying severity of the message (verbose, info, warning, error)</param>
/// <param name="messageType">Bitmask specifying type of message (general, validation, performance)</param>
/// <param name="pCallbackData">Contains all callback information including the message itself</param>
/// <param name="pUserData">Optional user-supplied data pointer (unused in this implementation)</param>
/// <returns>
/// Always returns VK_FALSE to indicate the Vulkan call should not be aborted
/// </returns>
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	std::cerr << "Validation layer: " << pCallbackData->pMessage << "\n";
	return VK_FALSE;
}