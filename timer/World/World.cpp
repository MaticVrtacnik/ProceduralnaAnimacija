#include "World.hpp"

#include "../Renderer/DeferredRenderer.hpp"
#include "../Input/Input.hpp"
#include "../GUI/GUI.hpp"
#include "../GUI/GUIWindow.hpp"


namespace Engine{
	namespace Entities{

		World::World() :
			World(MAX_ENTITIES)
		{
		}

		World::World(std::size_t maxEntities) :
			m_entityIdPool(maxEntities),
			m_entityAttributes(maxEntities),
			m_variables(this)
		{
		}


		void World::addSystem(System &system, std::size_t systemId){
			//TODO ASSERT(system.m_world == nullptr, "System already in other world!");
			ASSERT(systemId == m_systems.size(), "System ID is not valid");

			m_systems.push_back(std::move(std::unique_ptr<System, SystemDeleter>(&system)));
			system.setWorld(*this);
		}

		System &World::getSystem(std::size_t systemId){
			ASSERT(hasSystem(systemId), "System doens't exist!");

			return *m_systems[systemId];
		}

		void World::removeSystem(std::size_t systemId){
			ASSERT(hasSystem(systemId), "System doens't exist!");

			m_systems.erase(m_systems.begin() + systemId);
		}

		bool World::hasSystem(std::size_t systemId) const{
			return systemId < m_systems.size() && m_systems[systemId] != nullptr;
		}

		void World::removeAllSystems(){
			m_systems.clear(); 
		}


		Entity World::addEntity(const std::string &name){
			//PRINT("ADDED: " + name);
			m_entityCache.m_added.emplace_back(*this, m_entityIdPool.getId());	

			auto _entityId = m_entityCache.m_added.back().getId();
			m_entityIdMap[name].insert(_entityId);
			m_entityNameMap[_entityId] = name;

			return m_entityCache.m_added.back();
		}

		World::EntityArray World::addEntities(const std::vector<std::string> &names){
			std::vector<Entity> _tmp(names.size());
			auto _newSize = getEntityCount() + names.size();
			if (_newSize > m_entityIdPool.getSize())
				resize(_newSize);

			for (auto &name : names){
				_tmp.push_back(this->addEntity(name));
			}

			return _tmp;
		}

		std::size_t World::getEntityCount() const{
			return m_entityCache.m_added.size();
		}

		const World::EntityArray &World::getEntities() const{
			return m_entityCache.m_added;
		}

		World::EntityArray World::getEntitiesCopy() const{
			return getEntities();
		}

		Entity World::getEntity(INT_TYPE index){
			return Entity{ *this, index };
		}

		Entity World::getEntity(const std::string &name, const std::size_t index){
			auto _entityItr = m_entityIdMap.find(name);

			if (_entityItr == m_entityIdMap.end()){
				ASSERT(0, "Cannot get entity '" + name + "'");
				return Entity();
			}
			else if (index >= m_entityIdMap[name].size()){
				ASSERT(0, "Entity id '" + std::to_string(index) + "' out of range");
				return Entity();
			}
			else return getEntity(*std::next((*_entityItr).second.begin(), index));
		}

		World::EntityArray World::getEntities(const std::string &name){
			auto _entityItr = m_entityIdMap.find(name);
			
			if (_entityItr == m_entityIdMap.end()){
				ASSERT(0, "Cannot get entity '" + name + "'");
				return std::vector<Entity>();
			}else{
				auto &_entitySet = (*_entityItr).second;
				std::vector<Entity> _entities;

				for (auto _entityId : _entitySet){
					_entities.push_back(getEntity(_entityId));
				}
	
				return _entities;
			}
		}

		const std::string World::getEntityName(std::size_t index){
			auto _entityItr = m_entityNameMap.find(index);

			if (_entityItr == m_entityNameMap.end()){
				ASSERT(0, "Cannot get entity name with id '" + std::to_string(index) + "'");
				return ""; //TODO remove reference?
			}else{
				return (*_entityItr).second;
			}
		}


		void World::removeEntity(Entity &entity){
			auto _entityId = entity.getId();
			auto _entityName = entity.getName();
			//m_entityNameMap //TODO REMOVE FROM NAME MAP

			m_entityIdMap[_entityName].erase(_entityId);
			m_entityNameMap.erase(_entityId);

			deactivateEntity(entity);
			m_entityCache.m_removed.push_back(entity);
		}

