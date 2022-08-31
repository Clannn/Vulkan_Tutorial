#pragma once
#include <iostream>

#ifdef ASSERTIONS_ENABLED
	#define ASSERT(expr) \
		if(expr) { } \
		else{ \
			std::cerr<<"Assertion Error: "<<#expr<<'\t'<<__FILE__<<'\t'<<__LINE__<<std::endl; \
			__debugbreak(); \
		}
#else
	#define ASSERT(expr)
#endif // ASSERTIONS_ENABLED
