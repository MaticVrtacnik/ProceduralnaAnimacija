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


#include "SkeletalAnimationKeyframe.hpp"


#include "SkeletalAnimationTransform.hpp"
#include "AnimationBoneTree.hpp"
#include "AnimationSkeleton.hpp"
#include "../Define/PrintDefines.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/MathUtilities.hpp"


namespace Engine{
	namespace Animation{

		SkeletalAnimationKeyframe::SkeletalAnimationKeyframe(){

		}

		SkeletalAnimationKeyframe::SkeletalAnimationKeyframe(const SkeletalAnimationKeyframe &that):
			m_name(that.m_name), m_duration(that.m_duration), m_transforms(that.m_transforms)
		{

		}


		SkeletalAnimationKeyframe::SkeletalAnimationKeyframe(const std::string &name,
			const std::vector<SkeletalAnimationTransform> &transforms
		):
			m_name(name), m_transforms(transforms)
		{
		}

		SkeletalAnimationKeyframe::SkeletalAnimationKeyframe(const std::string &name,
			Animation::AnimationSkeleton &skeleton, const std::vector<glm::mat4> &transforms
		):
			m_name(name)
		{
			setKeyframeTransformsFromFinalTransforms(skeleton, transforms);
		}

		SkeletalAnimationKeyframe::~SkeletalAnimationKeyframe(){

		}


		bool SkeletalAnimationKeyframe::isValid() const{
			return !m_transforms.empty() && !m_name.empty() && m_duration > 0.0f;
		}

		void SkeletalAnimationKeyframe::getFinalTransforms(const AnimationBoneTree *node,
			std::vector<glm::mat4> *transforms, const glm::mat4 &parentTransform) const
		{
			if (node == nullptr || transforms->size() != m_transforms.size()){
				WARNING("Couldn't get keyframe transforms!");
				return;
			}

			auto _boneId = node->m_boneId;
			auto &_transform = m_transforms[_boneId];

			glm::mat4 _parentTransform = parentTransform * _transform.getTransform();
			(*transforms)[_boneId] = _parentTransform;


			for (auto &_child : node->m_children){
				getFinalTransforms(_child.get(), transforms, _parentTransform);
			}
		}


		
		void SkeletalAnimationKeyframe::inverseTransformRecursion(
			const AnimationBoneTree *node,
			Animation::AnimationSkeleton &skeleton,
			const std::vector<glm::mat4> &transforms, 
			const glm::mat4 &inverseParentTransform)
		{
			auto _boneId = node->m_boneId;
			glm::mat4 _thisTransform = inverseParentTransform * transforms[_boneId];
	
			auto &_transform = m_transforms[_boneId];
			_transform.m_boneId = _boneId;
			_transform.m_name = node->m_name;
			_transform.m_location = glm::vec3(_thisTransform[3]);
			_transform.m_rotation = glm::normalize(glm::quat_cast(glm::mat3(_thisTransform)));
			_transform.m_scale = Scale(1.0f); 

			glm::mat4 _inverseParentTransform = glm::inverse(_thisTransform) * inverseParentTransform;

			for (auto &_child : node->m_children){
				inverseTransformRecursion(_child.get(), skeleton, transforms, _inverseParentTransform);
			}
		}

		void SkeletalAnimationKeyframe::setKeyframeTransformsFromFinalTransforms(
			Animation::AnimationSkeleton &skeleton, const std::vector<glm::mat4> &transforms)
		{		
			m_transforms.clear();
			m_transforms.resize(transforms.size());

			auto _rootNode = skeleton.m_animationBoneTreeRoot.get();
			inverseTransformRecursion(_rootNode, skeleton, transforms, glm::mat4(1.0f));
		}

	}
}
