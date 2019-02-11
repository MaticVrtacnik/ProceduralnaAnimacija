#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <array>
#include <memory>

#include "../Define/Assert.hpp"
#include "../Component/Component.hpp"


namespace Engine{ namespace Entities{
	class World;
} }


namespace Engine{
	namespace Entities{

		class Entity{
		private:
			INT_TYPE m_id;
			World *m_world;

		private:
			void addComponent(Components::Component *component, INT_TYPE componentId);

		public:
			Entity();

			Entity(const Entity &entity);

			Entity(World &world, INT_TYPE id);

			Entity(Entity &&entity);


			Entity &operator=(const Entity& entity);

			Entity &operator=(Entity &&entity);

			bool operator==(const Entity &entity) const;

			bool operator!=(const Entity &entity) const;

			bool operator<(const Entity &entity) const;


			bool isValid() const;

			const INT_TYPE &getId() const;

			const std::string getName() const;

			World &getWorld() const;


			bool isActivated() const;

			void activate();

			void deactivate();

			void remove();


			bool hasComponent(std::size_t componentId) const;

			Components::Component &getComponent(std::size_t componentId) const;

			std::vector<Components::Component*> getComponents() const;

			std::array<bool, MAX_COMPONENTS> getComponentTypeArray() const;

			void removeComponent(std::size_t componentId);

			void removeAllComponents();


			template <typename T> T &addComponent(T *component);

			template <typename T, typename... Args> T &addComponent(Args&&... args);

			template <typename T> void removeComponent();

			template <typename T> T &getComponent() const;

			template<typename T> bool hasComponent() const;

		};


		template <typename T>
		T &Entity::addComponent(T *component){
			static_assert(std::is_base_of<Components::Component, T>::value,
				"Can't add component! Template value not a component!");

			ASSERT(component != nullptr, "Component is a null pointer");

			addComponent(component, getComponentId<T>());
			return *component;
		}

		template <typename T, typename... Args>
		T &Entity::addComponent(Args&&... args){
			static_assert(std::is_base_of<Components::Component, T>::value,
				"Can't add component! Template value not a component!");

			auto component = new T{ std::forward<Args>(args)... };
			addComponent(component, getComponentId<T>());
			return *component;
		}

		template <typename T> 
		void Entity::removeComponent(){
			static_assert(std::is_base_of<Components::Component, T>::value,
				"Can't remove component! Template value not a component!");
			removeComponent(getComponentId<T>());
		}

		template <typename T>
		T &Entity::getComponent() const{
			static_assert(std::is_base_of<Components::Component, T>::value,
				"Can't remove component! Template value not a component!");
			return static_cast<T&>(getComponent(getComponentId<T>()));
		}

		template <typename T>
		bool Entity::hasComponent() const{
			static_assert(std::is_base_of<Components::Component, T>::value,
				"Can't remove component! Template value not a component!");
			return hasComponent(getComponentId<T>());
		}

	}
}

#endif //ENTITY_HPP