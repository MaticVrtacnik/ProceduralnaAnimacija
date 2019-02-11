#ifndef BOX_BLUR_HPP
#define BOX_BLUR_HPP

#include "PostProcessingBase.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class BoxBlur : public PostProcessingBase{
			private:
				static const unsigned int MAX_MIPMAP_LEVEL = 7;
				std::vector<unsigned int> m_textureMipmaps;

			private:
				GLint m_format = GL_RGB16F;
				GLint m_internalFormat = GL_RGB;

			public:
				BoxBlur(std::shared_ptr<Shaders::Shader>, 
					unsigned int width, unsigned int height,
					GLint format = GL_RGB16F, GLint internalFormat = GL_RGB);

				~BoxBlur();


				unsigned int useEffect(unsigned int textureId, unsigned int mipmapLevel);

			};

		}
	}
}

#endif //BOX_BLUR_HPP