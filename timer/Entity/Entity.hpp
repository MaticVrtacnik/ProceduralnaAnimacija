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