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


#ifndef FABRIK_HPP
#define FABRIK_HPP

#include <memory>
#include <unordered_map>

#include "IKSolver.hpp"


namespace Engine{
	namespace Animation{

		class AnimationBoneTree;


		class FABRIK : public IKSolver{
		private:
			const unsigned int c_maxIterations = 10;

			glm::vec3 m_previousTargetLocation = glm::vec3(0.0f);
			glm::vec3 m_previousStartLocation = glm::vec3(0.0f);


			glm::vec3 m_chainBaseLocation = glm::vec3(0.0f);
			glm::vec3 m_endEffectorLocation = glm::vec3(0.0f);



			float m_currentSolveDistance = 9999999.0f;
			std::vector<glm::mat4> m_bestSolution;
			std::vector<glm::mat4> m_previousBestSolution;

			//Required for proper bone lengths
			std::vector<glm::mat4> m_transformsCopy;


			std::unique_ptr<AnimationBoneTree> m_subBaseRoot = nullptr;

		private:
			void solveForwardIteration(const std::vector<AnimationBoneTree> &chain,
				std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation);
			void solveBackwardIteration(const std::vector<AnimationBoneTree> &chain,
				std::vector<glm::mat4> &transforms, const glm::vec3 &baseLocation);


			float solveIteration(const std::vector<AnimationBoneTree> &chain,
				std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation);

			float solveChain(const std::vector<AnimationBoneTree> &chain, 
				std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation);

			float calcSolveDistance(const std::vector<AnimationBoneTree> &chain, 
				const std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation);


			void updateChainSubtree(AnimationBoneTree *chainNode, const std::vector<AnimationBoneTree> &chain,
				std::vector<glm::mat4> &transforms, const glm::mat4 &parentChange, unsigned int depth = 0);

			std::vector<AnimationBoneTree> getChain(AnimationSkeleton &skeleton,
				AnimationBoneTree *startNode, const std::string &boneName);


			int getSubBases(AnimationBoneTree *node, std::vector<glm::mat4> &transforms,
				const std::unordered_map<std::string, glm::vec3> &endEffectorLocations);

			void updateSubBases(AnimationBoneTree *node, AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms,
				std::unordered_map<std::string, glm::vec3> &endEffectorLocations);

		public:
			float solve(AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms, 
				const std::string &targetBoneAlias, const glm::vec3 &targetLocation, unsigned int maxChainLength = 5);
			
			float solveMultiple(AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms,
				const std::unordered_map<std::string, glm::vec3> &endEffectorLocations);
		};


	}
}

#endif //FABRIK_HPP