#include "Allocator.h"
#include <malloc.h>

void* VKAPI_CALL VDAllocator::Allocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope scope)
{
	return static_cast<VDAllocator*>(pUserData)->Allocation(size,alignment);
}

void* VDAllocator::Allocation(size_t size, size_t alignment)
{
	return _aligned_malloc(size,alignment);
}

void* VKAPI_CALL VDAllocator::Reallcation(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope scope)
{
	return static_cast<VDAllocator*>(pUserData)->Reallcation(pOriginal,size,alignment);
}

void* VDAllocator::Reallcation(void* pOriginal, size_t size, size_t alignment)
{
	return _aligned_realloc(pOriginal,size,alignment);
}


void VKAPI_CALL VDAllocator::Free(void* pUserData, void* pMemory)
{
	return static_cast<VDAllocator*>(pUserData)->Free(pMemory);
}

void VDAllocator::Free(void* pMemory)
{
	return _aligned_free(pMemory);
}
