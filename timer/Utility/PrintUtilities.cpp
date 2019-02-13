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