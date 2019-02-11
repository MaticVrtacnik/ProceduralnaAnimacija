#ifndef GAUSSIAN_BLUR_HPP
#define GAUSSIAN_BLUR_HPP

#include "PostProcessingBase.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class GaussianBlur : public PostProcessingBase{
			private:
				static const unsigned int MAX_MIPMAP_LEVEL = 7;

			private:
				GLint m_format = GL_RGB16F;
				GLint m_internalFormat = GL_RGB;

			public:
				std::vector<unsigned int> m_blurPingTextures;
				std::vector<unsigned int> m_blurPongTextures;

			public:
				GaussianBlur(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height,
					GLint format = GL_RGB16F, GLint internalFormat = GL_RGB);

				~GaussianBlur();


				unsigned int useEffect(unsigned int inputTexture, unsigned int numPasses,
					unsigned int mipmapLevel = 0, bool changeInput = false);

			};

		}
	}
}

#endif //GAUSSIAN_BLUR_HPP