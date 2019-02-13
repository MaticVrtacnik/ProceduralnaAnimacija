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