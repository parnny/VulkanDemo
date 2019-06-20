#pragma once
#include "GlobalInclude.h"
#include <list>
#include <vector>

using namespace std;

class VkApp
{
public:
    VkApp();
    ~VkApp();

   VkResult Init();
   void Destroy();

   void GetPhysicalDeviceinfo();
   void GetLayerInfo();
   void GetExtensionInfo();

   VkResult CreateLogicDevice();

private:
    VkInstance* mVkInstance;
	vector<VkPhysicalDevice> mVkPhysicalDeviceList;
	vector<VkDevice> mVkLogicDeviceList;
	vector<VkQueueFamilyProperties> mVkQueueFamilyPropertiesList;
	vector<VkLayerProperties> mVkLayerPropertiesList;
	vector<VkExtensionProperties> mVkExtensionPropertiesList;
};

