#include "WorldVariables.hpp"

#include <glm\gtc\matrix_transform.hpp>
#include "../Component/ComponentIncluder.hpp"

#include "../Utility/MathUtilities.hpp"
#include "../GUI/GUIWindow.hpp"
#include "../GUI/GUITab.hpp"
#include "../World/World.hpp"

#include "../Renderer/DeferredRenderer.hpp"
#include "../GUI/GUI.hpp"
#include "../Collision/Collision.hpp"
#include "../Input/Input.hpp"
#include "../Camera/Camera.hpp"


namespace Engine{
	using namespace Entities;
	using namespace Components;


	Variables::Variables(World *world){
		ASSERT(world != nullptr, "World is a null pointer");
		m_world = world;

		m_screenWidth = DEFAULT_SCREEN_WIDTH;
		m_screenHeight = DEFAULT_SCREEN_HEIGHT;
		m_fieldOfView = DEFAULT_FOV;

		m_input = std::make_unique<Input::Input>();
		m_collision = std::make_shared<Physics::Collision>();
		m_gui = std::make_unique<GUI::GUI>(m_screenWidth, m_screenHeight);
		m_renderer = std::make_unique<Graphics::Renderer>(m_screenWidth, m_screenHeight);
		m_camera = std::make_unique<Camera::Camera>(
			glm::vec3(0.0f, 0.0f, 0.0f), m_screenWidth, m_screenHeight);


		float _screenWidth = static_cast<float>(m_screenWidth);
		float _screenHeight = static_cast<float>(m_screenHeight);
		float _aspectRatio = _screenWidth / _screenHeight;

		auto &_quad = m_gui->getIndex().getChild<GUI::GUIWindow>("tab").
			getChild<GUI::GUITab>("tab").getWindow("quad");
		const int _windowTop = _quad.getScale().y;
		const int _width = _quad.getScale().x;
		const int _height = static_cast<int>(_width / _aspectRatio);

		_quad.addChild<GUI::GUIButton>("post1", 0, _windowTop, _width, -_height, 2).setTexture(
			//_renderer->getShadowFramebuffer().getTexture(0)
			m_renderer->getVelocityTexture()
		);
		//_renderer->getLocationTexture());


		_quad.addChild<GUI::GUIButton>("post2", 0, _windowTop - (_height + 5),
			_width, -_height, 2).setTexture(m_renderer->getLocationTexture());
		_quad.addChild<GUI::GUIButton>("post3", 0, _windowTop - 2 * (_height + 5),
			_width, -_height, 2).setTexture(m_renderer->getAlbedoTexture());
		_quad.addChild<GUI::GUIButton>("post4", 0, _windowTop - 3 * (_height + 5),
			_width, -_height, 2).setTexture(m_renderer->getNormalTexture());
		_quad.addChild<GUI::GUIButton>("post5", 0, _windowTop - 4 * (_height + 5),
			_width, -_height, 2).setTexture(m_renderer->getRoughMetalOccluEmissTexture());

		/*m_projectionMatrix = glm::perspective(
		90.0f, (m_screenWidth * 1.0f) / (m_screenHeight * 1.0f), 0.1f, 10000.0f);*/

			
		m_mouseVectorProjectionMatrix = glm::perspective(
			m_fieldOfView, _aspectRatio, 1.0f, DEFAULT_DRAW_DISTANCE);

		glViewport(0, 0, m_screenWidth, m_screenHeight);
	}

	Variables::~Variables()
	{
	}



	void Variables::update(float fps){

		//
		/*if (SDL_GetTicks() > 10000 && g_h8r){
			if (SDL_GetTicks() - g_counter > 500){
				if (g_counter > 10) g_counter--;
				g_timer = SDL_GetTicks();
					
				int _rx = Utilities::fastRandom(10, m_screenWidth - 100);
				int _ry = Utilities::fastRandom(10, m_screenHeight - 10);

				auto &_text = m_gui.getIndex().addChild<GUI::GUIText>
					(std::to_string(g_timer), "Wow h8r", _rx, _ry, static_cast<float>(30 + g_timer % 50));
				_text.setColor(glm::vec4(Utilities::fastRandom(0.2f, 0.9f), 
					Utilities::fastRandom(0.2f, 0.9f), Utilities::fastRandom(0.2f, 0.9f), 0.8f));
			}

			if (input.m_buttonDown == SDLK_SPACE){
				g_h8r = false;
				auto &_index = m_gui.getIndex();
				auto &_order = _index.getInsertionOrder();

				for (int i = g_numChildren; i < _order.size(); ++i){
					_index.getChildren()[_order[i]]->setVisible(false);
				}
			}
		}*/

		auto &_window = m_gui->getIndex().getChild<GUI::GUIWindow>("tab");
		//REDO 


		/*int _width = m_editing ? 300 : _window.getBorder() * 3;
		_window.setLocation(m_screenWidth - _width, _window.getLocation().y);*/
			
			
		//TODO fps?
		float _fpsFactor = (60.0f / fps);
		m_collision->updateCollisionWorld(60.0f);

		m_gui->getIndex().setVisible(m_editing);
		//m_gui->getIndex().setVisible(true);

		//Update input variables and invert mouseY to match GUI
		m_input->updateUserInput();
		m_input->m_mouseY = m_screenHeight - m_input->m_mouseY;


		//bool _controllCamera = m_input->m_mouseRightHold;
		bool _controlCamera = m_input->isButtonHeld(SDL_SCANCODE_LSHIFT);
		if (!m_editing || _controlCamera){
			m_camera->setMouseIn(true);
			m_camera->control(*m_input, !_controlCamera);
			m_camera->transform(!m_editing);
			//TODO update with camera
			m_mouseVector = m_camera->getVector();
			m_gui->setBeingUsed(false);
		}
		else{
			m_camera->setMouseIn(false);
			//SDL_SetRelativeMouseMode(SDL_FALSE);
				
			m_mouseVector = Utilities::getMouseVector(
				m_screenWidth, m_screenHeight, m_camera->getLocation(),
				m_mouseVectorProjectionMatrix, m_camera->getViewMatrix());
		}

	}

}
