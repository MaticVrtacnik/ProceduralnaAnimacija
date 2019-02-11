#include "CollisionSystem.hpp"

#include "../World/World.hpp"
#include "../Collision/Collision.hpp"
#include "../Component/CollisionComponent.hpp"

namespace Engine{
	namespace Entities{
		namespace Systems{

			CollisionSystem::CollisionSystem(){
				addRequired<CollisionComponent>();
			}

			CollisionSystem::~CollisionSystem(){

			}


			void CollisionSystem::init(){

			}

			void CollisionSystem::onEntityAdded(Entity &entity){

			}

			void CollisionSystem::onEntityRemoved(Entity &entity){
				/*auto &_variables = getWorld().getVariables();
				auto &_collision = _variables.m_collision;

				auto &_collisionComponent = entity.getComponent<CollisionComponent>();
				_collisionComponent.removeBodies();*/
			}


			void CollisionSystem::preUpdate(float fps){

			}

			void CollisionSystem::update(float fps){

			}

			void CollisionSystem::postUpdate(float fps){

			}


		}
	}
}
