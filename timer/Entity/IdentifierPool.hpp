#ifndef IDENTIFIER_POOL_H
#define IDENTIFIER_POOL_H

#include <vector>
#include <string>
#include <array>

#include "../Define/Assert.hpp"
#include "../Define/Defines.hpp"

namespace Engine{
	namespace Entities{
		namespace Generator{

			class EntityIdPool{
			private:
				std::size_t m_defaultPoolSize, m_maxPoolSize;
				std::vector<INT_TYPE> m_freeIds;

			public:
				EntityIdPool(std::size_t size) :
					m_freeIds(size),
					m_defaultPoolSize(size),
					m_maxPoolSize(size)
				{
					for (INT_TYPE _i = 0; _i < size; ++_i){
						m_freeIds.push_back(_i);
					}
				}

				INT_TYPE getId(){
					if (!m_freeIds.empty()){
						int _id = m_freeIds.back();
						m_freeIds.pop_back();
						return _id;
					}
					else{
						//TODO ?
						ASSERT(0, "Ran out of entity Ids!");
						return -1;
					}
				}
				std::size_t getSize() const{
					return m_freeIds.size();
				}

				void remove(INT_TYPE id){
					m_freeIds.push_back(id);
				}

				void resize(std::size_t size){
					m_freeIds.resize(size);
					m_maxPoolSize = size;
				}

				void clear(){
					m_freeIds.clear();
					//TODO ???
					//m_freeIds.resize(m_defaultPoolSize);
				}
			};

		}
	}
}

#endif //IDENTIFIER_POOL_H