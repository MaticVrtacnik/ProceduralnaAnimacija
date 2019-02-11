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