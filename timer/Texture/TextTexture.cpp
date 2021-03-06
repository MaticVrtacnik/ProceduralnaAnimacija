/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


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
