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


#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include <SDL2\SDL.h>
#include "GUIBase.hpp"
#include "GUIButton.hpp"
#include "GUIText.hpp"

#include "../Utility/StringUtilities.hpp"
#include "../Utility/PrintUtilities.hpp"

namespace Engine{
	namespace GUI{
		using namespace Utilities;

		static enum Resizable{
			RESIZABLE_X = 0x01,
			RESIZABLE_Y = 0x02,
			RESIZABLE_SX = 0x04,
			RESIZABLE_SY = 0x08,
			RESIZABLE_ALL = 0x0F
		};

		class GUIWindow : public GUIBase{
		protected:
			static const int _height = 30;
			static const int _resizeSize = 10;
			static enum ResizeMode{
				RESIZE_X = 0x01,
				RESIZE_Y = 0x02,
				RESIZE_SCALE_X = 0x04,
				RESIZE_SCALE_Y = 0x08
			};		
		

			bool m_visibleOutside;
			bool m_movable;
			bool m_moving;
			int m_movingOffsetX;
			int m_movingOffsetY;
			int m_resizeMode;
			int m_resizableFlags; 

			glm::ivec2 m_maxLocation;

			void onChildAdded(GUIBase *child) override{
				auto _loc = child->getScale() + child->getLocation();
				if (_loc.x > m_maxLocation.x) m_maxLocation.x = _loc.x;
				if (_loc.y > m_maxLocation.y) m_maxLocation.y = _loc.y;
			}

			//SDL_Cursor m_cursor;

			void onMouseDown(int x, int y, bool left) override{
				m_active = true;

				if (left){
					auto &_loc1 = getLocation();
					auto &_loc2 = _loc1 + getScale();

					//TODO resizable for all
					if ((m_resizableFlags & RESIZABLE_X) && 
						x >= _loc1.x && 
						x <= _loc1.x + _resizeSize &&
						y >= _loc1.y + 2 * _resizeSize && 
						y <= _loc2.y - 2 * _resizeSize)
					{
						m_resizeMode |= RESIZE_X;
					}

					else if ((m_resizableFlags & RESIZABLE_Y) &&
						x >= _loc1.x + 2 * _resizeSize && 
						x <= _loc2.x - 2 * _resizeSize &&
						y >= _loc1.y && 
						y <= _loc1.y + _resizeSize)
					{
						m_resizeMode |= RESIZE_Y;
					}

					else if ((m_resizableFlags & RESIZABLE_SX) &&
						x >= _loc2.x - _resizeSize && 
						x <= _loc2.x &&
						y >= _loc1.y + 2 * _resizeSize &&
						y <= _loc2.y - 2 * _resizeSize)
					{
						m_resizeMode |= RESIZE_SCALE_X;
					}

					else if ((m_resizableFlags & RESIZABLE_SY) &&
						x >= _loc1.x + 2 * _resizeSize &&
						x <= _loc2.x - 2 * _resizeSize &&
						y >= _loc2.y - _resizeSize && 
						y <= _loc2.y)
					{
						m_resizeMode |= RESIZE_SCALE_Y;
					}			

					else if ((m_resizableFlags & RESIZABLE_X) &&
						(m_resizableFlags & RESIZABLE_Y) &&
						x >= _loc1.x && 
						x <= _loc1.x + 2 * _resizeSize &&
						y >= _loc1.y && 
						y <= _loc1.y + 2 * _resizeSize)
					{
						m_resizeMode |= RESIZE_X;
						m_resizeMode |= RESIZE_Y;
					}

					else if ((m_resizableFlags & RESIZABLE_SX) &&
						(m_resizableFlags & RESIZABLE_Y) &&
						x >= _loc2.x - 2 * _resizeSize &&
						x <= _loc2.x &&
						y >= _loc1.y && 
						y <= _loc1.y + 2 * _resizeSize)
					{
						m_resizeMode |= RESIZE_SCALE_X;
						m_resizeMode |= RESIZE_Y;
					}

					else if ((m_resizableFlags & RESIZABLE_X) &&
						(m_resizableFlags & RESIZABLE_SY) &&
						x >= _loc1.x &&
						x <= _loc1.x + 2 * _resizeSize &&
						y >= _loc2.y - 2 * _resizeSize && 
						y <= _loc2.y)
					{
						m_resizeMode |= RESIZE_X;
						m_resizeMode |= RESIZE_SCALE_Y;
					}

					else if ((m_resizableFlags & RESIZABLE_SX) &&
						(m_resizableFlags & RESIZABLE_SY) &&
						x >= _loc2.x - 2 * _resizeSize && 
						x <= _loc2.x &&
						y >= _loc2.y - 2 * _resizeSize &&
						y <= _loc2.y)
					{
						m_resizeMode |= RESIZE_SCALE_X;
						m_resizeMode |= RESIZE_SCALE_Y;
					}

					else{
						if (hasChild("caption")){
							auto &_button = getChild<GUIButton>("caption");
							auto &_bloc1 = _button.getLocation();
							auto &_bloc2 = _bloc1 + _button.getScale();
							if (m_movable &&
								x >= _bloc1.x && x <= _bloc2.x &&
								y >= _bloc1.y && y <= _bloc2.y)
							{
								m_movingOffsetX = x;
								m_movingOffsetY = y;
								m_moving = true;
							}
						}					
					}
				}

			}

