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


#ifndef GUI_TOGGLE_HPP
#define GUI_TOGGLE_HPP
#include "GUIBase.hpp"
#include "GUIButton.hpp"
#include "GUIText.hpp"

namespace Engine{
	namespace GUI{

		class GUIToggle : public GUIBase{
		private:
			bool m_on;
			std::function<void()> m_action;

			void onMouseDown(int x, int y, bool left){
				if (m_action)m_action();
				setOn(!m_on);
			}

		public:
			GUIToggle(
				const std::string &name, 
				int x, int y, 
				bool on = false
			)
				: GUIBase(name, x, y, 40, 15)
			{
				m_on = on;
				m_color = glm::vec4(0.08, 0.08, 0.08, 0.0); //off
			
				//unsigned int _texture = Textures::load2DTexture("icons/radioChecked.png", false);
				auto &_switch = addChild<GUIButton>("switch", 0, 0, 15, 15);
				_switch.setColor(c_elementColor2);

				addChild<GUIText>("on", "ON", 5, 1, 10.0f).setVisible(false);
				addChild<GUIText>("off", "OFF", 38, 1, 10.0f, ALIGN_RIGHT);

			}

			void setOn(bool on){
				if (on != m_on){
					m_on = on;
					m_updated = true;

					getChild<GUIText>("off").setVisible(!m_on);
					getChild<GUIText>("on").setVisible(m_on);
					getChild<GUIText>("switch").setLocation(m_on ? 25 : 0, 0);

					m_color = (m_on ? glm::vec4(0.7, 0.4, 0.1, 0.0) :
						glm::vec4(0.08, 0.08, 0.08, 0.0));
				}		
			}


			bool isOn() const{
				return m_on;
			}


			//TODO DUNNO MAYBE REMOVE
			void setToggleAction(std::function<void()> &&action){
				m_action = std::move(action);
			}

		};

	}
}

#endif //GUI_TOGGLE_HPP