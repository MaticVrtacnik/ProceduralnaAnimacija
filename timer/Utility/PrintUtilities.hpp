#ifndef PRINT_UTILITIES_HPP
#define PRINT_UTILITIES_HPP

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>


namespace Engine{
	namespace Utilities{

		void print(const glm::vec2 &vector);

		void print(const glm::vec3 &vector);

		void print(const glm::vec4 &vector);

		void print(const glm::quat &quaternion);

		void print(const glm::mat3 &matrix);

		void print(const glm::mat4 &matrix);

		
		void printBits(char x);

		void printBits(short x);

		void printBits(int x);

		void printBits(float x);


		/*template <typename... Args> 
		void print(Args&&... args){
			for (auto &_it : { std::forward <Args>(args)... }){
				std::cout << _it << " ";
			}std::cout << std::endl;
		}*/

	}
}


#endif //PRINT_UTILITIES_HPP