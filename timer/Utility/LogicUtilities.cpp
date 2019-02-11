#include "LogicUtilities.hpp"


namespace Engine{
	namespace Utilities{

		bool insideRange(float location, float start, float end, bool includeEdge){
			return includeEdge ? (location >= start && location <= end) :
				(location > start && location < end);
		}

		bool insideRangeScale(float location, 
			float start, float scale, bool includeEdge)
		{
			return insideRange(location, start, start + scale);
		}


		bool insideBorder(float location, float center, 
			float innerBorder, float outerBorder, bool includeEdge)
		{
			bool _insideOuter = insideRange(location,
				center - outerBorder, center + outerBorder, includeEdge);
			bool _insideInner = insideRange(location,
				center - innerBorder, center + innerBorder, !includeEdge);

			return  _insideOuter &&	!_insideInner;
		}

		bool insideBorder(float location, float innerBorder, 
			float outerBorder, bool includeEdge)
		{
			return insideBorder(location, 0.0f, 
				innerBorder, outerBorder, includeEdge);
		}

		bool insideBorderScale(float location, float center, 
			float innerBorder, float borderWidth, bool includeEdge)
		{
			return insideBorder(location, center, innerBorder, 
				innerBorder + borderWidth, includeEdge);
		}

		bool insideBorderScale(float location, float innerBorder,
			float borderWidth, bool includeEdge)
		{
			return insideBorderScale(location, 0.0f,
				innerBorder, borderWidth, includeEdge);
		}


		bool insideQuad(const glm::vec2 &location,
			const glm::vec2 &start, const glm::vec2 &end, bool includeEdge)
		{
			return insideRange(location.x, start.x, end.x, includeEdge) &&
				insideRange(location.y, start.y, end.y, includeEdge);
		}

		bool insideQuadScale(const glm::vec2 &location,
			const glm::vec2 &start, const glm::vec2 &scale, bool includeEdge)
		{
			return insideQuad(location, start, start + scale);
		}

		bool insideSquare(const glm::vec2 &location,
			float start, float end, bool includeEdge)
		{
			return insideQuad(location,
				glm::vec2(start), glm::vec2(end), includeEdge);
		}

		bool insideSquareScale(const glm::vec2 &location,
			float start, float scale, bool includeEdge)
		{
			return insideQuadScale(location,
				glm::vec2(start), glm::vec2(scale), includeEdge);
		}


		bool insideBorder(const glm::vec2 &location, const glm::vec2 &center,
			const glm::vec2 &innerBorder, const glm::vec2 &outerBorder, bool includeEdge)
		{
			bool _insideOuter = insideQuad(location,
				center - outerBorder, center + outerBorder, includeEdge);
			bool _insideInner = insideQuad(location,
				center - innerBorder, center + innerBorder, !includeEdge);

			return  _insideOuter &&	!_insideInner;
		}

		bool insideBorder(const glm::vec2 &location, const glm::vec2 &innerBorder,
			const glm::vec2 &outerBorder, bool includeEdge)
		{
			return insideBorder(location, glm::vec2(0.0f),
				innerBorder, outerBorder, includeEdge);
		}

		bool insideBorderScale(const glm::vec2 &location, const glm::vec2 &center, 
			const glm::vec2 &innerBorder, const glm::vec2 &borderWidth, bool includeEdge)
		{
			return insideBorder(location, center, innerBorder,
				innerBorder + borderWidth, includeEdge);
		}

		bool insideBorderScale(const glm::vec2 &location, const glm::vec2 &innerBorder,
			const glm::vec2 &borderWidth, bool includeEdge)
		{
			return insideBorderScale(location, glm::vec2(0.0f),
				innerBorder, borderWidth, includeEdge);
		}


		bool insideCuboid(const glm::vec3 &location,
			const glm::vec3 &start, const glm::vec3 &end, bool includeEdge)
		{
			return insideRange(location.x, start.x, end.x, includeEdge) &&
				insideRange(location.y, start.y, end.y, includeEdge) &&
				insideRange(location.z, start.z, end.z, includeEdge);
		}

		bool insideCuboidScale(const glm::vec3 &location,
			const glm::vec3 &start, const glm::vec3 &scale, bool includeEdge)
		{
			return insideCuboid(location, start, start + scale);
		}

		bool insideCube(const glm::vec3 &location,
			float start, float end, bool includeEdge)
		{
			return insideCuboid(location, 
				glm::vec3(start), glm::vec3(end), includeEdge);
		}

		bool insideCubeScale(const glm::vec3 &location,
			float start, float scale, bool includeEdge)
		{
			return insideCuboidScale(location, 
				glm::vec3(start), glm::vec3(scale), includeEdge);
		}


		bool insideBorder(const glm::vec3 &location, const glm::vec3 &center, 
			const glm::vec3 &innerBorder, const glm::vec3 &outerBorder, bool includeEdge)
		{
			bool _insideOuter = insideCuboid(location,
				center - outerBorder, center + outerBorder, includeEdge);
			bool _insideInner = insideCuboid(location, 
				center - innerBorder, center + innerBorder, !includeEdge);

			return  _insideOuter &&	!_insideInner;
		}

		bool insideBorder(const glm::vec3 &location, const glm::vec3 &innerBorder,
			const glm::vec3 &outerBorder, bool includeEdge)
		{
			return insideBorder(location, glm::vec3(0.0f),
				innerBorder, outerBorder, includeEdge);
		}

		bool insideBorderScale(const glm::vec3 &location, const glm::vec3 &center, 
			const glm::vec3 &innerBorder, const glm::vec3 &borderWidth, bool includeEdge)
		{
			return insideBorder(location, center, innerBorder, 
				innerBorder + borderWidth, includeEdge);
		}

		bool insideBorderScale(const glm::vec3 &location, const glm::vec3 &innerBorder,
			const glm::vec3 &borderWidth, bool includeEdge)
		{
			return insideBorderScale(location, glm::vec3(0.0f),
				innerBorder, borderWidth, includeEdge);
		}

	}
}
