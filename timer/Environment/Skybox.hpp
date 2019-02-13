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


#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glm\gtx\transform.hpp>

#include "../Buffer/ArrayBuffer.hpp"
#include "../Shader/Shader.hpp"

namespace Engine{
	namespace Environment{

		class Skybox{
		private:
			std::vector<unsigned int> m_cubemaps;
			int m_selectedIndex = 0;

			Buffers::ArrayBuffer m_vertexBuffer;

		public:
			unsigned int m_vertexArray = 0;

		public:
			Skybox();

			~Skybox();


			const std::vector<unsigned int> &getCubemaps() const;

			const unsigned int &getCubemap(std::size_t index) const;


			const unsigned int &getSelectedCubemap() const;

			//void setCubemap(unsigned int cubemap);

			void setNextCubemap();

			void setPreviousCubemap();

		};

	}
}

#endif //SKYBOX_HPP