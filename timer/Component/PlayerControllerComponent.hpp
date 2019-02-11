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