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