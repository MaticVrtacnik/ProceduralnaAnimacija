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