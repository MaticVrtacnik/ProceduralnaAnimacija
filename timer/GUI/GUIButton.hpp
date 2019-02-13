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


#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP
#include "GUIBase.hpp"
#include <functional>

namespace Engine{
	namespace GUI{
		
		class GUIButton : public GUIBase{
		private:
			std::function<void()> m_action;
			glm::vec4 m_originalColor;

		public:
			GUIButton(
				const std::string &name, 
				int x, int y, 
				int scaleX, int scaleY, 
				int border = 0,
				unsigned int texture = 0
			)
				: GUIBase(name, x, y, scaleX, scaleY, border)
			{	
				m_texture = texture;
				m_color = c_elementColor2;
				m_originalColor = m_color;
				//m_color = glm::vec4(0.2f, 0.2f, 0.21f, 0.0f);
			}

			~GUIButton(){

			}

			void setAction(const std::function<void()> &action){
				m_action = action;
			}

			const std::function<void()> &getAction() const{
				return m_action;
			}

			void onHover() override{

			}

			void onMouseDown(int x, int y, bool left) override{
				if (left){
					if (m_action)m_action();
		
					//m_color -= glm::vec4(0.1, -0.1, 0.1, 0.0);
				}
			}

			void onMouseUp(int x, int y, bool left) override{
				if (left){
					//m_color += glm::vec4(0.1, -0.1, 0.1, 0.0);
				}
			}

			void onChildAdded(GUIBase *child) override{
				//if (child == nullptr) return; //TODO

				/*auto _scale = child->getScale();
				auto _location = child->getLocation();
		
				if (m_x > _location.x - 2){
					m_x = _location.x - 2;
				}if (m_y > _location.y - 2){
					m_y = _location.y - 2;
				}

				if (m_scaleX < _scale.x + 2){
					m_scaleX = _scale.x + 2;
				}if (m_scaleY < _scale.y + 2){
					m_scaleY = _scale.y + 2;
				}*/

				//onChildAdded(child->getParent());
			}

		};
	}
}

#endif //GUI_BUTTON_HPP