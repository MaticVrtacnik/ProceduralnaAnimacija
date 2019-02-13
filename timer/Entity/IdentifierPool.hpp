///
/// anax
/// An open source C++ entity system.
///
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///


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