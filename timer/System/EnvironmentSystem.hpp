#ifndef ENVIRONMENT_SYSTEM_HPP
#define ENVIRONMENT_SYSTEM_HPP

#include "System.hpp"
#include "../Environment/Skybox.hpp"

//#define DRAW_TERRAIN
//#define DRAW_VEGETATION
//#define DRAW_WATER


namespace Engine{
	namespace Entities{
		namespace Systems{

			class EnvironmentSystem : public System{
			private:
				Environment::Skybox m_skybox;
				void drawSkybox();

				void updateSunLocation();
				void updateShadows();

			public:
				EnvironmentSystem();

				~EnvironmentSystem();


				void init() override;

				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;


				void onEntityAdded(Entities::Entity &entity) override;

				void onEntityRemoved(Entities::Entity &entity) override;


				void reload();

				Environment::Skybox &getSkybox();
				
			};

		}
	}
}

#endif //ENVIRONMENT_SYSTEM_HPP