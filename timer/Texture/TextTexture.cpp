#include "TextTexture.hpp"

#include <memory>
#include <GL\glew.h>
#include <SDL2\SDL_ttf.h>

#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Textures{

		TextTextureData createTextTexture(const std::string &string, int size, const std::string &font){
			TextTextureData _textureData;

			if (string.empty()){
				return _textureData;
			}


			TTF_Font *_font = TTF_OpenFont(("data/fonts/" + font).c_str(), size);
			if (_font == nullptr){
				PRINT(SDL_GetError());
			}

			std::string _textString = string;
			/*if (_textString[0] == '!'){
				TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
				_textString.erase(0, 1);
			}*/

			TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);

			Uint32 _lineLengthPixels = 1920;
			SDL_Color _color = { 255, 255, 255 };
			SDL_Surface *_text = TTF_RenderText_Blended_Wrapped(_font, _textString.c_str(), _color, _lineLengthPixels);

			_textureData.m_textWidth = _text->w;
			_textureData.m_textHeight = _text->h;

			glGenTextures(1, &_textureData.m_textureId);
			glBindTexture(GL_TEXTURE_2D, _textureData.m_textureId);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _text->w, _text->h,
				0, GL_RGBA, GL_UNSIGNED_BYTE, _text->pixels);

			glBindTexture(GL_TEXTURE_2D, 0);
			SDL_FreeSurface(_text);

			return _textureData;
		}

	}
}
