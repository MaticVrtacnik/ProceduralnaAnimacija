#include "FABRIK.hpp"

#include <glm\gtx\transform.hpp>

#include "../../Define/PrintDefines.hpp"
#include "../AnimationSkeleton.hpp"
#include "../../Utility/LogicUtilities.hpp"
#include "../../Utility/MathUtilities.hpp"
#include "../../Utility/PrintUtilities.hpp"



namespace Engine{
	namespace Animation{
		

		void FABRIK::solveForwardIteration(const std::vector<AnimationBoneTree> &chain,
			std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation)
		{
			if (m_transformsCopy.size() < transforms.size()){
				//Error this function called without resizing bestSolution member
				WARNING("RETURN WITHOUT CHANGE");
				return;
			}


			for (int chainLength = chain.size(), i = chainLength - 1; i >= 0; --i)
			{
				// Get the length of the bone we're working on
				glm::mat4 &_thisBone = transforms[chain[i].m_boneId];
				glm::vec3 _thisBoneLocation(_thisBone[3]);

				if (i == chainLength - 1) //last bone (has length of 0!!!)
				{
					_thisBone[3] = glm::vec4(targetLocation, 1.0f);
				} else
				{
					glm::mat4 _nextBone = transforms[chain[i + 1].m_boneId];
					glm::vec3 _nextBoneLocation(_nextBone[3]);

					glm::vec3 _toThisBoneNormalizedDir = glm::normalize(_thisBoneLocation - _nextBoneLocation);

					//Get correct distances bone lengths from previous best solution
					float _thisBoneLength = glm::length(glm::vec3(m_transformsCopy[chain[i].m_boneId][3]) - glm::vec3(m_transformsCopy[chain[i + 1].m_boneId][3]));


					glm::vec3 _newLocation = _nextBoneLocation + _toThisBoneNormalizedDir * _thisBoneLength;
					//_newLocation = glm::vec3(0.0f);

					_thisBone[3] = glm::vec4(_newLocation, 1.0f);
				}

			}
		}

		void FABRIK::solveBackwardIteration(const std::vector<AnimationBoneTree> &chain,
			std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation)
		{
			if (m_transformsCopy.size() < transforms.size()){
				//Error this function called without resizing bestSolution member
				WARNING("RETURN WITHOUT CHANGE");
				return;
			}


			for (int i = 0, chainSize = chain.size(); i < chainSize; ++i)
			{
				glm::mat4 &_thisBone = transforms[chain[i].m_boneId];
				glm::vec3 _thisBoneLocation(_thisBone[3]);

				float _thisBoneLength = 0.1f; //default => if last bone
				if (i < chainSize - 1){
					glm::mat4 _nextBone = transforms[chain[i + 1].m_boneId];
					_thisBoneLength = glm::distance(glm::vec3(_thisBone[3]), glm::vec3(_nextBone[3]));
				}


				//Base bone
				if (i == 0)
				{
					//Snap back to origin
					_thisBone[3] = glm::vec4(m_chainBaseLocation, 1.0f);
				} else
				{
					glm::mat4 &_prevBone = transforms[chain[i - 1].m_boneId];
					glm::vec3 _prevBoneLocation(_prevBone[3]);

					glm::vec3 _toThisBoneNormalizedDir = glm::normalize(_thisBoneLocation - _prevBoneLocation);

					float _thisBoneLength = glm::length(glm::vec3(m_transformsCopy[chain[i].m_boneId][3]) - glm::vec3(m_transformsCopy[chain[i - 1].m_boneId][3]));

					glm::vec3 _newLocation = _prevBoneLocation + _toThisBoneNormalizedDir * _thisBoneLength;
					//_newLocation = glm::vec3(0.0f);

					_thisBone[3] = glm::vec4(_newLocation, 1.0f);

				}
			}
		}



		float FABRIK::solveIteration(const std::vector<AnimationBoneTree> &chain, 
			std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation)
		{
			solveForwardIteration(chain, transforms, targetLocation);
			solveBackwardIteration(chain, transforms, targetLocation);

			return glm::distance(glm::vec3(transforms[chain.back().m_boneId][3]), targetLocation);
		}


