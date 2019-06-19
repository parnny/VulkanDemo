#include "GlobalInclude.h"


class Allocator
{
public:
	inline operator VkAllocationCallbacks() const
	{
		VkAllocationCallbacks result;
		result.pUserData = (void*)this;
		result.pfnAllocation = &Allocator::Allocation;
		result.pfnReallocation = &Allocator::Reallcation;
		result.pfnFree = &Allocator::Free;
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
