#include "Allocator.h"
#include <malloc.h>

void* VKAPI_CALL Allocator::Allocation(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope scope)
{
	return static_cast<Allocator*>(pUserData)->Allocation(size,alignment);
}

void* Allocator::Allocation(size_t size, size_t alignment)
{
	return _aligned_malloc(size,alignment);
}

void* VKAPI_CALL Allocator::Reallcation(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope scope)
{
	return static_cast<Allocator*>(pUserData)->Reallcation(pOriginal,size,alignment);
}

void* Allocator::Reallcation(void* pOriginal, size_t size, size_t alignment)
{
	return _aligned_realloc(pOriginal,size,alignment);
}


void VKAPI_CALL Allocator::Free(void* pUserData, void* pMemory)
{
	return static_cast<Allocator*>(pUserData)->Free(pMemory);
}

void Allocator::Free(void* pMemory)
{
	return _aligned_free(pMemory);
}
