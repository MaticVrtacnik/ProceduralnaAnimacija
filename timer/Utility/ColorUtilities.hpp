#ifndef COLOR_UTILITIES_HPP
#define COLOR_UTILITIES_HPP

#include <glm/glm.hpp>


namespace Engine{
	namespace Utilities{

		//Expected H[0, 360], SV[0.0, 1.0] Returns RGB[0,1]
		glm::vec3 getColorWheel(float h, float s, float v);

		//Expects RGB[0,1] Returns HSV[0,1]
		glm::vec3 rgb2hsv(const glm::vec3 &rgb);

		//Expects RGB[0,1] Returns HSV[0,1]
		glm::vec3 rgb2hsv(float r, float g, float b);

		//Expects HSV[0,1] Returns RGB[0,1]
		glm::vec3 hsv2rgb(const glm::vec3 &hsv);

		//Expects HSV[0,1] Returns RGB[0,1]
		glm::vec3 hsv2rgb(float h, float s, float v);

	}
}

#endif //COLOR_UTILITIES_HPP