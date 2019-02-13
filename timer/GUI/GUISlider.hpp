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


#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP
#include "GUITextBox.hpp"

namespace Engine{
	namespace GUI{

		class GUISlider : public GUIBase{
		private:
			float m_value;
			float m_max;
			float m_min;

			bool m_editing;
			void onMouseDown(int x, int y, bool left){
				if (left)m_editing = true;
				
			}
			void onMouseUp(int x, int y, bool left){
				if (left)m_editing = false;
			}

			void onButtonDown(int button) override{
				//TODO fix
				if (button == SDLK_RETURN || button == SDLK_RETURN2){
					auto &_textbox = getChild<GUITextBox>("value");
					//if (_textbox.isUpdated()){
						setValue(_textbox.getFloatValue());
						m_updated = true;
						//PRINT("UPDATED") 
					//}
				}		
			}

			void onMouseHold(int x, int y, bool left){
				if (left){
					if (m_editing){
						m_updated = true;

						auto &_button = getChild<GUIButton>("slider");
						auto _buttonScale = _button.getScale().x;
						auto _buttonY = _button.getLocation().y;

						auto _loc1 = getLocation();
						auto _loc2 = _loc1 + getScale();

						if (x < (_loc1.x + _buttonScale / 2)){
							_button.setAbsoluteLocation(
								_loc1.x, _buttonY);

							m_value = m_min;
						}
						else if (x > _loc2.x - _buttonScale / 2){
							_button.setAbsoluteLocation(
								_loc2.x - _buttonScale, _buttonY);

							m_value = m_max;
						}
						else{
							_button.setAbsoluteLocation(
								x - _buttonScale / 2, _buttonY);
			
							m_value = m_min + (_button.getRelativeLocation().x * 1.0f / 
								(m_scaleX - _buttonScale)) * (m_max - m_min);
						}

						if (m_value > m_max)m_value = m_max;
						if (m_value < m_min)m_value = m_min;

						auto &_textbox = getChild<GUITextBox>("value");
						if (_textbox.getType() == TEXT_BOX_INT)
							_textbox.setValue(static_cast<int>(m_value));
						else _textbox.setValue(m_value);			
					}

				}
			}

		public:
			GUISlider(
				const std::string &name, 
				float min, float max,
				int x, int y, 
				int scaleX, int scaleY, 
				int border = 0, 
				TextBoxType type = TEXT_BOX_FLOAT
			)
				: GUIBase(name, x, y, scaleX, scaleY, border)
			{
				ASSERT(type != TEXT_BOX_STRING, "Slider value can't be a string");

				m_color = c_elementColor2;
				m_max = max;
				m_min = min;
				m_editing = false;

				unsigned int _texture = Textures::load2DTexture("icons/arrowDown.png", false);
				auto &_slider = addChild<GUIButton>("slider", 1, 1, scaleY - 2, scaleY - 2, 0, _texture);
				_slider.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

				int _len = std::to_string(static_cast<int>(m_max)).size() + 
					(type == TEXT_BOX_FLOAT ? 4 : 1);

				auto &_box = addChild<GUITextBox>("value", m_scaleX + 5, -3, 
					m_scaleY * _len, m_scaleY + 4, type, 2);

				if (type == TEXT_BOX_INT)
					_box.setValue(static_cast<int>(m_min));
				else _box.setValue(m_min);

			}

			void setValue(float value){
				m_value = value;

				auto &_button = getChild<GUIButton>("slider");

				float _value = m_value;
				if (_value < m_min)_value = m_min;
				else if (_value > m_max)_value = m_max;

				int _locX = static_cast<int>(((_value - m_min) / (m_max - m_min)) *
					(m_scaleX - _button.getScale().x));

				_button.setLocation(_locX, _button.getRelativeLocation().y);

				auto &_textbox = getChild<GUITextBox>("value");
				if (_textbox.getType() == TEXT_BOX_INT)
					_textbox.setValue(static_cast<int>(m_value));
				else _textbox.setValue(m_value);
				//_textbox.setUpdated(false);
			}

			float getFloatValue(){//TODO replace with return m_value;
				return getChild<GUITextBox>("value").getFloatValue();
			}

			int getIntValue(){
				return getChild<GUITextBox>("value").getIntValue();
			}

			const std::string &getStringValue(){
				return getChild<GUITextBox>("value").getStringValue();
			}

		};

	}
}


#endif //GUI_SLIDER_HPP