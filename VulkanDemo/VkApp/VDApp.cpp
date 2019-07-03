#include "VDApp.h"
#include "Memory/Allocator.h"
#include "Memory/Resources.h"
#include "Memory/Memory.h"


//#define USE_CUSTOM_ALLOCATOR 0

VDApp::VDApp()
{
    mVkInstance = new VkInstance();
    mVkPhysicalDeviceList.clear();
    mVkQueueFamilyPropertiesList.clear();
    mVkLayerPropertiesList.clear();
    mVkExtensionPropertiesList.clear();
    mVkLogicDeviceList.clear();
}

VDApp::~VDApp()
{
}

VkResult VDApp::Init()
{
    VkResult result = VK_SUCCESS;

	result = CreateInstance();
    if (result != VK_SUCCESS) return result;

	result = EnumeratePhysicalDevice();
	if (result != VK_SUCCESS) return result;

	GetPhysicalDeviceinfo();
	GetLayerInfo();
	GetExtensionInfo();

	result = CreateLogicDevice();
	CreateResources();
	CreateMemory();

    return result;
}

void VDApp::Destroy()
{
	vkDestroyInstance(*mVkInstance,nullptr);
	delete mVkInstance;
}

void VDApp::GetPhysicalDeviceinfo()
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

void VDApp::GetLayerInfo()
{
	unsigned int layer_property_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_property_count,nullptr);
	if (layer_property_count == 0) return;
	mVkLayerPropertiesList.resize(layer_property_count);
	vkEnumerateInstanceLayerProperties(&layer_property_count,mVkLayerPropertiesList.data());
}

void VDApp::GetExtensionInfo()
{
	unsigned int inst_extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr,&inst_extension_count,nullptr);
	if (inst_extension_count == 0) return;
	mVkExtensionPropertiesList.resize(inst_extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &inst_extension_count, mVkExtensionPropertiesList.data());
	return;
}

VkResult VDApp::CreateInstance()
{
	VkResult result = VK_SUCCESS;
	VkApplicationInfo app_info = {};
	VkInstanceCreateInfo inst_create_info = {};

	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Parnny Vulkan App";
	app_info.applicationVersion = 1;
	app_info.apiVersion = VK_MAKE_VERSION(0, 0, 1);

	inst_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_create_info.pApplicationInfo = &app_info;


#ifdef USE_CUSTOM_ALLOCATOR
	VDAllocator allocator;
	VkAllocationCallbacks allocation_callbacks = (VkAllocationCallbacks)allocator;

	result = vkCreateInstance(&inst_create_info, &allocation_callbacks, mVkInstance);
#else
	result = vkCreateInstance(&inst_create_info, nullptr, mVkInstance);
#endif // USE_CUSTOM_ALLOCATOR

	return result;
}

VkResult VDApp::CreateLogicDevice()
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
	mVkLogicDeviceList.push_back(logic_device);
	return result;
}

VkResult VDApp::EnumeratePhysicalDevice()
{
	VkResult result = VK_SUCCESS;
	unsigned int physical_device_count = 0;
	result = vkEnumeratePhysicalDevices(*mVkInstance, &physical_device_count, nullptr);
	if (result != VK_SUCCESS) return result;
	if (physical_device_count == 0) return result;

	mVkPhysicalDeviceList.resize(physical_device_count);
	result = vkEnumeratePhysicalDevices(*mVkInstance, &physical_device_count, mVkPhysicalDeviceList.data());
	return result;
}

void VDApp::CreateResources()
{
	VDResources* resource = new VDResources(&mVkPhysicalDeviceList.front(), &mVkLogicDeviceList.front());
	resource->Init();
}


void VDApp::CreateMemory()
{
	VDMemory* memory = new VDMemory(&mVkPhysicalDeviceList.front(), &mVkLogicDeviceList.front());
	memory->Init();
}

void VDApp::Test()
{
	VkInstance inst;
	VkResult res = VK_SUCCESS;

	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "app_short_name";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "app_short_name";
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = NULL;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = 0;

	res = vkCreateInstance(&inst_info, NULL, &inst);

	VkDeviceQueueCreateInfo queue_info = {};

	std::vector<const char *> device_extension_names;
	device_extension_names.push_back("TEST");
	std::vector<VkPhysicalDevice> gpus;

	uint32_t gpu_count = 1;
	res = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);
	gpus.resize(gpu_count);
	res = vkEnumeratePhysicalDevices(inst, &gpu_count, gpus.data());

	float queue_priorities[1] = { 0.0 };
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = NULL;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = queue_priorities;
	queue_info.queueFamilyIndex = 0;

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = NULL;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.enabledExtensionCount = (uint32_t)device_extension_names.size();
	device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? device_extension_names.data() : NULL;
	device_info.pEnabledFeatures = NULL;

	VkDevice device;
	res = vkCreateDevice(gpus[0], &device_info, NULL, &device);

	float texels[] = { 1.0, 0.0, 1.0 };

	VkBufferCreateInfo buf_info = {};
	buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf_info.pNext = NULL;
	buf_info.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
	buf_info.size = sizeof(texels);
	buf_info.queueFamilyIndexCount = 0;
	buf_info.pQueueFamilyIndices = NULL;
	buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	buf_info.flags = 0;
	VkBuffer texelBuf;
	res = vkCreateBuffer(device, &buf_info, NULL, &texelBuf);

	VkBufferView texel_view;
	VkBufferViewCreateInfo view_info = {};
	view_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
	view_info.pNext = NULL;
	view_info.buffer = texelBuf;
	view_info.format = VK_FORMAT_R32_SFLOAT;
	view_info.offset = 0;
	view_info.range = sizeof(texels);
	vkCreateBufferView(device, &view_info, NULL, &texel_view);

	return;
}
