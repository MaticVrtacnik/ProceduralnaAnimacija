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


#include "AnimationSkeleton.hpp"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

//#include "../Animation/AnimationBoneTree.hpp"
#include "../Define/Assert.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Utility/AnimationUtilities.hpp"
#include "../Utility/StringUtilities.hpp"
#include "../WorldLoader/pugixml.hpp"


namespace Engine{
	namespace Animation{

		AnimationSkeleton::VertexBoneData::VertexBoneData(){ 
			for (std::size_t i = 0; i < c_numBonesPerVertex; ++i){
				m_indices[i] = 0;
				m_weights[i] = 0.0f;
			}
		}

		void AnimationSkeleton::VertexBoneData::addBone(unsigned int index, float weight){
			for (std::size_t i = 0; i < c_numBonesPerVertex; ++i){
				//find last entry
				if (m_weights[i] == 0.0f){
					m_indices[i] = index;
					m_weights[i] = weight;
					return;
				}
			}
		}



		AnimationSkeleton::AnimationSkeleton(){
			m_animationBoneTreeRoot = std::make_shared<AnimationBoneTree>();

			/*aiMatrix4x4 _globalTransform = scene->mRootNode->mTransformation;
			m_globalInverseTransform = Utilities::convertMat4(_globalTransform.Inverse());*/

			//std::cout << "NUM_ANIMATIONS: " << m_animations.size() << std::endl;
		}

		/*AnimationSkeleton::AnimationSkeleton(const AnimationSkeleton &mesh) :
			m_vertexBoneData(mesh.m_vertexBoneData),
			m_boneTransforms(mesh.m_boneTransforms),
			m_boneMap(mesh.m_boneMap)
		{
		}*/

		/*AnimationSkeleton::AnimationSkeleton(AnimationSkeleton &&mesh) :
			m_vertexBoneData(std::move(mesh.m_vertexBoneData)),
			m_boneTransforms(std::move(mesh.m_boneTransforms)),
			m_boneMap(std::move(mesh.m_boneMap))
		{
		}*/

		/*AnimationSkeleton::AnimationSkeleton(
			std::vector<VertexBoneData> &&bones,
			std::vector<BoneTransform> &&transforms,
			std::unordered_map<std::string, unsigned int> &&map
		):
			m_vertexBoneData(std::move(bones)),
			m_boneTransforms(std::move(transforms)),
			m_boneMap(std::move(map))
		{
		}*/

		AnimationSkeleton::~AnimationSkeleton(){

		}


		bool AnimationSkeleton::loadBoneAliases(const std::string &skeletonMeshPath){
			std::string _fileDirectory = Utilities::removeFormat(skeletonMeshPath);
			std::string _actionsConfigPath = "data/animations/" + _fileDirectory + "/bones.cfg";

			auto f_wrongFormat = [&_actionsConfigPath](const std::string &type){
				PRINT("Error loading bone aliases file '" << _actionsConfigPath << "': " << type);
			};

			pugi::xml_document _file;
			if (!_file.load_file(_actionsConfigPath.c_str())){
				ASSERT(0, "Bone aliases file '" + _xmlPath + "' not found or not valid");
				f_wrongFormat("File not found or not valid");
				return false;
			}

			pugi::xml_node _rootNode = _file.child("Bones");
			if (!_rootNode){
				f_wrongFormat("Invalid root node");
				return false;
			}

			for (auto _bone = _rootNode.first_child(); _bone; _bone = _bone.next_sibling()){
				pugi::xml_attribute _nameAttribute = _bone.attribute("name");
				pugi::xml_attribute _aliasAttribute = _bone.attribute("alias");

				if (_nameAttribute && _aliasAttribute){
					std::string _name(_nameAttribute.as_string());
					std::string _alias(_aliasAttribute.as_string());

					//Prints bone aliases onload
					//PRINT(_name << " -> " << _alias)

					m_boneAliases[_alias] = _name;

				} else if (!_nameAttribute){
					WARNING("Bone name not set in bones configuration file.");
				}
			}

			return true;
		}

		unsigned int AnimationSkeleton::getBoneId(const std::string &boneAlias) const{
			auto _aliasItr = m_boneAliases.find(boneAlias);

			if (_aliasItr != m_boneAliases.end()){
				std::string _boneName((*_aliasItr).second);

				auto _boneItr = m_boneMap.find(_boneName);
				if (_boneItr != m_boneMap.end()){
					return (*_boneItr).second;
				} else{
					PRINT("Invalid bone name set for alias '" + boneAlias + "'");
					return -1;
				}
			} else{
				auto _boneItr = m_boneMap.find(boneAlias);
				if (_boneItr != m_boneMap.end()){
					return (*_boneItr).second;
				} else{
					PRINT("Unable to find a bone for alias '" + boneAlias + "'");
					return -1;
				}
			}
		}

		std::string AnimationSkeleton::getBoneName(const std::string &boneAlias) const{
			auto _aliasItr = m_boneAliases.find(boneAlias);

			if (_aliasItr != m_boneAliases.end()){
				std::string _boneName((*_aliasItr).second);
				return _boneName;
			} else{
				PRINT("No bone name for alias '" << boneAlias << "'");
				return "";
			}
		}

		//TODO maybe optimize with caching if required
		bool AnimationSkeleton::hasAliasForBone(const std::string &boneName) const{
			for (auto &_boneAlias : m_boneAliases){
				if (_boneAlias.second == boneName){
					return true;
				}
			}
				
			return false;
		}

		std::vector<unsigned int> AnimationSkeleton::getBonesWithoutAliases() const{
			std::vector<bool> _hasAlias(m_boneMap.size(), false);
			std::vector<unsigned int> _bones;

			for (auto &_alias : m_boneAliases){
				auto _itr = m_boneMap.find(_alias.second);
				if (_itr != m_boneMap.end()){
					_hasAlias[_itr->second] = true;
				}
			}

			for (int i = 0; i < _hasAlias.size(); ++i){
				if (!_hasAlias[i]){
					_bones.push_back(i);
				}
			}

			return _bones;
		}

		
	}
}
