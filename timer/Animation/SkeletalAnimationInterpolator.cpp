#include "SkeletalAnimationInterpolator.hpp"

#include "AnimationBoneTree.hpp"
#include "SkeletalAnimationTransform.hpp"


namespace Engine{
	namespace Animation{

		SkeletalAnimationInterpolator::SkeletalAnimationInterpolator(){

		}

		SkeletalAnimationInterpolator::SkeletalAnimationInterpolator(
			Utilities::InterpolationType interpolationType
		) :
			m_interpolationType(interpolationType)
		{
		}

		SkeletalAnimationInterpolator::~SkeletalAnimationInterpolator(){

		}


		void SkeletalAnimationInterpolator::interpolate(
			float factor,
			const SkeletalAnimationKeyframe &keyframe1,
			const SkeletalAnimationKeyframe &keyframe2,
			const AnimationBoneTree *node,
			const glm::mat4 &parentTransform)
		{
			unsigned int _boneId = node->m_boneId;
			const auto &_t1 = keyframe1.m_transforms[_boneId];
			const auto &_t2 = keyframe2.m_transforms[_boneId];

			auto &_tc = m_currentKeyframe.m_transforms[_boneId]; 
			_tc.m_boneId = _boneId;
			_tc.m_name = node->m_name;
			_tc.m_location = _t1.m_location + factor * (_t2.m_location - _t1.m_location);
			_tc.m_rotation = glm::normalize(glm::slerp(_t1.m_rotation, _t2.m_rotation, factor));
			_tc.m_scale = _t1.m_scale + factor * (_t2.m_scale - _t1.m_scale);


			glm::mat4 _parentTransform = parentTransform * _tc.getTransform();
			m_transforms[_boneId] = _parentTransform;

			for (auto &_child : node->m_children){
				interpolate(factor, keyframe1, keyframe2, _child.get(), _parentTransform);
			}
		}


		const SkeletalAnimationKeyframe &SkeletalAnimationInterpolator::getInterpolatedKeyframe(
			float linearFactor, 
			const SkeletalAnimationKeyframe &keyframe1,
			const SkeletalAnimationKeyframe &keyframe2,
			const AnimationBoneTree *animationBoneTreeRoot)
		{
			//Resize if current transforms empty or not same size as keyframes
			/*if (m_transforms.size() != keyframe2.m_transforms.size()){
				m_transforms.resize(keyframe2.m_transforms.size(), glm::mat4(1.0f));

				m_currentKeyframe = SkeletalAnimationKeyframe("current", keyframe2.m_transforms);
			}*/

			float _factor = linearFactor;
			switch (m_interpolationType)
			{
				case Utilities::INTERPOLATION_COS:
					_factor = Utilities::cosInterpolation(_factor, 0.0f, 1.0f);
					break;

				/*case Utilities::INTERPOLATION_QUADRIC: //TODO
					_factor = Utilities::cubicInterpolation(_factor, 0.0f, 1.0f);
					break;*/

				/*case Utilities::INTERPOLATION_CUBIC:
					_factor = Utilities::cubicInterpolation(_factor, 0.0f, 1.0f);
					break;*/

				case Utilities::INTERPOLATION_SPRING:
					_factor = Utilities::springInterpolation(_factor, 0.0f, 1.0f);
					break;

				default: //INTERPOLATION_LINEAR
					break; 
			}
				

			interpolate(_factor, keyframe1, keyframe2, animationBoneTreeRoot);

			return m_currentKeyframe;
		}

		const SkeletalAnimationKeyframe &SkeletalAnimationInterpolator::getInterpolatedKeyframe(
			float startTime, float endTime, float currentTime,
			const SkeletalAnimationKeyframe &keyframe1,
			const SkeletalAnimationKeyframe &keyframe2,
			const AnimationBoneTree *animationBoneTreeRoot)
		{
			float _timeFactor = Utilities::getTimeFactor(currentTime, startTime, endTime);
			float _keyframeTime = Utilities::linearInterpolation(_timeFactor, startTime, endTime);

			float _factor = endTime == startTime ? 1.0f :
				((_keyframeTime - startTime) / (endTime - startTime));

			return getInterpolatedKeyframe(_factor,
				keyframe1, keyframe2, animationBoneTreeRoot);
		}


		const SkeletalAnimationKeyframe &SkeletalAnimationInterpolator::getResult() const{
			return m_currentKeyframe;
		}

		const std::vector<glm::mat4> &SkeletalAnimationInterpolator::getTransforms() const{
			return m_transforms;
		}


		void SkeletalAnimationInterpolator::resizeKeyframe(unsigned int newSize){
			if (m_currentKeyframe.m_transforms.size() != newSize){
				m_transforms.resize(newSize, glm::mat4(1.0f));
				m_currentKeyframe = SkeletalAnimationKeyframe("current", 
					std::vector<SkeletalAnimationTransform>(newSize));
			}
		}

		void SkeletalAnimationInterpolator::resizeKeyframe(
			const std::vector<SkeletalAnimationTransform> &transforms)
		{
			if (m_currentKeyframe.m_transforms.size() != transforms.size()){
				m_transforms.resize(transforms.size(), glm::mat4(1.0f));
				m_currentKeyframe = SkeletalAnimationKeyframe("current", transforms);
			}			
		}


		Utilities::InterpolationType SkeletalAnimationInterpolator::getInterpolationType() const{
			return m_interpolationType;
		}

		void SkeletalAnimationInterpolator::setInterpolationType(
			Utilities::InterpolationType intepolationType)
		{
			m_interpolationType = intepolationType;
		}

	}
}
