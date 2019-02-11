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