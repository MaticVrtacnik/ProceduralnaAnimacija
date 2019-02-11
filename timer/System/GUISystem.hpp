#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include <set>

#include "System.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			class GUISystem : public System{
			private:
		
			public:
				GUISystem();

				~GUISystem();


				void init() override;

				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;


				void setGUIComponent(const std::string &windowName, int windowHeight, bool on);

				void setGUIValuesFromEntity(const Entity &entity);

				void setEntityValuesFromGUI(const Entity &entity, 
					const std::set<Entity*> &selectedEntities);

			};

		}
	}
}

#endif //GUI_SYSTEM_HPP