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