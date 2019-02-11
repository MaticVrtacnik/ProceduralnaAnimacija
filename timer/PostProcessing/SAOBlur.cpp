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
