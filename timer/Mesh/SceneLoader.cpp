#include "SceneLoader.hpp"

#include <numeric>
#include <queue>
#include <SDL2/SDL_timer.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <glm\gtx\transform.hpp>

#include "Mesh.hpp"
#include "../Animation/AnimationSkeleton.hpp"
#include "../Animation/SkeletalAnimationTransform.hpp"
#include "../Texture/Texture.hpp"
#include "../Define/PrintDefines.hpp"
#include "../Define/Assert.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/AnimationUtilities.hpp"

#define SMOOTH_NORMALS
#define CALCULATE_CENTER
#define CORRECT_LOCATION
#define FLIP_UVS


namespace Engine{
	namespace Meshes{
		


		TextureData::TextureData(const std::string &path, unsigned int type) :
			m_path(path), m_type(type)
		{
		}


		MeshPropertiesData::MeshPropertiesData(const std::string &name,
			std::shared_ptr<Mesh> mesh, const std::vector<TextureData> &textureData
		):
			m_name(name), m_mesh(mesh), m_textureData(textureData)
		{
		}


		const bool MeshData::hasSkeleton()const {
			return m_skeleton != nullptr && !m_skeleton->m_boneMap.empty();
		}

		const bool MeshData::isValid()const {
			return !m_meshes.empty();
		}



		
		std::size_t findLocationIndex(float animationTime, const aiNodeAnim *animationNode){
			for (std::size_t i = 0; i < animationNode->mNumPositionKeys - 1; ++i){
				if (animationTime < static_cast<float>
					(animationNode->mPositionKeys[i + 1].mTime)){
					return i;
				}
			}return 0;
		}

		std::size_t findRotationIndex(float animationTime, const aiNodeAnim *animationNode){
			for (std::size_t i = 0; i < animationNode->mNumRotationKeys - 1; ++i){
				if (animationTime < static_cast<float>
					(animationNode->mRotationKeys[i + 1].mTime)){
					return i;
				}
			}return 0;
		}

		std::size_t findScaleIndex(float animationTime, const aiNodeAnim *animationNode){
			for (std::size_t i = 0; i < animationNode->mNumScalingKeys - 1; ++i){
				if (animationTime < static_cast<float>
					(animationNode->mScalingKeys[i + 1].mTime)){
					return i;
				}
			}return 0;
		}

		void loadBoneTransforms(
			const std::unordered_map<std::string, unsigned int> &boneMap, const aiAnimation *animation, 
			std::vector<Animation::SkeletalAnimationTransform> &transforms, float keyframeStartTime)
		{
			for (std::size_t i = 0; i < animation->mNumChannels; ++i){
				const aiNodeAnim *_animationNode = animation->mChannels[i];
				std::string _boneName(_animationNode->mNodeName.data);

				auto _boneItr = boneMap.find(_boneName);
				if (_boneItr != boneMap.end())
				{
					int _locationKey = findLocationIndex(keyframeStartTime, _animationNode);
					int _rotationKey = findRotationIndex(keyframeStartTime, _animationNode);
					int _scaleKey = findScaleIndex(keyframeStartTime, _animationNode);

					bool _hasLocationTransform = _animationNode->mNumPositionKeys > _locationKey;
					bool _hasRotationTransform = _animationNode->mNumRotationKeys > _rotationKey;
					bool _hasScaleTransform = _animationNode->mNumScalingKeys > _scaleKey;
					bool _hasTransform = _hasLocationTransform || _hasRotationTransform || _hasScaleTransform;


					glm::vec3 _locationTransform(0.0f);
					if (_hasLocationTransform){
						aiVector3D _location = _animationNode->mPositionKeys[_locationKey].mValue;
						_locationTransform = glm::vec3(_location.x, _location.y, _location.z);
					}

					glm::quat _rotationTransform(1.0f, 0.0f, 0.0f, 0.0f);
					if (_hasRotationTransform){
						aiQuaternion _rotation = _animationNode->mRotationKeys[_rotationKey].mValue;
						_rotationTransform = glm::quat(_rotation.w, _rotation.x, _rotation.y, _rotation.z);
					}

					glm::vec3 _scaleTransform(1.0f);
					if (_hasScaleTransform){
						aiVector3D _scale = _animationNode->mScalingKeys[_scaleKey].mValue;
						_scaleTransform = glm::vec3(_scale.x, _scale.y, _scale.z);
					}


					unsigned int _boneId = (*_boneItr).second;
					ASSERT(_boneId < transforms.size(), "Bone ID out of transforms range");

					transforms[_boneId].m_name = _boneName;
					transforms[_boneId].m_boneId = _boneId;
					transforms[_boneId].m_location = _locationTransform;
					transforms[_boneId].m_rotation = _rotationTransform;
					transforms[_boneId].m_scale = _scaleTransform;


					if (!_hasTransform){
						ASSERT(0, "Animation node is missing a transformation keyframe");
						ERROR("Animation node is missing a transformation keyframe " <<
							" | COUNTS: " << _animationNode->mNumPositionKeys << " " <<
							_animationNode->mNumRotationKeys << " " << _animationNode->mNumScalingKeys);
						return;
					}//*/

				}

			}
		}


