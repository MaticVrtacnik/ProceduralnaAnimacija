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