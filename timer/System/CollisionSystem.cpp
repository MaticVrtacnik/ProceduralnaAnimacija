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
