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