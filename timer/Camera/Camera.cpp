#include "Camera.hpp"

#include <utility>
#include <SDL2\SDL.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Define/Defines.hpp"
#include "../Define/PrintDefines.hpp"
#include "../Define/Assert.hpp"
#include "../Input/Input.hpp"

#define MAX_INTERPOLATION_STEPS 100


namespace Engine{
	namespace Camera{

		Camera::Camera(){
			updateProjectionMatrix();
		}
		
		Camera::Camera(float fov, float aspectRatio){
			setProjectionMatrix(fov, aspectRatio);
		}

		Camera::Camera(int screenWidth, int screenHeight, float fov){
			setProjectionMatrix(fov, screenWidth, screenHeight);
		}

		Camera::Camera(const glm::vec3 &location, float fov, float aspectRatio) :
			m_location(location)
		{
			setProjectionMatrix(fov, aspectRatio);
		}

		Camera::Camera(const glm::vec3 &location,
			int screenWidth, int screenHeight, float fov
		) :
			m_location(location)
		{
			setProjectionMatrix(fov, screenWidth, screenHeight);
		}


		void Camera::checkLimits(){
			const float c_pitchLimit = 89.999999f;
			const float c_yawLimit = 360.0f;
			const float c_rollLimit = 360.0f;


			if (m_pitch > c_pitchLimit){
				m_pitch = c_pitchLimit;
			} else if (m_pitch < -c_pitchLimit){
				m_pitch = -c_pitchLimit;
			}

			if (m_yaw < 0.0f){
				m_yaw += c_yawLimit;
			} else if (m_yaw > c_yawLimit) {
				m_yaw -= c_yawLimit;
			}
			
			if (m_roll < 0.0f){
				m_roll += c_rollLimit;
			} else if (m_roll > c_rollLimit) {
				m_roll -= c_rollLimit;
			}
		}



		void Camera::move(float direction, bool running){
			float _runMultiplier = running ? c_runningMultiplier : 1.0f;
			//setFov(DEFAULT_FOV + _runMultiplier * 20.0f);

			float _radianAngle = glm::radians(m_yaw + direction);
			m_location.x -= sin(_radianAngle) * m_moveSpeed * _runMultiplier;
			m_location.z -= cos(_radianAngle) * m_moveSpeed * _runMultiplier;
		}

		void Camera::moveUp(float direction, bool running){
			float _runMultiplier = running ? c_runningMultiplier : 1.0f;
			float _radianAngle = glm::radians(m_pitch + direction);
			m_location.y += sin(_radianAngle) * m_moveSpeed * _runMultiplier;
		}

		void Camera::control(const Input::Input &input, bool canRun){
			if (!m_mouseIn){
				//SDL_ShowCursor(SDL_ENABLE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
				return;
			}

			resetViewMatrix();

			int _mouseX, _mouseY;
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_GetRelativeMouseState(&_mouseX, &_mouseY);
			float offsetX = static_cast<float>(-_mouseX) / 2.5f;
			float offsetY = static_cast<float>(-_mouseY) / 2.5f;
				

			//printVec2(mouseX, mouseY);
			m_yaw += m_mouseSpeed * offsetX;
			m_pitch += m_mouseSpeed * offsetY;

			checkLimits();
			m_moving = false;
			bool _running = input.isButtonHeld(SDL_SCANCODE_LSHIFT) && canRun;

			if (input.isButtonHeld(SDL_SCANCODE_W)){
				m_moving = true;
				if (m_pitch != 90.0f && m_pitch != -90.0f){
					move(0.0f, _running);
				}moveUp(0.0f, _running);
			}
			else if (input.isButtonHeld(SDL_SCANCODE_S)){
				m_moving = true;
				if (m_pitch != 90 && m_pitch != -90){
					move(180.0f, _running);
				}moveUp(180.0f, _running);
			}

			if (input.isButtonHeld(SDL_SCANCODE_A)){
				m_moving = true;
				move(90.0f, _running);
			}
			else if (input.isButtonHeld(SDL_SCANCODE_D)){
				m_moving = true;
				move(270.0f, _running);
			}


			float _newDistance = m_maxThirdPersonDistance;
			if (input.m_mouseWheelUp){
				_newDistance = m_maxThirdPersonDistance - c_zoomSpeed;
			} else if (input.m_mouseWheelDown){
				_newDistance = m_maxThirdPersonDistance + c_zoomSpeed;
			}

			//Update max third person distance
			if (input.m_mouseWheelUp || input.m_mouseWheelDown){
				const float c_changeFactor = 1.0f; //TODO remove => doesnt make a difference
				float _interpolatedDistance = glm::mix(
					m_maxThirdPersonDistance, _newDistance, c_changeFactor);
				setMaxThirdPersonDistance(_interpolatedDistance);
				m_thirdPersonDistance = _interpolatedDistance;
			}		
		}

