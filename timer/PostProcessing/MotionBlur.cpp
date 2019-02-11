#include "MotionBlur.hpp"

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			MotionBlur::MotionBlur(std::shared_ptr<Shaders::Shader> shader,
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(
					m_textureWidth, m_textureHeight, false, GL_RGBA));
			}

			MotionBlur::~MotionBlur(){

			}


			unsigned int MotionBlur::useEffect(float fps, 
				unsigned int textureId, unsigned int velocityTextureId)
			{
				PostProcessingBase::useEffect();

				startTimer();
				{
					m_shader->setUniform("fpsFactor", 60.0f / fps);

					Textures::bind2DTexture(textureId, 0);
					Textures::bind2DTexture(velocityTextureId, 1);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				stopTimer();
				

				return m_framebuffer.getTexture(0);
			}

		}
	}
}
