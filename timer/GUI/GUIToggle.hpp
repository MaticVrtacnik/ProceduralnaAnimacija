#ifndef GUI_TOGGLE_HPP
#define GUI_TOGGLE_HPP
#include "GUIBase.hpp"
#include "GUIButton.hpp"
#include "GUIText.hpp"

namespace Engine{
	namespace GUI{

		class GUIToggle : public GUIBase{
		private:
			bool m_on;
			std::function<void()> m_action;

			void onMouseDown(int x, int y, bool left){
				if (m_action)m_action();
				setOn(!m_on);
			}

		public:
			GUIToggle(
				const std::string &name, 
				int x, int y, 
				bool on = false
			)
				: GUIBase(name, x, y, 40, 15)
			{
				m_on = on;
				m_color = glm::vec4(0.08, 0.08, 0.08, 0.0); //off
			
				//unsigned int _texture = Textures::load2DTexture("icons/radioChecked.png", false);
				auto &_switch = addChild<GUIButton>("switch", 0, 0, 15, 15);
				_switch.setColor(c_elementColor2);

				addChild<GUIText>("on", "ON", 5, 1, 10.0f).setVisible(false);
				addChild<GUIText>("off", "OFF", 38, 1, 10.0f, ALIGN_RIGHT);

			}

			void setOn(bool on){
				if (on != m_on){
					m_on = on;
					m_updated = true;

					getChild<GUIText>("off").setVisible(!m_on);
					getChild<GUIText>("on").setVisible(m_on);
					getChild<GUIText>("switch").setLocation(m_on ? 25 : 0, 0);

					m_color = (m_on ? glm::vec4(0.7, 0.4, 0.1, 0.0) :
						glm::vec4(0.08, 0.08, 0.08, 0.0));
				}		
			}


			bool isOn() const{
				return m_on;
			}


			//TODO DUNNO MAYBE REMOVE
			void setToggleAction(std::function<void()> &&action){
				m_action = std::move(action);
			}

		};

	}
}

#endif //GUI_TOGGLE_HPP