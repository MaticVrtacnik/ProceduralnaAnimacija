#ifndef GUI_HPP
#define GUI_HPP

#include <vector>
#include <memory>

#include "../Buffer/ArrayBuffer.hpp"


struct SDL_Surface;
struct SDL_Cursor;

namespace Engine{ namespace GUI{
	class GUIWindow;
	class GUIBase;
} }

namespace Engine{ namespace Shaders{
	class Shader;
} }

namespace Engine{ namespace Input{
		struct Input;
} }


namespace Engine{
	namespace GUI{
	
		class GUI{
		private:
			std::vector<std::string> m_timerNames;

		private:
			std::vector<SDL_Surface*> m_cursorSurfaces;
			std::vector<SDL_Cursor*> m_cursors;

			std::unique_ptr<GUIBase> m_gui = nullptr;
			Buffers::ArrayBuffer m_vertexBuffer;
			bool m_beingUsed = false;

		public:
			//TODO move screenWidth, screenHeight to init method
			GUI(int screenWidth, int screenHeight);

			~GUI();


			void loadCursors();


			GUI::GUIWindow &getTabWindow(const std::string &tab);

			GUI::GUIWindow &getTabStackWindow(
				const std::string &tab, const std::string &window);


			void recursiveDraw(GUIBase &element, 
				Shaders::Shader &shader, Input::Input &input);

			void draw(Shaders::Shader &shader, Input::Input &input,
				int screenWidth, int screenHeight, bool editing);
			

			GUIBase &getIndex() const;

			bool isBeingUsed() const;

			void setBeingUsed(bool used);

		};

	}
}

#endif //GUI_HPP