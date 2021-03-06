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


#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <array>
#include <SDL2\SDL_events.h>

#define KEYBOARD_BUTTON(button) SDL_SCANCODE_##button


namespace Engine{
	namespace Input{

		/*enum MouseInput{
			INPUT_MOUSE_LEFT_DOWN	= 1 << 0,
			INPUT_MOUSE_LEFT_UP		= 1 << 1,
			INPUT_MOUSE_LEFT_HOLD	= 1 << 2,
			INPUT_MOUSE_RIGHT_DOWN	= 1 << 3,
			INPUT_MOUSE_RIGHT_UP	= 1 << 4,
			INPUT_MOUSE_RIGHT_HOLD	= 1 << 5,
			INPUT_MOUSE_MIDDLE_DOWN = 1 << 6,
			INPUT_MOUSE_MIDDLE_UP	= 1 << 7,
			INPUT_MOUSE_MIDDLE_HOLD	= 1 << 8
		};*/


		struct Input{
		private:
			std::array<Uint8, SDL_NUM_SCANCODES> m_buttonHold;
			std::array<Uint8, SDL_NUM_SCANCODES> m_buttonDown;
			std::array<Uint8, SDL_NUM_SCANCODES> m_buttonUp;

		public:
			bool m_mouseEnabled = true;
			bool m_keyboardEnabled = true;

		public:
			SDL_Keymod m_keymod = SDL_Keymod::KMOD_NONE;

			int	 m_mouseX = 0;
			int  m_mouseY = 0;

			int  m_buttonLastDown			= 0;
			int  m_buttonLastUp				= 0;

			int  m_mouseLeftNumClicks		= 0;
			bool m_mouseLeftDoubleClick		= false;
			bool m_mouseLeftDown			= false;
			bool m_mouseLeftUp				= false;
			bool m_mouseLeftHold			= false;

			int  m_mouseRightNumClicks		= 0;
			bool m_mouseRightDoubleClick	= false;
			bool m_mouseRightDown			= false;
			bool m_mouseRightUp				= false;
			bool m_mouseRightHold			= false;

			int  m_mouseMiddleNumClicks		= 0;
			bool m_mouseMiddleDoubleClick	= false;
			bool m_mouseMiddleDown			= false;
			bool m_mouseMiddleUp			= false;
			bool m_mouseMiddleHold			= false;

			bool m_mouseMotion				= false;
			bool m_mouseWheelUp				= false;
			bool m_mouseWheelDown			= false;
			bool m_applicationExit			= false;

		public:
			void script_addRef(){}

			void script_delRef(){}

		public:
			Input();

			~Input();


			void reset();


			bool isButtonHeld(int index) const;

			bool isButtonDown(int index) const;

			bool isButtonUp(int index) const;


			bool isWalking() const;


			void handleKeyMod();

			void updateUserInput();

		}; 

	}
}

#endif //USER_INPUT_HPP