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