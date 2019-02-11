#ifndef TEXTURE_SIZE_CHANGER_HPP
#define TEXTURE_SIZE_CHANGER_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class TextureSizeChanger : public PostProcessingBase{
			private:
				static const unsigned int MAX_MIPMAP_LEVEL = 7;		
				std::vector<unsigned int> m_textureMipmaps;

			public:
				TextureSizeChanger(std::shared_ptr<Shaders::Shader> shader,
					unsigned int width, unsigned int height);

				~TextureSizeChanger();


				unsigned int useEffect(unsigned int textureId, unsigned int mipmapLevel);
			
			};

		}
	}
}

#endif //TEXTURE_SIZE_CHANGER_HPP