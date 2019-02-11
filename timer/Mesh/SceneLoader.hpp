#ifndef SCENE_LOADER_HPP
#define SCENE_LOADER_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "../Animation/SkeletalAnimationAction.hpp"


struct aiNode;
struct aiMesh;
struct aiScene;
struct aiNodeAnim;
struct aiMaterial;
struct aiAnimation;

namespace Engine{ namespace Meshes{
	struct Mesh;
} }

namespace Engine{ namespace Animation{
	struct AnimationSkeleton;
	struct SkeletalAnimationTransform;
	struct SkeletalAnimationAction;
	struct AnimationBoneTree;
} }


namespace Engine{
	namespace Meshes{

		struct TextureData{
		public:
			std::string m_path = "";
			unsigned int m_type = 0;

		public:
			TextureData(const std::string &path, unsigned int type);

		};


		struct MeshPropertiesData{
		public:
			std::string m_name = "";
			std::shared_ptr<Mesh> m_mesh = nullptr;
			std::vector<TextureData> m_textureData;

		public:
			MeshPropertiesData(const std::string &name, std::shared_ptr<Mesh> mesh,
				const std::vector<TextureData> &textureData);

		};


		struct MeshData{
		public:
			std::string m_path = "";
			std::shared_ptr<Animation::AnimationSkeleton> m_skeleton = nullptr;		
			std::vector<MeshPropertiesData> m_meshes;

		public:
			const bool hasSkeleton() const;

			const bool isValid() const;

		};



		void printSceneTree(const aiNode *node, std::vector<std::string> strings = {});

		std::vector<TextureData> loadTextures(const aiMaterial* material);

		std::unique_ptr<Mesh> loadMesh(const aiMesh *mesh, const aiScene *scene);

		void loadBones(const aiMesh *mesh, Animation::AnimationSkeleton *skeleton);


		aiNode *findAnimationBoneRoot(const aiNode *rootNode,
			const std::unordered_map<std::string, unsigned int> &boneMap);

		void loadAnimationBoneTree(const aiNode *node, std::shared_ptr<Animation::AnimationBoneTree> boneNode,
			const Animation::AnimationSkeleton &skeleton, std::vector<std::string> strings = {});

		Engine::Animation::SkeletalAnimationAction loadAnimationAction(
			const std::unordered_map<std::string, unsigned int> &boneMap, 
			const aiAnimation &animation);

		void loadAnimationActions(const aiScene *scene, MeshData &meshData);

		void loadBoneTransforms(
			const std::unordered_map<std::string, unsigned int> &boneMap, 
			const aiAnimation *animation, 
			std::vector<Engine::Animation::SkeletalAnimationTransform> &transforms,
			float keyframeStartTime = 0);

		void loadSkeleton(const aiNode *node, const aiAnimation *animation);

		void recursion(const aiNode *node, const aiScene *scene, MeshData &meshData);

		MeshData loadSceneData(const std::string &path);

	}
}

#endif //SCENE_LOADER_HPP