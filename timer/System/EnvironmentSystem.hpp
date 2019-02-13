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