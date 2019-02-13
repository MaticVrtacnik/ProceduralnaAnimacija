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


#ifndef ANIMATION_SKELETON_HPP
#define ANIMATION_SKELETON_HPP

#include <array>
#include <unordered_map>
#include <glm\glm.hpp>

#include "AnimationBoneTree.hpp"
#include "../Animation/SkeletalAnimationAction.hpp"


namespace Engine{ namespace Animation{
	//struct SkeletalAnimationAction;
} }


namespace Engine{
	namespace Animation{
		static const unsigned int c_numBonesPerVertex = 4;
		static const unsigned int c_maxBones = 50;


		//TODO move bone aliases in here
		struct AnimationSkeleton
		{
			struct VertexBoneData{
			public:
				std::array<unsigned int, c_numBonesPerVertex> m_indices;
				std::array<float, c_numBonesPerVertex> m_weights;

			public:
				VertexBoneData();

				void addBone(unsigned int index, float weight);

			};


			struct BoneTransform{
			public:
				glm::mat4 m_originalOffset = glm::mat4(1.0f);
				glm::mat4 m_originalInverseOffset = glm::mat4(1.0f);

				glm::mat4 m_bvhOffset = glm::mat4(1.0f);
				glm::mat4 m_inverseBvhOffset = glm::mat4(1.0f);

				bool m_isDynamic = true; //for collision bodies
				unsigned int m_depth = 0;
			};


		public:
			std::string m_filePath = "";
			std::shared_ptr<AnimationBoneTree> m_animationBoneTreeRoot = nullptr;
			std::unordered_map<std::string, SkeletalAnimationAction> m_animationActions;
		
		public: //TODO make private and make getters, setters
			glm::mat4 m_globalInverseTransform = glm::mat4(1.0f);
			std::vector<VertexBoneData> m_vertexBoneData;
			std::vector<BoneTransform> m_boneTransforms;
			std::unordered_map<std::string, unsigned int> m_boneMap;
			std::unordered_map<std::string, std::string> m_boneAliases;
			std::vector<std::string> m_insertionOrder;

		public:
			AnimationSkeleton();

			//AnimationSkeleton(const AnimationSkeleton &mesh);

			/*AnimationSkeleton(std::vector<VertexBoneData> &&bones,
				std::vector<BoneTransform> &&transforms,
				std::unordered_map<std::string, unsigned int> &&map
			);*/

			~AnimationSkeleton();


			bool loadBoneAliases(const std::string &skeletonMeshPath);

			unsigned int getBoneId(const std::string &boneAlias) const;

			std::string getBoneName(const std::string &boneAlias) const;

			bool hasAliasForBone(const std::string &boneName) const;

			std::vector<unsigned int> getBonesWithoutAliases() const;

		};

	}
}

#endif //ANIMATION_SKELETON_HPP