		void Camera::translate(bool thirdPerson){
			//if (!m_mouseIn) return;
			//THIRD PERSON

			//TODO re-enable third person
			if (thirdPerson){
				const float _distance = m_thirdPersonDistance;

				float _heightDistance = cos(glm::radians(m_pitch)) * _distance;

				m_thirdPersonOffset.y = -sin(glm::radians(m_pitch)) * _distance;
				m_thirdPersonOffset.x = sin(glm::radians(m_yaw)) * _heightDistance;
				m_thirdPersonOffset.z = cos(glm::radians(m_yaw)) * _heightDistance;

				/*float _height = m_location.y + _thirdPersonOffset.y;
				_thirdPersonOffset.y = _height <= 0.1f ? -m_location.y : _height;*/
			} else{
				m_thirdPersonOffset = glm::vec3(0.0f);
			}

			//TODO make third person camera location member and return method
			m_viewMatrix *= glm::translate(-m_location - m_thirdPersonOffset);
		}

		void Camera::rotate(){
			//if (!m_mouseIn) return;
			m_viewMatrix *= glm::rotate(-m_roll, 0.0f, 0.0f, 1.0f);
			m_viewMatrix *= glm::rotate(-m_pitch, 1.0f, 0.0f, 0.0f);
			m_viewMatrix *= glm::rotate(-m_yaw, 0.0f, 1.0f, 0.0f);
		}

		void Camera::transform(bool thirdPerson){
			m_viewMatrix = glm::mat4(1.0f);
			rotate();
			translate(thirdPerson);
		}



		void Camera::invertCameraPitch(){
			m_pitch = -m_pitch;
			m_location = m_location -
				glm::vec3(0.0f, 2.0f * m_location.y, 0.0f);
	
			resetViewMatrix();
			rotate();

			//to keep camera centered to body
			//TODO maybe remove and add argument
			translate(false);
		}

		void Camera::invertCameraYaw(){
			//TODO IF NEEDED
		}

		void Camera::invertCameraRoll(){
			//TODO IF NEEDED
		}

		
		float Camera::getFov() const{
			return m_fov;
		}

		void Camera::setFov(float fov){
			ASSERT(fov > 0, "Field of view must be > 0");

			if (fov <= 0.0f)fov = DEFAULT_FOV;
			
			m_fov = fov;
			updateProjectionMatrix();
		}


		float Camera::getAspectRatio() const{
			return m_aspectRatio;
		}

		void Camera::setAspectRatio(float aspectRatio){
			ASSERT(aspectRatio > 0, "Aspect ratio must be > 0");

			if (aspectRatio <= 0.0f)aspectRatio = DEFAULT_ASPECT_RATIO;

			m_aspectRatio = aspectRatio;
			updateProjectionMatrix();
		}

		void Camera::setAspectRatio(int screenWidth, int screenHeight){
			ASSERT(screenWidth > 0, "Screen width must be > 0");
			ASSERT(screenHeight > 0, "Screen height must be > 0");

			if (screenWidth <= 0 || screenHeight <= 0) m_aspectRatio = DEFAULT_ASPECT_RATIO;
			else m_aspectRatio = (1.0f * screenWidth) / (1.0f * screenHeight);

			updateProjectionMatrix();
		}


		float Camera::getNearPlane() const{
			return m_nearPlane;
		}

		float Camera::getFarPlane() const{
			return m_farPlane;
		}

		glm::vec2 Camera::getNearFarPlanes() const{
			return glm::vec2(getNearPlane(), getFarPlane());
		}

		void Camera::setNearPlane(float nearPlane){
			ASSERT(nearPlane > 0, "Near plane must be > 0");

			if (nearPlane <= 0.0f)nearPlane = DEFAULT_NEAR_PLANE;

			m_nearPlane = nearPlane;
			updateProjectionMatrix();
		}