		Animation::SkeletalAnimationAction loadAnimationAction(
			const std::unordered_map<std::string, unsigned int> &boneMap, 
			const aiAnimation &animation)
		{
			const float c_defaultTicksPerSecond = 60.0f;

			float _ticksPerSecond = static_cast<float>(animation.mTicksPerSecond);
			float _keyframeTime = 1.0f / (0.0f ? c_defaultTicksPerSecond : _ticksPerSecond);
			//PRINT("KEYFRAME TIME: " << _keyframeTime);

			std::string _actionName(animation.mName.data);

		
			auto &_animationChannels = animation.mChannels;
			std::size_t _numKeyframes = 0;
			for (std::size_t i = 0; i < animation.mNumChannels; ++i){
				std::size_t _numP = animation.mChannels[0]->mNumPositionKeys;
				std::size_t _numR = animation.mChannels[0]->mNumRotationKeys;
				std::size_t _numS = animation.mChannels[0]->mNumScalingKeys;

				std::size_t _maxNumKeyframes = _numP > _numR ? _numP : _numR;
				_maxNumKeyframes = _numS > _maxNumKeyframes ? _numS : _maxNumKeyframes;

				_numKeyframes = _maxNumKeyframes > _numKeyframes ? _maxNumKeyframes : _maxNumKeyframes;
			}

			
			std::vector<float> _keyframeDurations(_numKeyframes, 0.016667f);
			std::size_t _idP = 0, _idR = 0, _idS = 0;
			float _currentTime = 0.0f;

			for (std::size_t i = 0; i < _keyframeDurations.size(); ++i){
				float _shortestP, _shortestR, _shortestS;
				_shortestP = _shortestR = _shortestS = animation.mDuration;

				for (std::size_t j = 0; j < animation.mNumChannels; ++j){
					const auto _channel = animation.mChannels[j];
			
					if (_channel->mNumPositionKeys > _idP){
						_shortestP = glm::min(_shortestP,
							static_cast<float>(_channel->mPositionKeys[_idP].mTime));
					}
					if (_channel->mNumRotationKeys > _idR){
						_shortestR = glm::min(_shortestP,
							static_cast<float>(_channel->mRotationKeys[_idR].mTime));
					}
					if (_channel->mNumScalingKeys > _idS){
						_shortestS = glm::min(_shortestP,
							static_cast<float>(_channel->mScalingKeys[_idS].mTime));
					}		
				}

				float _lowestTime = glm::min(_shortestP, glm::min(_shortestR, _shortestS));

				if (_lowestTime == _shortestP)
					++_idP;
				if (_lowestTime == _shortestR)
					++_idR;
				if (_lowestTime == _shortestS)
					++_idS;
			
				_keyframeDurations[i] = _lowestTime - _currentTime;
				_currentTime = _lowestTime;
			}//*/


			Animation::SkeletalAnimationAction _animationAction;
			_currentTime = 0.0f;

			for (std::size_t i = 0; i < _numKeyframes; ++i){
				std::vector<Animation::SkeletalAnimationTransform> _transforms(animation.mNumChannels);

				//start times for each keyframe
				Meshes::loadBoneTransforms(boneMap, &animation,
					_transforms, _currentTime);
			
				std::string _keyframeName = _actionName + std::to_string(i);		
				auto _keyframe = std::make_shared<Animation::SkeletalAnimationKeyframe>(_keyframeName, _transforms);
				_animationAction.addKeyframe(_keyframe, _keyframeDurations[i]);

				//PRINT(_keyframeName << " " << animation.mChannels[0]->mRotationKeys[i].mTime);

				_currentTime += _keyframeDurations[i];
			}

			return _animationAction;
		}


