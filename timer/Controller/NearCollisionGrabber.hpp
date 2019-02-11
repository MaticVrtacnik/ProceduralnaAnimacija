#ifndef NEAR_COLLISION_GRABBER_HPP
#define NEAR_COLLISION_GRABBER_HPP

#include <memory>
#include <vector>
#include <SDL2\SDL_timer.h>

#include "../TransformVariables.hpp"


class btPairCachingGhostObject;
class btCylinderShape;
class btManifoldPoint;

namespace Engine{
	namespace Entities{
		class Entity;
	}
}

namespace Engine{
	namespace Physics{
		class Collision;
	}
}


namespace Engine{
	namespace Controller{

		class NearCollisionGrabber{

		public:
			std::unique_ptr<btPairCachingGhostObject> m_sphereBody = nullptr;

			//Location pair for debug draw lines
			std::vector<std::pair<Location, Location>> m_debugLines;

		private:
			std::vector<const btManifoldPoint> getContactPoints(
				Physics::Collision &collision, btPairCachingGhostObject &ghostObject);

		public:
			NearCollisionGrabber();

			~NearCollisionGrabber();

			//Returns pair of locations one for each of the arms
			std::pair<Location, Location> getNearbyHandCollisionPoints(
				Physics::Collision &collision, Entities::Entity &playerEntity);

			void setMaxDistance(Physics::Collision &collision, float distance);


			const std::vector<std::pair<Location, Location>> &getDebugLines() const;
		};

	}
}

#endif //NEAR_COLLISION_GRABBER_HPP