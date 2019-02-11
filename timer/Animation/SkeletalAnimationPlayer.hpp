#ifndef SKELETAL_ANIMATION_PLAYER
#define SKELETAL_ANIMATION_PLAYER

#include <memory>

#include "AnimationSkeleton.hpp"
#include "SkeletalAnimationInterpolator.hpp"


struct aiNode;
struct aiScene;


namespace Engine{
	namespace Animation{

		class SkeletalAnimationPlayer{
		private:
			struct TimelineData{
			public:
				std::string m_actionName = "";
				SkeletalAnimationKeyframe m_keyframe;
				bool m_cancelable = true;
				float m_duration = 0.0f;
				int m_numActionKeyframes = 1;
				int m_keyframeId = 0;
				//double m_startTimeInSeconds = 0.0;
				//double m_endTimeInSeconds = 0.0;

			public:
				TimelineData(){

				}

				TimelineData(const std::string &actionName,
					SkeletalAnimationKeyframe keyframe,
					bool cancelable, float duration,
					int numActionKeyframes, int keyframeId
				) :
					m_actionName(actionName),
					m_keyframe(keyframe),
					m_cancelable(cancelable),
					m_duration(duration),
					m_numActionKeyframes(numActionKeyframes),
					m_keyframeId(keyframeId)
				{
				}

				/*TimelineData(const std::string &actionName, SkeletalAnimationKeyframe* keyframe,
				bool cancelable, float startTimeInSeconds, float endTimeInSeconds
				) :
				m_actionName(actionName),
				m_keyframe(keyframe),
				m_cancelable(cancelable),
				m_startTimeInSeconds(startTimeInSeconds),
				m_endTimeInSeconds(endTimeInSeconds)
				{
				}*/

			};

		private:
			SkeletalAnimationInterpolator m_interpolator;

			std::vector<TimelineData> m_timeline;
			float m_timelineDuration = 0.0f;
			bool m_playingAnimation = false;
			bool m_finishedPlayingCurrentAction = false;
			bool m_justFinishedPlayingKeyframe = false;
			double m_playTimer = 0.0;

			float m_interpolationFactor = 0.0f;

		private:
			void updateTimelineDuration();



		public:
			SkeletalAnimationPlayer();

			~SkeletalAnimationPlayer();


			float getInterpolationFactor() const;

			const bool justFinishedPlayingAction() const;

			const bool justFinishedPlayingKeyframe() const;

			const bool isPlaying() const;

			void playAnimation();

			void clearTimelineToLastKeyframe();

	
			void clearTimeline();

			void playAction(SkeletalAnimationAction &animationAction);

			std::vector<glm::mat4> getInterpolatedTransforms(
				AnimationBoneTree *animationBoneTreeRoot, double timeInSeconds);


			const SkeletalAnimationKeyframe &getCurrentKeyframe() const;

			const std::string getPlayingAction() const;

			void updateDurationsForAction(
				const SkeletalAnimationAction &animationAction);

			void updateNextKeyframeFromFinalTransforms(
				Animation::AnimationSkeleton &skeleton, const std::vector<glm::mat4> &transforms);

			void updateCurrentKeyframeFromFinalTransforms(
				Animation::AnimationSkeleton &skeleton, const std::vector<glm::mat4> &transforms);


			void updateNextKeyframe(const std::string &name, const SkeletalAnimationKeyframe &keyframe);

			void updateAllKeyframes(const std::string &name, const SkeletalAnimationKeyframe &keyframe);

			const SkeletalAnimationKeyframe &getNextKeyframe() const;

		};




		int getNumBones(const aiNode *node, const aiScene *scene);

		/*void loadTransforms(
		const aiNode *node,
		const aiAnimation *animation,
		const glm::mat4 &parentTransform,
		std::vector<glm::mat4> &transforms,
		std::unordered_map<std::string, unsigned int> &boneMap
		);*/

		/*std::vector<SkeletalAnimationKeyframe> loadSkeletalAnimationKeyframe(
		const std::string &path,
		std::unordered_map<std::string, unsigned int> &boneMap,
		float duration = 1.0f
		);*/

		/*std::unique_ptr<Animation::SkeletalAnimationAction> loadSkeletonAnimatedAction(
		const std::string &path,
		std::unordered_map<std::string, unsigned int> &boneMap,
		float duration = 1.0f
		);*/

	}
}

#endif //SKELETAL_ANIMATION_PLAYER