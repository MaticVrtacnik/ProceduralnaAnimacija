#include "SkeletalAnimationComponent.hpp"

#include <queue>
#include <assimp/scene.h>
#include <assimp\Importer.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Mesh/SceneLoader.hpp"

#include "../WorldLoader/pugixml.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Animation/AnimationSkeleton.hpp"
#include "../Animation/SkeletalAnimationPlayer.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/StringUtilities.hpp"
#include "../Utility/AnimationUtilities.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			struct ActionData{
			public:
				std::string m_path = "";
				std::string m_name = "";

				//interpolation time to start the action from previous transforms
				float m_nextActionInterpolationDuration = 0.3f;
				bool m_cancelable = false;

				std::vector<std::pair<std::string, float>> m_keyframeTimes;

			public:
				ActionData(){

				}

				ActionData(const std::string &path, const std::string &name = "",
					bool cancelable = false, float nextActionInterpolationDuration = 0.3f
					) :
					m_path(path), m_name(name), m_cancelable(cancelable),
					m_nextActionInterpolationDuration(nextActionInterpolationDuration)
				{
				}


				bool hasKeyframes() const{
					return !m_keyframeTimes.empty();
				}

			};



			SkeletalAnimationComponent::SkeletalAnimationComponent(AnimationSkeleton &skeleton) :
				m_skeleton(std::make_shared<AnimationSkeleton>(skeleton))
			{
				ASSERT(skeleton != nullptr, "Skeleton is a null pointer");
				ASSERT(skeleton->m_scene != nullptr, "Scene is a null pointer");
				ASSERT(!skeleton->m_animations.empty(), "Skeleton has no animations");
				ASSERT(!skeleton->m_boneMap.empty(), "Skeleton has no bones");

				m_boneBuffer.loadBuffer(m_skeleton->m_vertexBoneData, GL_ARRAY_BUFFER);

				m_transforms.resize(getNumBones(), glm::mat4(1.0f));
				m_offsetTransforms.resize(getNumBones(), glm::mat4(1.0f));
				m_previousOffsetTransforms.resize(getNumBones(), glm::mat4(1.0f));
				
				m_animationPlayer = std::make_unique<SkeletalAnimationPlayer>();
				m_fileDirectory = Utilities::removeFormat(m_skeleton->m_filePath);
			}

			SkeletalAnimationComponent:: ~SkeletalAnimationComponent(){
				//PRINT("SKELETAL ANIMATION COMPONENT DELETED")
			}


			void SkeletalAnimationComponent::changeOffsets(const aiNode *node, const aiScene *scene){

				auto &_boneMap = m_skeleton->m_boneMap;

				for (std::size_t i = 0; i < node->mNumMeshes; ++i){
					const aiMesh *_mesh = scene->mMeshes[node->mMeshes[i]];
					std::string _meshName = std::string(_mesh->mName.data);

					//PRINT(_meshName << " " << _mesh->HasBones());
					if (_mesh->HasBones()){
						for (std::size_t i = 0; i < _mesh->mNumBones; ++i){
							std::string _boneName(_mesh->mBones[i]->mName.data);
							//PRINT(_boneName);

							if (_boneMap.find(_boneName) != _boneMap.end()){
								glm::mat4 _bvhOffset = Utilities::convertMat4(_mesh->mBones[i]->mOffsetMatrix);

								auto &_transform = m_skeleton->m_boneTransforms[_boneMap[_boneName]];
								glm::mat4 &_originalOffset = _transform.m_originalOffset;

								glm::vec3 _scale(glm::length(_originalOffset[0]),
									glm::length(_originalOffset[1]), glm::length(_originalOffset[2]));

								//Utilities::print(_scale);

								_transform.m_bvhOffset = _bvhOffset; // *glm::scale(_scale);
								_transform.m_inverseBvhOffset = glm::inverse(_bvhOffset);
								_transform.m_isDynamic = false;
								//TODO watch out if any BVH files have scaling or rotation ; remove otherwise
								//_transform.m_bvhOffset = glm::translate(glm::vec3(_bvhOffset[3])) * glm::scale(_scale);



								//Utilities::print(_offset);
							}
						}
					}

				}

				for (std::size_t i = 0; i < node->mNumChildren; ++i){
					changeOffsets(node->mChildren[i], scene);
				}
			}


			glm::vec3 SkeletalAnimationComponent::getInterpolatedLocation(
				float animationTime, const aiNodeAnim *animationNode)
			{
				if (animationNode->mNumPositionKeys == 1){
					aiVector3D _result = animationNode->mPositionKeys[0].mValue;
					return glm::vec3(_result.x, _result.y, _result.z);
				}

				std::size_t _currentLocationIndex = 0; // findLocationIndex(animationTime, animationNode);
				const auto &_currentLocation = animationNode->mPositionKeys[_currentLocationIndex];
				const auto &_nextLocation = animationNode->mPositionKeys[_currentLocationIndex + 1];
				float _deltaTime = static_cast<float>(_nextLocation.mTime - _currentLocation.mTime);
				float _factor = (animationTime - static_cast<float>(_currentLocation.mTime)) / _deltaTime;

				aiVector3D _diff = _nextLocation.mValue - _currentLocation.mValue;
				aiVector3D _result = _currentLocation.mValue + _factor * _diff;

				return glm::vec3(_result.x, _result.y, _result.z);
			}

			glm::quat SkeletalAnimationComponent::getInterpolatedRotation(
				float animationTime, const aiNodeAnim *animationNode)
			{
				if (animationNode->mNumRotationKeys == 1){
					aiQuaternion _result = animationNode->mRotationKeys[0].mValue;
					return glm::quat(_result.w, _result.x, _result.y, _result.z);
				}

				std::size_t _currentRotationIndex = 0; // findRotationIndex(animationTime, animationNode);
				const auto &_currentRotation = animationNode->mRotationKeys[_currentRotationIndex];
				const auto &_nextRotation = animationNode->mRotationKeys[_currentRotationIndex + 1];
				float _deltaTime = static_cast<float>(_nextRotation.mTime - _currentRotation.mTime);
				float _factor = (animationTime - static_cast<float>(_currentRotation.mTime)) / _deltaTime;

				aiQuaternion _result;
				aiQuaternion::Interpolate(_result, _currentRotation.mValue, _nextRotation.mValue, _factor);
				_result = _result.Normalize();

				return glm::quat(_result.w, _result.x, _result.y, _result.z);
			}

			glm::vec3 SkeletalAnimationComponent::getInterpolatedScale(
				float animationTime, const aiNodeAnim *animationNode)
			{
				if (animationNode->mNumScalingKeys == 1){
					aiVector3D _result = animationNode->mScalingKeys[0].mValue;
					return glm::vec3(_result.x, _result.y, _result.z);
				}

				std::size_t _currentScaleIndex = 0; // findScaleIndex(animationTime, animationNode);
				const auto &_currentScale = animationNode->mScalingKeys[_currentScaleIndex];
				const auto &_nextScale = animationNode->mScalingKeys[_currentScaleIndex + 1];
				float _deltaTime = static_cast<float>(_nextScale.mTime - _currentScale.mTime);
				float _factor = (animationTime - static_cast<float>(_currentScale.mTime)) / _deltaTime;

				aiVector3D _diff = _nextScale.mValue - _currentScale.mValue;
				aiVector3D _result = _currentScale.mValue + _factor * _diff;

				return glm::vec3(_result.x, _result.y, _result.z);
			}

			int SkeletalAnimationComponent::setChildTransforms(float animationTime, const aiNode *node, 
				const aiAnimation *animation, const glm::mat4 &parentTransform, int depth)
			{
				std::string _nodeName(node->mName.data);
				glm::mat4 _nodeTransform(Utilities::convertMat4(node->mTransformation));

				//TODO convert to hashmap
				const aiNodeAnim* _animationNode = nullptr;
				for (std::size_t i = 0; i < animation->mNumChannels; ++i){
					const aiNodeAnim *_node = animation->mChannels[i];
					if (std::string(_node->mNodeName.data) == _nodeName)
						_animationNode = _node;
				}


				if (_animationNode != nullptr){
					glm::vec3 _location = getInterpolatedLocation(animationTime, _animationNode);
					glm::mat4 _locationMatrix(glm::translate(_location));

					glm::quat _rotation = getInterpolatedRotation(animationTime, _animationNode);
					glm::mat4 _rotationMatrix(glm::toMat4(_rotation));

					glm::vec3 _scale = getInterpolatedScale(animationTime, _animationNode);
					glm::mat4 _scaleMatrix(glm::scale(_scale));

					//TODO add scale back
					_nodeTransform = _locationMatrix * _rotationMatrix * _scaleMatrix;
				}


				glm::mat4 _parentTransform = parentTransform * _nodeTransform;
				auto &_boneMap = m_skeleton->m_boneMap;

				if (_boneMap.find(_nodeName) != _boneMap.end()){
					unsigned int _boneId = _boneMap[_nodeName];
					/*auto &_boneTransform = m_skeleton->m_boneTransforms[_boneId].m_transform;

					_boneTransform = //m_skeleton->m_globalInverseTransform * 
						_parentTransform * m_skeleton->m_boneTransforms[_boneId].m_offset;*/
				}

				int _count = node->mNumChildren;
				//std::cout << _nodeName << " COUNT: " << _count << std::endl;

				for (std::size_t i = 0; i < node->mNumChildren; ++i){
					_count += setChildTransforms(animationTime,
						node->mChildren[i], animation, _parentTransform, depth + 1);
				}

				//std::cout << depth << " COUNT: " << _count << std::endl;

				return _count;//TODO remove count
			}



			bool SkeletalAnimationComponent::loadBVHKeyframe(const std::string &path, const std::string &name){
				ASSERT(m_skeleton != nullptr, "Skeleton is a null pointer");
				if (!m_skeleton)return false;

				//check if keyframe already exists
				std::string _keyframeName = name.empty() ? Utilities::removeFormat(path) : name;
				if (m_keyframes.find(_keyframeName) != m_keyframes.end()){
					return true;
				}


				Assimp::Importer _importer;
				std::string _filename = "data/animations/" + m_fileDirectory + "/keyframes/" + path;
				//std::string _filename = "data/animations/arms.bvh";
				const aiScene* _scene = _importer.ReadFile(_filename, 0);

				//PRINT(_filename);

				if (!_scene){
					ERROR("Can't load file '" + _filename + "'");
					return false;
				} else if (_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode){
					ERROR("Can't load animation '" + _filename + "'");
					return false;
				} else if (_scene->mNumAnimations == 0){
					ERROR("BVH file '" + _filename + "' contains no animations");
					return false;
				} else{
					//int _numBones = Meshes::Animation::getNumBones(_scene->mRootNode, _scene);
					auto &_boneMap = m_skeleton->m_boneMap;
					int _numBones = _boneMap.size();
					//PRINT("BONES::" << _numBones << ";BONEMAP::" << m_skeleton->m_boneMap.size());


					if (_numBones != 0 /*&& _numBones == m_skeleton->m_boneMap.size()*/)
					{			
						/*auto _globalInverseTransform = _scene->mRootNode->mTransformation;
						_globalInverseTransform = _globalInverseTransform.Inverse();
						m_skeleton->m_globalInverseTransform =
							Utilities::convertMat4(_globalInverseTransform);*/

						//PRINT("ADDED NEW FRAME: " << path);

						//Change original mesh offsets to BVH bone offsets
						changeOffsets(_scene->mRootNode, _scene);
						

						//create local bone transforms array
						
						std::vector<SkeletalAnimationTransform> _transforms(_numBones);
						Meshes::loadBoneTransforms(_boneMap, _scene->mAnimations[0], _transforms);


						//PRINT("KEYFRAME: " << _keyframeName);
						m_keyframes[_keyframeName] = std::make_shared<SkeletalAnimationKeyframe>(
							_keyframeName, _transforms);

					} else{
						ERROR("NUM BONES NOT THE SAME AS BONEMAP => BONES::" <<
							_numBones << ";BONEMAP::" << m_skeleton->m_boneMap.size());
						return false;
					}
				}

				return true;
			}

			bool SkeletalAnimationComponent::loadBVHAction(const std::string &path, 
				const std::string &name, bool cancelable, float nextActionInterpolationDuration)
			{
				ASSERT(m_skeleton != nullptr, "Skeleton is a null pointer");
				if (!m_skeleton)return false;

				//check if keyframe already exists
				std::string _actionName = name.empty() ? Utilities::removeFormat(path) : name;
				if (m_animationActions.find(_actionName) != m_animationActions.end()){
					return true;
				}

				
				Assimp::Importer _importer;
				std::string _filename = "data/animations/" + m_fileDirectory + "/keyframes/" + path;
				//std::string _filename = "data/animations/arms.bvh";
				const aiScene* _scene = _importer.ReadFile(_filename, 0);

				//PRINT(_filename);

				if (!_scene){
					ERROR("Can't load file '" + _filename + "'");
					return false;
				} else if (_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode){
					ERROR("Can't load animation '" + _filename + "'");
					return false;
				} else if (_scene->mNumAnimations == 0){
					ERROR("BVH file '" + _filename + "' contains no animations");
					return false;
				} else{
					//int _numBones = Meshes::Animation::getNumBones(_scene->mRootNode, _scene);
					int _numBones = _scene->mAnimations[0]->mNumChannels;

					//PRINT("BONES::" << _numBones << ";BONEMAP::" << m_skeleton->m_boneMap.size());

					if (_numBones != 0 /*&& _numBones == m_skeleton->m_boneMap.size()*/)
					{			
						//Root bone already skeleton root
						auto _rootBone = nullptr; // Meshes::findAnimationBoneRoot(_scene->mRootNode, m_skeleton->m_boneMap);

						/*glm::mat4 _globalTransform(1.0f);
						aiNode *_node = _rootBone;
						while (_node != nullptr){
							_globalTransform *= Utilities::convertMat4(_node->mTransformation);
							_node = _node->mParent;
						}*/

						/*auto _globalInverseTransform = ->mTransformation;
						_globalInverseTransform = _globalInverseTransform.Inverse();*/

						//Utilities::print(Utilities::convertMat4(_scene->mRootNode->mTransformation));


						//change original mesh offsets to BVH bone offsets
						changeOffsets(_scene->mRootNode, _scene);

						PRINT("ADDED NEW ACTION: " << path << " " << _scene->mRootNode->mName.data);

						m_animationActions[_actionName] = SkeletalAnimationAction(_actionName);
						m_animationActions[_actionName].m_cancelable = cancelable;
						m_animationActions[_actionName].m_nextActionInterpolationDuration = nextActionInterpolationDuration;


						auto _animation = _scene->mAnimations[0];
						float _ticksPerSecond = static_cast<float>(_animation->mTicksPerSecond);
						float _keyframeTime = 1.0f / (0.0f ? c_defaultTicksPerSecond : _ticksPerSecond);
						//PRINT("KEYFRAME TIME: " << _keyframeTime);

						for (std::size_t i = 0; i < _animation->mChannels[0]->mNumPositionKeys; ++i){
							std::vector<SkeletalAnimationTransform> _transforms(_numBones);

							//start times for each keyframe
							auto &_boneMap = m_skeleton->m_boneMap;
							Meshes::loadBoneTransforms(_boneMap,_scene->mAnimations[0], 
								_transforms, static_cast<float>(i));

							std::string _keyframeName = _actionName + std::to_string(i);
							m_keyframes[_keyframeName] = std::make_shared<SkeletalAnimationKeyframe>(
								_keyframeName, _transforms);
							m_keyframes[_keyframeName]->m_duration = _keyframeTime;

							m_animationActions[_actionName].addKeyframe(m_keyframes[_keyframeName], _keyframeTime);
						}

						return true;
					} else{
						ERROR("NUM BONES NOT THE SAME AS BONEMAP => BONES::" <<
							_numBones << ";BONEMAP::" << m_skeleton->m_boneMap.size());
						return false;
					}
				}

				return true;
			}

			bool SkeletalAnimationComponent::loadAnimationActionConfig(const std::string &path){
				std::string _xmlPath = "data/animations/" + Utilities::removeFormat(path) + ".cfg";

				auto f_wrongFormat = [&_xmlPath](const std::string &type){
					PRINT("Error loading animation config file '" << _xmlPath << "': " << type);
				};

				pugi::xml_document _file;
				if (!_file.load_file(_xmlPath.c_str())){
					ASSERT(0, "Animation configuration file '" + _xmlPath + "' not found or not valid");
					f_wrongFormat("File not found or not valid");
					return false;
				}

				pugi::xml_node _rootNode = _file.child("Animation_config");
				if (!_rootNode){
					f_wrongFormat("Invalid root node");
					return false;
				}


				std::unordered_map<std::string, std::vector<
					std::pair<std::string, float >> > _actionKeyframeNameTimePairs;

				for (auto action = _rootNode.first_child(); action; action = action.next_sibling()){
					pugi::xml_attribute _actionAttribute = action.attribute("name");
					if (!_actionAttribute){
						f_wrongFormat("Missing action value 'name'");
						return false;
					}

					std::string _actionName = _actionAttribute.as_string();
					_actionKeyframeNameTimePairs[_actionName] = {};

					for (auto keyframe = action.first_child(); keyframe; keyframe = keyframe.next_sibling()){
						pugi::xml_attribute _keyframeNameAttribute = keyframe.attribute("name");
						if (!_keyframeNameAttribute){
							f_wrongFormat("Missing keyframe value 'name'");
							return false;
						}

						//defaults to 1.0f (1 second)
						pugi::xml_attribute _keyframeTimeAttribute = keyframe.attribute("time");
						if (!_keyframeTimeAttribute){
							_actionKeyframeNameTimePairs[_actionName].push_back(
								std::make_pair(_keyframeNameAttribute.as_string(), 1.0f));
						} else{
							_actionKeyframeNameTimePairs[_actionName].push_back(std::make_pair(
								_keyframeNameAttribute.as_string(),
								_keyframeTimeAttribute.as_float())
								);
						}
					}

					if (_actionKeyframeNameTimePairs[_actionName].empty()){
						f_wrongFormat("No keyframes found in action '" + _actionName + "'");
						return false;
					}
				}

				//if no errors occurred load the keyframe configuration
				bool _validAnimationActions = true;
				for (auto &_animationName : _actionKeyframeNameTimePairs){
					_validAnimationActions &= createAnimationAction(_animationName.first, _animationName.second);
				}

				return _validAnimationActions;
			}

			bool SkeletalAnimationComponent::loadBVHAnimationActionConfig(const std::string &dir){
				std::string _actionsConfigPath = "data/animations/" + dir + "/actions.cfg";

				auto f_wrongFormat = [&_actionsConfigPath](const std::string &type){
					PRINT("Error loading animation config file '" << _actionsConfigPath << "': " << type);
				};

				pugi::xml_document _file;
				if (!_file.load_file(_actionsConfigPath.c_str())){
					ASSERT(0, "Animation configuration file '" + _xmlPath + "' not found or not valid");
					f_wrongFormat("File not found or not valid");
					return false;
				}

				pugi::xml_node _rootNode = _file.child("Animation_config");
				if (!_rootNode){
					f_wrongFormat("Invalid root node");
					return false;
				}

		
				std::vector<ActionData> _actionDataArray;
				std::vector<std::pair<std::string, std::string>> _keyframePathNamePairs;

				for (auto node = _rootNode.first_child(); node; node = node.next_sibling())
				{
					pugi::xml_attribute _actionPathAttribute = node.attribute("path");
					pugi::xml_attribute _actionNameAttribute = node.attribute("name");
					pugi::xml_attribute _actionCancelableAttribute = node.attribute("cancelable");
					pugi::xml_attribute _actionInterpolationDurationAttribute = node.attribute("interpolation_duration");

					bool _isAction = true;
					std::string _cmpString(Utilities::strUpper(node.name()));
					if (_cmpString == "ACTION"){
						_isAction = true;
					} else if (_cmpString == "KEYFRAME"){
						_isAction = false; 
					} else{
						f_wrongFormat("Unknown animation config type (not keyframe/action)");
						return false;
					}	


					ActionData _actionData;
					_actionData.m_cancelable = _actionCancelableAttribute ? _actionCancelableAttribute.as_bool() : false;
					_actionData.m_nextActionInterpolationDuration = _actionInterpolationDurationAttribute ?
						_actionInterpolationDurationAttribute.as_float() : 0.3f;

					if (_actionPathAttribute && _isAction){
						//if action will be loaded from separate file... 
						//use name to overwrite file name as the key

						_actionData.m_path = _actionPathAttribute.as_string();
						_actionData.m_name = _actionNameAttribute ? _actionNameAttribute.as_string() : "";
						_actionDataArray.push_back(_actionData);

						//Skip all keyframes if path is already specified (then use keyframes from path)
						continue;
					} else if (!_actionNameAttribute && _isAction){
						f_wrongFormat("Missing action value 'name'");
						return false;
					}

					_actionData.m_name = _actionNameAttribute.as_string();

					for (auto _keyframe = _isAction ? node.first_child() : node; _keyframe; _keyframe = _keyframe.next_sibling())
					{
						pugi::xml_attribute _keyframePathAttribute = _keyframe.attribute("path");
						if (!_keyframePathAttribute){
							f_wrongFormat("Missing keyframe value 'path'");
							return false;
						}

						pugi::xml_attribute _keyframeNameAttribute = _keyframe.attribute("name");
						std::string _keyframePath = _keyframePathAttribute.as_string();
						std::string _keyframeName = _keyframeNameAttribute ?
							_keyframeNameAttribute.as_string() : Utilities::removeFormat(_keyframePath);
						PRINT("ADDED KEYFRAME: " << _keyframePath);
						_keyframePathNamePairs.push_back(std::make_pair(_keyframePath, _keyframeName));


						//defaults to 1.0f (1 second)
						pugi::xml_attribute _keyframeTimeAttribute = _keyframe.attribute("time");
						if (!_keyframeTimeAttribute){
							_actionData.m_keyframeTimes.push_back(std::make_pair(_keyframeName, 1.0f));
						} else{
							_actionData.m_keyframeTimes.push_back(
								std::make_pair(_keyframeName, _keyframeTimeAttribute.as_float()));
						}

						//If its only a keyframe only repeat once
						if (!_isAction)break;
					}

					//Dont add any actions if we're only loading a keyframe
					if (!_isAction)continue;

					if (_actionData.m_keyframeTimes.empty()){
						f_wrongFormat("No keyframes found in action '" + _actionData.m_name + "'");
						return false;
					} else{
						_actionDataArray.push_back(_actionData);
					}
				}
			

				//load all found keyframes
				for (auto &_pathNamePair : _keyframePathNamePairs){
					bool _keyframeValid = loadBVHKeyframe(_pathNamePair.first, _pathNamePair.second);

					//PRINT("LOADED KEYFRAME: " << _pathNamePair.first);
	
					if (!_keyframeValid){
						f_wrongFormat("Unable to load keyframe '" + _pathNamePair.first + "'");
						return false;
					}
				}

		
				for (auto _animationAction : _actionDataArray){
					//PRINT("ACTION: " + _animationAction.m_name);

					//If there was no path specified
					if (_animationAction.m_path.empty()){
						//Load the keyframe action configurations
						bool _actionValid = createAnimationAction(_animationAction.m_name, _animationAction.m_keyframeTimes,
							_animationAction.m_cancelable, _animationAction.m_nextActionInterpolationDuration);
						if (!_actionValid){
							f_wrongFormat("Unable to load action '" + _animationAction.m_name + "'");
							return false;
						}

					} else{
						//If we are trying to load animated action from single BVH file (multiple keyframes)
						bool _actionValid = loadBVHAction(_animationAction.m_path, _animationAction.m_name,
							_animationAction.m_cancelable, _animationAction.m_nextActionInterpolationDuration);
						if (!_actionValid){
							f_wrongFormat("Unable to load action '" + _animationAction.m_path + "'");
							return false;
						}

						//PRINT(_animationAction.m_nextActionInterpolationDuration)
					}

				}

				return true;
			}

			bool SkeletalAnimationComponent::hasKeyframe(const std::string &name){
				return m_keyframes.find(name) != m_keyframes.end();
			}

			bool SkeletalAnimationComponent::createAnimationAction(const std::string &actionName,
				const std::vector<std::pair<std::string, float>> &keyframeNamesTimes,
				bool cancelable, float nextActionInterpolationDuration)
			{
				std::string _missingKeyframe;
				for (auto &_keyframeName : keyframeNamesTimes){
					if (!hasKeyframe(_keyframeName.first)){
						_missingKeyframe = _keyframeName.first;
						break;
					}
				}

				if (_missingKeyframe.empty()){
					m_animationActions[actionName] = SkeletalAnimationAction(actionName);
					m_animationActions[actionName].m_cancelable = cancelable;
					m_animationActions[actionName].m_nextActionInterpolationDuration = nextActionInterpolationDuration;

					PRINT("ANIMATION ACTION: " << actionName);
					for (auto &_nameTimePair : keyframeNamesTimes){
						m_keyframes[_nameTimePair.first]->m_duration = _nameTimePair.second;
						//PRINT(_nameTimePair.first << " " << _nameTimePair.second);

						m_animationActions[actionName].addKeyframe(
							m_keyframes[_nameTimePair.first], _nameTimePair.second);
					}
				} else{
					ASSERT(0, "Animation action '" + actionName + "' is missing the keyframe '" + _missingKeyframe + "'");
					ERROR("Animation action '" + actionName + "' is missing the keyframe '" + _missingKeyframe + "'");
					//TODO remove error
				}

				return _missingKeyframe.empty();
			}


			
			void SkeletalAnimationComponent::loadBVHKeyframes(const std::string &path){
				//TODO maybe remove already set from a skeleton path in ctor
				m_fileDirectory = Utilities::removeFormat(path);
				loadBVHAnimationActionConfig(m_fileDirectory);
			}
	
			void SkeletalAnimationComponent::loadKeyframes(const std::string &path){
				ASSERT(m_skeleton != nullptr, "Skeleton is a null pointer");
				if (!m_skeleton)return;

				Assimp::Importer _importer;
				//std::string _filename = "data/animations/" + path; //TODO put back
				std::string _filename = "data/models/arms.bvh";
				const aiScene* _scene = _importer.ReadFile(_filename, 0);

				if (!_scene){
					ERROR("Can't load file '" + _filename + "'");
					return;
				} else if (_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode){
					ERROR("Can't load animation '" + _filename + "'");
					return;
				} else{
					//set current path to a valid path
					m_fileDirectory = Utilities::removeFormat(path);

					int _numBones = Animation::getNumBones(_scene->mRootNode, _scene);
					ERROR("BONES::" << _numBones << ";BONEMAP::" << m_skeleton->m_boneMap.size());

					auto &_boneMap = m_skeleton->m_boneMap;
					for (auto _s : _boneMap){
						PRINT(_s.first);
					}


					if (_numBones != 0 && _numBones == m_skeleton->m_boneMap.size()){

						//hardcoded root bone SceneRoot->Armature->RootBone
						//PRINT("NODE NAME: " << findBoneRoot(_scene->mRootNode)->mName.data);


						//m_sceneTransform *= convertMat4(_scene->mRootNode->mTransformation);
						//m_sceneTransform *= convertMat4(_scene->mRootNode->mChildren[0]->mTransformation);


						//m_sceneTransform = glm::inverse(m_sceneTransform);
						//Utilities::print(m_sceneTransform);
						//Utilities::print(m_skeleton->m_globalInverseTransform);

						/*auto _globalInverseTransform = _scene->mRootNode->mTransformation;
						_globalInverseTransform = _globalInverseTransform.Inverse();
						m_skeleton->m_globalInverseTransform = 
							Utilities::convertMat4(_globalInverseTransform);*/


						//m_sceneTransform = glm::mat4(1.0f);

						//const aiNode *_rootBone = _scene->mRootNode->mChildren[0];
						//const aiNode *_rootBone = _scene->mRootNode->mChildren[0]->mChildren[0];


						changeOffsets(_scene->mRootNode, _scene);

						//m_skeleton->getAnimationAction("index").m_scene = _importer.GetOrphanedScene();
						//m_tmpScene = _importer.GetOrphanedScene();


						/*for (int i = 0; i < _scene->mNumAnimations; ++i){
						std::string _an = _scene->mAnimations[i]->mName.C_Str();
						std::string::size_type _dotId = _an.find_last_of('|');
						std::string _keyframeName = _dotId == std::string::npos ? _an : _an.substr(_dotId + 1);

						std::vector<SkeletalAnimationTransform> _transforms(_numBones);
						loadBoneTransforms(_scene->mAnimations[i], _transforms);


						//PRINT("OFFSETS: " << _offsets.size() << " BONES: " << _transforms.size());
						m_keyframes[_keyframeName] = SkeletalAnimationKeyframe(_keyframeName, _transforms);
						}*/
					} else{
						ERROR("NUM BONES NOT THE SAME AS BONEMAP => BONES::" <<
							_numBones << ";BONEMAP::" << m_skeleton->m_boneMap.size());
						return;
					}
				}


				//load keyframe times from XML file
				//loadAnimationActionConfig(path);
			}

			void SkeletalAnimationComponent::reloadAnimationActionConfig(){
				m_animationActions.clear();
				m_keyframes.clear();
				loadBVHAnimationActionConfig(m_fileDirectory);
				m_animationPlayer->clearTimeline();
			}



			const Buffers::ArrayBuffer &SkeletalAnimationComponent::getBuffer() const{
				return m_boneBuffer;
			}

			std::size_t SkeletalAnimationComponent::getNumBones() const{
				return m_skeleton->m_boneMap.size();
			}

			AnimationSkeleton &SkeletalAnimationComponent::getSkeleton() const{
				ASSERT(m_skeleton != nullptr, "Skeleton is invalid");

				return *m_skeleton.get();
			}

			std::vector<glm::mat4> getDefaultBoneMatrices(){
				//TODO globalInverse * offset 
				std::vector<glm::mat4> _transforms;

				return _transforms;
			}



			void SkeletalAnimationComponent::addAction(
				const std::string &name, const SkeletalAnimationAction &action)
			{
				m_animationActions[name] = action;
			}

			void SkeletalAnimationComponent::playAction(const std::string &name){
				const auto _itr = m_animationActions.find(name);
				if (!name.empty() && _itr == m_animationActions.end()){
					ASSERT(0, "Cannot find animation action '" + name + "'");
					PRINT("Cannot find animation action '" + name + "'");
					return;
				}

				if (!name.empty()){
					auto &_animationAction = (*_itr).second;
					m_animationPlayer->playAction(_animationAction);

					//PRINT("PLAYING ACTION: " << name << " ");
					//for (auto &_keyframe : _animationAction.addKeyframe);



				} else{
					m_animationPlayer->playAction((*m_animationActions.begin()).second);
				}


				/*if (m_nextAnimationAction != nullptr){
					if (m_currentAnimationAction == m_nextAnimationAction || 
						m_nextAnimationAction->m_name == "stand") //TODO remove hardcoded
					{
						if (m_nextAnimationAction != (&_animationAction)){
							m_nextAnimationAction = &_animationAction;
						}
					}

				} else if (m_currentAnimationAction != nullptr){
						m_nextAnimationAction = &_animationAction;
				} else{
					m_currentAnimationAction = &_animationAction;
					m_nextAnimationAction = nullptr;

					if (m_lastAnimationAction == nullptr){
						m_lastAnimationAction = m_currentAnimationAction;
					}

					m_lastAnimationAction->playAnimation();
					//m_currentAnimationAction->playAnimation();
				}*/
			}

			const std::string SkeletalAnimationComponent::getPlayingAction() const{
				return m_animationPlayer->getPlayingAction();
			}

			void SkeletalAnimationComponent::updateBoneTransforms(double timeInSeconds){
				ASSERT(animationId < m_skeleton->m_animations.size(), "Animation ID out of range");

				/*if (m_currentAnimationAction == nullptr)return;

				if ((m_lastAnimationAction == nullptr || !m_lastAnimationAction->m_beingPlayed) && 
					!m_currentAnimationAction->m_beingPlayed)
				{
					m_lastAnimationAction = m_currentAnimationAction;

					m_currentAnimationAction = m_nextAnimationAction;
					m_nextAnimationAction = nullptr;

					if (m_currentAnimationAction != nullptr){
						m_currentAnimationAction->playAnimation();
					} else{
						//playAction("stand");

						//if action "stand" is not found
						if (m_currentAnimationAction == nullptr){
							return;
						}
					}
				}

				if (m_lastAnimationAction != nullptr && m_lastAnimationAction->m_beingPlayed){
					m_transforms = m_lastAnimationAction->getInterpolatedTransforms(
						m_boneTreeRoot.get(), timeInSeconds, m_currentAnimationAction, true);

					//now start to play the current animation
					if (!m_lastAnimationAction->m_beingPlayed && m_lastAnimationAction != m_currentAnimationAction){
						m_currentAnimationAction->playAnimation();
					}
				} else{
					m_transforms = m_currentAnimationAction->getInterpolatedTransforms(
						m_boneTreeRoot.get(), timeInSeconds, m_nextAnimationAction, false);
				}*/
				
				//Utilities::print(m_skeleton->m_globalInverseTransform);

				//TODO remove timeInSeconds arguments -> NOT USED
				m_transforms = m_animationPlayer->getInterpolatedTransforms(
					m_skeleton->m_animationBoneTreeRoot.get(), timeInSeconds);

				updateOffsetTransforms();


				//auto &_animationAction = m_skeleton->getAnimationAction("index");
				//const aiAnimation *_animation = _animationAction.m_animations[0];

				/*const aiAnimation *_animation = m_tmpScene->mAnimations[0];
				const float _animationDuration = static_cast<float>(_animation->mDuration);
				float _ticksPerSecond = static_cast<float>(_animation->mTicksPerSecond);
				_ticksPerSecond = _ticksPerSecond == 0.0f ? c_defaultTicksPerSecond : _ticksPerSecond;
				const float _ticks = timeInSeconds * _ticksPerSecond;
				const float _animationTime = fmod(_ticks, _animationDuration);

				int _count = setChildTransforms(_animationTime,
				m_tmpRoot, _animation, glm::mat4(1.0f));


				auto &_transforms = m_skeleton->m_boneTransforms;
				for (std::size_t i = 0; i < getNumBones(); ++i){
				m_finalTransforms[i] = _transforms[i].m_transform;
				}*/

			}



			void SkeletalAnimationComponent::updateOffsetTransforms(){
				bool _playOriginalFile = false; //BVH or original transforms

				for (int i = 0; i < m_transforms.size(); ++i){
					auto &_boneTransform = m_skeleton->m_boneTransforms[i];
			
					//For collision only updated bones
					if (_boneTransform.m_isDynamic){
						m_offsetTransforms[i] = m_transforms[i];
					} else{
						glm::mat4 _offset = _playOriginalFile ? _boneTransform.m_originalOffset :
							_boneTransform.m_bvhOffset;

						m_offsetTransforms[i] = m_skeleton->m_globalInverseTransform * m_transforms[i] * _offset;
					}
				}
			}

			std::vector<glm::mat4> &SkeletalAnimationComponent::getTransforms(){
				return m_transforms;
			}

			const std::vector<glm::mat4> &SkeletalAnimationComponent::getTransforms() const{
				return m_transforms;
			}

			void SkeletalAnimationComponent::setTransforms(const std::vector<glm::mat4> &transforms){
				m_transforms = transforms;
			}

			const std::vector<glm::mat4> &SkeletalAnimationComponent::getOffsetTransforms() const{
				return m_offsetTransforms;
			}
			
			const std::vector<glm::mat4> &SkeletalAnimationComponent::getPreviousOffsetTransforms() const{
				return m_previousOffsetTransforms;
			}

			void SkeletalAnimationComponent::setPreviousOffsetTransforms(const std::vector<glm::mat4> &transforms){
				m_previousOffsetTransforms = transforms;
			}


			unsigned int SkeletalAnimationComponent::getBoneId(const std::string &boneAlias) const{
				ASSERT(m_skeleton != nullptr, "Skeleton is a nullptr");

				return m_skeleton->getBoneId(boneAlias);
			}


			void SkeletalAnimationComponent::setKeyframeDuration(const std::string &actionName, 
				const std::string &keyframeName, float duration) 
			{
				auto _itr = m_animationActions.find(actionName);

				if (_itr != m_animationActions.end()){
					(*_itr).second.setKeyframeDuration(keyframeName, duration);
				} else{
					PRINT("Unable to set keyframe duration: unable to find action '" << actionName << "'");
				}
			}


			SkeletalAnimationPlayer &SkeletalAnimationComponent::getAnimationPlayer() const{
				return *m_animationPlayer;
			}


			void SkeletalAnimationComponent::addFlags(int flags){
				m_flags |= flags;
			}

			void SkeletalAnimationComponent::removeFlags(int flags){
				m_flags &= ~flags;
			}

			const bool SkeletalAnimationComponent::isTypeLooping() const{
				return m_flags & ANIMATION_FLAG_LOOP;
			}


			SkeletalAnimationKeyframe* SkeletalAnimationComponent::getKeyframe(const std::string &name){
				auto _itr = m_keyframes.find(name);
				if (_itr != m_keyframes.end()){
					return (*_itr).second.get();
				} else{
					return nullptr;
				}
			}

			SkeletalAnimationAction *SkeletalAnimationComponent::getAction(const std::string &name){
				auto _itr = m_animationActions.find(name);
				if (_itr != m_animationActions.end()){
					return &(*_itr).second;
				} else{//TODO error?
					return nullptr;
				}
			}

			void SkeletalAnimationComponent::updateDurationsForAction(
				const std::string &actionName)
			{
				auto _action = getAction(actionName);
				if (_action){
					m_animationPlayer->updateDurationsForAction(*_action);
				}
			}


		}
	}
}
