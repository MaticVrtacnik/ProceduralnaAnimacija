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


#ifndef LOGIC_UTILITIES_HPP
#define LOGIC_UTILITIES_HPP

#include <glm\glm.hpp>


namespace Engine{
	namespace Utilities{
	
		bool insideRange(float location, float start, 
			float end, bool includeEdge = true);

		bool insideRangeScale(float location, float start, 
			float scale, bool includeEdge = true);


		bool insideBorder(float location, float center, float innerBorder,
			float outerBorder, bool includeEdge = true);

		bool insideBorder(float location, float innerBorder,
			float outerBorder, bool includeEdge = true);

		bool insideBorderScale(float location, float center, 
			float innerBorder, float borderWidth, bool includeEdge = true);

		bool insideBorderScale(float location, float innerBorder, 
			float borderWidth, bool includeEdge = true);


		bool insideQuad(const glm::vec2 &location, const glm::vec2 &start, 
			const glm::vec2 &end, bool includeEdge = true);

		bool insideQuadScale(const glm::vec2 &location, const glm::vec2 &start, 
			const glm::vec2 &scale, bool includeEdge = true);

		bool insideSquare(const glm::vec2 &location, float start,
			float end, bool includeEdge = true);

		bool insideSquareScale(const glm::vec2 &location, float start,
			float scale, bool includeEdge = true);


		bool insideBorder(const glm::vec2 &location, const glm::vec2 &center, 
			const glm::vec2 &innerBorder, const glm::vec2 &outerBorder, 
			bool includeEdge = true);

		bool insideBorder(const glm::vec2 &location, const glm::vec2 &innerBorder,
			const glm::vec2 &outerBorder, bool includeEdge = true);

		bool insideBorderScale(const glm::vec2 &location, const glm::vec2 &center, 
			const glm::vec2 &innerBorder, const glm::vec2 &borderWidth, 
			bool includeEdge = true);

		bool insideBorderScale(const glm::vec2 &location, const glm::vec2 &innerBorder, 
			const glm::vec2 &borderWidth, bool includeEdge = true);


		bool insideCuboid(const glm::vec3 &location, const glm::vec3 &start, 
			const glm::vec3 &end, bool includeEdge = true);

		bool insideCuboidScale(const glm::vec3 &location, const glm::vec3 &start, 
			const glm::vec3 &scale, bool includeEdge = true);

		bool insideCube(const glm::vec3 &location, float start, 
			float end, bool includeEdge = true);

		bool insideCubeScale(const glm::vec3 &location, float start,
			float scale, bool includeEdge = true);


		bool insideBorder(const glm::vec3 &location, const glm::vec3 &center, 
			const glm::vec3 &innerBorder, const glm::vec3 &outerBorder, 
			bool includeEdge = true);

		bool insideBorder(const glm::vec3 &location, const glm::vec3 &innerBorder, 
			const glm::vec3 &outerBorder, bool includeEdge = true);

		bool insideBorderScale(const glm::vec3 &location, const glm::vec3 &center, 
			const glm::vec3 &innerBorder, const glm::vec3 &borderWidth, 
			bool includeEdge = true);

		bool insideBorderScale(const glm::vec3 &location, const glm::vec3 &innerBorder, 
			const glm::vec3 &borderWidth, bool includeEdge = true);
		
	}
}

#endif //LOGIC_UTILITIES_HPP