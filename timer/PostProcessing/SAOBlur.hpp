#ifndef SAO_BLUR_HPP
#define SAO_BLUR_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{


			class SAOBlur : public PostProcessingBase{
			private:
				unsigned int m_pingTexture = 0;
				unsigned int m_pongTexture = 0;

			public:
				SAOBlur(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height);

				~SAOBlur();


				unsigned int useEffect(unsigned int saoDepthTexture, 
					unsigned int numPasses = 1);

			};

		}
	}
}

#endif //SAO_BLUR_HPP