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


#ifndef GUI_PROGRESS_BAR_HPP
#define GUI_PROGRESS_BAR_HPP
#include "GUIBase.hpp"
#include "GUIText.hpp"
#include "GUIButton.hpp"

namespace Engine{
	namespace GUI{

		class GUIProgressBar : public GUIBase{
		private:
			float m_progress;

			//DEBUG
			/*void onMouseHold(int x, int y, bool left){
				if (left)increaseProgress(1);
				else increaseProgress(-1);
			}*/

		public:
			GUIProgressBar(
				const std::string &name, 
				int x, int y, 
				int scaleX, int scaleY
			)
				: GUIBase(name, x, y, scaleX, scaleY)
			{
				m_color = c_elementColor1;

				addChild<GUIButton>("bar", 0, 0, 0, scaleY)
					.setColor(glm::vec4(0.2, 0.5, 0.2, 0.0));

				addChild<GUIText>("progress","", 0, 0, scaleY - 4.0f, ALIGN_CENTER);

				setProgress(0);
			}

			int getProgress() const{
				return static_cast<int>(m_progress);
			}

			void setProgress(float progress){
				if (progress > 100.0f) 
					m_progress = 100.0f;
				else if (progress < 0.0f)
					m_progress = 0.0f;
				else m_progress = progress;

				float _step = m_scaleX / 100.0f;

				getChild<GUIButton>("bar").setScale(
					static_cast<int>(m_progress * _step), m_scaleY);

				getChild<GUIText>("progress").setString(
					std::to_string(static_cast<int>(m_progress)) + "%", ALIGN_CENTER);
			}

			void increaseProgress(float increase){			
				setProgress(m_progress + increase);		
			}

		};
	}
}


#endif //GUI_PROGRESS_BAR_HPP