#include "Texture.hpp"

#include "../Define/PrintDefines.hpp"
#include "../Define/Assert.hpp"

#define PRINT_TEXTURE_ERRORS


namespace Engine{
	namespace Textures{

		//first layer...0
		void setActiveTextureLayer(GLint layer){
			glActiveTexture(GL_TEXTURE0 + layer);
		}

		//TEXTURE_1D
		void bind1DTexture(unsigned int texture){
			glBindTexture(GL_TEXTURE_1D, texture);
		}
		void bind1DTexture(unsigned int texture, GLint layer){
			setActiveTextureLayer(layer);
			bind1DTexture(texture);
		}
		void unbind1DTexture(){
			glBindTexture(GL_TEXTURE_1D, 0);
		}

		//TEXTURE_2D
		void bind2DTexture(unsigned int texture){
			glBindTexture(GL_TEXTURE_2D, texture);
		}

		void bind2DTexture(unsigned int texture, GLint layer){
			setActiveTextureLayer(layer);
			bind2DTexture(texture);
		}

		void unbind2DTexture(){
			bind2DTexture(0);
		}

		void unbind2DTexture(GLint layer){
			bind2DTexture(0, layer);
		}


		void bind2DTextureArray(unsigned int texture){
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		}
		void bind2DTextureArray(unsigned int texture, GLint layer){
			setActiveTextureLayer(layer);
			bind2DTextureArray(texture);
		}
		void unbind2DTextureArray(){
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}

		//TEXTURE_3D
		void bind3DTexture(unsigned int texture){
			glBindTexture(GL_TEXTURE_3D, texture);
		}
		void bind3DTexture(unsigned int texture, GLint layer){
			setActiveTextureLayer(layer);
			bind3DTexture(texture);
		}
		void unbind3DTexture(){
			glBindTexture(GL_TEXTURE_3D, 0);
		}

		void removeTexture(unsigned int texture){
			glDeleteTextures(1, &texture);
		}


		SDL_Surface *getTextureData(const std::string &path){
			SDL_Surface *_loadedImage = IMG_Load(("data/textures/" + path).c_str());

			if (!_loadedImage){
#ifdef PRINT_TEXTURE_ERRORS
				ERROR(IMG_GetError());
#endif 
				//SDL_FreeSurface(loadedImage);
				return nullptr;
			}

			SDL_PixelFormat _textureFormat = {
				NULL, NULL, 32, 4, 0, 0,
				0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};

			SDL_Surface* _convertedImage = SDL_ConvertSurface(_loadedImage, &_textureFormat, SDL_SWSURFACE);
			SDL_FreeSurface(_loadedImage);

			if (!_convertedImage){
#ifdef PRINT_TEXTURE_ERRORS
				ERROR(IMG_GetError());
#endif 	

				//SDL_FreeSurface(convertedImage);
				return nullptr;
			}

			return _convertedImage;
		}


		unsigned int load2DTexture(const std::string &path, 
			bool mipmap, GLint format, GLint wrap)
		{
			SDL_Surface *_textureData = getTextureData(path);		
			if (_textureData == nullptr) return 0;

			//TODO remove previous errors (set to NO_ERROR)
			glGetError();

	
			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_2D, _textureId);

			glTexImage2D
			(
				GL_TEXTURE_2D,
				0, //level
				format,
				_textureData->w,
				_textureData->h,
				0, //border
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				_textureData->pixels
			);

			if (mipmap)glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? 
				GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

			GLfloat _maxAnisotropy = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_maxAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 
				DEFAULT_ANISOTROPY > _maxAnisotropy ? _maxAnisotropy : DEFAULT_ANISOTROPY);

#ifdef PRINT_TEXTURE_ERRORS
			GLenum _error = glGetError();
			if (_error){
				std::cout << "Texture '" << path << "' loading error : '" << 
					gluErrorString(_error) << "'" << std::endl;
			}

#endif //PRINT_TEXTURE_ERRORS
			
			glBindTexture(GL_TEXTURE_2D, 0);
			SDL_FreeSurface(_textureData);

