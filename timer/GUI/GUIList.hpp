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