		void loadAnimationActions(const aiScene *scene, MeshData &meshData){
			if (meshData.m_skeleton == nullptr || scene == nullptr){
				PRINT("Skeleton is a null pointer");
				return;
			}

			auto &_boneMap = meshData.m_skeleton->m_boneMap;
			for (int i = 0; i < scene->mNumAnimations; ++i){
				const aiAnimation &_animation = *scene->mAnimations[i];
				const std::string _animationName(_animation.mName.data);

				meshData.m_skeleton->m_animationActions[_animationName] = 
					loadAnimationAction(_boneMap, _animation);
			}
		}


		aiNode *findAnimationBoneRoot(aiNode *rootNode,
			const std::unordered_map<std::string, unsigned int> &boneMap)
		{
			if (!rootNode){
				return nullptr;
			}


			std::queue<aiNode*> _queue;
			_queue.push(rootNode);

			while (!_queue.empty()){
				aiNode *_node = _queue.front();
				_queue.pop();

				if (!_node){
					return nullptr;
				}

				std::string _childName(_node->mName.data);
				if (boneMap.find(_childName) != boneMap.end()){
					return _node;
				}

				for (std::size_t i = 0; i < _node->mNumChildren; ++i){
					_queue.push(_node->mChildren[i]);
				}
			}

			return rootNode;
		}

		void loadAnimationBoneTree(const aiNode *node, std::shared_ptr<Animation::AnimationBoneTree> boneNode,
			const Animation::AnimationSkeleton &skeleton, std::vector<std::string> strings)
		{
			//return to prevent adding new bones
			std::string _nodeName(node->mName.data);

			auto &_boneMap = skeleton.m_boneMap;

			auto _boneMapItr = _boneMap.find(_nodeName);
			if (_boneMapItr != _boneMap.end()){
				boneNode->m_name = _nodeName;
				boneNode->m_boneId = (*_boneMapItr).second;
				//boneNode.m_boneId = boneMap[_nodeName];
			}


			if (node->mNumChildren <= 0){
				for (auto _s : strings){
					std::cout << _s << " -> ";
				}std::cout << _nodeName << std::endl;
			}strings.push_back(_nodeName); 


			for (std::size_t i = 0; i < node->mNumChildren; ++i){
				aiNode *child = node->mChildren[i];

				//Bones without known aliases will not be added to the animation tree
				//Therefore we will skip animating those in keyframes
				std::string _childName(child->mName.data);
				if (skeleton.hasAliasForBone(_childName)){
					boneNode->m_children.emplace_back(
						std::make_shared<Animation::AnimationBoneTree>());

					auto _childNode = boneNode->m_children.back();
					_childNode->m_parent = boneNode;

					loadAnimationBoneTree(child, _childNode, skeleton, strings);
				}
			}
		}


