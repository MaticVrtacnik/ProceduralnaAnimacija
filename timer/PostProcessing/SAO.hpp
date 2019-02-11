#ifndef SAO_HPP
#define SAO_HPP

#include <glm\glm.hpp>

#include "SAOBlur.hpp"
#include "GaussianBlur.hpp"
#include "TextureSizeChanger.hpp"
#include "LinearizeDepth.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{


			class SAO : public PostProcessingBase{
			private:
				static const unsigned int NUM_MIPMAPS = 5;

				SAOBlur m_blur;
				TextureSizeChanger m_textureSizeChanger;
				LinearizeDepth m_linearizeDepth;

				std::vector<unsigned int> m_depthTextureMipmaps;
				//GaussianBlur m_gaussianBlur;

			public:
				SAO(std::shared_ptr<Shaders::Shader> shader,
					std::shared_ptr<Shaders::Shader> blurShader,
					std::shared_ptr<Shaders::Shader> linearDepthShader,
					std::shared_ptr<Shaders::Shader> sizeChangerShader,
					unsigned int width, unsigned int height);

				~SAO();


				unsigned int useEffect(const glm::mat4 &projectionMatrix, 
					unsigned int depthTextureId);

				const unsigned int &getResult(const std::size_t &index = 0) const override;

			};

		}
	}
}

#endif //SAO_HPP