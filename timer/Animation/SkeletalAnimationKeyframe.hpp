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