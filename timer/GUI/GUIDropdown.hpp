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


#ifndef DROPDOWN_HPP
#define DROPDOWN_HPP
#include "GUIText.hpp"

namespace Engine{
	namespace GUI{

		class GUIDropdown : public GUIBase{
		private:
			int m_selectedId;
			std::string m_selected;
			std::vector<std::string> m_values;
			unsigned int m_arrowLeftTexture;
			unsigned int m_arrowDownTexture;
			bool m_opened;
			std::function<void(std::string)> m_onChangeActionFunction;

		private:
			void updateVisibility(){
				auto &_drop = getChild<GUIButton>("selected").getChild<GUIButton>("drop");
				_drop.setTexture(m_opened ? m_arrowDownTexture : m_arrowLeftTexture);

				for (auto _it = m_children.begin(); _it != m_children.end(); ++_it){
					if (_it != m_children.begin()){
						auto _child = (*_it).second;
						_child->setVisible(!_child->isVisible());
					}
				}
			}

		public:
			GUIDropdown(
				const std::string &name,
				int x, int y, 
				int scaleX, int scaleY, 
				int border = 0,
				const std::vector<std::string> &values = {}
			)
				: GUIBase(name, x, y, scaleX, scaleY, border)
			{

				m_color = c_elementColor2;

				m_selected = "-";
				m_opened = false;

				m_arrowLeftTexture = Textures::load2DTexture("icons/arrowLeft.png", false);
				m_arrowDownTexture = Textures::load2DTexture("icons/arrowDown.png", false);

				auto &_button = addChild<GUIButton>("selected", 0, 0, m_scaleX, m_scaleY + 4, border);
				auto &_text = _button.addChild<GUIText>("selected", m_selected, 4, 0, 
					static_cast<float>(m_scaleY), ALIGN_LEFT);
				_text.updateAlignment();

				auto &_drop = _button.addChild<GUIButton>("drop",
					m_scaleX - m_scaleY, 4, m_scaleY - 4, m_scaleY - 4, 0, m_arrowLeftTexture);
				_drop.setColor(glm::vec4(0.8f, 0.4f, 0.1f, 0.0f));

				_button.setAction([this]{
					m_opened = !m_opened;		
					updateVisibility();
				});

				for (auto &_str : values){
					addItem(_str);
				}

			}

			~GUIDropdown(){
				glDeleteTextures(1, &m_arrowLeftTexture);
				glDeleteTextures(1, &m_arrowDownTexture);
			}


			void setOnChangeAction(const std::function<void(std::string)> &action){
				m_onChangeActionFunction = action;
			}

			void setValue(const std::string &value){
				auto _itr = std::find(m_values.begin(), m_values.end(), value);
				if (_itr == m_values.end()){			
					ERROR("Can't set value: value not in dropdown");
					ASSERT(0, "Value not in the dropdown");
					return;
				}

				auto &_selected = getChild<GUIButton>("selected").getChild<GUIText>("selected");
				auto &_value = getChild<GUIButton>(value).getChild<GUIText>(value);

				if (_value.getString() != _selected.getString()){
					_selected.setString(_value.getString(), ALIGN_LEFT);
					m_selected = value;

					//Call onChangeFunction
					if (m_onChangeActionFunction){
						m_onChangeActionFunction(value);
					}
				}
			}

			void setValue(std::size_t index){
				ASSERT(index < m_values.size(), "Index out of range");

				setValue(m_values[index]);
			}

			const std::string &getValue() const{
				return m_selected;
			}

			int getValueId() const{
				return m_selectedId;
			}

			void afterDraw() override{
				if (!m_active && m_opened){
					m_opened = false;
					updateVisibility();
				}

				m_active = m_opened;
			}

			void addItem(const std::string &value){
				if (std::find(m_values.begin(), m_values.end(), value) != m_values.end()){
					ERROR("Value already in dropdown list!");
					return;
				}

				m_values.push_back(value);
				int _y = m_values.size() * (m_scaleY + 6);

				auto &_button = addChild<GUIButton>(value, 0, -_y, m_scaleX, m_scaleY + 4, m_border);
				auto &_text = _button.addChild<GUIText>
					(value, value, 4, 0, static_cast<float>(m_scaleY), ALIGN_LEFT);
				_text.updateAlignment();

				_button.setVisible(false);

				int _id = m_values.size() - 1;
				_button.setAction([this, value, _id]{
					setValue(value);
					m_selectedId = _id;
					m_opened = false;
					m_updated = true;
					updateVisibility();
				});


				/*int _scaleX = _text.getScale().x;
				if (m_scaleX < _scaleX){
					m_scaleX = _scaleX;

					for (auto &_child : m_children){//resize button
						auto _button = _child.second;
						_button->setScale(m_scaleX + 4, _button->getScale().y);

						auto _text = _button->getChild<GUIText>(_button->getName());
						_text.updateAlignment();
					}
				}*/
			}

			template <typename... Args>
			void addItems(Args&&... args){
				for (auto _str : { std::forward<Args>(args)... }){
					addItem(_str);
				}
			}

		};

	}
}


#endif //DROPDOWN_HPP