#ifndef SKELETAL_ANIMATION_INTERPOLATOR_HPP
#define SKELETAL_ANIMATION_INTERPOLATOR_HPP

#include <glm\glm.hpp>

#include "SkeletalAnimationKeyframe.hpp"
#include "../Utility/InterpolationUtilities.hpp"


namespace Engine{ namespace Animation{
	struct AnimationBoneTree;
} }


namespace Engine{
	namespace Animation{


		class SkeletalAnimationInterpolator{
		private:
			Utilities::InterpolationType m_interpolationType = Utilities::INTERPOLATION_LINEAR;

		private:
			//Current keyframe interpolated position between 2 keyframes
			SkeletalAnimationKeyframe m_currentKeyframe;
			std::vector<glm::mat4> m_transforms;

		private:
			void interpolate(float factor,
				const SkeletalAnimationKeyframe &keyframe1,
				const SkeletalAnimationKeyframe &keyframe2,
				const AnimationBoneTree *node,
				const glm::mat4 &parentTransform = glm::mat4(1.0f));

		public:
			SkeletalAnimationInterpolator();

			SkeletalAnimationInterpolator(Utilities::InterpolationType interpolationType);

			~SkeletalAnimationInterpolator();


			const SkeletalAnimationKeyframe &getInterpolatedKeyframe(
				float linearFactor,
				const SkeletalAnimationKeyframe &keyframe1,
				const SkeletalAnimationKeyframe &keyframe2,
				const AnimationBoneTree *animationBoneTreeRoot
			);

			const SkeletalAnimationKeyframe &getInterpolatedKeyframe(
				float startTime, float endTime, float currentTime,
				const SkeletalAnimationKeyframe &keyframe1,
				const SkeletalAnimationKeyframe &keyframe2,
				const AnimationBoneTree *animationBoneTreeRoot
			);


			const SkeletalAnimationKeyframe &getResult() const;

			const std::vector<glm::mat4> &getTransforms() const;

			void resizeKeyframe(unsigned int newSize);

			void resizeKeyframe(const std::vector<SkeletalAnimationTransform> &transforms);


			Utilities::InterpolationType getInterpolationType() const;

			void setInterpolationType(Utilities::InterpolationType intepolationType);

		};

	}
}

#endif //SKELETAL_ANIMATION_INTERPOLATOR_HPP