#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL\glew.h>
#include <SDL2\SDL_image.h>
#include <string>
#include <vector>

#include "TextureTypes.hpp"


namespace Engine{
	namespace Textures{

		static const float DEFAULT_ANISOTROPY = 16.0f;


		//first layer...0
		void setActiveTextureLayer(GLint layer); 


		void bind1DTexture(unsigned int texture);

		void bind1DTexture(unsigned int texture, GLint layer);

		void unbind1DTexture();


		void bind2DTexture(unsigned int texture);

		void bind2DTexture(unsigned int texture, GLint layer);

		void unbind2DTexture();
		
		void unbind2DTexture(GLint layer);


		void bind2DTextureArray(unsigned int texture);

		void bind2DTextureArray(unsigned int texture, GLint layer);

		void unbind2DTextureArray();


		void bind3DTexture(unsigned int texture);

		void bind3DTexture(unsigned int texture, GLint layer);

		void unbind3DTexture();


		void removeTexture(unsigned int texture);


		SDL_Surface *getTextureData(const std::string &path);

		unsigned int load2DTexture(
			const std::string &path,
			bool mipmap,
			GLint format = GL_RGBA,
			GLint wrap = GL_REPEAT
		);

		unsigned int create2DTextureArray(
			const std::vector<unsigned int> &textures,
			GLint width, unsigned int height, 
			GLint format = GL_RGBA
		);

		unsigned int load1DTexture(
			const std::string &path,
			bool mipmap,
			GLint format = GL_RGBA,
			GLint wrap = GL_REPEAT
		);

		unsigned int createEmptyTexture(
			GLsizei width, 
			GLsizei height, 
			bool depthTexture, 
			GLint format, 
			GLint filter = GL_NEAREST,
			GLint internalFormat = GL_RGBA, 
			GLenum dataType = GL_FLOAT
		);



		struct Texture{
		public:
			TextureType m_type = TEXTURE_UNKNOWN;
			std::string m_path = "";
			unsigned int m_textureId = 0;
			GLint m_format = GL_RGBA;
			GLint m_wrap = GL_REPEAT;
			GLint m_filter; //TODO
			GLfloat m_anisotrophy = DEFAULT_ANISOTROPY;
			bool m_mipmap = false;

		public:
			Texture();

			Texture(const Texture &texture);

			Texture(Texture &&texture);

			Texture(unsigned int texture, TextureType type = TEXTURE_DIFFUSE);

			Texture(const std::string &path, bool mipmap, TextureType type = TEXTURE_DIFFUSE,
				GLint format = GL_RGBA, GLint wrap = GL_REPEAT);

			~Texture();


			Texture &operator=(const Texture &texture);

			Texture &operator=(Texture &&texture);


			void bind();

			void bind(GLint layer);

			void unbind();

		};

	}
}

#endif //TEXTURE_HPP