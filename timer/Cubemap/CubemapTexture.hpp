#ifndef CUBEMAP_TEXTURE_HPP
#define CUBEMAP_TEXTURE_HPP

#include <vector>
#include "../Texture/Texture.hpp"
#include "../Define/Assert.hpp"

namespace Engine{
	namespace Textures{

		void bindCubemapTexture(unsigned int texture);
		void bindCubemapTexture(unsigned int texture, GLint layer);
		void unbindCubemapTexture();
		void bindCubemapTextureArray(unsigned int texture);
		void bindCubemapTextureArray(unsigned int texture, GLint layer);
		void unbindCubemapTextureArray();

		unsigned int createEmptyCubemap(unsigned int resolution);
		unsigned int createCubemap(const std::vector<std::string> &paths);

		unsigned int createCubemapArray(
			const std::vector<unsigned int> &textures,
			GLint width, unsigned int height,
			GLint format = GL_RGBA
		);
	}
}


#endif //CUBEMAP_TEXTURE_HPP