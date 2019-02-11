#ifndef GUI_TEXT_HPP
#define GUI_TEXT_HPP

#include <SDL2\SDL_ttf.h>
#include "GUIBase.hpp"

#define FONT "data/fonts/Roboto.ttf"


namespace Engine{
	namespace GUI{

		enum AlignText{
			ALIGN_LEFT,
			ALIGN_CENTER,
			ALIGN_RIGHT
		};

		class GUIText : public GUIBase{
		private:
			TTF_Font *m_font;
			std::string m_string;
			float m_size;
			AlignText m_align;
			std::function<void()> m_action;

			void onMouseDown(int x, int y, bool left)override{
				if (left){
					if (m_action)m_action();
				}
			}

		public:
			GUIText(
				const std::string &name,
				const std::string &string,
				int x, int y, 
				float size = 15.0f,
				AlignText align = ALIGN_LEFT,
				const glm::vec4 &color = c_textColor
			)
				: GUIBase(name, x, y, 0, 0, 0)
			{	
				m_size = size;
				m_color = color;
				m_align = align;

				m_font = TTF_OpenFont(FONT, static_cast<int>(m_size));
				if (m_font == nullptr){
					ERROR(SDL_GetError());
				}

				setString(string, m_align);	
			}


			void updateAlignment(){
				if (m_align != ALIGN_CENTER || m_parent == nullptr)
					return;

				auto &_rLoc = m_parent->getRelativeLocation();
				auto &_scale = m_parent->getScale();
				//TODO maybe update button here

				m_x = (_scale.x - m_scaleX) / 2;
			}

			void setString(const std::string &string){
				setString(string, m_align);
			}

			void setString(const std::string &string, AlignText align){
				//TODO optimize
				m_string = string;

				glDeleteTextures(1, &m_texture);
				m_texture = 0;
				m_scaleX = 0;
				m_scaleY = 0;
				m_align = align;

				if (!m_string.empty()){
					if (m_string[0] == '!'){
						TTF_SetFontStyle(m_font, TTF_STYLE_BOLD);
						m_string.erase(0, 1);
					}

					Uint32 _lineLengthPixels = 1920; //TODO
					SDL_Color Color = { /*color.z * 255*/0, /*color.y * 255*/ 0, /*color.x * 255*/ 0 };
					SDL_Surface *_text = TTF_RenderText_Blended_Wrapped(m_font, m_string.c_str(), Color, _lineLengthPixels);

					m_scaleX = _text->w;
					m_scaleY = _text->h;

					if (m_align == ALIGN_RIGHT){
						m_x -= m_scaleX;
					}
					else updateAlignment();
					

					glGenTextures(1, &m_texture);
					glBindTexture(GL_TEXTURE_2D, m_texture);

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_scaleX, m_scaleY,
						0, GL_RGBA, GL_UNSIGNED_BYTE, _text->pixels);

					glBindTexture(GL_TEXTURE_2D, 0);
					SDL_FreeSurface(_text);
				}

			}

			const std::string &getString() const{
				return m_string;
			}

			float getSize() const{
				return m_size;
			}

			void setAction(const std::function<void()> &action){
				m_action = std::move(action);
			}

		};
	}
}

#endif //GUI_TEXT_HPP