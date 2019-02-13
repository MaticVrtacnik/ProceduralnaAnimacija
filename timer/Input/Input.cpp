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


#include "Input.hpp"

#include <iostream>


namespace Engine{
	namespace Input{

		Input::Input(){
			//m_buttonHold = SDL_GetKeyboardState(nullptr);
		}

		Input::~Input(){
			
		}


		void Input::reset(){
			//reset all keyboard states to 0
			//m_buttonHold.fill(0);
			//std::fill(m_buttonHold.begin(), m_buttonHold.end(), 0);

			m_buttonLastDown = 0;
			m_buttonLastUp = 0;
			m_mouseLeftNumClicks = 0;
			m_mouseRightNumClicks = 0;
			m_mouseMiddleNumClicks = 0;
			m_mouseLeftDoubleClick = false;
			m_mouseRightDoubleClick = false;
			m_mouseMiddleDoubleClick = false;
			m_mouseLeftDown = false;
			m_mouseRightDown = false;
			m_mouseMiddleDown = false;
			m_mouseLeftUp = false;
			m_mouseRightUp = false;
			m_mouseMiddleUp = false;
			m_mouseMotion = false;
			m_mouseWheelUp = false;
			m_mouseWheelDown = false;
		}



		bool Input::isButtonHeld(int index) const{
			return m_keyboardEnabled && m_buttonHold[index];
		}

		bool Input::isButtonDown(int index) const{
			return m_keyboardEnabled && m_buttonDown[index];
		}

		bool Input::isButtonUp(int index) const{
			return m_keyboardEnabled && m_buttonUp[index];
		}

		bool Input::isWalking() const{
			return isButtonHeld(SDL_SCANCODE_W) || isButtonHeld(SDL_SCANCODE_A) ||
				isButtonHeld(SDL_SCANCODE_S) || isButtonHeld(SDL_SCANCODE_D);
		}

		void Input::handleKeyMod(){
			SDL_Keymod _keymod = m_keymod;
			/*if (mod & KMOD_NUM) printf("NUMLOCK ");
			if (mod & KMOD_CAPS) printf("CAPSLOCK ");
			if (mod & KMOD_LCTRL) printf("LCTRL ");
			if (mod & KMOD_RCTRL) printf("RCTRL ");
			if (mod & KMOD_RSHIFT) printf("RSHIFT ");
			if (mod & KMOD_LSHIFT) printf("LSHIFT ");
			if (mod & KMOD_RALT) printf("RALT ");
			if (mod & KMOD_LALT) printf("LALT ");
			if (mod & KMOD_CTRL) printf("CTRL ");
			if (mod & KMOD_SHIFT) printf("SHIFT ");
			if (mod & KMOD_ALT) printf("ALT ");*/
		}



		void Input::updateUserInput(){
			this->reset();
			SDL_GetMouseState(&m_mouseX, &m_mouseY);

			const Uint8 *_keyboardState = SDL_GetKeyboardState(nullptr);
			auto _oldState = m_buttonHold;
			std::copy(_keyboardState, _keyboardState + SDL_NUM_SCANCODES, m_buttonHold.begin());


			//Set value to 1 if key was just pressed down
			for (int i = 0; i < SDL_NUM_SCANCODES; ++i){
				m_buttonDown[i] = !_oldState[i] && m_buttonHold[i];
			}

			//Set value to 1 if key was just lifted up
			for (int i = 0; i < SDL_NUM_SCANCODES; ++i){			
				m_buttonUp[i] = _oldState[i] && !m_buttonHold[i];
			}


			//std::cout << m_keyboardEnabled << std::endl;

			SDL_Event m_event;
			while (SDL_PollEvent(&m_event)){
				switch (m_event.type){
					case SDL_QUIT:
					{
						m_applicationExit = true;

						break;
					}
					case SDL_KEYDOWN:
					{
						if (!m_keyboardEnabled)break;

						m_buttonLastDown = m_event.key.keysym.sym;
						m_keymod = SDL_GetModState();
						handleKeyMod();

						m_applicationExit = (m_buttonLastDown == SDLK_ESCAPE);

						break;
					}
					case SDL_KEYUP:
					{
						if (!m_keyboardEnabled)break;

						m_buttonLastUp = m_event.key.keysym.sym;

						break;
					}
					case SDL_MOUSEMOTION:
					{
						if (!m_mouseEnabled)break;

						m_mouseMotion = true;

						break;
					}
					case SDL_MOUSEWHEEL:
					{
						if (!m_mouseEnabled)break;
						//std::cout << "X: " << event.wheel.x << " Y: " << event.wheel.y << std::endl;

						if (m_event.wheel.y > 0)
							m_mouseWheelUp = true;
						else m_mouseWheelDown = true;

						break;
					}
					case SDL_MOUSEBUTTONDOWN:
					{
						if (!m_mouseEnabled)break;
						//std::cout << event.button.clicks << std::endl;

						if (m_event.button.button == SDL_BUTTON_LEFT){
							m_mouseLeftNumClicks = m_event.button.clicks;
							m_mouseLeftDoubleClick = m_event.button.clicks > 1;
							m_mouseLeftDown = true;
							m_mouseLeftHold = true;
						} else if (m_event.button.button == SDL_BUTTON_RIGHT){
							m_mouseRightNumClicks = m_event.button.clicks;
							m_mouseRightDoubleClick = m_event.button.clicks > 1;
							m_mouseRightDown = true;
							m_mouseRightHold = true;
						} else if (m_event.button.button == SDL_BUTTON_MIDDLE){
							m_mouseMiddleNumClicks = m_event.button.clicks;
							m_mouseMiddleDoubleClick = m_event.button.clicks > 1;
							m_mouseMiddleDown = true;
							m_mouseMiddleHold = true;
						}

						break;
					}
					case SDL_MOUSEBUTTONUP:
					{
						if (!m_mouseEnabled)break;

						if (m_event.button.button == SDL_BUTTON_LEFT){
							m_mouseLeftDown = false; //TODO test
							m_mouseLeftUp = true;
							m_mouseLeftHold = false;
						} else if (m_event.button.button == SDL_BUTTON_RIGHT){
							m_mouseRightDown = false; //TODO test
							m_mouseRightUp = true;
							m_mouseRightHold = false;
						} else if (m_event.button.button == SDL_BUTTON_MIDDLE){
							m_mouseMiddleDown = false; //TODO test
							m_mouseMiddleUp = true;
							m_mouseMiddleHold = false;
						}

						break;
					}

					default: break;
				}
			}

			//m_mouseEnabled = true;
			//m_keyboardEnabled = true;
		}

	}
}
