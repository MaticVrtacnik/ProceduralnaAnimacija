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


#ifndef GUI_TEXT_BOX_HPP
#define GUI_TEXT_BOX_HPP
#include <SDL2\SDL_timer.h>
#include <numeric>
#include "GUIBase.hpp"
#include "GUIText.hpp"
#include "GUIButton.hpp"

namespace Engine{
	namespace GUI{

		const int c_interval = 300; //ms

		enum TextBoxType{
			TEXT_BOX_INT,
			TEXT_BOX_FLOAT,
			TEXT_BOX_STRING
		};

		class GUITextBox : public GUIBase{
		private:
			TextBoxType m_type;
			Uint32 m_timer;
			int m_selectedChar;
			std::vector<int> m_sizes;

			void onMouseDown(int x, int y, bool left) override{
				auto _rLoc = glm::ivec2(x, y) - getLocation();

				if (left){ 
					//move under clicked char

					int _sum = 0;
					for (std::size_t i = 0; i < m_sizes.size(); ++i){
						_sum += m_sizes[i];
						if (_sum > _rLoc.x){
							getChild<GUIText>("selector").
								setLocation(2 + _sum - m_sizes[i], 1);

							m_selectedChar = i;
							return;
						}
					}
		
					m_selectedChar = m_sizes.size();
					getChild<GUIText>("selector").setLocation(
						2 + getChild<GUIText>("string").getScale().x, 1);
					
				}
			}

			void onButtonDown(int button) override{
				if (!m_active) return;

				auto &_string = getChild<GUIText>("string");
				auto &_selector = getChild<GUIText>("selector");

				if (button == SDLK_RETURN || 
					button == SDLK_RETURN2)
				{
					const auto &_str = _string.getString();
					if ((_str.length() == 1 && _str[0] == '-') ||
						_str.empty())
					{
						return;
					}
			
					m_active = false;
					m_updated = true;
					return;
				}

				

				switch (button){
					case SDLK_BACKSPACE:
					{
						if (m_selectedChar > 0){
							m_selectedChar--;

							auto &_str = _string.getString();
							_string.setString(_str.substr(0, m_selectedChar) + 
								(_str.size() != m_selectedChar ? _str.substr(m_selectedChar + 1) : ""), ALIGN_LEFT);

							int _change = m_sizes[m_selectedChar];
							m_sizes.erase(m_sizes.begin() + m_selectedChar);
							_selector.setLocation(_selector.getRelativeLocation().x - _change, 1);
						}
					}break;

					case SDLK_DELETE:
					{
						//TODO MAYBE
						/*if (m_selectedChar < m_sizes.size() && !m_sizes.empty()){

							auto &_str = _string.getString();
							_string.setString(_str.substr(0, m_selectedChar) +
								(_str.size() != m_selectedChar ? _str.substr(m_selectedChar + 1) : ""));

							int _change = m_sizes[m_selectedChar];
							m_sizes.erase(m_sizes.begin() + m_selectedChar);
							m_selectedChar--;

						}*/
					}break;

					case SDLK_LEFT:
					{
						if (m_selectedChar > 0){
							int _before = _selector.getRelativeLocation().x;
							_selector.setLocation(_before - m_sizes[--m_selectedChar], 1);
						}
					}break;

					case SDLK_RIGHT:
					{						
						int _size = m_sizes.size();
						if (m_selectedChar < _size){
							int _before = _selector.getRelativeLocation().x;
							_selector.setLocation(_before + m_sizes[m_selectedChar++], 1);
						}
					}break;
					
					default:
					{
						const auto &_str = _string.getString();

						if (m_type == TEXT_BOX_INT){
							if (button == '-'){
								if (!_str.empty())return;
							}
							else if (button < '0' || button > '9')
								return;
						}
						else if (m_type == TEXT_BOX_FLOAT){
							if (button == '-'){
								if (!_str.empty())return;
							}
							else if (button == '.'){
								if (_str.empty())
									return;
								else if (std::count(
									_str.begin(), _str.end(), '.') > 0)
								{
									return;
								}
							}
							else if (button < '0' || button > '9'){
								return;
							}
						}

						//limit text length
						if (_string.getScale().x > m_scaleX - m_border * 4) {
							break;
						}

						int _before = _string.getScale().x;
						_string.setString(_str.substr(0, m_selectedChar) + static_cast<char>(button) +
							(_str.size() != m_selectedChar ? _str.substr(m_selectedChar) : ""), ALIGN_LEFT);

						int _change = _string.getScale().x - _before;
						_selector.setLocation(_selector.getRelativeLocation().x + _change, 2);
							
						m_sizes.insert(m_sizes.begin() + m_selectedChar, _change);
						m_selectedChar++;
					}break;
				}

			}

		public:
			GUITextBox(
				const std::string &name,
				int x, int y,
				int scaleX, int scaleY,
				TextBoxType type = TEXT_BOX_STRING,
				int border = 0,
				unsigned int texture = 0
			)
				: GUIBase(name, x, y, scaleX, scaleY, border)
			{
				m_texture = texture;
				m_color = glm::vec4(0.15f, 0.15f, 0.16f, -0.15f);
				m_selectedChar = 0;
				m_type = type;
				m_timer = SDL_GetTicks();
		
				float _size = scaleY - 4.0f - (scaleY / 5.0f);
				addChild<GUIText>("string", "", 2, 2, _size);
				auto &_selector = addChild<GUIText>("selector", "!_", 2, 1);
				_selector.setColor(glm::vec4(1, 0, 0, 0));
				_selector.setVisible(false);

			}

			void afterDraw() override{
				if (SDL_GetTicks() - m_timer > c_interval){
					m_timer = SDL_GetTicks();

					auto &_selector = getChild<GUIText>("selector");
					_selector.setVisible(
						m_active ? !_selector.isVisible() : false);

				}

			}

			TextBoxType getType() const{
				return m_type;
			}

			float getFloatValue(){
				std::string _string(getChild<GUIText>("string").getString());
	
				return _string.empty() ? 0.0f : 
					static_cast<float>(std::atof(_string.c_str()));
			}

			int getIntValue(){
				std::string _string(getChild<GUIText>("string").getString());
				return _string.empty() ? 0 : std::atoi(_string.c_str());
			}

			const std::string &getStringValue(){
				return getChild<GUIText>("string").getString();
			}

			void setValue(int value){
				std::string _string = std::to_string(value);
				getChild<GUIText>("string").setString("", ALIGN_LEFT);

				m_type = TEXT_BOX_INT;
				m_active = true;
				m_selectedChar = 0;
				m_sizes.clear();

				for (auto _char : _string)
					onButtonDown(_char);
				
				m_active = false;
			}

			void setValue(float value){
				std::string _string(std::move(std::to_string(value)));
				_string = _string.substr(0, _string.length() - 3);
				getChild<GUIText>("string").setString("", ALIGN_LEFT);

				m_type = TEXT_BOX_FLOAT;
				m_active = true;
				m_selectedChar = 0;
				m_sizes.clear();

				for (auto _char : _string){
					onButtonDown(_char);
				}

				m_active = false;
			}

			void setValue(double value){
				setValue(static_cast<float>(value));
			}

			void setValue(const std::string &value){
				getChild<GUIText>("string").setString("", ALIGN_LEFT);

				m_type = TEXT_BOX_STRING;
				m_active = true;
				m_selectedChar = 0;
				m_sizes.clear();

				for (auto _char : value){
					onButtonDown(_char);
				}

				m_active = false;
			}

		};
	}
}


#endif //GUI_TEXT_BOX_HPP