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


#include "GaussianBlur.hpp"

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			GaussianBlur::GaussianBlur(std::shared_ptr<Shaders::Shader> shader, 
				unsigned int width, unsigned int height, GLint format, GLint internalFormat
			) :
				PostProcessingBase(shader, width, height, false), 
				m_format(format), m_internalFormat(internalFormat)
			{
				m_blurPingTextures.resize(MAX_MIPMAP_LEVEL, 0);
				m_blurPongTextures.resize(MAX_MIPMAP_LEVEL, 0);

				//placeholder for ping-pong 
				m_framebuffer.addTexture(0);
			}

			GaussianBlur::~GaussianBlur(){

			}


			unsigned int GaussianBlur::useEffect(unsigned int inputTexture, 
				unsigned int numPasses, unsigned int mipmapLevel, bool changeInput)
			{
				ASSERT(mipmapLevel < MAX_MIPMAP_LEVEL, "mipmap level out of range");

				unsigned int _desiredWidth = m_textureWidth >> mipmapLevel;
				unsigned int _desiredHeight = m_textureHeight >> mipmapLevel;

				if (!m_blurPingTextures[mipmapLevel]){
					m_blurPingTextures[mipmapLevel] = Textures::createEmptyTexture(_desiredWidth, 
						_desiredHeight, false, m_format, GL_LINEAR, m_internalFormat, GL_FLOAT);
				}
				if (!m_blurPongTextures[mipmapLevel]){
					m_blurPongTextures[mipmapLevel] = Textures::createEmptyTexture(_desiredWidth, 
						_desiredHeight, false, m_format, GL_LINEAR, m_internalFormat, GL_FLOAT);
				}

				//unsigned int _ping = m_sizeChanger.useEffect(mipmapLevel, { inputTexture });

				glViewport(0, 0, _desiredWidth, _desiredHeight);

				m_framebuffer.bind();
				{
					//TODO N-DIMENSIONS with glColorMask() optimizations
					
					glDisable(GL_DEPTH_TEST);

					m_shader->bind();

					unsigned int _pingTexture = m_blurPingTextures[mipmapLevel];
					unsigned int _pongTexture = m_blurPongTextures[mipmapLevel];

					m_framebuffer.setTexture(_pingTexture, 0);
					glClear(GL_COLOR_BUFFER_BIT);
					m_framebuffer.setTexture(_pongTexture, 0);
					glClear(GL_COLOR_BUFFER_BIT);

					Textures::bind2DTexture(inputTexture, 0);

					bool _horizontal = true;
					for (int i = 0; i < numPasses * 2; ++i){
						m_framebuffer.setTexture(_horizontal ? _pongTexture : _pingTexture, 0);

						if (i != 0){
							Textures::bind2DTexture(_horizontal ? _pingTexture : _pongTexture, 0);
						}

						//TODO change to vector
						m_shader->setUniform("horizontal", static_cast<int>(_horizontal));

						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
						_horizontal = !_horizontal;
					}

					glDisable(GL_DEPTH_TEST);
				}
				m_framebuffer.unbind();

				return  m_framebuffer.getTexture(0);
			}

		}
	}
}