			return _textureId; //Texture{path, mipmap, format, wrap};
		}


		unsigned int create2DTextureArray(const std::vector<unsigned int> &textures,
			GLint width, unsigned int height, GLint format)
		{
			if (textures.empty() || width <= 0 || height <= 0)return 0;

			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_2D_ARRAY, _textureId);

			glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, format, width, height, textures.size());
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, textures.size(), format, GL_FLOAT, NULL);
			

			//TODO for lesser than opengl4.3
			//glBlitFramebuffer();

			//glTexSubImage3D()
			for (int i = 0; i < textures.size(); ++i){
				glCopyImageSubData(
					textures[i], GL_TEXTURE_2D, 0, 0, 0, 0, 
					_textureId, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 
					width, height, 1
				);
			}

			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

			return _textureId;
		}

		unsigned int load1DTexture(const std::string &path,
			bool mipmap, GLint format, GLint wrap)
		{
			SDL_Surface *_textureData = getTextureData(path);
			if (_textureData == nullptr){

				return 0;
			}

			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_1D, _textureId);

			glTexImage1D
			(
				GL_TEXTURE_1D,
				0, //level
				format,
				_textureData->w,
				0, //border
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				_textureData->pixels
			);

			if (mipmap)glGenerateMipmap(GL_TEXTURE_1D);

			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, mipmap ? 
				GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap);

			GLfloat _maxAnisotropy = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_maxAnisotropy);
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 
				DEFAULT_ANISOTROPY > _maxAnisotropy ? _maxAnisotropy : DEFAULT_ANISOTROPY);

#ifdef PRINT_TEXTURE_ERRORS
			GLenum error = glGetError();
			if (error){
				std::cout << gluErrorString(error) << std::endl;
			}

#endif //PRINT_TEXTURE_ERRORS

			glBindTexture(GL_TEXTURE_1D, 0);
			SDL_FreeSurface(_textureData);

			return _textureId;
		}


		unsigned int createEmptyTexture(
			GLsizei width,
			GLsizei height, 
			bool depthTexture, 
			GLint format,
			GLint filter, 
			GLint internalFormat, 
			GLenum dataType)
		{
			unsigned int _textureId;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_2D, _textureId);

			glTexImage2D(
				GL_TEXTURE_2D,
				0, //level
				format,
				width,
				height,
				0, //border
				depthTexture ? GL_DEPTH_COMPONENT : internalFormat,
				dataType,
				0 //texture data
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

#ifdef PRINT_TEXTURE_ERRORS
			//TODO wut?
			/*GLenum _error = glGetError();
			if (_error){
				std::cout << gluErrorString(_error) << std::endl;
			}*/

#endif //PRINT_TEXTURE_ERRORS
			
			glBindTexture(GL_TEXTURE_2D, 0);

			return _textureId;
		}
		


		Texture::Texture(){

		}

		Texture::Texture(const Texture &texture){
			*this = texture;
		}

		Texture::Texture(Texture &&texture){
			*this = std::move(texture);
		}

		Texture::Texture(unsigned int textureId, TextureType type):
			m_path(""), m_textureId(textureId), m_type(type)
		{
			m_format = GL_RGBA;
			m_wrap = GL_REPEAT;
			m_anisotrophy = DEFAULT_ANISOTROPY;
			m_mipmap = false;
			//TODO GET format, wrap ... FROM TEXTURE ID DATA
		}

		Texture::Texture(const std::string &path, bool mipmap, 
			TextureType type, GLint format, GLint wrap
		):
			m_type(type),
			m_path(path),
			m_format(format),
			m_wrap(wrap),
			m_mipmap(mipmap),
			m_anisotrophy(DEFAULT_ANISOTROPY)
		{
			m_textureId = load2DTexture(path, mipmap, format, wrap);
		}

		Texture::~Texture(){
			glDeleteTextures(1, &m_textureId);
			//std::cout << "TEXTURE DELETED: " <<  m_path << std::endl;
		}


		Texture &Texture::operator=(const Texture &texture){
			this->m_type = texture.m_type;
			this->m_path = texture.m_path;
			this->m_textureId = texture.m_textureId;
			this->m_format = texture.m_format;
			this->m_wrap = texture.m_wrap;
			this->m_mipmap = texture.m_mipmap;
			this->m_anisotrophy = texture.m_anisotrophy;

			return *this;
		}

		Texture &Texture::operator=(Texture &&texture){
			this->m_type = std::move(texture.m_type);
			this->m_path = std::move(texture.m_path);
			this->m_textureId = std::move(texture.m_textureId);
			this->m_format = std::move(texture.m_format);
			this->m_wrap = std::move(texture.m_wrap);
			this->m_mipmap = std::move(texture.m_mipmap);
			this->m_anisotrophy = std::move(texture.m_anisotrophy);

			return *this;
		}


		void Texture::bind(){
			bind2DTexture(m_textureId);
		}

		void Texture::bind(GLint layer){
			bind2DTexture(m_textureId, layer);
		}

		void Texture::unbind(){
			unbind2DTexture();
		}

	}
}
