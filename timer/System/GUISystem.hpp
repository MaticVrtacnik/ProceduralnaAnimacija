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