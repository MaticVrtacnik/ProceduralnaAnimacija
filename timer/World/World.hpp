#ifndef WORLD_HPP
#define WORLD_HPP

#include <set>
#include <unordered_map>

#include "WorldVariables.hpp"
#include "ResourceManager.hpp"
#include "../Entity/EntityComponentStorage.hpp"
#include "../Entity/IdentifierPool.hpp"
#include "../Entity/IdentifierGenerator.hpp"
#include "../Component/Component.hpp"
#include "../System/System.hpp"




namespace Engine{
	namespace Entities{
		using namespace Systems;

		class World{
		private:
			friend class Entity;

			struct SystemDeleter{ void operator()(System *system){
				system->m_world = nullptr;
				system->m_entities.clear();
			}};

		private:
			using EntityArray = std::vector<Entity>;
			using EntityIdSet = std::unordered_map<std::string, std::set<INT_TYPE>>;
			using EntityNameMap = std::unordered_map<INT_TYPE, std::string>;
			//using EntityNameArray = std::unordered_map<std::string, std::vector<INT_TYPE>>;
			using SystemArray = std::vector<std::unique_ptr<System, SystemDeleter>>;
			

			//for sharing with systems
			Variables m_variables;
			ResourceManager m_resourceManager;
			EntityIdSet m_entityIdMap;
			EntityIdSet m_entityGroups;//TODO
			EntityNameMap m_entityNameMap;
			
		private:
			void addSystem(System &system, std::size_t systemId);

			System &getSystem(std::size_t systemId);

			void removeSystem(std::size_t systemId);

			bool hasSystem(std::size_t systemId) const;

		public:
			World();

			World(std::size_t maxEntities);	


			Variables &getVariables();

			ResourceManager &getResourceManager();
	

			Entity addEntity(const std::string &name);

			EntityArray addEntities(const std::vector<std::string> &names);

			std::size_t getEntityCount() const;

			const EntityArray &getEntities() const;

			EntityArray getEntitiesCopy() const;

			Entity getEntity(INT_TYPE index);

			Entity getEntity(const std::string &name, const std::size_t index = 0);

			EntityArray getEntities(const std::string &name);

			const std::string getEntityName(std::size_t index);

			void removeEntity(const std::string &name, const std::size_t index = 0);

			void removeEntities(const std::string &name);

			void removeEntity(Entity &entity);

			void removeEntities(EntityArray &entities);

			bool isEntityValid(const Entity &entity) const;

			void activateEntity(Entity &entity);

			void deactivateEntity(Entity &entity);

			bool isEntityActivated(const Entity &entity) const;


			void resize(std::size_t size);

			void refresh();

			void update(float fps);

			void render();

			void clear();

			void removeAllSystems();
			

			template <typename T> T &addSystem(T &system);

			template <typename T> T &getSystem();

			template <typename T> void removeSystem();

			template <typename T> bool hasSystem();

		private:
			SystemArray m_systems;
			Generator::EntityIdPool m_entityIdPool;

			struct EntityAttributes{
				EntityAttributes(std::size_t numEntities) :
					m_attributes(numEntities),
					m_componentStorage(numEntities)
				{
				}

				struct Attribute{
					bool m_activated;
					std::string m_name; //TODO

					std::vector<bool> m_activeSystems;
				};

				std::vector<Attribute> m_attributes;
				EntityComponentStorage m_componentStorage;

				void clear(){
					m_attributes.clear();
					m_componentStorage.clear();
				}
				void resize(std::size_t numEntities){
					m_attributes.resize(numEntities);
					m_componentStorage.resize(numEntities);
				}

			}m_entityAttributes;

			struct EntityCache{
				EntityArray m_added;
				EntityArray m_removed;
				EntityArray m_activated;
				EntityArray m_deactivated;

				void clearTmp(){
					m_removed.clear();
					m_activated.clear();
					m_deactivated.clear();
				}

				void clear(){
					clearTmp();
					m_added.clear();
				}

			}m_entityCache;
		};

		template <typename T>
		T &World::addSystem(T &system){
			static_assert(std::is_base_of<System, T>::value, "Template is not a system!");

			//To increase counter to match vector id
			addSystem(system, getSystemId<T>());
			return system;
		}

		template <typename T>
		T &World::getSystem(){
			static_assert(std::is_base_of<System, T>::value, "Template is not a system!");

			return static_cast<T&>(getSystem(getSystemId<T>()));
		}

		template <typename T>
		void removeSystem(){
			static_assert(std::is_base_of<System, T>::value, "Template is not a system!");

			removeSystem(getSystemId<T>());
		}

		template <typename T>
		bool hasSystem(){
			static_assert(std::is_base_of<System, T>::value, "Template is not a system!");

			return hasSystem(getSystemId<T>());
		}
	}
}

#endif //WORLD_HPP