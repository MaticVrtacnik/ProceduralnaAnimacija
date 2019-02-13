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


#ifndef GUI_TAB_HPP
#define GUI_TAB_HPP

#include "GUIWindow.hpp"
#include "../Define/Assert.hpp"

namespace Engine{
	namespace GUI{

		class GUITab : public GUIBase{
		private:
			GUIWindow *m_opened;
			unsigned int m_leftArrowTexture;
			unsigned int m_rightArrowTexture;
			std::vector<unsigned int> m_textures;

		public:
			GUITab(
				const std::string &name, 
				int x, int y, 
				int scaleX, int scaleY, 
				int border = 0, 
				unsigned int texture = 0
			)
				: GUIBase(name, x, y, scaleX, scaleY, border)
			{
				m_texture = texture;
				m_color = c_windowColor;
	
				m_opened = nullptr;
				
				m_leftArrowTexture = Textures::load2DTexture("icons/arrowLeft.png", false);
				m_rightArrowTexture = Textures::load2DTexture("icons/arrowRight.png", false);

				auto &_left = addChild<GUIButton>("left", 
					3, m_scaleY - 35, 20, 20, 0, m_leftArrowTexture);

				_left.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				_left.setAction([this]{
				
				});


				auto &_right = addChild<GUIButton>("right", 
					m_scaleX - 30, m_scaleY - 35, 20, 20, 0, m_rightArrowTexture);

				_right.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				_right.setAction([this]{
				
				});
			}

			~GUITab(){
				glDeleteTextures(1, &m_leftArrowTexture);
				glDeleteTextures(1, &m_rightArrowTexture);

				for (auto &_texture : m_textures)
					glDeleteTextures(1, &_texture);
			}

			void addWindow(GUIWindow *window){
				ASSERT(window != nullptr, "Window is a null pointer!");

				int _offset = 35 + ((m_children.size() - 2) * 50);
				
				std::string _str = window->getName();
				m_textures.push_back(Textures::load2DTexture("icons/" + _str + ".png", false));
				
				auto &_button = addChild<GUIButton>(
					_str, _offset, m_scaleY - 45, 50, 40, 1, m_textures.back());
				_button.setColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

				_button.addChild(window);	
				window->setVisible(false);
				window->setScale(m_scaleX, m_scaleY - 27);
				window->setLocation(-_offset - 3, -m_scaleY + 27);
				window->updateScroll(0, 50);
				
				if (m_opened == nullptr){
					m_opened = window;
					m_opened->setVisible(true);		

					auto &_loc = _button.getRelativeLocation();
					_button.setLocation(_loc.x, _loc.y - 5);
					_button.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				}

				_button.setAction([this, &_button, _str]{
					m_opened->setVisible(false);
					auto _parent = m_opened->getParent();
					auto &_loc = _parent->getRelativeLocation();
					_parent->setLocation(_loc.x, _loc.y + 5);
					_parent->setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

					m_opened = &_button.getChild<GUIWindow>(_str);
					m_opened->setVisible(true);

					_parent = m_opened->getParent();
					_loc = _parent->getRelativeLocation();
					_parent->setLocation(_loc.x, _loc.y - 5);
					_parent->setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				});
			}

			GUIWindow &getWindow(const std::string &name){
				return getChild<GUIButton>(name).getChild<GUIWindow>(name);
			}

		};

	}
}


#endif //GUI_TAB_HPP