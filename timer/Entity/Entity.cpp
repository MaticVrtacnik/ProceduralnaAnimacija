#include "Entity.hpp"

#include "EntityComponentStorage.hpp"
#include "../World/World.hpp"

namespace Engine{
	namespace Entities{
		using namespace Components;

		Entity::Entity() :
			m_id(-1),
			m_world(nullptr)
		{
		}

		Entity::Entity(const Entity &entity) :
			m_id(entity.m_id),
			m_world(entity.m_world)
		{
		}

		Entity::Entity(World &world, INT_TYPE id) :
			m_id(id),
			m_world(&world)
		{
		}

		Entity::Entity(Entity &&entity) :
			m_id(std::move(entity.m_id)),
			m_world(std::move(entity.m_world))
		{
		}



		Entity &Entity::operator=(const Entity& entity){
			m_id = entity.m_id;
			m_world = entity.m_world;
			return *this;
		}

		Entity &Entity::operator=(Entity &&entity){
			m_id = std::move(entity.m_id);
			m_world = std::move(entity.m_world);
			return *this;
		}

		bool Entity::operator==(const Entity &entity) const{
			return m_id == entity.m_id && m_world == entity.m_world;
		}

		bool Entity::operator!=(const Entity &entity) const{
			return !operator==(entity);
		}

		bool Entity::operator<(const Entity &entity) const{
			return m_id < entity.m_id;
		}



		bool Entity::isValid() const{
			return m_world != nullptr && m_id >= 0 && m_id < MAX_ENTITIES;
		}

		const INT_TYPE &Entity::getId() const{
			return m_id;
		}

		const std::string Entity::getName() const{
			return m_world->getEntityName(m_id);
		}

		World &Entity::getWorld() const{
			ASSERT(m_world != nullptr, "Entity not attached to world!");
			return *m_world;
		}



		bool Entity::isActivated() const{
			return m_world->isEntityActivated(*this);
		}

		void Entity::activate(){
			m_world->activateEntity(*this);
		}

		void Entity::deactivate() {
			m_world->deactivateEntity(*this);
		}

		void Entity::remove() {
			m_world->removeEntity(*this);
		}



		void Entity::addComponent(Component *component, INT_TYPE componentId){
			m_world->m_entityAttributes.m_componentStorage.addComponent(*this, component, componentId);
		}

		void Entity::removeComponent(std::size_t componentId){
			m_world->m_entityAttributes.m_componentStorage.removeComponent(*this, componentId);
		}

		void Entity::removeAllComponents(){
			m_world->m_entityAttributes.m_componentStorage.removeAllComponents(*this);
		}

		bool Entity::hasComponent(std::size_t componentId) const{
			return m_world->m_entityAttributes.m_componentStorage.hasComponent(*this, componentId);
		}

		Components::Component &Entity::getComponent(std::size_t componentId) const{
			return m_world->m_entityAttributes.m_componentStorage.getComponent(*this, componentId);
		}

		std::vector<Components::Component*> Entity::getComponents() const{
			return m_world->m_entityAttributes.m_componentStorage.getComponents(*this);
		}

		std::array<bool, MAX_COMPONENTS> Entity::getComponentTypeArray() const{
			return m_world->m_entityAttributes.m_componentStorage.getComponentTypes(*this);
		}

		

	}
}
