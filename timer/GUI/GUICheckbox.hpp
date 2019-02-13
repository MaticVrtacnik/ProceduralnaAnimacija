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


#ifndef GUI_CHECKBOX_HPP
#define GUI_CHECKBOX_HPP
#include "GUIBase.hpp"
#include "GUIButton.hpp"

namespace Engine{
	namespace GUI{

		//TODO
		class GUICheckbox : public GUIBase{
		private:
			bool m_checked;

			void onMouseDown(int x, int y, bool left) override{
				m_checked = !m_checked;
				m_color = !m_checked ? c_elementColor1 :
					glm::vec4(0.3, 0.7, 0.3, 0.0);

			}

		public:
			GUICheckbox(
				const std::string &name, 
				int x, int y, int scale = 40
			)
				: GUIBase(name, x, y, scale, scale)
			{
				m_border = 3;
				m_color = c_elementColor1;
				m_checked = false;
			}

			bool isChecked(){
				return m_checked;
			}

			~GUICheckbox(){

			}

		};
	}
}

#endif //GUI_CHECKBOX_HPP