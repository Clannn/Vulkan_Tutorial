#include "StackAllocator.h"

namespace Clan
{
	StackAllocator::StackAllocator(uint32_t stackSize_bytes)
	{
		m_pBottom = new uint8_t[stackSize_bytes];
		m_pTop = m_pBottom;
		m_pCapability = m_pBottom + stackSize_bytes;
	}

	StackAllocator::~StackAllocator()
	{
		delete[] m_pBottom;
		m_pBottom = m_pTop = m_pCapability = nullptr;
	}
}