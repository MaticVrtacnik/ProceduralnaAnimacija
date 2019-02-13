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