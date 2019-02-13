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


#ifndef SKELETAL_ANIMATION_RENDERER_SYSTEM_HPP
#define SKELETAL_ANIMATION_RENDERER_SYSTEM_HPP

#include "System.hpp"
#include "../Shader/Shader.hpp"
#include "../Component/SkeletalAnimationComponent.hpp"
#include "../TransformVariables.hpp"

//TODO remove
#include "../Collision/Collision.hpp"


#include "../Animation/InverseKinematics/IKSolver.hpp"

#include "../Controller/NearCollisionGrabber.hpp"


namespace Engine{

	namespace Animation{ 
		class FABRIK; 
		class AnimationBoneTree;
	}

	namespace Entities{
		namespace Systems{

			struct KinematicCollision{
			public:
				std::unique_ptr<btBroadphaseInterface> m_broadphase = nullptr;
				std::unique_ptr<btCollisionDispatcher>	m_dispatcher = nullptr;
				std::unique_ptr<btConstraintSolver>	m_solver = nullptr;
				std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration = nullptr;
				std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld = nullptr;

			public:
				KinematicCollision();

				~KinematicCollision();
				

				void init();

			};


			class SkeletalAnimationRendererSystem : public System{
			private:
				KinematicCollision m_kinematicCollision;
				Controller::NearCollisionGrabber m_nearCollisionGrabber;


				Physics::RigidBodyPtr m_body1 = nullptr;
				Physics::RigidBodyPtr m_body2 = nullptr;

				//TODO remove hardcoded;
				std::vector<std::pair<Location, Location>> m_debugLines;

				std::vector<GLint> m_boneUniformLocations;
				//std::vector<std::vector<Entity>> m_skeletons;

				std::unique_ptr<Animation::FABRIK> m_ikSolver;

			private:
				void reloadAnimationConfigs();

				void drawShadows();

				void drawOccluded();


				//TODO remove
				void equipKnife();


				//returns true if there was a ray hit
				bool getRayClosestData(const glm::vec3 &rayStartLocation,
					glm::vec3 &location, glm::vec3 &normal);

				void updateBoneTransforms();

				void updateBoneCollision();

				void correctBoneTransforms();

				void correctCollisionConstraints();


				//Procedural animation

				void crouchToAvoidObstacles();


				void generateBoneRotationsFromLocations(
					const Animation::AnimationBoneTree *node, 
					Animation::AnimationSkeleton &skeleton,
					std::vector<glm::mat4> &transforms);

			public:
				SkeletalAnimationRendererSystem();

				~SkeletalAnimationRendererSystem();


				void init() override;
				
				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;

				void onEntityAdded(Entity &entity) override;

				void onEntityRemoved(Entity &entity) override;

			};

		}
	}
}

#endif //SKELETAL_ANIMATION_RENDERER_SYSTEM_HPP