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


#include "ArrayBuffer.hpp"

namespace Engine{
	namespace Buffers{

		/*template <typename T>
		unsigned int createBuffer(const std::vector<T> &data, GLenum target, GLenum usage){
			if (data.empty()) return 0;
			//ASSERT(!data.empty(), "No data for buffer");

			unsigned int _bufferId;
			glGenBuffers(1, &_bufferId);
			glBindBuffer(target, _bufferId);
			glBufferData(target, data.size() * sizeof(T), &data[0], usage);

			/*int size = 0;
			glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
			PRINT("SIZESSS"); PRINT(size)*/
			//glBindBuffer(target, 0);

		/*	return _bufferId;
		}*/

		int getBufferSize(GLenum target){
			int _size = 0;
			glGetBufferParameteriv(target, GL_BUFFER_SIZE, &_size);

			return _size;
		}


		void bindBuffer(GLenum target, unsigned int bufferId){
			glBindBuffer(target, bufferId);
		}

		void unbindBuffer(GLenum target){
			glBindBuffer(target, 0);
		}

	}
}