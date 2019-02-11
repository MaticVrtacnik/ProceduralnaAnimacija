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