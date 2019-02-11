
#include "CollisionUtilities.hpp"

#include <utility>
#include <glm\gtc\type_ptr.hpp>
#include <BulletCollision\CollisionShapes\btCollisionShape.h>

#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Physics{

		btVector3 convertVec3(const glm::vec3 &vector){
			return btVector3(vector.x, vector.y, vector.z);
		}

		btVector3 convertVec3(glm::vec3 &&vector){
			return btVector3(std::move(vector.x), std::move(vector.y), std::move(vector.z));
		}

		glm::vec3 convertVec3(const btVector3 &vector){
			return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
		}

		/*glm::vec3 convertVec3(btVector3 &&vector){
			return glm::vec3(std::move(vector));  //TODO FIXXXXXX
		}*/

		btQuaternion convertQuat(const glm::quat &quaternion){
			return btQuaternion(
				quaternion.x, quaternion.y,
				quaternion.z, quaternion.w
			);
		}

		btQuaternion convertQuat(glm::quat &&quaternion){
			return btQuaternion(
				std::move(quaternion.x), std::move(quaternion.y),
				std::move(quaternion.z), std::move(quaternion.w)
			);
		}

		glm::quat convertQuat(const btQuaternion &quaternion){
			return glm::quat(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
		}

		/*
		glm::quat convertQuat(btQuaternion &&quaternion){
			return glm::quat(
			quaternion.w(), quaternion.x(), 
			quaternion.y(), quaternion.z()
		);
		}
		*/



		btTransform convertMat4(const glm::mat4 &transform){
			/*if (glm::length(glm::vec3(transform[0])) != 1.0f) 
				WARNING("BulletPhysics Matrix X-axis scale not 1.0");
			if (glm::length(glm::vec3(transform[1])) != 1.0f)
				WARNING("BulletPhysics Matrix Y-axis scale not 1.0");
			if (glm::length(glm::vec3(transform[2])) != 1.0f) 
				WARNING("BulletPhysics Matrix Z-axis scale not 1.0");*/

			btTransform _transform = btTransform::getIdentity();
			_transform.setFromOpenGLMatrix(glm::value_ptr(transform));

			return _transform;
		}

		glm::mat4 convertMat4(const btTransform &transform){
			glm::mat4 _transform; 
			transform.getOpenGLMatrix(glm::value_ptr(_transform));

			return _transform;
		}



		glm::vec3 getBodyScale(btRigidBody &body){
			btVector3 _aabbMin, _aabbMax;
			body.getAabb(_aabbMin, _aabbMax);

			return glm::abs(Physics::convertVec3(_aabbMax - _aabbMin));
		}

		glm::mat2x3 getBoundingBox(btCollisionShape *collisionShape){
			btVector3 _aabbMin, _aabbMax;
			collisionShape->getAabb(btTransform::getIdentity(), _aabbMin, _aabbMax);

			return glm::mat2x3(convertVec3(_aabbMin), convertVec3(_aabbMax));
		}

	}
}