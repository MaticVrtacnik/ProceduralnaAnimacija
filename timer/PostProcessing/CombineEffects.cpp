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