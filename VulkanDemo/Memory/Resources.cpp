
#include "Resources.h"
#include <vector>
#include <assert.h>

void VDResources::Init()
{
	FormatAndSupportInfo();
	BufferOperation();
}

void VDResources::BufferOperation()
{
	VkResult result = VK_SUCCESS;

	VkPhysicalDeviceProperties physical_device_properties;
	vkGetPhysicalDeviceProperties(*mPhysicalDevice, &physical_device_properties);

	//VkBufferUsageFlags usage_flag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	VkBufferUsageFlags usage_flag = mFormatProperties->bufferFeatures;
	static const VkBufferCreateInfo buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,		// sType
		nullptr,									// pNext
		0,											// flags
		1024*1024,									// size
		usage_flag,									// usage
		VK_SHARING_MODE_EXCLUSIVE,					// sharingMode
		0,											// queueFamilyIndexCount
		nullptr,									// pQueueFamilyIndices
	};

	VkBuffer buffer = VK_NULL_HANDLE;
	result = vkCreateBuffer(*mLogicDevice, &buffer_create_info, nullptr, &buffer);

	static const VkBufferViewCreateInfo buffer_view_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,		// sType
		nullptr,										// pNext
		0,												// flags
		buffer,											// buffer
		VK_FORMAT_R8G8B8A8_UNORM,						// format
		0,												// offset
		32,												// range
	};

// 	assert(physical_device_properties.limits.maxTexelBufferElements >= (buffer_view_create_info.range*(32)));
// 	VkBufferView buffer_view;
// 	result = vkCreateBufferView(*mLogicDevice,&buffer_view_create_info,nullptr,&buffer_view);

	vkDestroyBuffer(*mLogicDevice,buffer,nullptr);

	return;
}

void VDResources::ImageOperation()
{
	VkImage image = VK_NULL_HANDLE;

	VkExtent3D extent = {1024,1024,1};
	static const VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,			// sType
		nullptr,										// pNext
		0,												// flags
		VK_IMAGE_TYPE_2D,								// imageType
		VK_FORMAT_R8G8B8A8_UNORM,						// format
		extent,											// extent
		10,												// mipLevels
		1,												// arrayLayers
		VK_SAMPLE_COUNT_1_BIT,							// samples
		VK_IMAGE_TILING_OPTIMAL,						// tiling
		VK_IMAGE_USAGE_SAMPLED_BIT,						// usage
		VK_SHARING_MODE_EXCLUSIVE,						// sharingMode
		0,												// queueFamilyIndexCount
		nullptr,										// pQueueFamilyIndices
		VK_IMAGE_LAYOUT_UNDEFINED						// initialLayout
	};

	vkCreateImage(*mLogicDevice,&image_create_info,nullptr,&image);

	vkDestroyImage(*mLogicDevice,image,nullptr);
}

void VDResources::FormatAndSupportInfo()
{
	VkFormat fmt = VK_FORMAT_R8G8B8A8_UNORM;
	vkGetPhysicalDeviceFormatProperties(*mPhysicalDevice,fmt,mFormatProperties);

	VkBufferUsageFlags usage_flag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	VkImageFormatProperties image_fmt_properties;
	vkGetPhysicalDeviceImageFormatProperties(
		*mPhysicalDevice,
		fmt,
		VK_IMAGE_TYPE_2D,
		VK_IMAGE_TILING_OPTIMAL,
		usage_flag,
		VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
		&image_fmt_properties);
	return;
}

