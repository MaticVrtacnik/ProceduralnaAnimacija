#include "LinearizeDepth.hpp"

#include "../Texture/Texture.hpp"
#include "../Shader/Shader.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{
		
			LinearizeDepth::LinearizeDepth(std::shared_ptr<Shaders::Shader> shader,
				unsigned int width, unsigned int height
			):
				PostProcessingBase(shader, width, height, false)
			{
				//TODO maybe change to 16bit
				m_framebuffer.addTexture(Textures::createEmptyTexture(
					width, height, false, GL_R32F, GL_LINEAR, GL_RED, GL_FLOAT));
			}

			LinearizeDepth::~LinearizeDepth(){

			}


			unsigned int LinearizeDepth::useEffect(unsigned int depthTexture){
				PostProcessingBase::useEffect();

				Textures::bind2DTexture(depthTexture, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				return getResult();
			}
			
		}
	}
}
