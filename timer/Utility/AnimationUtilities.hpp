#ifndef ANIMATION_UTILITIES_HPP
#define ANIMATION_UTILITIES_HPP

#include <string>
#include <vector>
#include <assimp\matrix4x4.h>

#include "../TransformVariables.hpp"


namespace Engine{ namespace Entities{
	class Entity;
} }

namespace Engine{
	namespace Animation{
		struct AnimationSkeleton;
	}
}

class btRigidBody;

namespace Engine{
	namespace Utilities{

		glm::mat4 convertMat4(const aiMatrix4x4 &matrix);


		glm::mat4 setCollisionBodyTransformFromBone(
			const std::string &key, Entities::Entity &skeletonEntity);

		/*glm::mat4 setCollisionBodyTransformFromBone(
			const std::string &key,
			Entities::Entity &skeletonEntity, 
			const Location &location,
			const Rotation &rotation = Rotation(0.0f),
			const Scale &scale = Scale(1.0f)
		);*/


		void correctFinalTransforms(const Animation::AnimationSkeleton &skeleton,
			std::vector<glm::mat4> *transforms);


		glm::mat4 setBoneTransformFromCollisionBody(
			const std::string &key, Entities::Entity &skeletonEntity);

		glm::mat4 getBoneTransformFromCollisionBody(const std::string &key,
			Entities::Entity &skeletonEntity, const btRigidBody &collisionBody);

		/*glm::mat4 setBoneTransformFromCollisionBody(
			const std::string &key,
			Entities::Entity &skeletonEntity,
			const Location &location,
			const Rotation &rotation = Rotation(0.0f),
			const Scale &scale = Scale(1.0f)
		);*/


		std::vector<glm::mat4> setCollisionBodyTransformsFromBones(
			Entities::Entity &skeletonEntity);

		std::vector<glm::mat4> setBoneTransformsFromCollisionBodies(
			Entities::Entity &skeletonEntity);


		void setTransformLocation(const Animation::AnimationSkeleton &skeleton,
			std::vector<glm::mat4> *transforms, const std::string &boneName, const Location &location);

		void setTransformRotation(const Animation::AnimationSkeleton &skeleton,
			std::vector<glm::mat4> *transforms, const std::string &boneName, const Rotation &rotation);

	}
}

#endif //ANIMATION_UTILITIES_HPP