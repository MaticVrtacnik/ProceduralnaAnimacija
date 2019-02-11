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