		void printSceneTree(const aiNode *node, std::vector<std::string> strings){
			if (node->mNumChildren == 0){
				/*for (auto _s : strings){
					std::cout << "'" << _s << "' -> ";
				}PRINT("");*/
			} else{

				strings.push_back(std::string(node->mName.data));
				//std::cout << node->mName.data << " -> ";
				//Utilities::print(Utilities::convertMat4(node->mTransformation));

				for (int i = 0; i < node->mNumChildren; ++i){
					printSceneTree(node->mChildren[i], strings);
				}
			}
		}


		void loadBones(const aiMesh *mesh, Animation::AnimationSkeleton *skeleton){
			auto &_boneMap = skeleton->m_boneMap;
			auto &_boneTransforms = skeleton->m_boneTransforms;
			auto &_vertexBoneData = skeleton->m_vertexBoneData;
			auto &_insertionOrder = skeleton->m_insertionOrder;

			std::size_t _baseIndex = _vertexBoneData.size();
			_vertexBoneData.resize(_baseIndex + mesh->mNumVertices);


			for (std::size_t i = 0; i < mesh->mNumBones; ++i){
				auto _bone = mesh->mBones[i];

				std::size_t _boneId = 0;
				std::string _boneName(_bone->mName.data);
				PRINT(_boneName);

				if (_boneMap.find(_boneName) == _boneMap.end()){
					_boneId = _boneMap.size();
					_boneMap[_boneName] = _boneId;
					_insertionOrder.push_back(_boneName);

					_boneTransforms.push_back(Animation::AnimationSkeleton::BoneTransform());

					glm::mat4 _offset = Utilities::convertMat4(_bone->mOffsetMatrix);
					_boneTransforms[_boneId].m_originalOffset = _offset;
					_boneTransforms[_boneId].m_originalInverseOffset = glm::inverse(_offset);
				}
				else{
					_boneId = _boneMap[_boneName];
				}

				for (std::size_t j = 0; j < _bone->mNumWeights; ++j){
					std::size_t _vertexId = _baseIndex + _bone->mWeights[j].mVertexId;
					float _weight = _bone->mWeights[j].mWeight;
					_vertexBoneData[_vertexId].addBone(_boneId, _weight);
				}
			}

		}
	

		std::vector<TextureData> loadTextures(const aiMaterial* material){
			auto _getTextureCount = [material](){
				int _counter = 0;
				for (int i = 0; i <= aiTextureType_UNKNOWN; i++){
					int _count = material->GetTextureCount(static_cast<aiTextureType>(i));
					std::cout << _count << " ";
					_counter += _count;
				}std::cout << "\n";

				return _counter;
			};


			std::vector<std::pair<aiTextureType, Textures::TextureType>> _types{
				{ aiTextureType_DIFFUSE, Textures::TEXTURE_ALBEDO },
				{ aiTextureType_SHININESS, Textures::TEXTURE_ROUGHNESS },
				{ aiTextureType_AMBIENT, Textures::TEXTURE_METALLIC },
				{ aiTextureType_OPACITY, Textures::TEXTURE_OPACITY },
				{ aiTextureType_SPECULAR, Textures::TEXTURE_OCCLUSION },
				{ aiTextureType_NORMALS, Textures::TEXTURE_NORMAL },
				{ aiTextureType_HEIGHT, Textures::TEXTURE_HEIGHT },
			};

			
			//PRINT("NORMALS COUNT: " << material->GetTextureCount(aiTextureType_NORMALS));
			//PRINT("HEIGHTS COUNT: " << material->GetTextureCount(aiTextureType_HEIGHT));
			//PRINT("------------------------------------------------------------");
			
			aiString _string;
			std::vector<TextureData> _textures;

			for (auto &_pair : _types){
				unsigned int _tCount = material->GetTextureCount(_pair.first);
				unsigned int _textureCount = _tCount > 0 ? 1 : 0;

				if (_pair.second == Textures::TEXTURE_UNKNOWN){
					PRINT("UNKNOWN_TEXTURE: " << _textureCount)
				}

				for (std::size_t i = 0; i < _textureCount; ++i){
					material->GetTexture(_pair.first, 0, &_string);
					std::string _path = _string.C_Str();
					std::replace(_path.begin(), _path.end(), '\\', '/');
					
					_textures.push_back(TextureData(_path, _pair.second));
				}

				//LOAD IF NOT FOUND
				/*if (_textureCount == 0){ 
					_textures.push_back(std::make_pair(_pair.second == Textures::TEXTURE_NORMAL ? 
						"defaultNormalmap.jpg" : "white.jpg", _pair.second));
				}*/

			}

			//std::cout << "OCCLUSION: " << material->GetTextureCount(aiTextureType_SPECULAR) << "\n";

			return _textures;
		}