		float FABRIK::calcSolveDistance(const std::vector<AnimationBoneTree> &chain, 
			const std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation)
		{
			return glm::distance(glm::vec3(transforms[chain.back().m_boneId][3]), targetLocation);
		}


		float FABRIK::solveChain(const std::vector<AnimationBoneTree> &chain, 
			std::vector<glm::mat4> &transforms, const glm::vec3 &targetLocation)
		{
			if (chain.empty()){
				return 0.0f; //Nothing to do here
			}
			if (chain.size() < 2) //Only single node
			{
				m_chainBaseLocation = targetLocation;
				transforms[chain.front().m_boneId][3] = glm::vec4(targetLocation, 1.0f);

				return 0.0f;
			} else{
				m_chainBaseLocation = glm::vec3(transforms[chain.front().m_boneId][3]);



			}

			
			//Already close enough to target
			if (Utilities::insideBorder(m_previousTargetLocation, targetLocation - glm::vec3(0.001f), targetLocation + glm::vec3(0.001f)) &&
				Utilities::insideBorder(m_previousStartLocation, m_chainBaseLocation - glm::vec3(0.001f), m_chainBaseLocation + glm::vec3(0.001f)))
			{
				return m_currentSolveDistance;
			}

			m_bestSolution.resize(transforms.size());
			std::copy(transforms.begin(), transforms.end(), m_bestSolution.begin());

			float lastPassSolveDistance = 9999999.0f;
			float bestSolveDistance = calcSolveDistance(chain, m_bestSolution, targetLocation);
		

			float _solveDistance;
			for (int i = 0; i < c_maxIterations; ++i)
			{
				std::vector<glm::mat4> _newTransforms(m_bestSolution.size());
				std::copy(m_bestSolution.begin(), m_bestSolution.end(), _newTransforms.begin());

				_solveDistance = solveIteration(chain, _newTransforms, targetLocation);


				if (_solveDistance < bestSolveDistance){
					bestSolveDistance = _solveDistance;
					m_bestSolution = _newTransforms;

					if (_solveDistance <= 0.005f){
						break;
					}
				} else 
				{
					if (glm::abs(_solveDistance - lastPassSolveDistance) < 0.01f){
						break;
					}
				}

				lastPassSolveDistance = _solveDistance;
			} 


			m_currentSolveDistance = bestSolveDistance;
			transforms = m_bestSolution;

			m_previousStartLocation = glm::vec3(transforms[chain.front().m_boneId][3]);
			m_previousTargetLocation = targetLocation;

			return m_currentSolveDistance;
		}


		//clusterfuck key:parent , value: child chain array
		//std::vector<std::unordered_map<std::string, 


		Animation::AnimationBoneTree *getIdPtr(Animation::AnimationBoneTree *node, unsigned int boneId){
			if (node == nullptr){
				return nullptr;
			} else if (node->m_boneId == boneId){
				return node;
			}

			for (auto &_node : node->m_children){
				auto _ptr = getIdPtr(_node.get(), boneId);

				if (_ptr != nullptr){
					return _ptr;
				}		
			}
		}

		std::vector<AnimationBoneTree> FABRIK::getChain(AnimationSkeleton &skeleton,
			AnimationBoneTree *startNode, const std::string &boneName)
		{
			std::vector<AnimationBoneTree> _chain;

			auto _bonePtr = getIdPtr(startNode, skeleton.getBoneId(boneName));
			if (_bonePtr == nullptr){
				//PRINT("No bone named '" << boneName << "' found");
				return _chain;
			}

			_chain.push_back(*_bonePtr);

			for (int i = 0; /*i < maxChainLength &&*/ _bonePtr->m_parent != startNode->m_parent; ++i){
				_bonePtr = _bonePtr->m_parent.get();
				if (_bonePtr != startNode && _bonePtr->m_marked){
					//If this is a sub-base below another sub-base, mark it as goal
					//PRINT("CLEARED CHAIN");
					_chain.clear();
				}

				_chain.push_back(*_bonePtr);
			}

			//turn chain around from [child->parent] to [parent->child] order
			std::reverse(_chain.begin(), _chain.end());

			return _chain;
		}




