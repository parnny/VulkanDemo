#include "GlobalInclude.h"


class VDAllocator
{
public:
	inline operator VkAllocationCallbacks() const
	{
		VkAllocationCallbacks result;
		result.pUserData = (void*)this;
		result.pfnAllocation = &VDAllocator::Allocation;
		result.pfnReallocation = &VDAllocator::Reallcation;
		result.pfnFree = &VDAllocator::Free;
		result.pfnInternalAllocation = nullptr;
		result.pfnInternalFree = nullptr;
		return result;
	};
private:
	static void* VKAPI_CALL Allocation(
		void* pUserData,
		size_t size, 
		size_t alignment, 
		VkSystemAllocationScope scope);

	static void* VKAPI_CALL Reallcation(
		void* pUserData,
		void* pOriginal,
		size_t size,
		size_t alignment,
		VkSystemAllocationScope scope);

	static void VKAPI_CALL Free(void* pUserData, void* pMemory);

private:
	void* Allocation(size_t size,size_t alignment);

	void* Reallcation(
		void* pOriginal,
		size_t size,
		size_t alignment);

	void Free(void* pMemory);
};
