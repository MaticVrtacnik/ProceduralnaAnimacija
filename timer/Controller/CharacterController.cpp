#include "CharacterController.hpp"

#include "../Define/PrintDefines.hpp"
#include "../Collision/Collision.hpp"
#include "../Utility/CollisionUtilities.hpp"


namespace Engine{
	namespace Controller{


		CharacterControllerInterface::CharacterControllerInterface(
			btPairCachingGhostObject* ghostObject, btConvexShape* convexShape,
			btScalar stepHeight, int upAxis
		) :
			btKinematicCharacterController(ghostObject, convexShape, stepHeight, upAxis)
		{

		}



		CharacterController::CharacterController(){

		}

		CharacterController::CharacterController(Physics::Collision &collision) :
			CharacterController(collision, c_playerWidth, c_playerHeight)
		{

		}
		
		CharacterController::CharacterController(
			Physics::Collision &collision, float width, float height
		)// :
			//m_playerWidth(width), m_playerHeight(height)
		{
			m_collisionShape = std::make_unique<btCapsuleShape>(c_playerWidth, c_playerHeight);

			btTransform _startTransform;
			_startTransform.setIdentity();
			_startTransform.setOrigin(Physics::convertVec3(c_playerStartLocation));

			m_ghostObject = std::make_unique<btPairCachingGhostObject>();
			m_ghostObject->setWorldTransform(_startTransform);
			m_ghostObject->setCcdSweptSphereRadius(0.2f);
			m_ghostObject->setHitFraction(1.0f);
			m_ghostObject->setRestitution(0.0f);
			m_ghostObject->setCollisionShape(m_collisionShape.get());
			m_ghostObject->setUserIndex(111111);
			m_ghostObject->setCollisionFlags(
				btCollisionObject::CF_KINEMATIC_OBJECT |
				btCollisionObject::CF_CHARACTER_OBJECT |
				btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK
			);

			m_controllerInterface = std::make_unique<CharacterControllerInterface>(
				m_ghostObject.get(), m_collisionShape.get(), c_stepHeight
			);

			//TODO not working wait for updated lib??
			m_controllerInterface->setMaxJumpHeight(0.0f);

			m_controllerInterface->setGravity(-c_playerGravityY);
			m_controllerInterface->setJumpSpeed(c_playerJumpY);
			m_controllerInterface->setFallSpeed(-c_playerGravityY);
			m_controllerInterface->setUseGhostSweepTest(true);

			m_controllerInterface->setMaxSlope(glm::radians(60.0f));

			
			collision.getWorld().addCollisionObject(
				m_ghostObject.get(), Physics::GROUP_CHARACTER_CONTROLLER,
				Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC
				//btBroadphaseProxy::CharacterFilter, 64
				//btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
			);

			collision.getWorld().addAction(m_controllerInterface.get());

			enableGravity();

			disableGravity();
		}
	
		CharacterController::~CharacterController(){

		}


		bool CharacterController::justJumped() const{
			return m_controllerInterface->m_wasOnGround;
		}

		bool CharacterController::justLanded() const{
			return m_controllerInterface->m_wasJumping && !canJump();
		}

		void CharacterController::jump() {
			m_controllerInterface->jump();	
		};

		bool CharacterController::canJump() const{
			return m_controllerInterface->canJump();
		}

		bool CharacterController::isMoving() const{
			return m_moving;
		}

		void CharacterController::setMaxSlopeAngle(float maxSlopeAngle){
			m_controllerInterface->setMaxSlope(glm::radians(maxSlopeAngle));
		}

		float CharacterController::getMaxSlopeAngle() const{
			return glm::degrees(m_controllerInterface->getMaxSlope());
		}


		const glm::vec3 &CharacterController::getWalkDirection() const{
			return m_walkDirection;
		}

		const glm::vec3 &CharacterController::getWalkDirectionChange() const{
			return m_walkDirectionChange;
		}

		glm::vec3 CharacterController::getLinearVelocity(const float fps) const{
			glm::vec3 _velocity = Physics::convertVec3(m_controllerInterface->m_walkDirection);
			_velocity.y += m_controllerInterface->m_verticalVelocity * (1.0f / fps);
			//_velocity.y += m_controllerInterface->m_verticalVelocity;

			return _velocity;
		}

		glm::vec3 CharacterController::getNormalizedLinearVelocity(const float fps) const{
			glm::vec3 _velocity = getLinearVelocity(fps);

			return (glm::length(_velocity) < 0.001f) ?
				glm::vec3(0.0f, -1.0f, 0.0f) : glm::normalize(_velocity);
		}


		void CharacterController::setWalkDirection(const glm::vec3 &direction){
			glm::vec3 _direction = glm::length(direction) == 0.0f ?
				glm::vec3(0.0f, -0.001f, 0.0f) : direction;

			m_walkDirectionChange = direction - m_walkDirection;
			m_walkDirection = direction;
			//Utilities::print(m_walkDirection);

			m_controllerInterface->setWalkDirection(Physics::convertVec3(m_walkDirection));
			m_moving = glm::length(m_walkDirection) > 0.001f;
		}

