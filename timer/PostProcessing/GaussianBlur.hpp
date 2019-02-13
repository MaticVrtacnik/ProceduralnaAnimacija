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


#ifndef GAUSSIAN_BLUR_HPP
#define GAUSSIAN_BLUR_HPP

#include "PostProcessingBase.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class GaussianBlur : public PostProcessingBase{
			private:
				static const unsigned int MAX_MIPMAP_LEVEL = 7;

			private:
				GLint m_format = GL_RGB16F;
				GLint m_internalFormat = GL_RGB;

			public:
				std::vector<unsigned int> m_blurPingTextures;
				std::vector<unsigned int> m_blurPongTextures;

			public:
				GaussianBlur(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height,
					GLint format = GL_RGB16F, GLint internalFormat = GL_RGB);

				~GaussianBlur();


				unsigned int useEffect(unsigned int inputTexture, unsigned int numPasses,
					unsigned int mipmapLevel = 0, bool changeInput = false);

			};

		}
	}
}

#endif //GAUSSIAN_BLUR_HPP