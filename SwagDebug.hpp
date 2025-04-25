#ifndef SWAGDEBUG_H
#define SWAGDEBUG_H

#include <vulkan/vulkan.h>
#include <iostream>

#ifndef NDEBUG
void printDebugSection(const char* sectionName, bool sub = false);
#endif // !NDEBUG


void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
void setupDebugMessenger(VkDebugUtilsMessengerEXT* messenger, bool enableLayers, VkInstance instance);

VkResult createDebugMessenger(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger
);

void destroyDebugMessenger(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator
);

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
);

#endif // !SWAGDEBUG_H
