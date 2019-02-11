#ifndef COMBINE_TEXTURES_HPP
#define COMBINE_TEXTURES_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class CombineTextures : public PostProcessingBase{
			private:

			public:
				CombineTextures(std::shared_ptr<Shaders::Shader> shader,
					unsigned int width, unsigned int height);

				~CombineTextures();


				unsigned int useEffect(unsigned int textureId1, unsigned int textureId2);

			};

		}
	}
}

#endif //COMBINE_TEXTURES_HPP