		std::unique_ptr<Mesh> loadMesh(const aiMesh *mesh, const aiScene *scene){
			ASSERT(mesh->mNumVertices, "Model has no vertices");
			ASSERT(mesh->mNumFaces, "Mesh has no faces");

			aiColor4D _color;
			aiMaterial* _material = scene->mMaterials[mesh->mMaterialIndex];
			aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &_color);
			//glm::vec3 _defaultColor(_color.r, _color.g, _color.b);
			glm::vec3 _defaultColor(0.0, 0.0, 1.0);

			std::vector<Meshes::VertexData> _tmpVertexData;
			std::vector<unsigned int> _tmpIndices;

			_tmpVertexData.reserve(mesh->mNumVertices);
			_tmpIndices.reserve(mesh->mNumFaces * mesh->mFaces[0].mNumIndices);

			glm::vec3 _tmp;
			VertexData _tmpData;
	
			auto &v1 = mesh->mVertices[0];
			glm::vec3 _max = glm::vec3(v1.x, v1.y, v1.z);
			glm::vec3 _min = glm::vec3(v1.x, v1.y, v1.z);
			glm::vec3 _center = glm::vec3(0.0f, 0.0f, 0.0f);

#ifdef CALCULATE_CENTER
			for (std::size_t i = 1; i < mesh->mNumVertices; ++i){
				_tmp.x = mesh->mVertices[i].x;
				_tmp.y = mesh->mVertices[i].y;
				_tmp.z = mesh->mVertices[i].z;

				if (_tmp.x > _max.x) _max.x = _tmp.x;
				else if (_tmp.x < _min.x) _min.x = _tmp.x;
				if (_tmp.y > _max.y) _max.y = _tmp.y;
				else if (_tmp.y < _min.y) _min.y = _tmp.y;
				if (_tmp.z > _max.z) _max.z = _tmp.z;
				else if (_tmp.z < _min.z) _min.z = _tmp.z;
			}

			//get center of mass
			_center = (_min + _max) / glm::vec3(2.0f);
			_min -= _center;
			_max -= _center;
#endif 

			bool _hasAnimation = scene->HasAnimations();

			for (std::size_t i = 0; i < mesh->mNumVertices; ++i){
				_tmp.x = mesh->mVertices[i].x;
				_tmp.y = mesh->mVertices[i].y;
				_tmp.z = mesh->mVertices[i].z;


#ifdef CORRECT_LOCATION
				if (_hasAnimation)_tmpData.setVertex(_tmp);
				else _tmpData.setVertex(_tmp - _center);

#else
				_tmpData.m_vertex = _tmp;

				if (_tmp.x > _max.x) _max.x = _tmp.x;
				else if (_tmp.x < _min.x) _min.x = _tmp.x;
				if (_tmp.y > _max.y) _max.y = _tmp.y;
				else if (_tmp.y < _min.y) _min.y = _tmp.y;
				if (_tmp.z > _max.z) _max.z = _tmp.z;
				else if (_tmp.z < _min.z) _min.z = _tmp.z;
#endif

				if (mesh->mNormals){
					_tmp.x = mesh->mNormals[i].x;
					_tmp.y = mesh->mNormals[i].y;
					_tmp.z = mesh->mNormals[i].z;
				}else _tmp = glm::vec3(0.0, 1.0f, 0.0f);
				_tmpData.setNormal(_tmp);


				if (mesh->mTangents){
					_tmp.x = mesh->mTangents[i].x;
					_tmp.y = mesh->mTangents[i].y;
					_tmp.z = mesh->mTangents[i].z;
				}else _tmp = glm::vec3(1.0, 0.0f, 0.0f);
				_tmpData.setTangent(_tmp);


				if (mesh->mColors[0]){
					_tmp.x = mesh->mColors[0][i].r;
					_tmp.y = mesh->mColors[0][i].g;
					_tmp.z = mesh->mColors[0][i].b;
					//Utilities::print(_tmp);

				}else _tmp = _defaultColor;
				_tmpData.setColor(_tmp);


				if (mesh->mTextureCoords[0]){
					_tmp.x = mesh->mTextureCoords[0][i].x;
					_tmp.y = mesh->mTextureCoords[0][i].y;
				}else _tmp = glm::vec3(0.0f);
				_tmpData.setUv(glm::vec2(_tmp.x, _tmp.y));


				_tmpVertexData.push_back(_tmpData);
			}

