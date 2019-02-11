#ifndef HDR_HPP
#define HDR_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class HDR : public PostProcessingBase{
			private:
				unsigned int m_averageBrightnessTexture = 0;
				bool m_setTrillinearFiltering = false;

			public:
				HDR(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height);

				~HDR();


				unsigned int useEffect(float fps, float exposureFactor, unsigned int textureId,
					unsigned int bloomTextureId1, unsigned int bloomTextureId2, 
					unsigned int bloomTextureId3, unsigned int bloomTextureId4);

			};

		}
	}
}

#endif //HDR_HPP