#include "NearCollisionGrabber.hpp"

#include <glm\gtx\intersect.hpp>

#include <BulletCollision\CollisionShapes\btCylinderShape.h>

#include "../World/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Collision/Collision.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/InterpolationUtilities.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Component/MatrixComponent.hpp"
#include "../Component/PlayerControllerComponent.hpp"



namespace Engine{
	namespace Controller{
		using namespace Entities;
		using namespace Components;


		NearCollisionGrabber::NearCollisionGrabber(){

		}

		NearCollisionGrabber::~NearCollisionGrabber(){

		}


		std::vector<const btManifoldPoint> NearCollisionGrabber::getContactPoints(
			Physics::Collision &collision, btPairCachingGhostObject &ghostObject)
		{
			std::vector<const btManifoldPoint> _contactPoints;

			auto _pairCache = collision.getWorld().getPairCache();
			auto &_pairArray = ghostObject.getOverlappingPairCache()->getOverlappingPairArray();

			for (int i = 0; i < _pairArray.size(); ++i){
				const btBroadphasePair& pair = _pairArray[i];

				const btBroadphasePair* collisionPair =
					_pairCache->findPair(pair.m_pProxy0, pair.m_pProxy1);
				if (!collisionPair){
					continue;
				}

				btManifoldArray _manifoldArray;
				if (collisionPair->m_algorithm){
					collisionPair->m_algorithm->getAllContactManifolds(_manifoldArray);
				}

				for (int j = 0; j < _manifoldArray.size(); j++){
					const btPersistentManifold* _manifold = _manifoldArray[j];

					//TODO maybe remove
					bool _isFirstBody = _manifold->getBody0() == (&ghostObject);
					btScalar _direction = _isFirstBody ? btScalar(-1.0f) : btScalar(1.0f);

					int _numContacts = _manifold->getNumContacts();
					for (int k = 0; k < _numContacts; ++k){
						const btManifoldPoint _point = _manifold->getContactPoint(k);

						if (_point.getDistance() < 0.0f){
							_contactPoints.emplace_back(std::move(_point));
						}
					}
				}
			}

			return _contactPoints;
		}

		std::pair<Location, Location> NearCollisionGrabber::getNearbyHandCollisionPoints(
			Physics::Collision &collision, Entities::Entity &playerEntity)
		{
			m_debugLines.clear();


			auto &_matrixComponent = playerEntity.getComponent<MatrixComponent>();
			auto &_playerComponent = playerEntity.getComponent<PlayerControllerComponent>();

			if (m_sphereBody == nullptr){
				setMaxDistance(collision, 1.75f);
			}

			//Set current sphere location at approximately arm height
			Location _armLocation = _matrixComponent.getLocation() + glm::vec3(0.0, 2.0f, 0.0f);
			m_sphereBody->getWorldTransform().setOrigin(Physics::convertVec3(_armLocation));


			//Get side vector to player
			glm::vec3 _rightSideVector =  Utilities::getVectorFromRotation(
				Rotation(0.0f, _playerComponent.m_latestYRotation, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			std::pair<Location, Location> _handLocations = std::make_pair(Location(0.0f), Location(0.0f));
			float _bestDotRight = -1.0f, _bestDotLeft = -1.0f;



			//<Hit location with wall, normal> pair
			std::vector<std::pair<Location, glm::vec3>> _potentialNormals;
			std::vector<glm::vec3> _nearbyCollisionPoints;

			auto _contactPoints = getContactPoints(collision, *m_sphereBody);
			for (auto &_contactPoint : _contactPoints){
				Location _hitBodyA = Physics::convertVec3(_contactPoint.getPositionWorldOnA());
				Location _hitBodyB = Physics::convertVec3(_contactPoint.getPositionWorldOnB());
				glm::vec3 _hitNormal = glm::normalize(Physics::convertVec3(_contactPoint.m_normalWorldOnB));

				//m_debugLines.push_back(std::make_pair(_hitBodyA, _hitBodyB));
				//m_debugLines.push_back(std::make_pair(_hitBodyA, _hitBodyA + glm::vec3(0.0f, 5.0f, 0.0f)));

				float _dotRight = glm::dot(_rightSideVector, _hitNormal);
				float _dotLeft = glm::dot(-_rightSideVector, _hitNormal);

				if (_dotRight > 0.8f && _dotRight > _bestDotRight){
					_bestDotRight = _dotRight;
					_handLocations.first = _hitBodyA;
				} 
				else if (_dotLeft > 0.8f && _dotLeft > _bestDotLeft){
					_bestDotLeft = _dotLeft;
					_handLocations.second = _hitBodyA;
				}


				_potentialNormals.push_back(std::make_pair(_hitBodyB, _hitNormal));
				_nearbyCollisionPoints.push_back(_hitBodyA);
			}



			//Correct locations using raycasts
			glm::vec3 _dirRight = _handLocations.first - _armLocation;
			glm::vec3 _dirLeft = _handLocations.second - _armLocation;

			if (_dirRight != -_armLocation)
			{
				auto _raycastRight = collision.rayTracingClosest(_armLocation,
					glm::normalize(_dirRight), glm::length(_dirRight), Physics::GROUP_STATIC);
				if (_raycastRight.hasHit()){
					_handLocations.first = Physics::convertVec3(_raycastRight.m_hitPointWorld);
				} else _handLocations.first = Location(0.0f);
			}
			
			if (_dirLeft != -_armLocation)
			{
				auto _raycastLeft = collision.rayTracingClosest(_armLocation,
					glm::normalize(_dirLeft), glm::length(_dirLeft), Physics::GROUP_STATIC);
				if (_raycastLeft.hasHit()){
					_handLocations.second = Physics::convertVec3(_raycastLeft.m_hitPointWorld);
				} else _handLocations.second = Location(0.0f);
			}


			return _handLocations;
		}




		void NearCollisionGrabber::setMaxDistance(Physics::Collision &collision, float distance){
			//distance = 1.0f;

			if (m_sphereBody != nullptr){
				delete m_sphereBody->getCollisionShape();
				m_sphereBody->setCollisionShape(collision.getSphereShape(distance));
			} else{
				m_sphereBody = std::make_unique<btPairCachingGhostObject>();
				m_sphereBody->setWorldTransform(btTransform::getIdentity());

				m_sphereBody->setCollisionShape(collision.getSphereShape(distance));
				m_sphereBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT |
					btCollisionObject::CF_NO_CONTACT_RESPONSE);

				collision.getWorld().addCollisionObject(m_sphereBody.get(),
					Physics::GROUP_GHOST, Physics::GROUP_STATIC);
			}
		}


		const std::vector<std::pair<Location, Location>> &NearCollisionGrabber::getDebugLines() const{
			return m_debugLines;
		}
	}
}
