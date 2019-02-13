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


#ifndef INTERPOLATION_UTILITIES_HPP
#define INTERPOLATION_UTILITIES_HPP

#include <string>


namespace Engine{
	namespace Utilities{

		enum InterpolationType{
			INTERPOLATION_LINEAR = 0,
			INTERPOLATION_COS = 1,
			INTERPOLATION_SPRING = 2
		};


		// Converts [start..end] to [0..1]
		// Returns interpolated value in range [0..1]
		float interpolate(double t, float start, double end,
			InterpolationType interpolationType);

		InterpolationType getInterpolationTypeFromString(const std::string &type);


		float getTimeFactor(double t, float start, float end);

		float linearInterpolation(double t, float start, float end);

		float cosInterpolation(double t, float start, float end);

		float springInterpolation(double t, float start, float end);

		float springInterpolation2(double t, float start, float end);

		float springInterpolationFactor(float f);

	}
}

#endif //INTERPOLATION_UTILITIES_HPP