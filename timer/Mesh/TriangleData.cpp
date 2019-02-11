#include "TriangleData.hpp"

#include <memory>
#include "../Utility/MathUtilities.hpp"
#include "../Utility/LogicUtilities.hpp"


namespace Engine{
	namespace Meshes{

		TriangleData::TriangleData(){

		}

		TriangleData::TriangleData(const TriangleData &triangleData):
			m_vertexData0(triangleData.m_vertexData0),
			m_vertexData1(triangleData.m_vertexData1),
			m_vertexData2(triangleData.m_vertexData2)
		{
			calculateAverageVertexData();
		}

		TriangleData::TriangleData(TriangleData && triangleData):
			m_vertexData0(std::move(triangleData.m_vertexData0)),
			m_vertexData1(std::move(triangleData.m_vertexData1)),
			m_vertexData2(std::move(triangleData.m_vertexData2))
		{
			calculateAverageVertexData();
		}

		TriangleData::TriangleData(const VertexData &vertexData0,
			const VertexData &vertexData1, const VertexData &vertexData2
		):
			m_vertexData0(vertexData0), 
			m_vertexData1(vertexData1),
			m_vertexData2(vertexData2)
		{
			calculateAverageVertexData();
		}

		TriangleData::TriangleData(VertexData &&vertexData0,
			VertexData &&vertexData1, VertexData &&vertexData2
		):
			m_vertexData0(std::move(vertexData0)),
			m_vertexData1(std::move(vertexData1)),
			m_vertexData2(std::move(vertexData2))
		{
			calculateAverageVertexData();
		}

		TriangleData::~TriangleData(){

		}


		void TriangleData::calculateAverageVertexData(){
			auto &_v0 = getVertexData0();
			auto &_v1 = getVertexData1();
			auto &_v2 = getVertexData2();

			m_averageVertexData.setVertex((_v0.getVertex() + _v1.getVertex() + _v2.getVertex()) * 0.33333f);
			m_averageVertexData.setNormal((_v0.getNormal() + _v1.getNormal() + _v2.getNormal()) * 0.33333f);
			m_averageVertexData.setTangent((_v0.getTangent() + _v1.getTangent() + _v2.getTangent()) * 0.33333f);
			m_averageVertexData.setColor((_v0.getColor() + _v1.getColor() + _v2.getColor())  * 0.33333f);
			m_averageVertexData.setUv((_v0.getUv() + _v1.getUv() + _v2.getUv()) * 0.33333f);
		}


		const VertexData &TriangleData::getVertexData0() const{
			return m_vertexData0;
		}

		const VertexData &TriangleData::getVertexData1() const{
			return m_vertexData1;
		}

		const VertexData &TriangleData::getVertexData2() const{
			return m_vertexData2;
		}

		const VertexData &TriangleData::getAverageVertexData() const{
			return m_averageVertexData;
		}


		bool TriangleData::isPointInside(const glm::vec3 &point){
			glm::vec3 _vertex0 = m_vertexData0.getVertex();
			glm::vec3 _vertex1 = m_vertexData1.getVertex();
			glm::vec3 _vertex2 = m_vertexData2.getVertex();

			m_barycentricWeights = Utilities::getBarycentricWeights(
				point, _vertex0, _vertex1, _vertex2);

			bool _validX = Utilities::insideRange(m_barycentricWeights.x, 0.0f, 1.0f);
			bool _validY = Utilities::insideRange(m_barycentricWeights.y, 0.0f, 1.0f);
			bool _validZ = Utilities::insideRange(m_barycentricWeights.z, 0.0f, 1.0f);

			return _validX && _validY && _validZ;
		}

		const glm::vec3 &TriangleData::getCalculatedBarycentricWeights() const{
			return m_barycentricWeights;
		}

	}
}