		void World::removeEntity(const std::string &name, std::size_t index){
			this->removeEntity(getEntity(name, index));
		}

		void World::removeEntities(EntityArray &entities){
			for (auto &_entity : entities){
				removeEntity(_entity);
			}
		}

		void World::removeEntities(const std::string &name){
			for (auto &entity : getEntities(name)){
				removeEntity(entity);
			}
		}

		bool World::isEntityValid(const Entity &entity) const{
			return true; //TODO entity.isValid();
		}

		void World::activateEntity(Entity &entity){
			ASSERT(isEntityValid(entity), "Entity is not valid!");
			m_entityCache.m_activated.push_back(entity);
		}

		void World::deactivateEntity(Entity &entity){
			ASSERT(isEntityValid(entity), "Entity is not valid!");
			m_entityCache.m_deactivated.push_back(entity);
		}

		bool World::isEntityActivated(const Entity &entity) const{
			ASSERT(isEntityValid(entity), "Entity is not valid!");
			return m_entityAttributes.m_attributes[entity.getId()].m_activated;
		}

		void World::resize(std::size_t size){
			m_entityIdPool.resize(size);
			m_entityAttributes.resize(size);
		}

		void World::refresh(){
			for (auto &_entity : m_entityCache.m_activated){
				auto &_attrib = m_entityAttributes.m_attributes[_entity.getId()];
				_attrib.m_activated = true;

				for (auto _it = m_systems.begin(); _it != m_systems.end(); ++_it){
					auto &_system = *_it;
					std::size_t _index = std::distance(m_systems.begin(), _it);

					if (_system->hasComponentRequirements(
						m_entityAttributes.m_componentStorage.getComponentTypes(_entity)))
					{
						if (_attrib.m_activeSystems.size() <= _index || !_attrib.m_activeSystems[_index]){

							_system->addEntity(_entity);
							if (_index >= _attrib.m_activeSystems.size())
								_attrib.m_activeSystems.resize(_index + 1);

							_attrib.m_activeSystems[_index] = true;
						}
					}
					else if (_attrib.m_activeSystems.size() > _index && _attrib.m_activeSystems[_index]){
						_system->removeEntity(_entity);
						_attrib.m_activeSystems[_index] = false;
					}
				}
			}

			for (auto &_entity : m_entityCache.m_deactivated){
				auto &_attrib = m_entityAttributes.m_attributes[_entity.getId()];
				_attrib.m_activated = false;

				for (std::size_t i = 0; i < m_systems.size(); ++i){
					if (_attrib.m_activeSystems.size() <= i) continue;
					if (_attrib.m_activeSystems[i]){
						m_systems[i]->removeEntity(_entity);
						_attrib.m_activeSystems[i] = false;
					}
				}
			}

			for (auto &_entity : m_entityCache.m_removed){
				auto _begin = m_entityCache.m_added.begin();
				auto _end = m_entityCache.m_added.end();
				m_entityCache.m_added.erase(std::remove(_begin, _end, _entity), _end);

				m_entityAttributes.m_componentStorage.removeAllComponents(_entity);
				m_entityIdPool.remove(_entity.getId());
			}

			m_entityCache.clearTmp();
		}

		void World::update(float fps) {
			m_variables.update(fps);
			int _width = m_variables.m_gui->getIndex().getChild<GUI::GUIWindow>("tab").getLocation().x;

			//glViewport(0, 0, _width, DEFAULT_SCREEN_HEIGHT - 70);

			//clear all framebuffers before draw
			m_variables.m_renderer->clearFramebuffers();

			for (auto &_system : m_systems){
				_system.get()->preUpdate(fps);
			}

			for (auto &_system : m_systems){
				_system.get()->update(fps);
			}

			for (auto &_system : m_systems){
				_system.get()->postUpdate(fps);
			}

			//glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

			auto &_guiShader = m_resourceManager.getShader("GUI");
			m_variables.m_gui->draw(_guiShader, *m_variables.m_input, 
				m_variables.m_screenWidth, m_variables.m_screenHeight, m_variables.m_editing);
		}

		void World::clear(){
			removeAllSystems();
			m_entityAttributes.clear();
			m_entityCache.clear();
			m_entityIdPool.clear();
		}



		Variables &World::getVariables(){
			return m_variables;
		}

		ResourceManager &World::getResourceManager(){
			return m_resourceManager;
		}

	}
}
