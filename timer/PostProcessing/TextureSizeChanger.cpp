#include "TextureSizeChanger.hpp"

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			TextureSizeChanger::TextureSizeChanger(
				std::shared_ptr<Shaders::Shader> shader,
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width, height, false)
			{
				m_textureMipmaps.resize(MAX_MIPMAP_LEVEL, 0);

				m_framebuffer.addTexture(m_textureMipmaps[0]);
			}

			TextureSizeChanger::~TextureSizeChanger(){

			}


			unsigned int TextureSizeChanger::useEffect(
				unsigned int textureId, unsigned int mipmapLevel)
			{
				ASSERT(mipmapLevel < MAX_MIPMAP_LEVEL, "desired mipmap level out of range");

				unsigned int _desiredWidth = m_textureWidth >> mipmapLevel;
				unsigned int _desiredHeight = m_textureHeight >> mipmapLevel;
				if (!m_textureMipmaps[mipmapLevel]){
					m_textureMipmaps[mipmapLevel] = Textures::createEmptyTexture(_desiredWidth, 
						_desiredHeight, false, GL_RGB16F, GL_LINEAR, GL_RGB, GL_FLOAT);
				}

				m_shader->bind();
				m_framebuffer.bind();
				m_framebuffer.setTexture(m_textureMipmaps[mipmapLevel], 0);
				glClear(GL_COLOR_BUFFER_BIT);

				glViewport(0, 0, _desiredWidth, _desiredHeight);

				Textures::bind2DTexture(textureId, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				//m_textureMipmaps[desiredLevel] = inputTexture;
				return m_framebuffer.getTexture(0);
			}

		}
	}
}
