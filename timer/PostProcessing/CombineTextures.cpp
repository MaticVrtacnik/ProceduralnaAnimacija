#include "CombineTextures.hpp"

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			CombineTextures::CombineTextures(std::shared_ptr<Shaders::Shader> shader,
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(
					m_textureWidth, m_textureHeight, false, GL_RGBA16F, GL_LINEAR, GL_RGBA));
			}

			CombineTextures::~CombineTextures(){

			}


			unsigned int CombineTextures::useEffect(
				unsigned int textureId1, unsigned int textureId2)
			{
				PostProcessingBase::useEffect();

				Textures::bind2DTexture(textureId1, 0);
				Textures::bind2DTexture(textureId2, 1);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				return m_framebuffer.getTexture(0);
			}

		}
	}
}
