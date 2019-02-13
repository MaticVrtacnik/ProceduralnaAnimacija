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