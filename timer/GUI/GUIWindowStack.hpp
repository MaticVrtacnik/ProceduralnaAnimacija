#ifndef GUI_WINDOW_STACK_HPP
#define GUI_WINDOW_STACK_HPP
#include "GUIWindow.hpp"

namespace Engine{
	namespace GUI{

		class GUIWindowStack : public GUIBase{
		private:
			unsigned int m_arrowRightTexture;
			unsigned int m_arrowDownTexture;

		public:
			GUIWindowStack(
				const std::string &name,
				int x, int y, 
				int maxWidth, 
				int border = 0, 
				unsigned int texture = 0
			)
				: GUIBase(name, x, y, maxWidth, 0, border)
			{
				m_texture = texture;
				//m_scaleY = 100; //TODO wut
				//m_color = c_elementColor2;

				m_arrowRightTexture = Textures::load2DTexture("icons/arrowRight.png", false);
				m_arrowDownTexture = Textures::load2DTexture("icons/arrowDown.png", false);
			}

			~GUIWindowStack(){
				glDeleteTextures(1, &m_arrowRightTexture);
				glDeleteTextures(1, &m_arrowDownTexture);
			}

			void updateLocations(){
				for (std::size_t i = 1; i < m_insertionOrder.size(); ++i){
					auto _topCaption = m_children[m_insertionOrder[i - 1]];
					auto _bottomCaption = m_children[m_insertionOrder[i]];

					auto &_topWindow = _topCaption->getChild<GUIWindow>(_topCaption->getName());

					int _offset = 17;
					if (_topWindow.isVisible())
						_offset += _topWindow.getScale().y;

					int _locX = _bottomCaption->getRelativeLocation().x;
					int _locY = _topCaption->getRelativeLocation().y - _offset;
					_bottomCaption->setLocation(_locX, _locY);
				}
			}

			void addWindow(GUIWindow *window){//x and y should be 0... scales normal
				ASSERT(window != nullptr, "Window is a null pointer");
				auto &_loc = window->getLocation();
				int _scaleY = window->getScale().y;
				window->setScale(m_scaleX, _scaleY); //set to parent width

				int _offset = 0;
				if (!m_children.empty()){
					auto _button = m_children[m_insertionOrder.back()];
					auto _scale = _button->getChildren()[_button->getInsertionOrder()[1]]->getScale().y;
					_offset += _button->getRelativeLocation().y - _scale;
				}

				auto &_caption = addChild<GUIButton>(
					window->getName(), 0, _offset - 17, m_scaleX, 15);
				_caption.setColor(glm::vec4(0.08));
				_caption.addChild<GUIText>("caption", window->getName(), 20, 1, 12.0f);

				_caption.addChild(window);
				window->setLocation(0, -_scaleY);
				
				auto &_closeButton = _caption.addChild<GUIButton>(
					window->getName() + "C", 2, 2, 11, 11, 0, m_arrowDownTexture);
				_closeButton.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

				_caption.setAction([window, &_closeButton, this]{
					window->setVisible(!window->isVisible());
					_closeButton.setTexture(window->isVisible() ? 
					m_arrowDownTexture : m_arrowRightTexture);

					updateLocations();
				});

				

			}

			void setWindowVisible(
				const std::string &name, 
				bool visible, 
				bool updateLocation = true)
			{
				auto &_caption = getChild<GUIButton>(name);
				auto &_window = _caption.getChild<GUIButton>(name);
				//if (_window.isVisible() == visible) return;

				_window.setVisible(visible);
				auto &_button = _caption.getChild<GUIButton>(name + "C");
				_button.setTexture(visible ? m_arrowDownTexture : m_arrowRightTexture);

				if (updateLocation)updateLocations();
			}
			
			void openWindow(				
				const std::string &name, 
				bool updateLocation = true)
			{
				setWindowVisible(name, true);
			}

			void closeWindow(
				const std::string &name, 
				bool updateLocation = true)
			{
				setWindowVisible(name, false);
			}

			GUIWindow &getWindow(const std::string &name){
				return getChild<GUIButton>(name).getChild<GUIWindow>(name);
			}

		};

	}
}

#endif //GUI_WINDOW_STACK_HPP