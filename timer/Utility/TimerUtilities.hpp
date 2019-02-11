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