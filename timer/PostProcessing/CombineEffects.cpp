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


#include "CombineEffects.hpp"

#include <glm\gtx\transform.hpp>

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			CombineEffects::CombineEffects(std::shared_ptr<Shaders::Shader> shader,
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(m_textureWidth,
					m_textureHeight, false, GL_RGBA16F, GL_LINEAR, GL_RGBA, GL_FLOAT));
			}

			CombineEffects::~CombineEffects(){
			}


			unsigned int CombineEffects::useEffect(const glm::mat4 &viewMatrix, 
				unsigned int screenTextureId, unsigned int lensFlareTextureId, 
				unsigned int lightScatteringTextureId,
				unsigned int sunLightScatteringTextureId)
			{
				PostProcessingBase::useEffect();

				Textures::bind2DTexture(screenTextureId, 0);
				Textures::bind2DTexture(lensFlareTextureId, 1);
				Textures::bind2DTexture(lightScatteringTextureId, 2);
				Textures::bind2DTexture(sunLightScatteringTextureId, 3);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				return m_framebuffer.getTexture(0);
			}

		}
	}
}
