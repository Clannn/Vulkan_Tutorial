#pragma once
#include <cstdint>
#include "macro.h"
namespace Clan 
{
	class StackAllocator
	{
	public:

		//Constructs a stack allocator with the given total size.
		explicit StackAllocator(uint32_t stackSize_bytes);

		StackAllocator(const StackAllocator&) = delete;

		StackAllocator& operator=(const StackAllocator&) = delete;

		~StackAllocator();

		//Allocates a new block of the given size from stack top.
		inline void* alloc(uint32_t size_bytes);

		//Aligned allocation function. 'align' must be a power of 2
		inline void* allocAligned(uint32_t bytes, uint8_t align);

		//Rolls the stack back to a specified pointer
		inline void free(void* pointer);

		//Rolls the stack back to a aligned pointer
		inline void freeAligned(void* pointer);

		//Clears the entire stack
		inline void clear();

		inline uint32_t getSize() { return static_cast<uint32_t>(m_pTop - m_pBottom); }

		inline uint32_t getCapability() { return static_cast<uint32_t>(m_pCapability - m_pBottom); }

	private:
		//Shift the given address and ensure it is aligned to the given 'align'
		inline uint64_t alignAddress(uint64_t addr, uint8_t align);

		//Shift the given pointer and ensure it is aligned to the given number of bytes
		inline uint8_t* alignPointer(uint8_t* ptr, uint8_t align);

	private:
		uint8_t* m_pBottom{ nullptr };
		uint8_t* m_pTop{ nullptr };
		uint8_t* m_pCapability{ nullptr };
	};

	inline void* StackAllocator::alloc(uint32_t size_bytes)
	{
		ASSERT(m_pCapability - m_pTop >= size_bytes);
		void* res = reinterpret_cast<void*>(m_pTop);
		m_pTop += size_bytes;
		return res;
	}

	inline void* StackAllocator::allocAligned(uint32_t bytes, uint8_t align)
	{
		uint32_t actualBytes = bytes + align;
		uint8_t* pRawMem = reinterpret_cast<uint8_t*>(alloc(actualBytes));
		uint8_t* pAlignedMem = alignPointer(pRawMem, align);
		if (pAlignedMem == pRawMem)
			pAlignedMem += align;
		int64_t shift = pAlignedMem - pRawMem;
		ASSERT(0 < shift && shift <= 256);
		pAlignedMem[-1] = static_cast<uint8_t>(shift & 0xFF);
		return reinterpret_cast<void*>(pAlignedMem);
	}

	inline void StackAllocator::free(void* pointer)
	{
		uint8_t* pMarker = reinterpret_cast<uint8_t*>(pointer);
		ASSERT(pMarker <= m_pTop && m_pTop < m_pCapability);
		m_pTop = pMarker;
	}

	inline void StackAllocator::freeAligned(void* pointer)
	{
		if (pointer)
		{
			uint8_t* pAlignedMem = reinterpret_cast<uint8_t*>(pointer);
			int64_t shift = pAlignedMem[-1];
			uint8_t* pRawMem = pAlignedMem - shift;
			delete[] pRawMem;
		}
	}

	inline void StackAllocator::clear()
	{
		m_pTop = m_pBottom;
	}

	inline uint64_t StackAllocator::alignAddress(uint64_t addr, uint8_t align)
	{
		const uint64_t mask = align - 1;
		ASSERT((align & mask) == 0);
		return (addr + mask) & ~mask;
	}

	inline uint8_t* StackAllocator::alignPointer(uint8_t* ptr, uint8_t align)
	{
		const uint64_t addr = reinterpret_cast<uint64_t>(ptr);
		const uint64_t addrAligned = alignAddress(addr, align);
		return reinterpret_cast<uint8_t*>(addrAligned);
	}
}
