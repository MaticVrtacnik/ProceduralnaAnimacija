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


#ifndef SKELETAL_ANIMATION_KEYFRAME_HPP
#define SKELETAL_ANIMATION_KEYFRAME_HPP

#include <vector>

#include "SkeletalAnimationTransform.hpp"


namespace Engine { namespace Animation{
	//struct SkeletalAnimationTransform;
	struct AnimationBoneTree;
	struct AnimationSkeleton;
} }


namespace Engine{
	namespace Animation{

		struct SkeletalAnimationKeyframe{
		public:
			std::string m_name = "";
			float m_duration = 0.0f; //TODO maybe remove (duplicate keyframes)
			std::vector<SkeletalAnimationTransform> m_transforms;

		private:
			void inverseTransformRecursion(
				const AnimationBoneTree *node, 
				Animation::AnimationSkeleton &skeleton,
				const std::vector<glm::mat4> &transforms, 
				const glm::mat4 &inverseParentTransform = glm::mat4(1.0f)
			);

		public:
			SkeletalAnimationKeyframe();

			SkeletalAnimationKeyframe(const SkeletalAnimationKeyframe &that);

			SkeletalAnimationKeyframe(const std::string &name,
				const std::vector<SkeletalAnimationTransform> &transforms);

			SkeletalAnimationKeyframe(const std::string &name, Animation::AnimationSkeleton &skeleton,
				const std::vector<glm::mat4> &transforms);

			~SkeletalAnimationKeyframe();


			bool isValid() const;

			void getFinalTransforms(const AnimationBoneTree *node, std::vector<glm::mat4> *transforms,
				const glm::mat4 &parentTransform = glm::mat4(1.0f))const;

			void setKeyframeTransformsFromFinalTransforms(Animation::AnimationSkeleton &skeleton,
				const std::vector<glm::mat4> &transforms);

		};

	}
}

#endif //SKELETAL_ANIMATION_KEYFRAME_HPP