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