		void CharacterController::setInterpolatedFactorWalkDirection(
			const glm::vec3 &direction, float newDirectionFactor) //Factor default is 0.15
		{
			glm::vec3 _direction(direction);

			float _newVelocity = glm::length(_direction);
			float _oldVelocity = glm::length(m_walkDirection);
			//float _avgVelocity = glm::mix(_oldVelocity, _newVelocity, 0.08f);
			//float _avgVelocity = glm::mix(_oldVelocity, _newVelocity, 0.1f);
			float _dotProduct = glm::dot(m_walkDirection, _direction);
			if (_dotProduct <= 0.0f){ //Facing opposite
				_newVelocity = 0.001f; //Slow down first
				newDirectionFactor *= 0.75f; //Turn slower
			}

			float _avgVelocity = glm::mix(_oldVelocity, _newVelocity, newDirectionFactor);
			glm::vec3 _newDirection(m_walkDirection + _direction * newDirectionFactor);
			//_newDirection = glm::normalize(_newDirection);

			_newDirection = glm::length(_newDirection) == 0.0f ?
				glm::vec3(0.0f, -0.001f, 0.0f) : (glm::normalize(_newDirection) *  _avgVelocity);

			setWalkDirection(_newDirection);
		}

		void CharacterController::setInterpolatedWalkDirection(
			const glm::vec3 &direction, float durationInSeconds)
		{
			m_walkDirectionBeforeInterpolation = m_walkDirection;
			m_walkDirectionAfterInterpolation = direction;

			m_interpolationStartTimer = SDL_GetTicks();
			m_interpolationEndTimer = m_interpolationStartTimer +
				static_cast<Uint32>(durationInSeconds * 1000);
		}

		void CharacterController::updateInterpolatedDirection(){
			float _duration = static_cast<float>(m_interpolationEndTimer - m_interpolationStartTimer);
			float _factor = (SDL_GetTicks() - m_interpolationStartTimer) / _duration;
			_factor = glm::clamp(_factor, 0.0f, 1.0f);

			setWalkDirection((1.0f - _factor) * m_walkDirectionBeforeInterpolation +
				_factor * m_walkDirectionAfterInterpolation);
		}


		Location CharacterController::getLocation() const{
			return Physics::convertVec3(m_ghostObject->getWorldTransform().getOrigin());
		}

		void CharacterController::setLocation(const Location &location){
			m_ghostObject->getWorldTransform().setOrigin(Physics::convertVec3(location));
		}

		void CharacterController::enableGravity(){
			m_controllerInterface->setFallSpeed(-c_playerGravityY);
		}

		void CharacterController::disableGravity(){
			m_controllerInterface->setFallSpeed(0.0f);
		}
		
		float CharacterController::getVerticalVelocity() const{
			return m_controllerInterface->m_verticalVelocity;
		}

		float CharacterController::getPreviousVerticalVelocity() const{
			return m_previousVerticalVelocity;
		}

		void CharacterController::setPreviousVerticalVelocity(float velocity){
			m_previousVerticalVelocity = velocity;
		}


		float CharacterController::getHeight() const{
			return c_playerHeight;
		}

		float CharacterController::getCurrentHeight() const{
			//TODO re-enable crouch height
			//return m_crouched ? (m_playerHeight * 0.5f) : m_playerHeight;
			return c_playerHeight;
		}

		float CharacterController::getWidth() const{
			return c_playerWidth;
		}

		btPairCachingGhostObject *CharacterController::getBody() const{
			return m_ghostObject.get();
		}

		btConvexShape *CharacterController::getCollisionShape() const{
			return m_collisionShape.get();
		}

		glm::vec3 CharacterController::getNormalizedDirection() const{
			return Physics::convertVec3(m_controllerInterface->m_normalizedDirection);
		}

		const glm::vec3 &CharacterController::getPreviousLinearVelocity() const{
			return m_previousLinearVelocity;
		}

		void CharacterController::setPreviousLinearVelocity(const glm::vec3 &linearVelocity){
			m_previousLinearVelocity = linearVelocity;
		}

		void CharacterController::testContacts(){
			auto &_manifoldArray = m_controllerInterface->m_manifoldArray;

			std::vector<const btManifoldPoint> _contactPoints;

			for (int j = 0; j < _manifoldArray.size(); j++){
				const btPersistentManifold* _manifold = _manifoldArray[j];

				bool _isFirstBody = _manifold->getBody0() == m_ghostObject.get();
				btScalar _direction = _isFirstBody ? btScalar(-1.0f) : btScalar(1.0f);

				int _numContacts = _manifold->getNumContacts();
				for (int k = 0; k < _numContacts; ++k){
					const btManifoldPoint _point = _manifold->getContactPoint(k);

					if (_point.getDistance() < 0.0f){
						_contactPoints.emplace_back(std::move(_point));
					}
				}
			}

			//PRINT("NUM CONTACTS: " << _contactPoints.size());
		}


		void CharacterController::setCollisionBodyHeightFactor(Physics::Collision &collision, float factor){
			if (factor <= 0.0f){
				WARNING("invalid player capsule body height factor (<= 0)");
				factor = 0.1f;
			}

			//collision.getWorld().removeCollisionObject(m_ghostObject.get());

			//m_collisionShape = std::make_unique<btCapsuleShape>(c_playerWidth, c_playerHeight * factor);
			//m_ghostObject->getCollisionShape()->setLocalScaling(btVector3(1.0f, factor, 1.0f));
			

			/*collision.getWorld().addCollisionObject(
				m_ghostObject.get(), Physics::GROUP_CHARACTER_CONTROLLER,
				Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC);*/

			
		}


	}
}
