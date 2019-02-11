#ifndef RAGDOLL_HPP
#define RAGDOLL_HPP

#include <vector>
#include <unordered_map>

#include "Collision.hpp"


/*namespace glm{
	struct mat2x3;
	struct vec3;
}

namespace Engine{ namespace Meshes{
	struct Mesh;
} }
*/
namespace Engine{ namespace Animation{
	struct AnimationSkeleton;
} }

/*
namespace Engine{ namespace Physics{
	class Collision;
	class RigidBodyPtr;
	class ConstraintPtr;
} }*/


namespace Engine{
	namespace Physics{
		namespace Ragdoll{

			//static const float c_ragdollScale = 3.0f;

			std::vector<glm::mat2x3> getBoneBoundingBoxes(const Meshes::Mesh &mesh,
				Animation::AnimationSkeleton &skeleton, float scale = 1.0);

			std::vector<RigidBodyPtr> createRagdoll(Collision &collision,
				Animation::AnimationSkeleton &skeleton, 
				const glm::vec3 &location, float scale = 1.0f);


			//returns mesh bounding boxes
			std::vector<glm::mat2x3> createHumanRagdoll(
				Collision &collision, 
				const Meshes::Mesh &mesh,
				Animation::AnimationSkeleton &skeleton, 		
				std::vector<Physics::RigidBodyPtr> &bodies, 
				std::unordered_map<std::string, Physics::ConstraintPtr> &constraints,			
				const glm::vec3& location, 
				float scale = 1.0f,
				bool kinematic = false
			);
		
		}
	}
}

#endif //RAGDOLL_HPP