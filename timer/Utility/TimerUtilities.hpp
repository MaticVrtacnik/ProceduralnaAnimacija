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


#ifndef TIMER_UTILITIES_HPP
#define TIMER_UTILITIES_HPP

#include <string>
#include <SDL2\SDL_timer.h>
#include "InterpolationUtilities.hpp"


namespace Engine{
	namespace Utilities{

		struct TimerData{
		public:
			Uint32 m_start = 0;
			Uint32 m_end = 0;

		public:
			TimerData(){
				
			}

			TimerData(Uint32 start, Uint32 end) :
				m_start(start), m_end(end)
			{
			}
		};


		struct TimerDataInterpolation : public TimerData{
		public:
			Utilities::InterpolationType m_interpolationType = INTERPOLATION_LINEAR;

		public:
			TimerDataInterpolation(){
				
			}

			TimerDataInterpolation(Uint32 start, Uint32 end,
				Utilities::InterpolationType interpolationType
			) :
				TimerData(start, end), 
				m_interpolationType(interpolationType)
			{
			}
		};

	}
}

#endif //TIMER_UTILITIES_HPP