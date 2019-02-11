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