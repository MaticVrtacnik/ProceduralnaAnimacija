#pragma once
#ifndef WORLD_VARIABLES_HPP
#define WORLD_VARIABLES_HPP

#include <glm\glm.hpp>
#include <memory>


namespace Engine { 
	namespace Physics{ class Collision; }
	namespace Camera{ class Camera; }
	namespace Input{ struct Input; }
	namespace GUI{ class GUI; }
	namespace Graphics{ class Renderer; }
	namespace Entities{ class World; }
}


namespace Engine{

	struct Variables{
	public:
		Entities::World *m_world = nullptr;

		std::unique_ptr<Camera::Camera> m_camera;
		std::unique_ptr<Input::Input> m_input;
		std::shared_ptr<Physics::Collision> m_collision;
		std::unique_ptr<GUI::GUI> m_gui;
		std::unique_ptr<Graphics::Renderer> m_renderer;

	public:
		int m_screenWidth;
		int m_screenHeight;
		float m_fieldOfView;

		//glm::mat4 m_projectionMatrix;
		glm::mat4 m_mouseVectorProjectionMatrix;
		glm::vec3 m_mouseVector =  glm::vec3(0.0f);
		bool m_editing = true;

	public:
		Variables(Entities::World *world);

		~Variables();


		void update(float fps);			

	};

}

#endif //WORLD_VARIABLES_HPP