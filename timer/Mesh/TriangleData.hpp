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


#ifndef TRIANGLE_DATA_HPP
#define TRIANGLE_DATA_HPP

#include "VertexData.hpp"


namespace Engine{
	namespace Meshes{

		struct TriangleData{
		private:
			//Default to invalid value so point isn't inside triangle by default
			glm::vec3 m_barycentricWeights = glm::vec3(-1.0f);

		private:
			VertexData m_vertexData0;
			VertexData m_vertexData1;
			VertexData m_vertexData2;
			VertexData m_averageVertexData;

		private:
			void calculateAverageVertexData();

		public:
			TriangleData();

			TriangleData(const TriangleData &triangleData);

			TriangleData(TriangleData && triangleData);

			TriangleData(const VertexData &vertexData0, 			
				const VertexData &vertexData1, const VertexData &vertexData2);

			TriangleData(VertexData &&vertexData0, 
				VertexData &&vertexData1, VertexData &&vertexData2);

			~TriangleData();


			const VertexData &getVertexData0() const;

			const VertexData &getVertexData1() const;

			const VertexData &getVertexData2() const;

			const VertexData &getAverageVertexData() const;


			bool isPointInside(const glm::vec3 &point);

			const glm::vec3 &getCalculatedBarycentricWeights() const;

		};


	}
}

#endif //TRIANGLE_DATA_HPP