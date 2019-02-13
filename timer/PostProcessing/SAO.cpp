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


#include "SAO.hpp"

#include <glm\gtc\type_ptr.hpp>

#include "../Texture/Texture.hpp"
#include "../Shader/Shader.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			SAO::SAO(
				std::shared_ptr<Shaders::Shader> shader,
				std::shared_ptr<Shaders::Shader> blurShader,
				std::shared_ptr<Shaders::Shader> linearDepthShader,
				std::shared_ptr<Shaders::Shader> sizeChangerShader,
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false),
				m_blur(blurShader, width, height),
				m_textureSizeChanger(sizeChangerShader, width, height),
				m_linearizeDepth(linearDepthShader, width, height)
				//m_gaussianBlur(blurShader, width, height)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(
					width, height, false, GL_RGB8, GL_NEAREST, GL_RGB, GL_FLOAT));

				m_depthTextureMipmaps.resize(NUM_MIPMAPS, 0);
			}

			SAO::~SAO(){

			}


			unsigned int SAO::useEffect(const glm::mat4 &projectionMatrix,
				unsigned int depthTextureId)
			{
				unsigned int _result = 0;

				startTimer();
				{
					m_depthTextureMipmaps[0] = m_linearizeDepth.useEffect(depthTextureId);

					Textures::bind2DTexture(m_depthTextureMipmaps[0]);
					Textures::bind2DTexture(m_depthTextureMipmaps[0], 0);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					glGenerateMipmap(GL_TEXTURE_2D);

					/*for (int i = 1; i < NUM_MIPMAPS; ++i){
						//TODO new minify shader
						m_depthTextureMipmaps[i] = m_textureSizeChanger.useEffect(
							m_depthTextureMipmaps[0], i);
					}*/

					//return m_depthTextureMipmaps[0];


					PostProcessingBase::useEffect();

					const glm::vec4 _projectionContant
						(float(-2.0 / (m_textureWidth * projectionMatrix[0][0])),
						float(-2.0 / (m_textureHeight * projectionMatrix[1][1])),
						float((1.0 - (double) projectionMatrix[0][2]) / projectionMatrix[0][0]),
						float((1.0 + (double) projectionMatrix[1][2]) / projectionMatrix[1][1]));

					m_shader->setUniform("u_projectionConstant", _projectionContant);

					Textures::bind2DTexture(m_depthTextureMipmaps[0], 0);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


					_result = m_blur.useEffect(PostProcessingBase::getResult(), 2);
				}
				stopTimer();
				

				//return getResult();
				//return m_gaussianBlur.useEffect(getResult(), 2);
				return _result;
				//return m_boxBlur.useEffect({ m_framebuffer.getTexture(0) });
				//return m_blur.useEffect(m_framebuffer.getTexture(0), 2, 0);
			}


			const unsigned int &SAO::getResult(const std::size_t &index) const{
				return m_blur.getResult(index);
			}

		}
	}
}
