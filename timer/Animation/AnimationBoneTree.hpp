#ifndef ANIMATION_BONE_TREE_HPP
#define ANIMATION_BONE_TREE_HPP

#include <string>
#include <vector>
#include <memory>


namespace Engine{
	namespace Animation{

		struct AnimationBoneTree{
		public:
			std::string m_name = "";
			unsigned int m_boneId = 0;
			bool m_marked = false;

			std::shared_ptr<AnimationBoneTree> m_parent = nullptr;
			std::vector<std::shared_ptr<AnimationBoneTree>> m_children;

		public:
			AnimationBoneTree();

			AnimationBoneTree(const std::string &name, unsigned int boneId);

			~AnimationBoneTree();

		};

	}
}

#endif //ANIMATION_BONE_TREE_HPP