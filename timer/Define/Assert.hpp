#ifndef ASSERT_HPP
#define ASSERT_HPP

#include "Defines.hpp"
#include "PrintDefines.hpp"
#include <cassert>


#ifdef DEBUG
	#define ASSERT(test, msg) assert((test) && msg)
#else 
	#define ASSERT(test, msg) ; //if(!test)std::cout << msg << " (" << __FILE__ << " : " << __LINE__ << ") \n";
#endif

#endif //ASSERT_HPP