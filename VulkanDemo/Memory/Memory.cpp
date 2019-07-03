#include "Memory.h"

void VDMemory::Init()
{
	AllocatingDeviceMemory();
}

void VDMemory::AllocatingDeviceMemory()
{
	VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
	vkGetPhysicalDeviceMemoryProperties(*mPhysicalDevice,&physical_device_memory_properties);
	
	assert(physical_device_memory_properties.memoryTypeCount > 1);

	VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.pNext = nullptr;
	memory_allocate_info.memoryTypeIndex = 0;
	memory_allocate_info.allocationSize = 1024*1024;

	VkResult result = vkAllocateMemory(*mLogicDevice,&memory_allocate_info,nullptr,mDeviceMemory);
	if (result == VK_SUCCESS)
	{
		VkDeviceSize device_size;
		vkGetDeviceMemoryCommitment(*mLogicDevice,*mDeviceMemory,&device_size);

		std::cout << "vkGetDeviceMemoryCommitment" << std::endl;

		vkFreeMemory(*mLogicDevice, *mDeviceMemory, nullptr);
	}
}

void VDMemory::HostAccess2DeviceMemory()
{
	void* data;
	VkResult result = vkMapMemory(*mLogicDevice,*mDeviceMemory,0,VK_WHOLE_SIZE,0,&data);
	if (result == VK_SUCCESS)
	{
		VkMappedMemoryRange mapped_memory_range = {};
		mapped_memory_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mapped_memory_range.pNext = nullptr;
		mapped_memory_range.offset = 0;
		mapped_memory_range.size = VK_WHOLE_SIZE;
		mapped_memory_range.memory = *mDeviceMemory;
		vkFlushMappedMemoryRanges(*mLogicDevice,1,&mapped_memory_range);
		vkUnmapMemory(*mLogicDevice,*mDeviceMemory);
	}
}
