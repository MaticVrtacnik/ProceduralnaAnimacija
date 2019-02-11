#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "../Entity/IdentifierGenerator.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			class Component{
			public:
				Component(){

				}

				virtual ~Component(){

				}

			};


			template <typename T>
			INT_TYPE getComponentId(){
				return Generator::IdGenerator<Component>::getId<T>();
			}

		}
	}
}

#endif //COMPONENT_HPP