			for (std::size_t i = 0; i < mesh->mNumFaces; ++i){
				const aiFace _face = mesh->mFaces[i];
				for (std::size_t j = 0; j < _face.mNumIndices; ++j){
					_tmpIndices.push_back(_face.mIndices[j]);
				}
			}

			
			/*int _count = 0;
			for (int i = 1; i <= aiTextureType_UNKNOWN; ++i){
				_count += _mat->GetTextureCount(static_cast<aiTextureType>(i));
			}

			PRINT(_mat->GetTextureCount(aiTextureType_NORMALS));
			PRINT(_mat->GetTextureCount(aiTextureType_HEIGHT));
			PRINT(_count)*/

			aiString _materialName;
			aiReturn _r = _material->Get(AI_MATKEY_NAME, _materialName);

			return std::make_unique<Mesh>(
				"", _r == AI_SUCCESS ? _materialName.C_Str() : "",
				std::move(_tmpVertexData),
				std::move(_tmpIndices),
				std::move(_min),
				std::move(_max),
				std::move(_center)
			);
		}


		bool hasSkeleton(const aiNode *node, const aiScene *scene){
			bool _hasSkeleton = false;

			for (std::size_t i = 0; i < node->mNumMeshes; ++i){
				const aiMesh *_mesh = scene->mMeshes[node->mMeshes[i]];
				if (_mesh->HasBones()){
					_hasSkeleton = true;
					break;
				}
			}

			if (!_hasSkeleton){
				for (std::size_t i = 0; i < node->mNumChildren; ++i){
					_hasSkeleton |= hasSkeleton(node->mChildren[i], scene);
				}
			}

			return _hasSkeleton;
		}

		void recursion(const aiNode *node, const aiScene *scene, MeshData &meshData){
			meshData.m_meshes.reserve(meshData.m_meshes.size() + node->mNumMeshes);

			
			//PRINT("BONEEEEEEES------------------------------------" << node->mName.data);
			//Utilities::print(Utilities::convertMat4(node->mTransformation));

			for (std::size_t i = 0; i < node->mNumMeshes; ++i){
				const aiMesh *_mesh = scene->mMeshes[node->mMeshes[i]];
				std::string _meshName = std::string(_mesh->mName.data);
				//PRINT("MESHNAME: " << _meshName);

				meshData.m_meshes.emplace_back(
					_meshName, 
					loadMesh(_mesh, scene),
					loadTextures(scene->mMaterials[_mesh->mMaterialIndex])
				);
				
				//PRINT(_meshName << " " << _mesh->HasBones());

				if (_mesh->HasBones()){
					//PRINT("BONEEEEEEES------------------------------------" << node->mName.data);
					//Utilities::print(Utilities::convertMat4(node->mTransformation));
					loadBones(_mesh, meshData.m_skeleton.get());
				}
			}

			//PRINT("NODE: " << std::string(node->mName.data) << "  NUM CHILD: " << node->mNumChildren)

			for (std::size_t i = 0; i < node->mNumChildren; ++i){
				recursion(node->mChildren[i], scene, meshData);
			}
		}