		void Camera::setFarPlane(float farPlane){
			ASSERT(farPlane > 0, "Far plane must be > 0");

			if (farPlane <= 0.0f)farPlane = DEFAULT_FAR_PLANE;		

			m_farPlane = farPlane;
			updateProjectionMatrix();
		}

		void Camera::setNearFarPlanes(float nearPlane, float farPlane){
			ASSERT(nearPlane > 0, "Near plane must be > 0");
			ASSERT(farPlane > 0, "Far plane must be > 0");

			if (nearPlane <= 0.0f)nearPlane = DEFAULT_NEAR_PLANE;
			if (farPlane <= 0.0f)farPlane = DEFAULT_FAR_PLANE;
			
			m_nearPlane = nearPlane;
			m_farPlane = farPlane;
			updateProjectionMatrix();
		}

		void Camera::setNearFarPlanes(const glm::vec2 &nearFarPlanes){
			setNearFarPlanes(nearFarPlanes.x, nearFarPlanes.y);
		}


		const glm::mat4 &Camera::getProjectionMatrix() const{
			return m_projectionMatrix;
		}

		void Camera::setProjectionMatrix(float fov,
			float aspectRatio, float nearPlane, float farPlane)
		{
			ASSERT(fov > 0, "Field of view must be > 0");
			ASSERT(aspectRatio > 0, "Aspect ratio must be > 0");
			ASSERT(nearPlane > 0, "Near plane must be > 0");
			ASSERT(farPlane > 0, "Far plane must be > 0");

			if (fov <= 0.0f)fov = DEFAULT_FOV;
			if (aspectRatio <= 0.0f)aspectRatio = DEFAULT_ASPECT_RATIO;
			if (nearPlane <= 0.0f)nearPlane = DEFAULT_NEAR_PLANE;
			if (farPlane <= 0.0f)farPlane = DEFAULT_FAR_PLANE;

			m_fov = fov;
			m_aspectRatio = aspectRatio;
			m_nearPlane = nearPlane;
			m_farPlane = farPlane;
			updateProjectionMatrix();
		}

		void Camera::setProjectionMatrix(float fov, int screenWidth,
			int screenHeight, float nearPlane, float farPlane)
		{
			ASSERT(fov > 0, "Field of view must be > 0");
			ASSERT(screenWidth > 0, "Screen width must be > 0");
			ASSERT(screenHeight > 0, "Screen height must be > 0");
			ASSERT(nearPlane > 0, "Near plane must be > 0");
			ASSERT(farPlane > 0, "Far plane must be > 0");

			if (fov <= 0.0f)fov = DEFAULT_FOV;
			if (nearPlane <= 0.0f)nearPlane = DEFAULT_NEAR_PLANE;
			if (farPlane <= 0.0f)farPlane = DEFAULT_FAR_PLANE;


			if (screenWidth <= 0 || screenHeight <= 0) m_aspectRatio = DEFAULT_ASPECT_RATIO;
			else m_aspectRatio = (1.0f * screenWidth) / (1.0f * screenHeight);

			m_fov = fov;
			m_nearPlane = nearPlane;
			m_farPlane = farPlane;
			updateProjectionMatrix();
		}

