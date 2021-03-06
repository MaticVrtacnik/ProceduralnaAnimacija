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


#include "HDR.hpp"

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			HDR::HDR(std::shared_ptr<Shaders::Shader> shader, 
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(m_textureWidth, 
					m_textureHeight, false, GL_RGBA, GL_LINEAR, GL_RGBA, GL_UNSIGNED_INT));

				m_framebuffer.addTexture(Textures::createEmptyTexture(m_textureWidth, 
					m_textureHeight, false, GL_R16F, GL_NEAREST, GL_RED, GL_FLOAT));

				//GL_R16F, GL_RED
				m_averageBrightnessTexture = Textures::createEmptyTexture(m_textureWidth, 
					m_textureHeight, false, GL_R16F, GL_NEAREST, GL_RED, GL_FLOAT); 

				m_framebuffer.bind();
				m_framebuffer.setDrawBuffers();
				m_framebuffer.unbind();

				m_setTrillinearFiltering = false;
			}

			HDR::~HDR(){
				glDeleteTextures(1, &m_averageBrightnessTexture);
			}


			unsigned int HDR::useEffect(float fps, float exposureFactor, unsigned int textureId,
				unsigned int bloomTextureId1, unsigned int bloomTextureId2,
				unsigned int bloomTextureId3, unsigned int bloomTextureId4)
			{
				glViewport(0, 0, m_textureWidth, m_textureHeight);

				m_shader->bind();
				m_framebuffer.bind();

				unsigned int _previousTexture = m_framebuffer.getTexture(1);
				m_framebuffer.setTexture(m_averageBrightnessTexture, 1);
				m_averageBrightnessTexture = _previousTexture;

				glClear(GL_COLOR_BUFFER_BIT);

				startTimer();
				{
					Textures::bind2DTexture(textureId, 0);

					if (!m_setTrillinearFiltering){
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
							GL_NEAREST_MIPMAP_NEAREST);
						m_setTrillinearFiltering = true;
						//glGenerateMipmap(GL_TEXTURE_2D);
					}

					glGenerateMipmap(GL_TEXTURE_2D);


					m_shader->setUniform("fpsFactor", 60.0f / fps);
					m_shader->setUniform("exposureFactor", exposureFactor);

					Textures::bind2DTexture(bloomTextureId1, 1);
					Textures::bind2DTexture(bloomTextureId2, 2);
					Textures::bind2DTexture(bloomTextureId3, 3);
					Textures::bind2DTexture(bloomTextureId4, 4);
					Textures::bind2DTexture(m_averageBrightnessTexture, 5);

					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				stopTimer();
				

				return m_framebuffer.getTexture(0);
			}

		}
	}
}
