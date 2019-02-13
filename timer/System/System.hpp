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


#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <array>

#include "../Define/Assert.hpp"
#include "../Entity/Entity.hpp"
#include "../Entity/IdentifierGenerator.hpp"

namespace Engine{
	namespace Entities{
		namespace Systems{

			class System{
			protected:
				friend class World;
				World *m_world;

				std::vector<Entity> m_entities;
				std::array<bool, MAX_COMPONENTS> m_requiredComponents;
				std::array<bool, MAX_COMPONENTS> m_excludedComponents;

			public:
				System();

				~System(){};

				//TODO CONVERT TO ARGUMENT LIST (template <typename... T>)
				template <typename T> System &addRequired();
				template <typename T> System &addExcluded();

				World &getWorld();
				const std::vector<Entity> &getEntities() const;
				bool hasComponentRequirements(const std::array<bool, MAX_COMPONENTS> &componentTypes);

			protected:
				virtual void init(){ }
				virtual void preUpdate	(float fps) { }
				virtual void update		(float fps) { }
				virtual void postUpdate	(float fps) { }
				virtual void onEntityAdded(Entity &entity){ }
				virtual void onEntityRemoved(Entity &entity) { }

				void addEntity(Entity &entity);
				void removeEntity(Entity &entity);
				void setWorld(World &world);
			};

			template <typename T> System &System::addRequired(){
				auto _id = getComponentId<T>();
				ASSERT(_id < MAX_COMPONENTS, "Component id out of range!");

				m_requiredComponents[_id] = true;
				return *this;
			}

			template <typename T> System &System::addExcluded(){
				auto _id = getComponentId<T>();
				ASSERT(_id < MAX_COMPONENTS, "Component id out of range!");

				m_excludedComponents[_id] = true;
				return *this;
			}

			template <typename T>
			INT_TYPE getSystemId(){
				return Generator::IdGenerator<System>::getId<T>();
			}

		}
	}
}

#endif //SYSTEM_HPP