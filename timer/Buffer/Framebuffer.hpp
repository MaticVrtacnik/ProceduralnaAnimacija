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


#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <vector>


namespace Engine{
	namespace Buffers{

		class Framebuffer{
		private:
			bool m_hasDepth = false;
			unsigned int m_id = 0;
			unsigned int m_depthTexture = 0;
			std::vector<unsigned int> m_textures;

		public:
			Framebuffer();

			~Framebuffer();


			void bind();

			void unbind();


			void init();

			void init(int width, int height, bool hasDepth = true, bool hasStencil = false);

			bool checkErrors();


			void addTexture(unsigned int texture);

			const unsigned int &getTexture(std::size_t id = 0) const;

			const std::vector<unsigned int> &getTextures() const;

			void setTexture(unsigned int texture, std::size_t id = 0);


			const unsigned int &getDepthTexture() const;

			void setDepthTexture(unsigned int texture, bool deleteOld = false);

			bool hasDepth() const;


			//if num == 0 then all attachments will be set
			void setDrawBuffers(std::size_t num = 0);

			void setDrawBuffers(const std::vector<unsigned int> &buffers);

		};

	}
}

#endif //FRAMEBUFFER_HPP