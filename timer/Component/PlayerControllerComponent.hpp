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


#ifndef PLAYER_CONTROLLER_COMPONENT_HPP
#define PLAYER_CONTROLLER_COMPONENT_HPP

#include "Component.hpp"
#include "../Controller/CharacterController.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			class PlayerControllerComponent : 
				public Component, public Controller::CharacterController
			{
			public:			
				float m_latestLeanAngle = 0.0f;
				float m_latestYRotation = 0.0f;
				float m_latestZRotation = 0.0f;

				float m_targetBounceYRotation = 0.0f;

				const float c_defaultAccelerationFactor = 1.75f;
				float m_maxAccelerationFactor = c_defaultAccelerationFactor;
				float m_accelerationFactor = 0.0f;

			public:
				PlayerControllerComponent();

				PlayerControllerComponent(Physics::Collision &collision);

				~PlayerControllerComponent();

			};

		}
	}
}

#endif //PLAYER_CONTROLLER_COMPONENT_HPP