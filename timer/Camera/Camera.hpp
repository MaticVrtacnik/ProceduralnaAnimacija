#ifndef CAMERA_HPP
#define CAMERA_HPP

//Camera defaults
#define DEFAULT_FOV 90.0f
#define DEFAULT_ASPECT_RATIO 16.0f / 9.0f
#define DEFAULT_DRAW_DISTANCE 1000.0f
#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE (DEFAULT_DRAW_DISTANCE + DEFAULT_NEAR_PLANE)

#include <glm\glm.hpp>


namespace Engine{ namespace Input{ 
	struct Input;
} }


namespace Engine{
	namespace Camera{

		//static const float c_defaultDistance = 3.0f;
		static const float c_defaultDistance = 0.0f;
		static const float c_maxDistance = 25.0f;
		static const float c_walkingSpeed = 0.65f; //TODO as member
		static const float c_runningMultiplier = 6.0f; //TODO as member
		static const float c_mouseSpeed = 0.2f; //TODO as member
		static const float c_zoomSpeed = 0.5f; //TODO as member


		class Camera{
		private:
			float m_fov = DEFAULT_FOV;
			float m_aspectRatio = DEFAULT_ASPECT_RATIO;
			float m_nearPlane = DEFAULT_NEAR_PLANE;
			float m_farPlane = DEFAULT_FAR_PLANE;

		public:
			//TODO add getter
			float m_maxThirdPersonDistance = c_defaultDistance;

		private:
			bool m_moving = false;
			bool m_mouseIn = false;
			float m_moveSpeed = c_walkingSpeed;
			float m_mouseSpeed = c_mouseSpeed;

			//TODO projection matrix variables

			glm::mat4   m_projectionMatrix = glm::mat4(1.0f);
			glm::mat4	m_viewMatrix = glm::mat4(1.0f);
			glm::mat4	m_previousViewMatrix = glm::mat4(1.0f);

			glm::vec3	m_location = glm::vec3(0.0f);
			glm::vec3	m_interpolationStepSize = glm::vec3(0.0f);

			float		m_pitch = 0.0f;
			float		m_yaw = 0.0f;
			float		m_roll = 0.0f;
			
			glm::vec3 m_thirdPersonOffset = glm::vec3(0.0f);		
			float m_thirdPersonDistance = c_defaultDistance;
			float m_originalThirdPersonDistance = c_defaultDistance;

		private:
			void checkLimits();

			void updateProjectionMatrix();

		public:
			Camera();

			Camera(float fov, float aspectRatio = DEFAULT_ASPECT_RATIO);

			Camera(int screenWidth, int screenHeight, float fov = DEFAULT_FOV);

			Camera(const glm::vec3 &location, 
				float fov = DEFAULT_FOV, float aspectRatio = DEFAULT_ASPECT_RATIO);

			Camera(const glm::vec3 &location, 
				int screenWidth, int screenHeight, float fov = DEFAULT_FOV);


			//0 = forward ... direction is the degrees offset
			void move(float direction = 0.0f, bool running = false); 

			//0 = up ... direction is the degrees offset
			void moveUp(float direction = 0.0f, bool running = false);

			void control(const Input::Input &input, bool canRun = true);

			void translate(bool	thirdPerson);

			void rotate();

			void transform(bool	thirdPerson);


			//will change viewMatrix
			void invertCameraPitch();

			void invertCameraYaw();

			void invertCameraRoll();


			/*bool	interpolate(const glm::vec3	&target);

			bool	interpolate(const glm::vec3	&start, const glm::vec3	&target);

			void	resetInterpolationCounter();*/


			float getFov() const;

			void setFov(float fov);


			float getAspectRatio() const;

			void setAspectRatio(float aspectRatio);

			void setAspectRatio(int screenWidth, int screenHeight);


			float getNearPlane() const;

			float getFarPlane() const;

			glm::vec2 getNearFarPlanes() const;

			void setNearPlane(float nearPlane);

			void setFarPlane(float farPlane);

			void setNearFarPlanes(float nearPlane, float farPlane);

			void setNearFarPlanes(const glm::vec2 &nearFarPlanes);


			const glm::mat4	&getProjectionMatrix() const;

			void setProjectionMatrix(float fov, float aspectRatio,
				float nearPlane = DEFAULT_NEAR_PLANE, float farPlane = DEFAULT_FAR_PLANE);

			void setProjectionMatrix(float fov, int screenWidth, int screenHeight,
				float nearPlane = DEFAULT_NEAR_PLANE, float farPlane = DEFAULT_FAR_PLANE);
	

			glm::vec3 getVector()	const;

			glm::vec3 getSideVector()	const;

			const glm::mat4	&getViewMatrix() const;

			const glm::mat4 &getPreviousViewMatrix() const;

			const glm::mat3 getRotationMatrix() const;

			void setViewMatrix(const glm::mat4 &viewMatrix);

			void lookAt(float pitch, float yaw);

			void resetViewMatrix();


			const glm::vec3	&getLocation() const;

			const glm::vec3	getThirdPersonLocation() const;

			void setLocation(const glm::vec3 &location);

			void setLocation(glm::vec3 &&location);

			void setLocation(float x, float y, float z);

		
			float getPitch() const;

			void setPitch(float pitch);

			float getYaw() const;

			void setYaw(float yaw);

			float getRoll() const;

			void setRoll(float roll);

			glm::vec2 getPitchYaw()	const;

			void setPitchYaw(float pitch, float yaw);

			void setPitchYaw(const glm::vec2 &angles);

			void setPitchYaw(glm::vec2 &&angles);

			glm::vec3 getPitchYawRoll()	const;

			void setPitchYawRoll(float pitch, float yaw, float roll);

			void setPitchYawRoll(const glm::vec3 &angles);

			void setPitchYawRoll(glm::vec3 &&angles);
	
		
			bool isMoving() const;

			float getMoveSpeed() const;

			void setMoveSpeed(float	speed);


			bool isMouseIn() const;

			void setMouseIn(bool mouseIn);

			float getMouseSpeed() const;

			void setMouseSpeed(float speed);


			const float &getThirdPersonDistance() const;

			void setThirdPersonDistance(float distance);

			void restoreThirdPersonDistance();


			const float &getMaxThirdPersonDistance() const;

			void setMaxThirdPersonDistance(float distance);

			const glm::vec3 &getThirdPersonOffset() const;	

		};
	
	}
}

#endif //CAMERA_HPP