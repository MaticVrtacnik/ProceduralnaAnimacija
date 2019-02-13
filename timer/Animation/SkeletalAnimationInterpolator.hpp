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