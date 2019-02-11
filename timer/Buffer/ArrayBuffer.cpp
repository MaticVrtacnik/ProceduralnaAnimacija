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