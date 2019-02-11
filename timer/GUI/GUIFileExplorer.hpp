#ifndef GUI_FILE_EXPLORER_HPP
#define GUI_FILE_EXPLORER_HPP
//#include <boost\filesystem.hpp>
//#include <boost\algorithm\string.hpp>

#include "GUIWindow.hpp"
#include "GUITreeView.hpp"
#include "GUITextBox.hpp"


namespace Engine{
	namespace GUI{
		//using namespace boost;

		class GUIFileExplorer : public GUIWindow{
		private:
			std::string m_directory;
			GUITextBox *m_textBox;

			void updateParent(const std::string &selected){
				if (m_textBox != nullptr){
					m_textBox->setValue(selected);
					m_textBox->setUpdated(true);
					m_visible = false;
				}
			}

			void createTree(const std::string &dir){
				/*filesystem::path _path(dir);
				if (!filesystem::exists(_path)) return;

				auto &_tree = getChild<GUITreeView>("index");

				//auto _s = SDL_GetTicks();

				filesystem::recursive_directory_iterator _end;
				for (filesystem::recursive_directory_iterator _it(_path); _it != _end; ++_it){
					const filesystem::path _p = *_it; 
					std::vector<std::string> _strings;
					split(_strings, _p.string(), is_any_of("\\"));
					std::string _last = _strings.back();

					//std::cout << _last << "\n";
					auto &_button = _tree.addNode(std::move(_strings));
					auto &_text = _button.getChild<GUIText>(_button.getName());
					_text.setAction([this, _last]{ updateParent(_last); });
				}*/
				
				//_tree.updateLocations();
				//PRINT("TOOK " << SDL_GetTicks() - _s)
			}

		public:
			GUIFileExplorer(
				const std::string &name,
				int x, int y, int scaleX, int scaleY,
				const std::string &directory = "data/textures",
				int border = 0
			)
				: GUIWindow(name, x, y, scaleX, scaleY, true, border, RESIZABLE_ALL)
			{
				m_textBox = nullptr;
				m_color = c_elementColor1;
				m_directory = directory;

				auto &_tree = addChild<GUITreeView>("index", 
					15, m_scaleY - 35, 15.0f, "index");
				createTree(m_directory);
			}

			~GUIFileExplorer(){

			}

			void setTextBox(GUITextBox *textBox){
				m_textBox = textBox;
			}

			/*const std::string &getSelectedValue() const{
				return m_selected;
			}

			void setSelectedValue(const std::string &selected){
				m_selected = selected;
			}*/

		};

	}
}


#endif //GUI_FILE_EXPLORER_HPP