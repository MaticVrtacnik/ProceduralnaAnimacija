#include "AnimationBoneTree.hpp"


namespace Engine{
	namespace Animation{

		AnimationBoneTree::AnimationBoneTree(){

		}

		AnimationBoneTree::AnimationBoneTree(
			const std::string &name, unsigned int boneId
		) :
			m_name(name), m_boneId(boneId)
		{
		}

		AnimationBoneTree::~AnimationBoneTree(){

		}

	}
}
