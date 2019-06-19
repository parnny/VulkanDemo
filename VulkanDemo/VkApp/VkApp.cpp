#include "VkApp.h"
#include <Memory/Allocator.h>


//#define USE_CUSTOM_ALLOCATOR 0

VkApp::VkApp()
{
    mVkInstance = new VkInstance();
    mVkPhysicalDeviceList.clear();
    mVkQueueFamilyPropertiesList.clear();
    mVkLayerPropertiesList.clear();
    mVkExtensionPropertiesList.clear();
}

VkApp::~VkApp()
{
}

VkResult VkApp::Init()
{
    VkResult result = VK_SUCCESS;

    VkApplicationInfo app_info = {};
    VkInstanceCreateInfo inst_create_info = {};

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Parnny Vulkan App";
    app_info.applicationVersion = 1;
    app_info.apiVersion = VK_MAKE_VERSION(0,0,1);

    inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_create_info.pApplicationInfo = &app_info;

	Allocator allocator;
	VkAllocationCallbacks allocation_callbacks = (VkAllocationCallbacks)allocator;

#ifdef USE_CUSTOM_ALLOCATOR
	result = vkCreateInstance(&inst_create_info, &allocation_callbacks, mVkInstance);
#else
	result = vkCreateInstance(&inst_create_info, nullptr, mVkInstance);
#endif // USE_CUSTOM_ALLOCATOR

	result = vkCreateInstance(&inst_create_info, &allocation_callbacks, mVkInstance);

    if (result != VK_SUCCESS) return result;

    unsigned int physical_device_count = 0;
    result = vkEnumeratePhysicalDevices(*mVkInstance, &physical_device_count, nullptr);
    if (result != VK_SUCCESS) return result;
    if (physical_device_count == 0) return result;

    mVkPhysicalDeviceList.resize(physical_device_count);
    result = vkEnumeratePhysicalDevices(*mVkInstance, &physical_device_count, mVkPhysicalDeviceList.data());
    if (result != VK_SUCCESS) return result;

	GetPhysicalDeviceinfo();
	GetLayerInfo();

	result = CreateLogicDevice();

    return result;
}

void VkApp::Destroy()
{
	vkDestroyInstance(*mVkInstance,nullptr);
	delete mVkInstance;
}

void VkApp::GetPhysicalDeviceinfo()
{
    VkPhysicalDeviceProperties physical_device_properties;
    vkGetPhysicalDeviceProperties(mVkPhysicalDeviceList.front(), &physical_device_properties);

    VkPhysicalDeviceFeatures physical_device_features;
    vkGetPhysicalDeviceFeatures(mVkPhysicalDeviceList.front(), &physical_device_features);

    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
    vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDeviceList.front(), &physical_device_memory_properties);

	unsigned int queue_family_property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDeviceList.front(),&queue_family_property_count,nullptr);
    if (queue_family_property_count > 0)
    {
        mVkQueueFamilyPropertiesList.resize(queue_family_property_count);
        vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDeviceList.front(), &queue_family_property_count, mVkQueueFamilyPropertiesList.data());
    }

    return;
}

void VkApp::GetLayerInfo()
{
	unsigned int layer_property_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_property_count,nullptr);
	if (layer_property_count == 0) return;
	mVkLayerPropertiesList.resize(layer_property_count);
	vkEnumerateInstanceLayerProperties(&layer_property_count,mVkLayerPropertiesList.data());
}

void VkApp::GetExtensionInfo()
{
	unsigned int inst_extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,&inst_extension_count,nullptr);
	if (inst_extension_count == 0) return;
	mVkExtensionPropertiesList.resize(inst_extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &inst_extension_count, mVkExtensionPropertiesList.data());
}

VkResult VkApp::CreateLogicDevice()
{
    VkResult result = VK_SUCCESS;
    VkPhysicalDeviceFeatures support_features;
    VkPhysicalDeviceFeatures require_features = {};

    vkGetPhysicalDeviceFeatures(mVkPhysicalDeviceList.front(),&support_features);

    require_features.multiDrawIndirect = support_features.multiDrawIndirect;
    require_features.tessellationShader = VK_TRUE;      // 细分曲面着色器
    require_features.geometryShader = VK_TRUE;          // 几何着色器

    const VkDeviceQueueCreateInfo device_queue_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // sType
        nullptr,                                    // pNext
		0,                                          // flags
        0,                                          // queueFamilyIndex
        1,                                          // queueCount
        nullptr,                                    // pQueuePriorities
    };
	
	const VkDeviceCreateInfo deveice_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,		// sType
		nullptr,									// pNext
		0,											// flags
		1,											// queueCreateInfoCount
		&device_queue_create_info,					// pQueueCreateInfos
		0,											// enabledLayerCount
		nullptr,									// ppEnabledLayerNames
		0,											// enabledExtensionCount
		nullptr,									// ppEnabledExtensionNames
		&require_features,							// pEnabledFeatures
	};

	VkDevice logic_device;
	result = vkCreateDevice(mVkPhysicalDeviceList.front(),&deveice_create_info,nullptr,&logic_device);
	return result;
}
