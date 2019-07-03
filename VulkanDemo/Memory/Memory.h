#include "GlobalInclude.h"


class VDMemory
{
public:
	VDMemory(VkPhysicalDevice* physical, VkDevice* logic)
		: mLogicDevice(logic)
		, mPhysicalDevice(physical)
	{
		mDeviceMemory = new VkDeviceMemory();
	}
	void Init();
	void AllocatingDeviceMemory();
	void HostAccess2DeviceMemory(); // CPU∑√Œ œ‘¥Ê

private:

	VkDevice* mLogicDevice;
	VkPhysicalDevice* mPhysicalDevice;
	VkDeviceMemory* mDeviceMemory;
};