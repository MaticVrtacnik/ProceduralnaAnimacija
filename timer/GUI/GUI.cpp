#include "GUI.hpp"

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <glm/gtx/transform.hpp>

#include "../Shader/Shader.hpp"
#include "GUICreator.hpp"
#include "GUIWindow.hpp"
#include "GUITab.hpp"
#include "GUIWindowStack.hpp"


namespace Engine{
	namespace GUI{

		GUI::GUI(int screenWidth, int screenHeight){
			TTF_Init();
			m_gui = std::make_unique<GUIBase>("index", 0, 0, screenWidth, screenHeight);

			static std::vector<GLfloat> _vertices({
				-0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f
			});


			m_vertexBuffer.loadBuffer(_vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			m_gui->addChild<GUIButton>("screen", 0, screenHeight, screenWidth, -screenHeight).setColor(glm::vec4(0.0f));
			m_gui->addChild<GUIButton>("center", screenWidth / 2 - 2,
				screenHeight / 2 - 2, 4, 4).setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

			m_gui->addChild<GUIText>("fps", "0ms", 5, 5, 20.0f).setColor(glm::vec4(1.0, 0.0, 0.0, 0.0));


			m_timerNames = {
				"light_scattering", "lens_flare", "motion_blur", "reflections", 
				"ambient_occlusion", "hdr", "lighting", "bloom", "fxaa"
			};
			for (std::size_t i = 0; i < m_timerNames.size(); ++i){
				int _height = screenHeight - 100 - i * 25;

				std::string _startString = Utilities::strUpper(m_timerNames[i]) + ": 0.0000ms";
				m_gui->addChild<GUIText>(m_timerNames[i], _startString, 5, _height, 20.0f)
					.setColor(glm::vec4(1.0, 0.0, 1.0, 0.0));
			}


			createGUI(screenWidth, screenHeight, m_gui.get());

			loadCursors();
			if (!m_cursors.empty()){
				SDL_SetCursor(m_cursors.back());
			}
		}

		GUI::~GUI(){
			TTF_Quit();

			for (auto _surface : m_cursorSurfaces){
				if (_surface)SDL_FreeSurface(_surface);
			}

			for (auto _cursor : m_cursors){
				if (_cursor)SDL_FreeCursor(_cursor);
			}

		}


		void GUI::loadCursors(){
			/*SDL_Surface *_cursorSurface = Textures::getTextureData("rscursor.png");

			if (_cursorSurface){
			m_cursorSurfaces.push_back(_cursorSurface);
			m_cursors.push_back(SDL_CreateColorCursor(_cursorSurface, 0, 0));
			}*/
		}


		GUIWindow &GUI::getTabWindow(const std::string &tab){
			auto &_tabs = getIndex().getChild<GUIWindow>("tab");
			auto &_tab = _tabs.getChild<GUITab>("tab").getWindow(tab);
			return _tab;
		}

		GUIWindow &GUI::getTabStackWindow(
			const std::string &tab, const std::string &window)
		{
			auto &_tab = getTabWindow(tab);
			auto &_window = _tab.getChild<GUIWindowStack>("stack").getWindow("render");
			return _window;
		}

		void GUI::recursiveDraw(GUIBase &element, 
			Shaders::Shader &shader, Input::Input &input)
		{
			auto &_children = element.getChildren();

			for (auto &_key : element.getInsertionOrder()){
				const auto &_child = _children[_key];

				if (_child->isVisible()){
					//TODO NO BUENO
					/*auto &_scale = _ptr->getScale();
					if (_scale.x == 0.0f || _scale.y == 0.0f){
					continue;
					}*/

					_child->handleInput(input);
					/*if (_ptr->isActive()){
					input.reset();
					}*/

					int _border = _child->getBorder();
					if (_border > 0){
						auto &_scale = _child->getScale();
						auto &_loc = _child->getLocation();

						shader.setUniform("u_color", 0.08f, 0.08f, 0.08f, 0.0f);
						Textures::unbind2DTexture();

						shader.setUniform("u_location", _loc.x, _loc.y);
						shader.setUniform("u_scale", _border, _scale.y);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

						shader.setUniform("u_location", _loc.x, _loc.y);
						shader.setUniform("u_scale", _scale.x, _border);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

						shader.setUniform("u_location", _loc.x + _scale.x - _border, _loc.y);
						shader.setUniform("u_scale", _border, _scale.y);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

						shader.setUniform("u_location", _loc.x, _loc.y + _scale.y - _border);
						shader.setUniform("u_scale", _scale.x, _border);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					}

					_child->beforeDraw(shader);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					recursiveDraw(*_child, shader, input);
					_child->afterDraw();

				}
			}
		}

		void GUI::draw(Shaders::Shader &shader, Input::Input &input,
			int screenWidth, int screenHeight, bool editing)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			//glEnable(GL_SCISSOR_TEST);

			shader.bind();
			{
				float _screenWidth = static_cast<float>(screenWidth);
				float _screenHeight = static_cast<float>(screenHeight);
				glm::mat4 _projectionMatrix = glm::ortho(
					0.0f, _screenWidth, 0.0f, _screenHeight, -1.0f, 1.0f);

				shader.setUniform("u_projectionMatrix", _projectionMatrix);


				//m_shader.setUniform("u_texture", 0);

				Textures::unbind2DTexture();

				glEnableVertexAttribArray(0);
				m_vertexBuffer.bind();
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

				if (/*true ||*/ editing){
					recursiveDraw(*m_gui, shader, input);
				} else{
					//draw framebuffer screen, center dot
					for (auto &_name : { "screen",  "center", "fps" }){
						auto &_child = m_gui->getChild<GUIBase>(_name);
						_child.handleInput(input);
						_child.beforeDraw(shader);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
						_child.afterDraw();
					}

					/*for (auto &_name : m_timerNames){
						auto &_child = m_gui->getChild<GUIBase>(_name);
						_child.handleInput(input);
						_child.beforeDraw(shader);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
						_child.afterDraw();
					}//*/
				}

				glDisableVertexAttribArray(0);
				m_vertexBuffer.unbind();
			}
			shader.unbind();

			//glDisable(GL_SCISSOR_TEST);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			/*int _mx = input.m_mouseX;
			int _my = input.m_mouseY;
			auto &_loc1 = m_gui.getLocation();
			auto &_loc2 = _loc1 + m_gui.getScale();
			if (_mx >= _loc1.x && _mx <= _loc2.x &&
			_my >= _loc1.y && _my <= _loc2.y){
			input.reset();
			}*/

			m_beingUsed = false;
			auto &_insertionOrder = m_gui->getInsertionOrder();
			auto &_children = m_gui->getChildren();
			for (std::size_t i = 2; i < _insertionOrder.size(); ++i){
				auto &_child = _children[_insertionOrder[i]];
				if (_child->isHovering() && _child->isVisible()){
					m_beingUsed = true;
					break;
				}
			}

		}


		GUIBase &GUI::getIndex() const{
			return *m_gui;
		}

		bool GUI::isBeingUsed() const{
			return m_beingUsed;
		}

		void GUI::setBeingUsed(bool used){
			m_beingUsed = used;
		}

	}
}
