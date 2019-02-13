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


#ifndef GUI_SELECTOR_LIST_HPP
#define GUI_SELECTOR_LIST_HPP
#include "GUIList.hpp"

namespace Engine{
	namespace GUI{

		class GUISelectorList : public GUIList{
		private:

		public:
			GUISelectorList(
				const std::string &name, 
				int x, int y, int scaleX, int scaleY, int border = 0, 
				const std::vector<std::string> &values = {}
			) 
				: GUIList(name, x, y, scaleX, scaleY, border, values)
			{
				m_visible = false;
				m_color = glm::vec4(-1);
			}

			~GUISelectorList(){

			}

			void afterDraw() override{
				m_visible = m_active;
				/*if (!m_active && m_visible){
					//m_visible = false;
				}*/
			}

			GUIButton &addItem(const std::string &value, const std::function<void()> &action) override{
				GUIList::addItem(value, action);
				auto &_button = getChild<GUIButton>(value);
				_button.setAction([action, this]{ action(); m_visible = false; });
				return _button;
			}

		};

	}

}

#endif //GUI_SELECTOR_LIST_HPP