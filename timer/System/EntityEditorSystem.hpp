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


#ifndef ENTITY_EDITOR_SYSTEM_HPP
#define ENTITY_EDITOR_SYSTEM_HPP

#include <set> //TODO maybe change to unordered_set

#include "System.hpp"
#include "GUISystem.hpp"
#include "../Collision/Collision.hpp"
#include "../Component/MeshComponent.hpp"
#include "../TransformVariables.hpp"
#include "../GUI/GUIWindowStack.hpp"
#include "../GUI/GUI.hpp"
#include "../Deprecated/CollisionRenderer.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			enum EditingMode{
				EDITING_TRANSLATE,
				EDITING_ROTATE,
				EDITING_SCALE,
			};


			class EntityEditorSystem : public System{
			private:
				Physics::CollisionRenderer m_collisionRenderer;

			private:
				Entity *m_selectedEntity = nullptr;
				std::set<Entity*> m_selectedEntities;

				EditingMode m_editingMode = EDITING_TRANSLATE;
				glm::vec3 m_previousLocation = glm::vec3(0.0f);
				glm::vec3 m_startingLocation = glm::vec3(0.0f);

				bool m_pickingX = false;
				bool m_pickingY = false;
				bool m_pickingZ = false;

			private:
				void rayPicking();

				void setSelectedEntity(Entity *entity);

				void calculateOffset();

				void editTranslation();

				void editRotation();

				void editScaleScreen();

				void editScaleWorld();


				void handleButtonInput();

				void renderEditorMeshes();

			public:
				EntityEditorSystem();

				~EntityEditorSystem();


				void init() override;

				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;

				void onEntityAdded(Entity &entity) override;

				void onEntityRemoved(Entity &entity) override;

			private:
				template <typename Component, typename... Args>
				void initComponentGUIAction(const std::string &windowName, Args&&... args);

			};


			template <typename Component, typename... Args>
			void EntityEditorSystem::initComponentGUIAction(const std::string &windowName, Args&&... args){
				auto &_stack = getWorld().getVariables().m_gui->getIndex().
					getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
					getWindow("components").getChild<GUI::GUIWindowStack>("stack");

				auto &_button = _stack.getWindow(windowName).getChild<GUI::GUIButton>("x");
				int _windowHeight = _stack.getWindow(windowName).getScale().y;

				_button.setAction([this, windowName, _windowHeight, &args...](){
					if (m_selectedEntity != nullptr){
						bool _hasComponent = m_selectedEntity->hasComponent<Component>();

						auto &_GUISystem = getWorld().getSystem<GUISystem>();
						_GUISystem.setGUIComponent(windowName, _windowHeight, !_hasComponent);

						if (_hasComponent){
							m_selectedEntity->removeComponent<Component>();
						} else{
							m_selectedEntity->addComponent<Component>
							{ std::forward<Args>(args)... };
						}

						setSelectedEntity(m_selectedEntity);
						m_selectedEntity->activate();
					}
				});
			}

		}
	}
}

#endif //ENTITY_EDITOR_SYSTEM_HPP