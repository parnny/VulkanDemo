#pragma once
#include "GlobalInclude.h"
#include <list>
#include <vector>

using namespace std;

class VDApp
{
public:
    VDApp();
    ~VDApp();

	void Test();

	VkResult Init();
	void Destroy();

	void GetPhysicalDeviceinfo();
	void GetLayerInfo();
	void GetExtensionInfo();

	VkResult CreateInstance();
	VkResult CreateLogicDevice();

	VkResult EnumeratePhysicalDevice();

private:
	void CreateResources();
	void CreateMemory();


private:
    VkInstance* mVkInstance;
	vector<VkPhysicalDevice> mVkPhysicalDeviceList;
	vector<VkDevice> mVkLogicDeviceList;
	vector<VkQueueFamilyProperties> mVkQueueFamilyPropertiesList;
	vector<VkLayerProperties> mVkLayerPropertiesList;
	vector<VkExtensionProperties> mVkExtensionPropertiesList;
};


struct sample_info {
};