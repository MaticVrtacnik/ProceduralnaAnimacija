#ifndef MATH_UTILITIES_HPP
#define MATH_UTILITIES_HPP

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>


namespace Engine{
	namespace Utilities{
		const double PI = 3.14159265359;


		glm::vec3 getMouseVector(
			int screenWidth, int screenHeight,
			const glm::vec3 &cameraLocation, 
			const glm::mat4 &projectionMatrix,
			const glm::mat4 &viewMatrix
		);


		glm::vec3 getLocationFromMatrix(const glm::mat4 &matrix);

		glm::vec3 getScaleFromMatrix(const glm::mat4 &matrix);

		glm::mat4 getRotationMatrixFromMatrix(const glm::mat4 &matrix);

		glm::mat4 getLookAtRotation(const glm::vec3 &from, const glm::vec3 &to,
			const glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f));


		//will return angle in range [0,360)
		float limitAngle(float angle);

		glm::vec2 limitAngles(const glm::vec2 &angles);

		glm::vec3 limitAngles(const glm::vec3 &angles);


		glm::vec3 getEulerRotationFromVector(const glm::vec3 &vector);

		glm::vec3 getEulerRotationFromVector(glm::vec3 &&vector);

		glm::vec3 getEulerRotationFromVector(float x, float y, float z);


		float getAngleBetweenVectors(const glm::vec3 &v0, const glm::vec3 &v1);

		glm::quat getQuatRotationFromVector(const glm::vec3 &vector,
			glm::vec3 orientation = glm::vec3(0.0f, 1.0f, 0.0f));

		/*glm::quat getQuatRotationFromVector(glm::vec3 &&vector, 
			glm::vec3 orientation = glm::vec3(0.0f, 1.0f, 0.0f));*/

		glm::quat getQuatRotationFromVector(float x, float y, float z,
			glm::vec3 orientation = glm::vec3(0.0f, 1.0f, 0.0f));



		glm::quat getRotationBetweenVectors(const glm::vec3 &start, const glm::vec3 &dest);



		glm::vec3 getVectorFromRotation(const glm::vec3 &angles);

		glm::vec3 getVectorFromRotation(float x, float y, float z);

		glm::vec3 getVectorFromRotation(const glm::quat &quaternion, const glm::vec3 &default = glm::vec3(0.0f, -1.0f, 0.0f));

		glm::vec3 getVectorFromRotation(float x, float y, float z, float w, const glm::vec3 &default = glm::vec3(0.0f, -1.0f, 0.0f));


		glm::vec3 calculateNormalVector(const glm::vec3 &p0,
			const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);

		glm::vec3 calculateNormalVector(const glm::vec3 &p0, 
			const glm::vec3 &p1, const glm::vec3 &p2);


		//Returns point in screen space [0...1]
		glm::vec2 getScreenLocation(const glm::vec3 &point,
			const glm::mat4 &toProjectionMatrix);


		glm::vec3 getBarycentricWeights(const glm::vec3 &point, 
			const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);


		glm::mat4 getShadowMatrix(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M);
	
	}
}

#endif //MATH_UTILITIES_HPP