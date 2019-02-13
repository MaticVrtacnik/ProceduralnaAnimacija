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


#ifndef RANDOM_UTILITIES_HPP
#define RANDOM_UTILITIES_HPP

#include <glm\glm.hpp>

namespace Engine{
	namespace Utilities{

		template <typename T> T random(T min, T max);


		int randomInt(int min, int max);
		float randomFloat(float min, float max);
		double randomDouble(double min, double max);
		glm::vec2 randomVec2(float min, float max);
		glm::vec3 randomVec3(float min, float max);
		glm::vec4 randomVec4(float min, float max);
		glm::ivec2 randomiVec2(int min, int max);
		glm::ivec3 randomiVec3(int min, int max);
		glm::ivec4 randomiVec4(int min, int max);


		int randomInt(int min, int max, int seed);
		float randomFloat(float min, float max, int seed);
		double randomDouble(double min, double max, int seed);
		glm::vec2 randomVec2(float min, float max, int seed);
		glm::vec3 randomVec3(float min, float max, int seed);
		glm::vec4 randomVec4(float min, float max, int seed);
		glm::ivec2 randomiVec2(int min, int max, int seed);
		glm::ivec3 randomiVec3(int min, int max, int seed);
		glm::ivec4 randomiVec4(int min, int max, int seed);

	}
}

#endif //RANDOM_UTILITIES_HPP