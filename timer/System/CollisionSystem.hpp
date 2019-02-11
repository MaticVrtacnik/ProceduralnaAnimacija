#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "System.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			class CollisionSystem : public System{
			private:

			public:
				CollisionSystem();

				~CollisionSystem();


				void init() override;

				void onEntityAdded(Entity &entity) override;

				void onEntityRemoved(Entity &entity) override;


				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;

			};
		}
	}
}

#endif //COLLISION_SYSTEM_HPP