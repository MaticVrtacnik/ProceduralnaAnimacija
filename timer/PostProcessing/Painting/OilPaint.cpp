#include "OilPaint.hpp"

#include "../../Shader/Shader.hpp"
#include "../../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			OilPaint::OilPaint(std::shared_ptr<Shaders::Shader> shader,
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(
					width, height, false, GL_RGBA8, GL_LINEAR, GL_RGBA, GL_UNSIGNED_INT));
			}

			OilPaint::~OilPaint(){

			}


			unsigned int OilPaint::useEffect(unsigned int textureId, float radius){
				startTimer();
				{
					PostProcessingBase::useEffect();

					m_shader->setUniform("u_radius", radius);

					Textures::bind2DTexture(textureId, 0);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
				stopTimer();

				return getResult();
			}

		}
	}
}
