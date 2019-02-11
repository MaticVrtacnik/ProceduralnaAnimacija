#include "BoxBlur.hpp"

#include "../Shader/Shader.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{
		
			BoxBlur::BoxBlur(std::shared_ptr<Shaders::Shader> shader, 
				unsigned int width, unsigned int height, GLint format, GLint internalFormat
			) :
				PostProcessingBase(shader, width, height, false),
				m_format(format), m_internalFormat(internalFormat)
			{
				m_textureMipmaps.resize(MAX_MIPMAP_LEVEL);
				m_framebuffer.addTexture(0);
			}

			BoxBlur::~BoxBlur(){

			}


			unsigned int BoxBlur::useEffect(unsigned int textureId, unsigned int mipmapLevel){
				ASSERT(mipmapLevel < MAX_MIPMAP_LEVEL, "mipmap level out of range");

				unsigned int _desiredWidth = m_textureWidth >> mipmapLevel;
				unsigned int _desiredHeight = m_textureHeight >> mipmapLevel;

				if (!m_textureMipmaps[mipmapLevel]){
					m_textureMipmaps[mipmapLevel] = Textures::createEmptyTexture(_desiredWidth, 
						_desiredHeight, false, m_format, GL_LINEAR, m_internalFormat, GL_FLOAT);
				}


				m_shader->bind();
				m_framebuffer.bind();
				m_framebuffer.setTexture(m_textureMipmaps[mipmapLevel], 0);
				glClear(GL_COLOR_BUFFER_BIT);

				glViewport(0, 0, _desiredWidth, _desiredHeight);

				Textures::bind2DTexture(textureId, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				

				return m_framebuffer.getTexture(0);
			}

		}
	}
}
