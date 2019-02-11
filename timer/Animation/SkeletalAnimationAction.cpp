#include "SkeletalAnimationAction.hpp"

//#include "SkeletalAnimationKeyframe.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Animation{

		/*
		std::vector<SkeletalAnimationKeyframe> loadSkeletalAnimationKeyframe(const std::string &path,
			std::unordered_map<std::string, unsigned int> &boneMap, float duration)
		{
		}//*/


		SkeletalAnimationAction::SkeletalAnimationAction(){

		}

		SkeletalAnimationAction::SkeletalAnimationAction(const std::string &name) :
			m_name(name)
		{
		}

		SkeletalAnimationAction::SkeletalAnimationAction(const std::string &name,
			std::shared_ptr<SkeletalAnimationKeyframe> keyframe1, float timeToNext1,
			std::shared_ptr<SkeletalAnimationKeyframe> keyframe2, float timeToNext2
		) :
			m_name(name)
		{
			addKeyframe(keyframe1, timeToNext1);
			addKeyframe(keyframe2, timeToNext2);
		}

		SkeletalAnimationAction::~SkeletalAnimationAction(){

		}


		void SkeletalAnimationAction::addKeyframe(
			std::shared_ptr<SkeletalAnimationKeyframe> keyframe, float duration)
		{
			keyframe->m_duration = duration;
			m_keyframeDurationPairs.emplace_back(keyframe, duration);
			m_totalDuration += duration;
		}

		void SkeletalAnimationAction::setKeyframeDuration(
			const std::string &keyframeName, float duration)
		{
			for (auto &_pair : m_keyframeDurationPairs){
				if (_pair.m_keyframe->m_name == keyframeName){
					float _diff = duration - _pair.m_duration;
					m_totalDuration += _diff;
					_pair.m_duration += _diff;
				}
			}
		}

		void SkeletalAnimationAction::setKeyframeDuration(std::size_t keyframeId, float duration){
			if (keyframeId >= m_keyframeDurationPairs.size()){
				ASSERT(0, "Cannot set keyframe time: keyframe ID out of range");
				return;
			}

			auto &_pair = m_keyframeDurationPairs[keyframeId];
			float _diff = duration - _pair.m_duration;
			m_totalDuration += _diff;
			_pair.m_duration += _diff;
	
			//_pair.m_keyframe->m_duration = _pair.m_duration;
		}

	}
}
