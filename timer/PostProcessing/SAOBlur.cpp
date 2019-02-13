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


#include "SAOBlur.hpp"

#include "../Texture/Texture.hpp"
#include "../Shader/Shader.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			SAOBlur::SAOBlur(std::shared_ptr<Shaders::Shader> shader, 
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_pingTexture = Textures::createEmptyTexture(
					width, height, false, GL_RGB8, GL_LINEAR, GL_RGB, GL_FLOAT);

				m_pongTexture = Textures::createEmptyTexture(
					width, height, false, GL_RGB8, GL_LINEAR, GL_RGB, GL_FLOAT);

				//Placeholder
				m_framebuffer.addTexture(0);
			}

			SAOBlur::~SAOBlur(){
				Textures::removeTexture(m_pingTexture);
				Textures::removeTexture(m_pongTexture);
			}


			unsigned int SAOBlur::useEffect(
				unsigned int saoDepthTexture, unsigned int numPasses)
			{
				PostProcessingBase::useEffect();

				m_framebuffer.setTexture(m_pingTexture, 0);
				glClear(GL_COLOR_BUFFER_BIT);
				m_framebuffer.setTexture(m_pongTexture, 0);
				glClear(GL_COLOR_BUFFER_BIT);

				Textures::bind2DTexture(saoDepthTexture, 0);

				bool _horizontal = true;
				for (int i = 0; i < numPasses * 2; ++i){
					m_framebuffer.setTexture(_horizontal ? m_pongTexture : m_pingTexture, 0);

					if (i != 0){
						Textures::bind2DTexture(_horizontal ? m_pingTexture : m_pongTexture, 0);
					}
				
					m_shader->setUniform("u_direction", glm::ivec2(_horizontal, !_horizontal));

					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					_horizontal = !_horizontal;
				}

				return getResult();
			}

		}
	}
}
