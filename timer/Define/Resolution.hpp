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


#ifndef RESOLUTION_HPP
#define RESOLUTION_HPP


#if (RESOLUTION_QUALITY == 1440)
	#define DEFAULT_SCREEN_WIDTH 2560
	#define DEFAULT_SCREEN_HEIGHT 1440

#elif (RESOLUTION_QUALITY == 1296)
	#define DEFAULT_SCREEN_WIDTH 2304	
	#define DEFAULT_SCREEN_HEIGHT 1296

#elif (RESOLUTION_QUALITY == 1152)
	#define DEFAULT_SCREEN_WIDTH 2048	
	#define DEFAULT_SCREEN_HEIGHT 1152

#elif (RESOLUTION_QUALITY == 1080)
	#define DEFAULT_SCREEN_WIDTH 1920
	#define DEFAULT_SCREEN_HEIGHT 1080

#elif (RESOLUTION_QUALITY == 900)
	#define DEFAULT_SCREEN_WIDTH 1600
	#define DEFAULT_SCREEN_HEIGHT 900

#elif (RESOLUTION_QUALITY == 768)
	#define DEFAULT_SCREEN_WIDTH 1366
	#define DEFAULT_SCREEN_HEIGHT 768

#elif (RESOLUTION_QUALITY == 720)
	#define DEFAULT_SCREEN_WIDTH 1280
	#define DEFAULT_SCREEN_HEIGHT 720

#else //DEFAULT
	#define DEFAULT_SCREEN_WIDTH 1920
	#define DEFAULT_SCREEN_HEIGHT 1080

#endif //RESOLUTION_QUALITY


#endif //RESOLUTION_HPP