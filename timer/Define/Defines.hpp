/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <stdint.h>

#define INT_TYPE std::size_t /*uint32_t*/

#define MAX_ENTITIES 1000
#define MAX_COMPONENTS 20
#define MAX_SYSTEMS 20

#define WORLD_SIZE 10000.0f
#define DEFAULT_FPS 60.0f


//#define RESOLUTION_QUALITY 1440
//#define RESOLUTION_QUALITY 1296 //NOT WORKING PROPERLY
//#define RESOLUTION_QUALITY 1080
#define RESOLUTION_QUALITY 1152
#include "Resolution.hpp"

//#define FULLSCREEN 1
//#define RESIZABLE 1


#endif //DEFINES_HPP