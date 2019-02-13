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


#ifndef PLAYER_CONTROLLER_SYSTEM_HPP
#define PLAYER_CONTROLLER_SYSTEM_HPP

#include <glm\glm.hpp>
#include <unordered_map>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "System.hpp"
#include "../Buffer/Framebuffer.hpp"
#include "../TransformVariables.hpp"
#include "../Collision/Collision.hpp"
#include "../Animation/SkeletalAnimationTransform.hpp"
#include "../Animation/SkeletalAnimationInterpolator.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			class PlayerControllerSystem : public System{
			private:
				bool m_increaseRight = true;

			private:
				//const float c_playerWalkingSpeed = 6.25f;
				const float c_playerWalkingSpeed = 7.5f;

				glm::vec3 m_previousRaycastNormal = glm::vec3(0.0f, 1.0f, 0.0f);	

				std::vector<std::pair<Location, Location>> m_debugLines;

				Location m_locationAfterLedgeClimb = Location(0.0f);
				Animation::SkeletalAnimationInterpolator m_interpolator;

				float m_previousVerticalVelocity = 0.0f;
				bool m_previousCanJump = true;


				const unsigned int c_clothingMapResolution = 4096;
				unsigned int m_originalClothingTexture = 0;
				Buffers::Framebuffer m_clothingRecollorFramebuffer;
				std::unordered_map<std::string, glm::vec3> m_clothingColors;

			private:
				bool testLedgeProximity(Entity &playerEntity, glm::vec3 &walkDirection);

				void correctCollisionConstraints(Entity &ragdollEntity, 
					const std::vector<Animation::SkeletalAnimationTransform> &boneTransforms,
					const glm::mat4 &inverseModelMatrix);

				void setLocomotionKeyframes(float velocity, 
					const float yaw, const float fps = 60.0f);

				//returns fall factor fallSpeed/gravity
				float setFallAbsorptionKeyframes(float fallSpeed);

				Entity getEntityUnderPlayer(const Entity &playerEntity);

				void mousePicking(const Entity &playerEntity);

				//Returns true if moving else return false
				bool controlMovement(glm::vec3 *movementDirection, float *speed, 
					const float yaw, bool canRun = true);

				void setDefaultClothingColors();

				void updatePlayerColors();

			public:
				PlayerControllerSystem();

				~PlayerControllerSystem();


				void init() override;

				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;

				void onEntityAdded(Entity &entity) override;

			};
		}
	}
}

#endif //PLAYER_CONTROLLER_SYSTEM_HPP