		void FABRIK::updateChainSubtree(AnimationBoneTree *chainNode, const std::vector<AnimationBoneTree> &chain,
			std::vector<glm::mat4> &transforms, const glm::mat4 &parentChange, unsigned int depth)
		{
			if (chainNode == nullptr){
				return;
			}

			for (auto &_childNode : chainNode->m_children){
				bool _inChain = false;
				for (auto &_chainNode : chain){
					if (_childNode->m_boneId == _chainNode.m_boneId){
						_inChain = true;
						break;
					}
				}

				if (!_inChain){
					unsigned int _boneId = _childNode->m_boneId;
					transforms[_boneId] = parentChange * transforms[_boneId];
				}

				/*if (chain.size() > depth && _childNode->m_boneId != chain[depth].m_boneId){
					unsigned int _boneId = _childNode->m_boneId;
					transforms[_boneId] = parentChange * transforms[_boneId];
				}*/

				updateChainSubtree(_childNode.get(), chain, transforms, parentChange, depth + 1);
			}
		}

		
		float FABRIK::solve(AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms,
			const std::string &targetBoneAlias, const glm::vec3 &targetLocation, unsigned int maxChainLength)
		{
			
			unsigned int _boneId = skeleton.m_boneMap[skeleton.m_boneAliases[targetBoneAlias]];
			if (transforms.size() < _boneId){
				return 0.0f;
			}

			//
			

			
			m_transformsCopy = std::vector<glm::mat4>(transforms.size());
			std::copy(transforms.begin(), transforms.end(), m_transformsCopy.begin());

			auto _chain = getChain(skeleton, skeleton.m_animationBoneTreeRoot.get(), targetBoneAlias);
			float chainLength = solveChain(_chain, transforms, targetLocation);


			//Update subtree transforms
			for (auto &_chainNode : _chain){
				unsigned int _boneId = _chainNode.m_boneId;
				glm::mat4 _change = transforms[_boneId] * glm::inverse(m_transformsCopy[_boneId]);

				updateChainSubtree(&_chainNode, _chain, transforms, _change, 1);
			}
	
			
			return 0.0f;
		}

	

		glm::vec3 getCentroid(const std::vector<glm::vec3> &locations){
			glm::vec3 _sum(0.0f);
			if (locations.empty()){
				return _sum;
			}

			for (auto &_location : locations){
				_sum += _location;
			}

			return (_sum * (1.0f / static_cast<float>(locations.size())));
		}

		//counts number of end-effectors affecting current joint
		int FABRIK::getSubBases(AnimationBoneTree *node, std::vector<glm::mat4> &transforms,
			const std::unordered_map<std::string, glm::vec3> &endEffectorLocations)
		{
			if (node == nullptr){
				return 0;
			}

			int _numAffected = 0;
			for (auto &_childNode : node->m_children){
				_numAffected += getSubBases(_childNode.get(), transforms, endEffectorLocations);
			}
			
			

			//If this is a subbase for 2 or more end-effectors process it
			//If this is the root node it will be skipped
			if (_numAffected > 1 && node->m_children.size() > 1 && node->m_parent != nullptr){
				node->m_marked = true;
			} else{
				node->m_marked = false;
			}

			if (endEffectorLocations.find(node->m_name) != endEffectorLocations.end()){	
				++_numAffected;
			}

			return _numAffected;
		}