			void resize(int x, int y, int scaleX, int scaleY){
				m_updated = true;

				m_x += x;
				m_y += y;
				m_scaleX += scaleX;
				m_scaleY += scaleY;

				if (hasChild("caption")){
					auto &_button = getChild<GUIButton>("caption");
					_button.setLocation(0, m_scaleY - _height);
					_button.setScale(m_scaleX - _height, _height);
					_button.getChild<GUIText>("caption").updateAlignment();
				}
				
				if (m_scaleX < 0)m_scaleX = 0;
				if (m_scaleY < 0)m_scaleY = 0;

				int x2 = m_x + m_scaleX;
				int y2 = m_y + m_scaleY;
				if (x2 > m_maxLocation.x) x2 = m_maxLocation.x;
				if (y2 > m_maxLocation.y) y2 = m_maxLocation.y;
				

			}

			void onMouseHold(int x, int y, bool left) override{
				if (left){
					if ((m_resizeMode & RESIZE_X) && (m_resizeMode & RESIZE_Y)){
						resize(x - m_x, y - m_y, m_x - x, m_y - y);
					}else if ((m_resizeMode & RESIZE_SCALE_X) && (m_resizeMode & RESIZE_Y)){
						resize(0, y - m_y, x - (m_x + m_scaleX), m_y - y);
					}else if ((m_resizeMode & RESIZE_X) && (m_resizeMode & RESIZE_SCALE_Y)){
						resize(x - m_x, 0, m_x - x, y - (m_y + m_scaleY));
					}else if ((m_resizeMode & RESIZE_SCALE_X) && (m_resizeMode & RESIZE_SCALE_Y)){
						resize(0, 0, x - (m_x + m_scaleX), y - (m_y + m_scaleY));
					}else if (m_resizeMode & RESIZE_X){
						resize(x - m_x, 0, m_x - x, 0);
					}else if (m_resizeMode & RESIZE_Y){
						resize(0,  y - m_y, 0, m_y - y);
					}else if (m_resizeMode & RESIZE_SCALE_X){
						resize(0, 0, x - (m_x + m_scaleX), 0);
					}else if (m_resizeMode & RESIZE_SCALE_Y){
						resize(0, 0, 0, y - (m_y + m_scaleY));
					}else if (m_moving){
						setLocation(
							m_x + (x - m_movingOffsetX),
							m_y + (y - m_movingOffsetY));

						m_movingOffsetX = x;
						m_movingOffsetY = y;
					}
				}
			}

			void onMouseUp(int x, int y, bool left) override{
				if (left){
					m_resizeMode = 0;
					m_moving = false;
				}
			}

			glm::ivec4 outsideOfWindow(GUIBase *child){
				auto &_loc1 = child->getLocation();
				auto &_loc2 = _loc1 + child->getScale();
				
				if (_loc2.x < _loc1.x){
					int _a = _loc2.x;
					_loc2.x = _loc1.x;
					_loc1.x = _a;
				}

				if (_loc2.y < _loc1.y){
					int _a = _loc2.y;
					_loc2.y = _loc1.y;
					_loc1.y = _a;
				}

				glm::ivec4 _extremes(_loc1.x, _loc1.y, _loc2.x, _loc2.y);

				for (auto _child : child->getChildren()){
					if (!_child.second->isVisible()) continue;

					glm::ivec4 _cExtremes = outsideOfWindow(_child.second);			
					if (_cExtremes.x < _extremes.x) _extremes.x = _cExtremes.x;
					if (_cExtremes.y < _extremes.y) _extremes.y = _cExtremes.y;
					if (_cExtremes.z > _extremes.z) _extremes.z = _cExtremes.z;
					if (_cExtremes.w > _extremes.w) _extremes.w = _cExtremes.w;
				}
				
				return _extremes; /*glm::clamp(_extremes, glm::ivec4(0),glm::ivec4(
					SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT));*/
			}

