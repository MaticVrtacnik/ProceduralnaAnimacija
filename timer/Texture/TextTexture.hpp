#ifndef TEXT_TEXTURE_HPP
#define TEXT_TEXTURE_HPP

#include <string>


namespace Engine{
	namespace Textures{

		struct TextTextureData{
			unsigned int m_textureId = 0;
			int m_textWidth = 0;
			int m_textHeight = 0;
		};


		TextTextureData createTextTexture(
			const ::std::string &string,
			int size = 12,
			const ::std::string &font = "Roboto.ttf"
		);

	}
}

#endif //TEXT_TEXTURE_HPP