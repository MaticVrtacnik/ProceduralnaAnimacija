#ifndef COLLISION_UTILITIES_HPP
#define COLLISION_UTILITIES_HPP

#include <glm\glm.hpp>
#include <LinearMath\btVector3.h>
#include <LinearMath\btQuaternion.h>
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <glm\gtc\quaternion.hpp>


class btCollisionShape;

namespace Engine{
	namespace Physics{

		btVector3 convertVec3(const glm::vec3 &vector);

		btVector3 convertVec3(glm::vec3 &&vector);

		glm::vec3 convertVec3(const btVector3 &vector);


		btQuaternion convertQuat(const glm::quat &quaternion);

		btQuaternion convertQuat(glm::quat &&quaternion);

		glm::quat convertQuat(const btQuaternion &quaternion);


		btTransform convertMat4(const glm::mat4 &transform);

		glm::mat4 convertMat4(const btTransform &transform);



		glm::vec3 getBodyScale(btRigidBody &body);

		glm::mat2x3 getBoundingBox(btCollisionShape *collisionShape);

	}
}

#endif //COLLISION_UTILITIES_HPP