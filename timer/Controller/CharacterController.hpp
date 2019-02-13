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


#ifndef CHARACTER_CONTROLLER_HPP
#define CHARACTER_CONTROLLER_HPP

#include <memory>
#include <glm\glm.hpp>
#include <SDL2\SDL_timer.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "../TransformVariables.hpp"


namespace Engine{ namespace Physics{
	class Collision;
} }


namespace Engine{
	namespace Controller{

		enum ControllerState{
			CONTROLLER_STAND = 0,
			CONTROLLER_CROUCH,
			CONTROLLER_SHIMMY,
			CONTROLLER_CLIMBING_LEDGE,
			CONTROLLER_MOVING,
			CONTROLLER_JUMP,
		};


		class CharacterController;
		class CharacterControllerInterface : public btKinematicCharacterController{
			friend class CharacterController;

		public:
			CharacterControllerInterface(btPairCachingGhostObject* ghostObject, 
				btConvexShape* convexShape, btScalar stepHeight, int upAxis = 1);

		};


		class CharacterController{
		public:
			const float c_playerWidth = 0.35f; //0.35f -> correct distance for ledge
			const float c_playerHeight = 1.5f;
			const float c_stepHeight = 0.5f;
			//const float c_playerGravityY = -75.0f;
			const float c_playerGravityY = -75.0f;
			const float c_playerJumpY = 25.0f; //25.0f
			const Location c_playerStartLocation = Location(0.0f, 0.0f, 0.0f);

		protected:		
			std::unique_ptr<CharacterControllerInterface> m_controllerInterface = nullptr;
			std::unique_ptr<btConvexShape> m_collisionShape = nullptr;
			std::unique_ptr<btPairCachingGhostObject> m_ghostObject = nullptr;

			bool m_moving = false;
			glm::vec3 m_walkDirection = glm::vec3(0.0f, -0.0001f, 0.0f);
			glm::vec3 m_walkDirectionChange = glm::vec3(0.0f);

			float m_interpolationFactor = 0.0f;
			glm::vec3 m_walkDirectionBeforeInterpolation = glm::vec3(0.0f);
			glm::vec3 m_walkDirectionAfterInterpolation = glm::vec3(0.0f);
			Uint32 m_interpolationStartTimer = 0;
			Uint32 m_interpolationEndTimer = 0;

			glm::vec3 m_previousLinearVelocity = glm::vec3(0.0f, 0.001f, 0.0f);
			float m_previousVerticalVelocity = 0.0f;

		public:
			ControllerState m_state = CONTROLLER_STAND;

		public:
			void script_addRef(){}

			void script_delRef(){}

			CharacterController();

			CharacterController(Physics::Collision &collision);

			CharacterController(Physics::Collision &collision, float width, float height);

			~CharacterController();

		public:
			void jump();

			bool canJump() const;

			//TODO
			bool justJumped() const;
			//TODO
			bool justLanded() const;


			bool isMoving() const;

			void setMaxSlopeAngle(float maxSlopeAngle);

			float getMaxSlopeAngle() const;

			const glm::vec3 &getWalkDirection() const;

			const glm::vec3 &getWalkDirectionChange() const;


			void setWalkDirection(const glm::vec3 &direction);

			void setInterpolatedFactorWalkDirection(
				const glm::vec3 &direction, float newDirectionFactor = 0.15f);

			void setInterpolatedWalkDirection(
				const glm::vec3 &direction, float durationInSeconds);

			void updateInterpolatedDirection();

			glm::vec3 getNormalizedDirection() const;

			glm::vec3 getLinearVelocity(const float fps = 60.0f) const;

			glm::vec3 getNormalizedLinearVelocity(const float fps = 60.0f) const;

			void setPreviousLinearVelocity(const glm::vec3 &linearVelocity);

			const glm::vec3 &getPreviousLinearVelocity() const;

			Location getLocation() const;

			void setLocation(const Location &location);

			void enableGravity();

			void disableGravity();

			float getVerticalVelocity() const;

			float getPreviousVerticalVelocity() const;

			void setPreviousVerticalVelocity(float velocity);



			float getHeight() const;

			float getCurrentHeight() const;

			float getWidth() const;

			btPairCachingGhostObject *getBody() const;

			btConvexShape *getCollisionShape() const;


			void setCollisionBodyHeightFactor(Physics::Collision &collision, float factor);


			
			void testContacts();

		};

	}
}

#endif //CHARACTER_CONTROLLER_HPP