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