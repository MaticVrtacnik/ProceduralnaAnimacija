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


#include "System.hpp"

#include "../World/World.hpp"
#include "../Entity/IdentifierPool.hpp"

namespace Engine{
	namespace Entities{
		namespace Systems{

			System::System() //:  m_world(nullptr)
			{
				for (std::size_t _i = 0; _i < m_requiredComponents.size(); ++_i)
					m_requiredComponents[_i] = false;

				for (std::size_t _i = 0; _i < m_excludedComponents.size(); ++_i)
					m_excludedComponents[_i] = false;
			}

			World &System::getWorld(){
				ASSERT(m_world != nullptr, "No world attached to system!");

				return *m_world;
			}

			const std::vector<Entity> &System::getEntities() const{
				return m_entities;
			}

			bool System::hasComponentRequirements(const std::array<bool, MAX_COMPONENTS> &componentTypes){
				for (std::size_t _i = 0; _i < m_requiredComponents.size(); ++_i){
					if (m_requiredComponents[_i] && !componentTypes[_i]){
						return false;
					}
				}

				for (std::size_t _i = 0; _i < m_excludedComponents.size(); ++_i){
					if (m_excludedComponents[_i] && componentTypes[_i]){
						return false;
					}
				}

				return true;
			}

			void System::addEntity(Entity &entity){
				m_entities.push_back(entity);
				onEntityAdded(entity);
			}

			void System::removeEntity(Entity &entity){
				auto _begin = m_entities.begin();
				auto _end = m_entities.end();
				m_entities.erase(std::remove(_begin, _end, entity), _end);
				onEntityRemoved(entity);
			}

			void System::setWorld(World &world){
				m_world = &world;
				init();
			}

		}
	}
}