		//TODO delete??
		/*void loadSkeleton(const aiNode *node, const aiAnimation *animation){
			//PRINT("NUM CHILDREN: " << node->mNumChildren)

			for (std::size_t i = 0; i < node->mNumChildren; ++i){
				loadSkeleton(node->mChildren[i], animation);
			}

		}*/

		
		MeshData loadSceneData(const std::string &path)
		{
			Assimp::Importer _importer;
			Uint32 _timer = SDL_GetTicks();
			std::string _filename = "data/models/" + path;

			MeshData _meshData;

			const aiScene* _scene = _importer.ReadFile(_filename, 0
				//aiProcessPreset_TargetRealtime_Quality 

				| aiProcess_Triangulate 
				//| aiProcess_JoinIdenticalVertices
				//| aiProcess_SortByPType 
				| aiProcess_CalcTangentSpace
#ifdef SMOOTH_NORMALS	
				| aiProcess_GenSmoothNormals
#endif
#ifdef FLIP_UVS			
				| aiProcess_FlipUVs
#endif							
			);
			
			if (!_scene){
				ERROR("Can't load file '" + _filename + "'");
			}else if (_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode){
				ERROR("Can't load model '" + _filename + "'");
			}else
			{
				_meshData.m_path = path;
				_meshData.m_skeleton = std::make_shared<Animation::AnimationSkeleton>();

				recursion(_scene->mRootNode, _scene, _meshData);
				//loadSkeleton(_scene->mRootNode, nullptr);


				std::cout << "Loaded model: " + _filename + " Meshes: " << _meshData.m_meshes.size() <<
					" Bones: " << _meshData.m_skeleton->m_boneMap.size() <<
					" Time: " << SDL_GetTicks() - _timer << "ms" << std::endl;

				//remove skeleton if mesh has no bones
				if (_meshData.m_skeleton->m_boneMap.empty()){
					_meshData.m_skeleton = nullptr;

					PRINT("NO SKELETON_________________: " << path);
				}
				else{
					_meshData.m_skeleton->m_filePath = path;
					_meshData.m_skeleton->loadBoneAliases(path);

					PRINT("SKELETON_________________: " << path);

					loadAnimationActions(_scene, _meshData);

					auto &_boneMap = _meshData.m_skeleton->m_boneMap;
					//const aiNode *_rootBone = _scene->mRootNode->mChildren[0]->mChildren[0]; 
					aiNode *_rootBone = findAnimationBoneRoot(_scene->mRootNode, _boneMap);
					
					glm::mat4 _globalTransform(1.0f);
					aiNode *_node = _rootBone->mParent;
					while (_node != nullptr){
						_globalTransform *= Utilities::convertMat4(_node->mTransformation);
						_node = _node->mParent;
					}

					_meshData.m_skeleton->m_globalInverseTransform = _globalTransform;

					PRINT("======================= SCENE GRAPH =======================")
					printSceneTree(_scene->mRootNode);
					PRINT("===========================================================")

					/*Utilities::print(_globalTransform);
					Utilities::print(Utilities::convertMat4(_rootBone->mTransformation));
					PRINT("ROOT BONE: " << _rootBone->mName.data);*/

					//Create bone tree hierarchy
					loadAnimationBoneTree(_rootBone,
						_meshData.m_skeleton->m_animationBoneTreeRoot, *_meshData.m_skeleton);

					/*for (auto &_boneMap : _meshData.m_skeleton->m_boneMap){
						PRINT(_boneMap.first);

						Utilities::print(_meshData.m_skeleton->m_boneTransforms[_boneMap.second].m_offset);
					}

					PRINT("-----------INVERSE--------------");
					Utilities::print(_meshData.m_skeleton->m_globalInverseTransform);*/
				}
				
			}

			return _meshData;
		}

	}
}