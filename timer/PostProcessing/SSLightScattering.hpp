#ifndef SS_LIGHT_SCATTERING_HPP
#define SS_LIGHT_SCATTERING_HPP

#include <glm\glm.hpp>

#include "GaussianBlur.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class SSLightScattering : public PostProcessingBase{
			private:
				GaussianBlur m_blur;

			public:
				SSLightScattering(std::shared_ptr<Shaders::Shader> shader, 
					std::shared_ptr<Shaders::Shader> gaussianBlurShader,
					unsigned int width, unsigned int height);

				~SSLightScattering();


				unsigned int useEffect(const glm::mat4 &viewMatrix, 
					const glm::mat4 &projectionMatrix, const glm::vec3 &lightLocation, 
					const glm::vec3 &lightScale, float decay, float density, float weight,
					unsigned int occludedLightTextureId);
			
			};

		}
	}
}

#endif //SS_LIGHT_SCATTERING_HPP