		void Camera::updateProjectionMatrix(){
			m_projectionMatrix = glm::perspective(
				m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
		}



		glm::vec3 Camera::getVector() const{
			return glm::normalize(glm::vec3(
				-cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw)),
				sin(glm::radians(m_pitch)),
				-cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw))
			));
		}

		glm::vec3 Camera::getSideVector() const{
			return glm::normalize(glm::vec3(
				-cos(glm::radians(m_pitch)) * sin(glm::radians((m_yaw + 90.0f))),
				sin(glm::radians(m_pitch)),
				-cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw + 90.0f))
			));
		}

		
		
		const glm::mat4 &Camera::getViewMatrix() const{
			return m_viewMatrix;
		}

		const glm::mat4 &Camera::getPreviousViewMatrix() const{
			return m_previousViewMatrix;
		}

		const glm::mat3 Camera::getRotationMatrix() const{
			return glm::mat3(m_viewMatrix);
		}

		void Camera::setViewMatrix(const glm::mat4 &viewMatrix){
			m_viewMatrix = viewMatrix;
		}

		void Camera::lookAt(float pitch, float yaw){
			m_pitch = pitch;
			m_yaw = yaw;
		}

		void Camera::resetViewMatrix(){
			m_previousViewMatrix = m_viewMatrix;
			m_viewMatrix = glm::mat4(1.0);
		}



		const glm::vec3 &Camera::getLocation() const{
			return m_location;
		}
	
		const glm::vec3	Camera::getThirdPersonLocation() const{
			return m_location + m_thirdPersonOffset;
		}

		void Camera::setLocation(const glm::vec3 &location){
			m_location = location;
		}

		void Camera::setLocation(glm::vec3 &&location){
			m_location = std::move(location);
		}

		void Camera::setLocation(float x, float y, float z){
			m_location = glm::vec3(x, y, z);
		}



		float Camera::getPitch() const{
			return m_pitch;
		}
		
		void Camera::setPitch(float pitch){
			m_pitch = pitch;
		}

		float Camera::getYaw() const{
			return m_yaw;
		}

		void Camera::setYaw(float yaw){
			m_yaw = yaw;
		}

		float Camera::getRoll() const{
			return m_roll;
		}

		void Camera::setRoll(float roll){
			m_roll = roll;
		}

		glm::vec2 Camera::getPitchYaw() const{
			return glm::vec2(m_pitch, m_yaw);
		}

		void Camera::setPitchYaw(const glm::vec2 &angles){
			m_pitch = angles.x;
			m_yaw = angles.y;
		}

		void Camera::setPitchYaw(glm::vec2 &&angles){
			m_pitch = std::move(angles.x);
			m_yaw = std::move(angles.y);
		}

		void Camera::setPitchYaw(float pitch, float yaw){
			m_yaw = yaw;
			m_pitch = pitch;
		}

		glm::vec3 Camera::getPitchYawRoll() const{
			return glm::vec3(m_pitch, m_yaw, m_roll);
		}

		void Camera::setPitchYawRoll(float pitch, float yaw, float roll){
			m_pitch = pitch;
			m_yaw = yaw;
			m_roll = roll;
		}

		void Camera::setPitchYawRoll(const glm::vec3 &angles){
			m_pitch = angles.x;
			m_yaw = angles.y;
			m_roll = angles.z;
		}

		void Camera::setPitchYawRoll(glm::vec3 &&angles){
			m_pitch = std::move(angles.x);
			m_yaw = std::move(angles.y);
			m_roll = std::move(angles.z);
		}



		bool Camera::isMoving() const{
			return m_moving;
		}

		float Camera::getMoveSpeed() const{
			return m_moveSpeed;
		}
		
		void Camera::setMoveSpeed(float speed){
			m_moveSpeed = speed;
		}



		bool Camera::isMouseIn() const{
			return m_mouseIn;
		}

		void Camera::setMouseIn(bool mouseIn){
			m_mouseIn = mouseIn;
		}
		
		float Camera::getMouseSpeed() const{
			return m_mouseSpeed;
		}

		void Camera::setMouseSpeed(float speed){
			m_mouseSpeed = speed;
		}



		const float &Camera::getThirdPersonDistance() const{
			return m_thirdPersonDistance;
		}

		void Camera::setThirdPersonDistance(float distance){
			if (distance >= 0.0f && distance < m_maxThirdPersonDistance){
				m_thirdPersonDistance = distance;
			}
		}

		void Camera::restoreThirdPersonDistance(){
			m_thirdPersonDistance = m_maxThirdPersonDistance;
		}



		const float &Camera::getMaxThirdPersonDistance() const{
			return m_maxThirdPersonDistance;
		}

		void Camera::setMaxThirdPersonDistance(float distance){
			m_maxThirdPersonDistance = distance;

			if (m_maxThirdPersonDistance < 0.0f){
				m_maxThirdPersonDistance = 0.0f;
			} else if (m_maxThirdPersonDistance > c_maxDistance){
				m_maxThirdPersonDistance = c_maxDistance;
			}

			if (m_maxThirdPersonDistance < m_thirdPersonDistance){
				m_thirdPersonDistance = m_maxThirdPersonDistance;
			}
		}

		const glm::vec3 &Camera::getThirdPersonOffset() const{
			return m_thirdPersonOffset;
		}

	}
}
