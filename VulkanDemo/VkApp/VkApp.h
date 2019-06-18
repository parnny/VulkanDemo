#pragma once
#include "include.h"
#include <list>

using namespace std;

class VkApp
{
public:
    VkApp();
    ~VkApp(){};

   VkResult Init();



private:
    VkInstance* mVkInstance;
    list<VkPhysicalDevice> mVkPhysicalDeviceList;
};

