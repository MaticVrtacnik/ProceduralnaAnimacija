#ifndef SKELETAL_ANIMATION_ACTION_HPP
#define SKELETAL_ANIMATION_ACTION_HPP

#include <memory>

//TODO placeholder -> add to translation unit
#include "SkeletalAnimationKeyframe.hpp"


namespace Engine{
	namespace Animation{

		class SkeletalAnimationPlayer;
		struct SkeletalAnimationAction{
		public:
			friend class SkeletalAnimationPlayer;

			struct KeyframeDurationPair{
			public:
				std::shared_ptr<SkeletalAnimationKeyframe> m_keyframe;
				float m_duration = 0.0f;

			public:
				KeyframeDurationPair(){

				}

				KeyframeDurationPair(
					std::shared_ptr<SkeletalAnimationKeyframe> keyframe, 
					float duration
				) :
					m_keyframe(keyframe), m_duration(duration)
				{
				}

			};

		private:
			std::vector<KeyframeDurationPair> m_keyframeDurationPairs;
			//std::shared_ptr<BoneTree> m_boneTreeRoot = nullptr;

			//TODO remove or maybe use
			bool m_canPlay = true;
			bool m_beingPlayed = false;

		public:
			std::string m_name = "";

			bool m_cancelable = true;
			float m_nextActionInterpolationDuration = 0.3f;
			float m_totalDuration = 0.0f;

		public:
			SkeletalAnimationAction();

			SkeletalAnimationAction(const std::string &name);

			SkeletalAnimationAction(const std::string &name,
				std::shared_ptr<SkeletalAnimationKeyframe> keyframe1, float timeToNext1,
				std::shared_ptr<SkeletalAnimationKeyframe> keyframe2, float timeToNext2);

			~SkeletalAnimationAction();


			//Set time to next in case keyframes in different actions have...
			//different duration times
			void addKeyframe(
				std::shared_ptr<SkeletalAnimationKeyframe> keyframe, 
				float duration
			);

			void setKeyframeDuration(const std::string &keyframeName, float duration);

			void setKeyframeDuration(unsigned int keyframeId, float duration);

		};

	}
}

#endif //SKELETAL_ANIMATION_ACTION_HPP