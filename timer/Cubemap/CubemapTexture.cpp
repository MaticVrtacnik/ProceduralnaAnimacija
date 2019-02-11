#include "CubemapTexture.hpp"

#include <iostream>

//#define PRINT_TEXTURE_ERRORS

namespace Engine{
	namespace Textures{

		void bindCubemapTexture(unsigned int texture){
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		}

		void bindCubemapTexture(unsigned int texture, GLint layer){
			glActiveTexture(GL_TEXTURE0 + layer);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		}

		void unbindCubemapTexture(){
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		void bindCubemapTextureArray(unsigned int texture){
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture);
		}

		void bindCubemapTextureArray(unsigned int texture, GLint layer){
			glActiveTexture(GL_TEXTURE0 + layer);
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture);
		}

		void unbindCubemapTextureArray(){
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
		}


		unsigned int createEmptyCubemap(unsigned int resolution){
			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);

			for (int i = 0; i < 6; ++i){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,
					resolution, resolution, 0, GL_RGBA, GL_FLOAT, 0);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

#ifdef PRINT_TEXTURE_ERRORS
				int error = glGetError();
				if (error){
					std::cout << gluErrorString(error) << std::endl;
				}

#endif //PRINT_TEXTURE_ERRORS

			}

			//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			return _textureId;
		}


		unsigned int createCubemap(const std::vector<std::string> &paths){
			ASSERT(paths.size() >= 6, "Wrong number of cubemap face paths provided (6 paths expected)");

			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);

			for (int i = 0; i < 6; ++i){
				auto _imageData = getTextureData(paths[i]);
				if (_imageData == nullptr)continue;

				GLint _width = _imageData == nullptr ? 0 : _imageData->w;
				GLint _height = _imageData == nullptr ? 0 : _imageData->h;
				const GLvoid *_pixels = _imageData == nullptr ? 0 : _imageData->pixels;
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,//TODO remove 16F
					_width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				SDL_FreeSurface(_imageData);

#ifdef PRINT_TEXTURE_ERRORS
				int error = glGetError();
				if (error){
					std::cout << gluErrorString(error) << std::endl;
				}

#endif //PRINT_TEXTURE_ERRORS

			}

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			return _textureId;
		}


		unsigned int createCubemapArray(
			const std::vector<unsigned int> &textures,
			GLint width, unsigned int height,
			GLint format)
		{
			if (textures.empty() || width <= 0 || height <= 0)return 0;

			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, _textureId);

			glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, format, width, height, textures.size() * 6);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, width, height, textures.size() * 6, format, GL_FLOAT, NULL);


			//TODO for lesser than opengl4.3
			//glBlitFramebuffer();

			//glTexSubImage3D()
			for (int i = 0; i < textures.size(); ++i){
				//for (int j = 0; j < 6; ++j){
					glCopyImageSubData(
						textures[i], GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
						_textureId, GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, i * 6,
						width, height, 6
					);
				//}
				
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

			std::cout << glewGetErrorString(glGetError()) << std::endl;

			return _textureId;
		}

	}
}