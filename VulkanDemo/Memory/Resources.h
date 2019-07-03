
#include "GlobalInclude.h"

class VDResources
{
public:
	VDResources(VkPhysicalDevice* physical, VkDevice* logic)
		: mPhysicalDevice(physical)
		, mLogicDevice(logic)
	{
		mFormatProperties = new VkFormatProperties();
	}

	void Init();
	void BufferOperation();
	void ImageOperation();
	void FormatAndSupportInfo();

private:
	VkDevice* mLogicDevice;
	VkPhysicalDevice* mPhysicalDevice;
	VkFormatProperties* mFormatProperties;
};