		public:
			GUIWindow(
				const std::string &name, 
				int x, int y, 
				int scaleX, int scaleY,
				bool movable = true, 
				int border = 0,
				int resizableFlags = 0, 
				unsigned int texture = 0
			)
				: GUIBase(name, x, y, scaleX, scaleY, border)
			{
				m_texture = texture;
				m_color = c_windowColor;

				m_moving = false;
				m_visibleOutside = false;
				m_movable = movable;
				m_resizableFlags = resizableFlags;
				m_maxLocation = glm::ivec2(0.0);
			
				if (movable){
					auto &_caption = addChild<GUIButton>(
						"caption", 0, scaleY - _height - m_border, scaleX - _height, _height, 5);
					_caption.addChild<GUIText>(
						"caption", "", 10, 0, 15.0f).setString("!" + strUpper(m_name), ALIGN_CENTER);

					auto &_close = addChild<GUIButton>(
						"x", scaleX - _height, scaleY - _height - m_border, _height, _height);
					_close.setAction([this]{ m_visible = false; });
					_close.addChild<GUIText>("x", "!X", 3, 5, 15.0f);
				}		

				unsigned int m_arrowUpTexture = Textures::load2DTexture("icons/arrowUp.png", false);
				unsigned int m_arrowDownTexture = Textures::load2DTexture("icons/arrowDown.png", false);


				auto &_vScroll = addChild<GUIButton>("vScroll", 
					m_scaleX - 10 - m_border, 0, 10, m_scaleY - 2 * m_border - (movable ? _height : 0));

				auto &_up = _vScroll.addChild<GUIButton>("up", 
					0, m_scaleY - 10, 10, 10, 0, m_arrowUpTexture);

				_up.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

				auto &_down = _vScroll.addChild<GUIButton>("down", 
					0, 0, 10, 10, 0, m_arrowDownTexture);

				_down.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			}

			void updateScroll(int offsetY = 0, int offsetScaleY = 0){
				if (m_visibleOutside) return;

				auto &_vScroll = getChild<GUIButton>("vScroll");
				_vScroll.setLocation(m_scaleX - 10 - 2 * m_border, 50+ offsetY);
				_vScroll.setScale(_vScroll.getScale().x, _vScroll.getScale().y + offsetScaleY);
			}


			void setCaption(const std::string &caption){
				ASSERT(hasChild("caption"), "No child named caption");
				getChild<GUIText>("caption").setString(caption, ALIGN_CENTER);
			}

			/*const std::string &getCaption(){
				//ASSERT(hasChild("caption"), "No child named caption");
				return getChild<GUIText>("caption").getString();
			}*/

			void beforeDraw(Shaders::Shader &shader) override{
				GUIBase::beforeDraw(shader);


				if (m_name == "tab"){
					//glm::ivec4 _extremes = outsideOfWindow(this);
					/*setAbsoluteLocation(_extremes.x, _extremes.y);
					m_scaleX = _extremes.z - _extremes.x;
					m_scaleY = _extremes.w - _extremes.y;*/

				//	std::cout << m_scaleX << "  " <<  m_scaleY << std::endl;
					//Utilities::print(glm::vec4(_extremes));
				}

				

				if (!m_visibleOutside){
					
				

					glEnable(GL_SCISSOR_TEST);
					auto &_loc = getLocation();
					glScissor(_loc.x + m_border, _loc.y + m_border,
						m_scaleX - 2 * m_border, m_scaleY - 2 * m_border);
				}		
			}

			void afterDraw() override{
				glDisable(GL_SCISSOR_TEST);
			}

			void setMovable(bool movable){
				m_movable = movable;
			}

			bool isMovable() const{
				return m_movable;
			}

			void setVisibleOutside(bool visible){
				m_visibleOutside = visible;
			}

			bool isVisibleOutside() const{
				return m_visibleOutside;
			}
			
		};


	}
}


#endif //GUI_WINDOW_HPP