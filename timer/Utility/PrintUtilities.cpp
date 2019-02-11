#include "PrintUtilities.hpp"

#include <iostream>
#include <typeinfo>
#include <bitset>


namespace Engine{
	namespace Utilities{

		void print(const glm::vec2 &vector){
			std::cout << "X: " << vector.x
				<< " | Y: " << vector.y
				<< "\n";
		}

		void print(const glm::vec3 &vector){
			std::cout << "X: " << vector.x
				<< " | Y: " << vector.y
				<< " | Z: " << vector.z
				<< "\n";
		}

		void print(const glm::vec4 &vector){
			std::cout << "X: " << vector.x
				<< " | Y: " << vector.y
				<< " | Z: " << vector.z
				<< " | W: " << vector.w
				<< "\n";
		}

		void print(const glm::quat &quaternion){
			std::cout << "X: " << quaternion.x
				<< " | Y: " << quaternion.y
				<< " | Z: " << quaternion.z
				<< " | W: " << quaternion.w
				<< "\n";
		}

		void print(const glm::mat3 &matrix){
			for (std::size_t i = 0; i < matrix.col_size(); ++i){
				std::cout << "|";

				for (std::size_t j = 0; j < matrix.row_size(); ++j){
					printf("%+.3f|", matrix[i][j]);
				}std::cout << "\n";
			}std::cout << "\n";
		}

		void print(const glm::mat4 &matrix){
			for (std::size_t i = 0; i < matrix.col_size(); ++i){
				std::cout << "|";

				for (std::size_t j = 0; j < matrix.row_size(); ++j){
					auto v = matrix[i][j];
					printf("%+.3f|", matrix[i][j]);
				}std::cout << "\n";
			}std::cout << "\n";
		}



		void printBits(char x){
			std::cout << std::bitset<sizeof(x) * 8>(x) << std::endl;
		}

		void printBits(short x){
			std::cout << std::bitset<sizeof(x) * 8>(x) << std::endl;
		}

		void printBits(int x){
			std::cout << std::bitset<sizeof(x) * 8>(x) << std::endl;
		}

		void printBits(float x){
			std::cout << std::bitset<sizeof(x) * 8>(x) << std::endl;
		}
	}
}