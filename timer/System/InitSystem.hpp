#ifndef INIT_SYSTEM_HPP
#define INIT_SYSTEM_HPP

#include "System.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			class InitSystem : public System{
			private:
				float m_currentCameraDistance = 0.0f;

			private:
				void controllCamera();

			public:
				InitSystem();

				~InitSystem();


				void init() override;

				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;

			};

		}
	}
}

#endif //INIT_SYSTEM_HPP