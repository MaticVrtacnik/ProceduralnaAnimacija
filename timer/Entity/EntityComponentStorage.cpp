#include "EntityComponentStorage.hpp"

#include <iostream>

#include "../Define/Assert.hpp"

namespace Engine{
	namespace Entities{
		using namespace Components;

		EntityComponentStorage::EntityComponentStorage() :
			m_entityComponents(MAX_ENTITIES)
		{
		}

		EntityComponentStorage::EntityComponentStorage(std::size_t numEntities) :
			m_entityComponents(numEntities)
		{
		}

		EntityComponentStorage::ComponentArray &EntityComponentStorage::getComponentArray(const Entity &entity){
			ASSERT(entity.isValid(), "Entity is not valid!");

			return m_entityComponents[entity.getId()].m_components;
		}

		const EntityComponentStorage::ComponentArray &EntityComponentStorage::getComponentArray(const Entity &entity) const{
			ASSERT(entity.isValid(), "Entity is not valid!");

			return m_entityComponents[entity.getId()].m_components;
		}

		EntityComponentStorage::ComponentTypeArray EntityComponentStorage::getComponentTypes(const Entity &entity) const{
			ASSERT(entity.isValid(), "Entity is not valid!");

			return m_entityComponents[entity.getId()].m_componentTypes;
		}

		Component &EntityComponentStorage::getComponent(const Entity &entity, std::size_t componentId) const{
			ASSERT(hasComponent(entity, componentId), "Can't get component!");

			return *getComponentArray(entity)[componentId];
		}

		std::vector<Component*> EntityComponentStorage::getComponents(const Entity &entity) const{
			ASSERT(entity.isValid(), "Entity is not valid!");

			auto& _components = getComponentArray(entity);
			std::vector<Component*> _newArray(_components.size());
			for (auto &_component : _components)
				_newArray.emplace_back(_component.get());

			return _newArray;
		}

		bool EntityComponentStorage::hasComponent(const Entity &entity, std::size_t componentId) const{
			ASSERT(entity.isValid(), "Entity is not valid!");

			auto &_components = getComponentArray(entity);
			bool _validId = _components.size() > componentId;
			bool _validPtr = _components[componentId] != nullptr;
			return _validId && _validPtr;
		}

		void EntityComponentStorage::addComponent(Entity &entity, Component *component, INT_TYPE componentId){
			auto &_data = m_entityComponents[entity.getId()];
			_data.m_components[componentId].reset(component);
			_data.m_componentTypes[componentId] = true;
		}

		void EntityComponentStorage::removeComponent(Entity &entity, std::size_t componentId){
			ASSERT(entity.isValid() && hasComponent(entity, componentId), "Can't remove component!");

			auto &_data = m_entityComponents[entity.getId()];
			_data.m_components[componentId].reset();
			_data.m_componentTypes[componentId] = false;
		}

		void EntityComponentStorage::removeAllComponents(Entity &entity){
			ASSERT(entity.isValid(), "Entity is not valid");

			auto &_data = m_entityComponents[entity.getId()];
			for (auto & _component : _data.m_components) _component.reset();
			for (auto & _types : _data.m_componentTypes) _types = false;
		}

		void EntityComponentStorage::resize(std::size_t size){
			m_entityComponents.resize(size);
		}

		void EntityComponentStorage::clear(){
			m_entityComponents.clear();
		}
	}
}