		void FABRIK::updateSubBases(AnimationBoneTree *node, AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms,
			std::unordered_map<std::string, glm::vec3> &endEffectorLocations)
		{
			if (node == nullptr){
				return;
			}

			for (auto &_childNode : node->m_children){
				updateSubBases(_childNode.get(), skeleton, transforms, endEffectorLocations);
			}

			/*if (node->m_parent == nullptr){
				return;
			}*/

			
			if (node->m_marked){
				//PRINT(node->m_name);

				std::vector<AnimationBoneTree> _parentChain;
				std::unordered_map<std::string, std::vector<AnimationBoneTree>> _childChains;

				for (auto &_pair : endEffectorLocations){



					auto _chain = getChain(skeleton, node, _pair.first);
					if (_chain.size() > 0){
						_chain.erase(_chain.begin()); //Remove first(current) node from chain;

						/*if (_chain.begin()->m_boneId == node->m_boneId){
							PRINT("EQUAL");
						}*/

						//std::reverse(_chain.begin(), _chain.end());
						_childChains[_pair.first] = _chain;
						
					}
				}

				if (node->m_parent != nullptr){
					for (auto _currentNode = node; _currentNode != nullptr && (_currentNode == node || !_currentNode->m_marked); _currentNode = _currentNode->m_parent.get()){
						_parentChain.push_back(*_currentNode);
					}

					//Make sure chain starts with highest parent
					std::reverse(_parentChain.begin(), _parentChain.end());
				}

				for (auto &_pair : _childChains){
					//Utilities::print(endEffectorLocations[_pair.first]);
					solveForwardIteration(_pair.second, transforms, endEffectorLocations[_pair.first]);
				}

				std::vector<glm::vec3> _childLocations;
				for (auto &_pair : _childChains){
					_childLocations.push_back(glm::vec3(transforms[_pair.second.front().m_boneId][3]));
				}

				glm::vec3 _centroid = getCentroid(_childLocations);

				//If its not root node
				if (!_parentChain.empty()){
					m_chainBaseLocation = glm::vec3(transforms[_parentChain.front().m_boneId][3]);		
					solveIteration(_parentChain, transforms, _centroid);
				}			

				m_chainBaseLocation = glm::vec3(transforms[node->m_boneId][3]);
				for (auto &_pair : _childChains){
					solveBackwardIteration(_pair.second, transforms, endEffectorLocations[_pair.first]);
				}

			}
		}



		void clearMarkers(AnimationBoneTree *node){
			if (node != nullptr){
				node->m_marked = false;

				for (auto &_childNode : node->m_children){
					clearMarkers(_childNode.get());
				}
			}
		}

		float FABRIK::solveMultiple(AnimationSkeleton &skeleton, std::vector<glm::mat4> &transforms,
			const std::unordered_map<std::string, glm::vec3> &endEffectorLocations)
		{
			//Copy for bone lengths
			m_transformsCopy = std::vector<glm::mat4>(transforms.size());
			std::copy(transforms.begin(), transforms.end(), m_transformsCopy.begin());


			//Replace alias names with real bone names
			std::unordered_map<std::string, glm::vec3> _endEffectorLocations; 
			for (auto &_pair : endEffectorLocations){
				_endEffectorLocations[skeleton.getBoneName(_pair.first)] = _pair.second;
			}

			//clearMarkers(skeleton.m_animationBoneTreeRoot.get());

			getSubBases(skeleton.m_animationBoneTreeRoot.get(), transforms, _endEffectorLocations);

			//Mark root not for correction when a sub-base is found
			if (_endEffectorLocations.size() > 1){
				//skeleton.m_animationBoneTreeRoot->m_marked = true;
			}	

			for (int i = 0; i < c_maxIterations; ++i){
				updateSubBases(skeleton.m_animationBoneTreeRoot.get(), skeleton, transforms, _endEffectorLocations);

				bool _acceptable = true;
				for (auto &_pair : _endEffectorLocations){
					float _distance = glm::distance(glm::vec3(transforms[skeleton.getBoneId(_pair.first)][3]), _pair.second);
					//PRINT(_distance);

					if (_distance > 0.1f){
						_acceptable = false;
						break;
					}
				}

				if (_acceptable){
					break;
				}
			}

			//Use normal IK on end-effectors not used by multi-IK
			for (auto &_pair : endEffectorLocations){
				bool _used = false;
				for (auto _node : getChain(skeleton, skeleton.m_animationBoneTreeRoot.get(), _pair.first)){
					if (_node.m_marked){
						_used = true;
						break;
					}
				}

				if (!_used){
					solve(skeleton, transforms, _pair.first, _pair.second, -1);
				}
			}
			



			return 0.0f;
		}

	}
}

