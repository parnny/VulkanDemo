#include "VkApp.h"

VkApp::VkApp()
{
    mVkInstance = new VkInstance();
    mVkPhysicalDeviceList.empty();
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

    result = vkCreateInstance(&inst_create_info,nullptr,mVkInstance);

    if (result == VK_SUCCESS)
    {
        unsigned int physical_device_count = 0;
        result = vkEnumeratePhysicalDevices(*mVkInstance,&physical_device_count,nullptr);

        if (result == VK_SUCCESS)
        {
            mVkPhysicalDeviceList.resize(physical_device_count);
            result = vkEnumeratePhysicalDevices(*mVkInstance, &physical_device_count, &mVkPhysicalDeviceList.front());
        }


        VkPhysicalDeviceProperties physical_device_properties;
        vkGetPhysicalDeviceProperties(mVkPhysicalDeviceList.front(),&physical_device_properties);


        VkPhysicalDeviceFeatures physical_device_features;
        vkGetPhysicalDeviceFeatures(mVkPhysicalDeviceList.front(),&physical_device_features);

        return result;
    }

    return result;
}
