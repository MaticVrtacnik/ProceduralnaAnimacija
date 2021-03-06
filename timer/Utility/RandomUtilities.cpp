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


#include "RandomUtilities.hpp"

#include <random>

#include "../Define/Assert.hpp"

namespace Engine{
	namespace Utilities{

		//TODO make faster
		std::random_device g_randomDevice;
		std::mt19937 g_generator(g_randomDevice());

		template <typename T> T random(T min, T max){
			std::uniform_real_distribution<T> _difference(min, max);
			return _difference(g_generator);
		}


		int randomInt(int min, int max){
			ASSERT(min < max, "Max is smaller than min");

			std::uniform_int_distribution<int> _difference(min, max);
			return _difference(g_generator);
		}

		float randomFloat(float min, float max){
			ASSERT(min < max, "Max is smaller than min");

			std::uniform_real_distribution<float> _difference(min, max);
			return  _difference(g_generator);
		}

		double randomDouble(double min, double max){
			ASSERT(min < max, "Max is smaller than min");

			std::uniform_real_distribution<double> _difference(min, max);
			return _difference(g_generator);
		}

		glm::vec2 randomVec2(float min, float max){
			return glm::vec2(
				randomFloat(min, max),
				randomFloat(min, max)
			);
		}

		glm::vec3 randomVec3(float min, float max){
			return glm::vec3(
				randomFloat(min, max),
				randomFloat(min, max),
				randomFloat(min, max)
			);
		}

		glm::vec4 randomVec4(float min, float max){
			return glm::vec4(
				randomFloat(min, max),
				randomFloat(min, max),
				randomFloat(min, max),
				randomFloat(min, max)
			);
		}

		glm::ivec2 randomiVec2(int min, int max){
			return glm::ivec2(
				randomInt(min, max),
				randomInt(min, max)
			);
		}

		glm::ivec3 randomiVec3(int min, int max){
			return glm::ivec3(
				randomInt(min, max),
				randomInt(min, max),
				randomInt(min, max)
			);
		}

		glm::ivec4 randomiVec4(int min, int max){
			return glm::ivec4(
				randomInt(min, max),
				randomInt(min, max),
				randomInt(min, max),
				randomInt(min, max)
			);
		}


		int randomInt(int min, int max, int seed){
			ASSERT(min < max, "Max is smaller than min");

			std::uniform_int_distribution<int> _difference(min, max);
			std::mt19937 _generator(seed);
			return _difference(_generator);
		}

		float randomFloat(float min, float max, int seed){
			ASSERT(min < max, "Max is smaller than min");

			std::uniform_real_distribution<float> _difference(min, max);
			std::mt19937 _generator(seed);
			return _difference(_generator);
		}

		double randomDouble(double min, double max, int seed){
			ASSERT(min < max, "Max is smaller than min");

			std::uniform_real_distribution<double> _difference(min, max);
			std::mt19937 _generator(seed);
			return _difference(_generator);
		}

		glm::vec2 randomVec2(float min, float max, int seed){
			return glm::vec2(
				randomFloat(min, max, seed),
				randomFloat(min, max, seed * 107 % 66)
			);
		}

		glm::vec3 randomVec3(float min, float max, int seed){
			return glm::vec3(
				randomFloat(min, max, seed),
				randomFloat(min, max, seed * 107 % 66),
				randomFloat(min, max, seed * 312 % 166)
			);
		}

		glm::vec4 randomVec4(float min, float max, int seed){
			return glm::vec4(
				randomFloat(min, max, seed),
				randomFloat(min, max, seed * 107 % 66),
				randomFloat(min, max, seed * 312 % 166),
				randomFloat(min, max, seed * 1072 % 43)
			);
		}

		glm::ivec2 randomiVec2(int min, int max, int seed){
			return glm::ivec2(
				randomInt(min, max, seed),
				randomInt(min, max, seed * 107 % 66)
			);
		}

		glm::ivec3 randomiVec3(int min, int max, int seed){
			return glm::ivec3(
				randomInt(min, max, seed),
				randomInt(min, max, seed * 107 % 66),
				randomInt(min, max, seed * 312 % 166)
			);
		}

		glm::ivec4 randomiVec4(int min, int max, int seed){
			return glm::ivec4(
				randomInt(min, max, seed),
				randomInt(min, max, seed * 107 % 66),
				randomInt(min, max, seed * 312 % 166),
				randomInt(min, max, seed * 1072 % 43)
			);
		}

	}
}