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