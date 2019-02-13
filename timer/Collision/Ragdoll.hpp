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