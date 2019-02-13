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


#ifndef GUI_LIST_HPP
#define GUI_LIST_HPP
#include "GUIBase.hpp"
#include "GUIButton.hpp"
#include "GUIText.hpp"

namespace Engine{
	namespace GUI{

		class GUIList : public GUIBase{
		protected:
			std::vector<std::string> m_values;
			int m_height;

		public:
			GUIList(
				const std::string &name,
				int x, int y, 
				int scaleX, int height, 
				int border = 0,
				const std::vector<std::string> &values = {}
			)
				: GUIBase(name, x, y, scaleX, 0, border)
			{					
				m_height = height;
				m_color = c_elementColor1;

				for (auto &_value : values){
					addItem(_value, []{});
				}
			}

			~GUIList(){

			}

		
			bool hasValue(const std::string &value){
				return std::find(m_values.begin(), m_values.end(), value) != m_values.end();
			}

			virtual GUIButton &addItem(
				const std::string &value, 
				const std::function<void()> &action)
			{
				if (hasValue(value)){
					ERROR("Value already in dropdown list!");
					return getChild<GUIButton>(value);
				}

				m_values.push_back(value);
				int _y = m_values.size() * (m_height + 6);

				auto &_button = addChild<GUIButton>(
					value, 0, -_y, m_scaleX, m_height + 4, m_border);
				_button.setAction(action);

				auto &_text = _button.addChild<GUIText>
					(value, value, 4, 0, static_cast<float>(m_height), ALIGN_LEFT);
	
				return _button;
			}

			template <typename... Args>
			void addItems(Args&&... args){
				for (auto _str : { std::forward<Args>(args)... }){
					addItem(_str, []{});
				}
			}

		};

	}
}

#endif //GUI_LIST_HPP