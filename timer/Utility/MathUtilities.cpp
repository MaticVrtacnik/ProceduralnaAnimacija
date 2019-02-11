#include "MathUtilities.hpp"

#include <glm\gtx\norm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <SDL2/SDL_mouse.h>
#include <iostream>

#include "PrintUtilities.hpp"
#include "../Define/Defines.hpp"
#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Utilities{

		glm::vec3 getMouseVector(
			int screenWidth, int screenHeight, const glm::vec3 &cameraLocation,
			const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix)
		{
			int _mouseX, _mouseY;
			SDL_GetMouseState(&_mouseX, &_mouseY);
			//Engine::Utilities::print(_mouseX, _mouseY);

			float _cx = ((2.0f * _mouseX) / screenWidth) - 1.0f;
			float _cy = ((2.0f * _mouseY) / screenHeight) - 1.0f;

			glm::vec4 _clip = glm::vec4(-_cx, _cy, 1.0, -1.0);
			glm::mat4 _invertedProjection = glm::inverse(projectionMatrix);
			glm::vec4 _view = _invertedProjection * _clip;

			glm::mat4 _invertedView = glm::inverse(viewMatrix);
			glm::vec4 _world = _invertedView * _view;
			glm::vec3 _mouseVector = glm::vec3(_world.x, _world.y, _world.z);

			_mouseVector += cameraLocation;
			_mouseVector = -_mouseVector;

			//if NaN
			if (std::isnan(_mouseVector.x)) _mouseVector.x = 0.0;
			if (std::isnan(_mouseVector.y)) _mouseVector.y = 0.0;
			if (std::isnan(_mouseVector.z)) _mouseVector.z = 0.0;

			return glm::normalize(_mouseVector);
		}



		glm::vec3 getLocationFromMatrix(const glm::mat4 &matrix){
			return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
		}

		glm::vec3 getScaleFromMatrix(const glm::mat4 &matrix){
			return glm::vec3(matrix[0][0], matrix[1][1], matrix[2][2]);
		}

		glm::mat4 getRotationMatrixFromMatrix(const glm::mat4 &matrix){
			return glm::transpose(glm::inverse(matrix));
		}

		glm::mat4 getLookAtRotation(const glm::vec3 &from, const glm::vec3 &to, const glm::vec3 upVector){
			glm::vec3 _upVector = glm::normalize(upVector);

			glm::mat4 _rotation = glm::mat4(glm::mat3(glm::lookAt(from, to, _upVector)));

			return glm::transpose(_rotation);
		}


		float limitAngle(float angle){
			const float c_angleLimit = 360.0f;

			if (angle < 0.0f){
				angle += c_angleLimit;
			} else if (angle >= c_angleLimit){
				angle -= c_angleLimit;
			}

			return angle;
		}

		glm::vec2 limitAngles(const glm::vec2 &angles){
			return glm::vec2(limitAngle(angles.x), 
				limitAngle(angles.y));
		}

		glm::vec3 limitAngles(const glm::vec3 &angles){
			return glm::vec3(limitAngle(angles.x), 
				limitAngle(angles.y), limitAngle(angles.z));
		}


		glm::vec3 getEulerRotationFromVector(const glm::vec3 &vector){
			float _length = glm::length(vector);
			float _newPitch = asin(vector.y / _length);
			float c = cos(_newPitch);
			float _newYaw = 0.0f;

			if (c != 0.0f){
				float _ax = asin(-vector.x / (c * _length));
				float _az = asin(-vector.z / (c * _length));
				_newYaw = _az < 0.0 ? PI - _ax : _ax;
			}

			glm::vec3 _degrees(glm::degrees(glm::vec3(_newPitch, _newYaw, 0.0f)));
			_degrees = limitAngles(_degrees);

			return _degrees;
		}

		glm::vec3 getEulerRotationFromVector(glm::vec3 &&vector){
			float _length = glm::length(std::move(vector));
			float _newPitch = asin(std::move(vector.y) / _length);
			float c = cos(_newPitch);
			float _newYaw = 0.0f;

			if (c != 0.0f){
				float _ax = asin(-std::move(vector.x) / (c * _length));
				float _az = asin(-std::move(vector.z) / (c * _length));
				_newYaw = _az < 0.0 ? PI - _ax : _ax;
			}

			glm::vec3 _degrees(glm::degrees(glm::vec3(_newPitch, _newYaw, 0.0f)));
			_degrees = limitAngles(_degrees);

			return _degrees;
		}

		glm::vec3 getEulerRotationFromVector(float x, float y, float z){
			return getEulerRotationFromVector(glm::vec3(x, y, z));
		}
		


		glm::quat getRotationBetweenVectors(const glm::vec3 &start, const glm::vec3 &dest){
			glm::vec3 _start = glm::normalize(start);
			glm::vec3 _dest = glm::normalize(dest);

			float cosTheta = glm::dot(start, dest);
			glm::vec3 rotationAxis;

			if (cosTheta < -1 + 0.001f){
				// special case when vectors in opposite directions:
				// there is no "ideal" rotation axis
				// So guess one; any will do as long as it's perpendicular to start
				rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
				if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
					rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

				rotationAxis = glm::normalize(rotationAxis);
				return glm::angleAxis(glm::radians(180.0f), rotationAxis);
			}

			rotationAxis = glm::cross(start, dest);

			float s = sqrt((1 + cosTheta) * 2);
			float invs = 1 / s;

			return glm::normalize(glm::quat(s * 0.5f, rotationAxis * invs));
		}

		float getAngleBetweenVectors(const glm::vec3 &v0, const glm::vec3 &v1){
			return  acos(glm::dot(glm::normalize(v0), glm::normalize(v1)));
		}

		glm::quat getQuatRotationFromVector(const glm::vec3 &vector, glm::vec3 orientation){
			glm::vec3 _vector(glm::normalize(vector));
			glm::vec3 _orientation(glm::normalize(orientation));

			glm::vec3 _axis(glm::cross(_vector, _orientation));
			float _angle = acos(glm::dot(_vector, _orientation));

			return glm::normalize(glm::quat(_angle, _axis));

			//return getRotationBetweenVectors(glm::vec3(0.0f, 1.0f, 0.0f), vector);
		}

		/*glm::quat getQuatRotationFromVector(glm::vec3 &&vector, glm::vec3 orientation){
			glm::vec3 _vector(glm::normalize(std::move(vector)));
			glm::vec3 _orientation(glm::normalize(orientation));

			glm::vec3 _axis(glm::cross(_vector, _orientation));
			float _angle = acos(glm::dot(_vector, _orientation));

			return glm::normalize(glm::quat(_angle, _axis));
		}*/

		glm::quat getQuatRotationFromVector(float x, float y, float z, glm::vec3 orientation){
			return getQuatRotationFromVector(x, y, z, orientation);
		}



		glm::vec3 getVectorFromRotation(const glm::vec3 &angles){
			return glm::normalize(glm::vec3(
				-cos(glm::radians(angles.x)) * sin(glm::radians(angles.y)),
				sin(glm::radians(angles.x)),
				-cos(glm::radians(angles.x)) * cos(glm::radians(angles.y))
			));
		}

		glm::vec3 getVectorFromRotation(float x, float y, float z){
			return getVectorFromRotation(glm::vec3(x, y, z));
		}

		glm::vec3 getVectorFromRotation(const glm::quat &quaternion, const glm::vec3 &default){
			//glm::vec3 _default(0.0f, -1.0f, 0.0f);

			return glm::normalize(quaternion * default);
		}

		glm::vec3 getVectorFromRotation(float x, float y, float z, float w, const glm::vec3 &default){
			return getVectorFromRotation(glm::quat(w, x, y, z), default);
		}



		glm::vec3 calculateNormalVector(
			const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
		{
			return glm::normalize(glm::cross(p1 - p0, p2 - p0));
		}

		glm::vec3 calculateNormalVector(
			const glm::vec3 &p0, const glm::vec3 &p1,
			const glm::vec3 &p2, const glm::vec3 &p3)
		{
			glm::vec3 _n0(calculateNormalVector(p0, p1, p2));
			glm::vec3 _n1(calculateNormalVector(p0, p1, p3));
			glm::vec3 _n2(calculateNormalVector(p0, p2, p3));

			return glm::normalize((_n0 + _n1 + _n2) / 3.0f);
		}


		glm::vec2 getScreenLocation(const glm::vec3 &point,
			const glm::mat4 &toProjectionMatrix)
		{
			glm::vec4 _projectedLocation(toProjectionMatrix * glm::vec4(point, 1.0));
			glm::vec2 _screenLocation(_projectedLocation.x, _projectedLocation.y);
			_screenLocation = (_screenLocation / _projectedLocation.w);
			_screenLocation = _screenLocation * 0.5f + 0.5f;

			return _screenLocation;
		}


		glm::vec3 getBarycentricWeights(const glm::vec3 &point,
			const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2)
		{
			glm::vec3 _vec0 = v1 - v0;
			glm::vec3 _vec1 = v2 - v0;
			glm::vec3 _vec2 = point - v0;

			float _d00 = glm::dot(_vec0, _vec0);
			float _d01 = glm::dot(_vec0, _vec1);
			float _d11 = glm::dot(_vec1, _vec1);
			float _d20 = glm::dot(_vec2, _vec0);
			float _d21 = glm::dot(_vec2, _vec1);
			float _denom = _d00 * _d11 - _d01 * _d01;

			glm::vec3 _weights;
			_weights.y = (_d11 * _d20 - _d01 * _d21) / _denom;
			_weights.z = (_d00 * _d21 - _d01 * _d20) / _denom;
			_weights.x = 1.0f - _weights.y - _weights.z;

			return _weights;
		}


		glm::mat4 getShadowMatrix(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M){
			static const glm::mat4 _bias(
				0.5f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 0.0f,
				0.5f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f
				);

			return _bias * P * V * M;
		}

	}
}
