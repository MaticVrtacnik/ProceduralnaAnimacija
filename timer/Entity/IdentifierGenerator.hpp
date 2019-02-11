#ifndef IDENTIFIER_GENERATOR_HPP
#define IDENTIFIER_GENERATOR_HPP

#include <cstddef>

#include "../Define/Defines.hpp"

namespace Engine{
	namespace Entities{
		namespace Generator{

			template <typename TBase>
			class IdGenerator{
			private:
				static INT_TYPE m_nextId;

			public:
				template <typename T>
				static INT_TYPE getId(){
					static const INT_TYPE _id = m_nextId++;
					return _id;
				}
			};

			template <typename TBase>
			INT_TYPE IdGenerator<TBase>::m_nextId = 0;
		}
	}
}

#endif //IDENTIFIER_GENERATOR_HPP