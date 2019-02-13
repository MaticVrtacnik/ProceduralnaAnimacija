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


#ifndef ARRAY_BUFFER_HPP
#define ARRAY_BUFFER_HPP

#include <GL/glew.h>
#include <vector>

#include "../Define/Defines.hpp"
#include "../Define/Assert.hpp"

#include <iostream>

namespace Engine{
	namespace Buffers{

		template <typename T>
		unsigned int createArrayBuffer(
			const std::vector<T> &data, 
			GLenum target, 
			GLenum usage = GL_STATIC_DRAW)
		{
			if (data.empty()) return 0;
			//ASSERT(!data.empty(), "No data for buffer");

			unsigned int _bufferId;
			glGenBuffers(1, &_bufferId);
			glBindBuffer(target, _bufferId);
			glBufferData(target, data.size() * sizeof(T), &data[0], usage);

			return _bufferId;
		}

		template <typename T>
		unsigned int createEmptyArrayBuffer(
			unsigned int size, GLenum target,
			GLenum usage = GL_STATIC_DRAW)
		{
			unsigned int _bufferId;
			glGenBuffers(1, &_bufferId);
			glBindBuffer(target, _bufferId);
			glBufferData(target, size * sizeof(T), NULL, usage);

			return _bufferId;
		}


		int getBufferSize(GLenum target);

		void bindBuffer(GLenum target, unsigned int bufferId);

		void unbindBuffer(GLenum target);

		struct ArrayBuffer{
			unsigned int m_bufferId;

			/* GL_ARRAY_BUFFER or GL_INDEX_ARRAY_BUFFER or ??? */
			GLenum m_target;

			/* GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW */
			GLenum m_usage; 

			/* size of buffer in Bytes */
			unsigned int m_byteSize;
			
			//number of elements
			unsigned int m_size;

			ArrayBuffer() :
				m_bufferId(0), m_byteSize(0), m_size(0),
				m_target(GL_ARRAY_BUFFER),
				m_usage(GL_STATIC_DRAW)
			{
			}

			template <typename T> 
			void loadBuffer(
				const std::vector<T> &data, 
				GLenum target, 
				GLenum usage = GL_STATIC_DRAW)
			{
				m_bufferId = createArrayBuffer(data, target, usage);
				m_byteSize = data.size() * sizeof(T);
				m_size = data.size();
				m_target = target;
				m_usage = usage;
			}

			template <typename T>
			void loadBufferEmpty(
				unsigned int count, GLenum target,
				GLenum usage = GL_STATIC_DRAW)
			{
				m_bufferId = createEmptyArrayBuffer<T>(count, target, usage);
				m_byteSize = count * sizeof(T);
				m_size = count;
				m_target = target;
				m_usage = usage;
			}

			void deleteBuffer(){
				if (m_bufferId != 0){
					glDeleteBuffers(1, &m_bufferId);
					m_bufferId = 0;
				}
				
				m_size = 0;
				m_byteSize = 0;
			}

			/*template <typename T>
			ArrayBuffer(const std::vector<T> &data, GLenum target, GLenum usage = GL_STATIC_DRAW) :
				m_buffer(createBuffer(data, target, usage)),
				m_target(target),
				m_usage(usage)
			{
			}*/

			~ArrayBuffer(){
				glDeleteBuffers(1, &m_bufferId);
				//PRINT("BUFFER DELETED");
			}

			void bind() const{
				glBindBuffer(m_target, m_bufferId);
			}

			void unbind() const{
				glBindBuffer(m_target, 0);
			}

			ArrayBuffer &operator=(const ArrayBuffer &buffer){
				m_bufferId = buffer.m_bufferId;
				m_target = buffer.m_target;
				m_usage = buffer.m_usage;
				m_byteSize = buffer.m_byteSize;
				m_size = buffer.m_size;

				return *this;
			}

			ArrayBuffer &operator=(ArrayBuffer &&buffer){
				m_bufferId = std::move(buffer.m_bufferId);
				m_target = std::move(buffer.m_target);
				m_usage = std::move(buffer.m_usage);
				m_byteSize = std::move(buffer.m_byteSize);
				m_size = std::move(buffer.m_size);

				return *this;
			}

		};


		template <typename T>
		void updateArrayBufferData(
			unsigned int bufferId,
			const std::vector<T> &data,
			unsigned int originalSize,
			GLint offset = 0,		
			GLenum usage = GL_STREAM_DRAW)
		{
			ASSERT((data.size() * sizeof(T)) <= originalSize, 
				"ArrayBuffer data exceeded buffer size.");

			glBindBuffer(GL_ARRAY_BUFFER, bufferId);
			glBufferData(GL_ARRAY_BUFFER, originalSize, NULL, usage);
			glBufferSubData(GL_ARRAY_BUFFER, offset, 
				data.size() * sizeof(T), &data[0]);
		}

		template <typename T>
		void updateArrayBufferData(
			const ArrayBuffer &buffer,
			const std::vector<T> &data,
			unsigned int offset = 0)
		{
			updateArrayBufferData(
				buffer.m_bufferId, data, buffer.m_byteSize,
				offset, buffer.m_usage
			);
		}

	}
}

#endif //ARRAY_BUFFER_HPP