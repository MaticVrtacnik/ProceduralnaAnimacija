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


#ifndef GUI_SPINNER_HPP
#define GUI_SPINNER_HPP
#include <set>
#include "GUIBase.hpp"
#include "GUIButton.hpp"
#include "GUIText.hpp"

namespace Engine{
	namespace GUI{

		class GUISpinner : public GUIBase{
		private:
			std::vector<std::string> m_values;
			unsigned int m_arrowUpTexture;
			unsigned int m_arrowDownTexture;
			int m_selectedIndex;

		public:
			GUISpinner(
				const std::string &name, 
				int x, int y, 
				int scaleX, int scaleY, 
				const std::vector<std::string> &values = {}
			)
				: GUIBase(name, x, y, scaleX, scaleY)
			{
				m_color = c_elementColor1;
				m_values = values;
				m_selectedIndex = 0;	

				m_arrowUpTexture = Textures::load2DTexture("icons/arrowUp.png", false);
				m_arrowDownTexture = Textures::load2DTexture("icons/arrowDown.png", false);

				auto &_prev = addChild<GUIButton>("previous",
					m_scaleX - m_scaleY, 0, scaleY, scaleY / 2, 0, m_arrowDownTexture);
				_prev.setColor(glm::vec4(0.0));

				auto &_next = addChild<GUIButton>("next", 
					m_scaleX - m_scaleY, scaleY / 2, scaleY, scaleY / 2, 0, m_arrowUpTexture);
				_next.setColor(glm::vec4(0.0));

				addChild<GUIText>("value", m_values.empty() ? "" : m_values[m_selectedIndex], 2, 1, scaleY - 2.0f);


				_prev.setAction([this]{
					if (++m_selectedIndex >= static_cast<int>(m_values.size()))
						m_selectedIndex = 0;
					
					getChild<GUIText>("value").
						setString(m_values[m_selectedIndex], ALIGN_LEFT);
				});

				_next.setAction([this]{
					if (--m_selectedIndex < 0)
						m_selectedIndex = m_values.size() - 1;
					
					getChild<GUIText>("value").
						setString(m_values[m_selectedIndex], ALIGN_LEFT);
				});
				
			}

			~GUISpinner(){
				glDeleteTextures(1, &m_arrowUpTexture);
				glDeleteTextures(1, &m_arrowDownTexture);
			}

			void addValue(const std::string &value){
				auto _itr = std::find(m_values.begin(), m_values.end(), value);
				if (_itr != m_values.end()){
					ASSERT(0, "Value already in spinner");
					return;
				}

				m_values.push_back(value);
			}

			const std::string &getValue() const{
				return m_values[m_selectedIndex];
			}

		};
	}
}


#endif //GUI_SPINNER_HPP
