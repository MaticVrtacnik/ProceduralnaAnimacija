#ifndef PRINT_DEFINES_HPP
#define PRINT_DEFINES_HPP

#include <iostream>
#include <SDL2\SDL_timer.h>

#define PRINT(msg) std::cout<< __FILE__ << " : " <<  __LINE__  << " : " << msg << " || " << SDL_GetTicks() << "\n";

//#define DEBUG
#ifdef DEBUG
	#define DEBUG_PRINT(msg) PRINT("DEBUG: " << msg)
#else //ndef DEBUG
	#define DEBUG_PRINT(msg) "";
#endif //DEBUG

#define WARNING(msg) PRINT("WARNING: " << msg)
#define ERROR(msg) PRINT("ERROR: " << msg)

#endif //PRINT_DEFINES_HPP