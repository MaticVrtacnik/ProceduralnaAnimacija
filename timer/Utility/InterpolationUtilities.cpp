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


#include "InterpolationUtilities.hpp"

#include "MathUtilities.hpp"


namespace Engine{
	namespace Utilities{


		float interpolate(double t, float start, double end,
			InterpolationType interpolationType)
		{
			float _factor = getTimeFactor(t, start, end);

			switch (interpolationType){
				case INTERPOLATION_COS: 
					return cosInterpolation(_factor, 0.0f, 1.0f);

				/*case INTERPOLATION_QUADRIC: //TODO
					return cubicInterpolation(_factor, 0.0f, 1.0f);

				case INTERPOLATION_CUBIC:
					return cubicInterpolation(_factor, 0.0f, 1.0f);*/

				case INTERPOLATION_SPRING:
					return springInterpolation(_factor, 0.0f, 1.0f);

				default: //INTERPOLATION_LINEAR:
					return linearInterpolation(_factor, 0.0f, 1.0f);
			}
		}

		InterpolationType getInterpolationTypeFromString(const std::string &type){
			if (type == "cos")			
				return INTERPOLATION_COS;	
			/*else if (type == "quadric")	
				return INTERPOLATION_QUADRIC;	
			else if (type == "cubic")	
				return INTERPOLATION_CUBIC;*/	
			else if (type == "spring")	
				return INTERPOLATION_SPRING;	
			else // type == "linear"						
				return INTERPOLATION_LINEAR; 
		}


		float getTimeFactor(double t, float start, float end){
			float _delta = end - start;
			float _result = _delta == 0.0f ? 0.0f : ((t - start) / _delta);

			return _result;
		}


		float linearInterpolation(double t, float start, float end){
			return t * end + (1.0f - t) * start;
		}

		float cosInterpolation(double t, float start, float end){
			return linearInterpolation(-0.5f * (cos(t * Utilities::PI) - 1.0f), start, end);
		}


		float springInterpolation(double t, float start, float end){
			float _duration = end - start;

			float _timeFactor = t;
			float ts = _timeFactor * _timeFactor;
			float tc = ts * _timeFactor;

			float _factor = (33 * tc*ts + -106 * ts*ts + 126 * tc + -67 * ts + 15 * t);
			//PRINT(_factor);
			//_factor *= glm::max(0.1, glm::smoothstep(0.1, 0.4, t));

			//_factor = pow(_factor, 0.5f);

			return start + _duration * _factor;
		}

		float springInterpolation2(double t, float start, float end){
			float _duration = end - start;

			if (t < (1 / 2.75f)) {
				return _duration * (7.5625f * t * t) + start;
			} else if (t < (2 / 2.75f)) {
				float postFix = t -= (1.5f / 2.75f);
				return _duration * (7.5625f * postFix * t + 0.75f) + start;
			} else if (t < (2.5 / 2.75)) {
				float postFix = t -= (2.25f / 2.75f);
				return _duration * (7.5625f * postFix * t + 0.9375f) + start;
			} else {
				float postFix = t -= (2.625f / 2.75f);
				return _duration * (7.5625f * postFix * t + 0.984375f) + start;
			}
		}


		float springInterpolationFactor(float f){
			const float c_amplitude = 1.0f;
			const float c_frequency = 8.0f;

			float a = -glm::pow(glm::e<float>(), -f / c_amplitude);
			return a * cos(f * c_frequency) + 1.0f;
		}

	}
}
