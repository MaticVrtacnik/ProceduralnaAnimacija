#ifndef IK_SOLVER_HPP
#define IK_SOLVER_HPP

#include <vector>
#include <string>
#include <glm\glm.hpp>


namespace Engine{
	namespace Animation{

		class AnimationSkeleton;

		class IKSolver{
		private:

		public:
			virtual float solve(AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms,
				const std::string &targetBoneAlias, const glm::vec3 &targetLocation)
			{
				return 999999999.0f;
			}

		};
		
	}
}

#endif //IK_SOLVER_HPP