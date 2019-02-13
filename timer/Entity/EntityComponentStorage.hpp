///
/// anax
/// An open source C++ entity system.
///
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///


#ifndef ENTITY_COMPONENT_STORAGE_HPP
#define ENTITY_COMPONENT_STORAGE_HPP

#include <array>
#include <vector>
#include <memory>

#include "Entity.hpp"
#include "../Define/Defines.hpp"
#include "../Component/Component.hpp"

namespace Engine{
	namespace Entities{
		using namespace Components;

		class EntityComponentStorage{
		private:

			//TODO change to unique_ptr
			using ComponentArray = std::array<std::shared_ptr<Component>, MAX_COMPONENTS>;
			using ComponentTypeArray = std::array<bool, MAX_COMPONENTS>;

			struct EntityComponents{

				EntityComponents(){
					for (auto &_type : m_componentTypes)
						_type = false;
				}

				ComponentArray m_components;
				ComponentTypeArray m_componentTypes;
			};

			std::vector<EntityComponents> m_entityComponents;

		public:
			EntityComponentStorage();
			EntityComponentStorage(std::size_t numEntities);

			ComponentArray &getComponentArray(const Entity &entity);
			const ComponentArray &getComponentArray(const Entity &entity) const;
			ComponentTypeArray getComponentTypes(const Entity &entity) const;
			Component &getComponent(const Entity &entity, std::size_t componentId) const;
			std::vector<Component*> getComponents(const Entity &entity) const;
			bool hasComponent(const Entity &entity, std::size_t componentId) const;

			void addComponent(Entity &entity, Component *component, INT_TYPE componentId);
			void removeComponent(Entity &entity, std::size_t componentId);
			void removeAllComponents(Entity &entity);

			void resize(std::size_t size);
			void clear();
		};

	}
}

#endif //ENTITY_COMPONENT_STORAGE_HPP