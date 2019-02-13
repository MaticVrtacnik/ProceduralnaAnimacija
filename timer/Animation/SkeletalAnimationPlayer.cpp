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


#include "SkeletalAnimationPlayer.hpp"

#include <SDL2\SDL_timer.h>
#include <assimp/scene.h>
#include <assimp\Importer.hpp>
#include <unordered_map>

#include "AnimationSkeleton.hpp"
#include "SkeletalAnimationAction.hpp"

#include "../Define/PrintDefines.hpp"
#include "../Utility/AnimationUtilities.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/InterpolationUtilities.hpp"


namespace Engine{
	namespace Animation{

		SkeletalAnimationPlayer::SkeletalAnimationPlayer() :
			m_interpolator(Utilities::INTERPOLATION_COS)
		{
			clearTimeline();
		}

		SkeletalAnimationPlayer::~SkeletalAnimationPlayer(){

		}


		void SkeletalAnimationPlayer::clearTimeline(){
			m_timeline.clear();
			//m_timeline.resize(1);

			/*float _currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
			m_animationTimeline.emplace_back("", &m_currentKeyframe, false,
			_currentTime, _currentTime + 0.3f);*/
		}




		void SkeletalAnimationPlayer::playAction(SkeletalAnimationAction &animationAction){
			//Resize current transforms if they are still empty to fit skeleton bone count
			if (m_interpolator.getTransforms().empty() &&
				!animationAction.m_keyframeDurationPairs.empty())
			{
				const auto &_firstPair = animationAction.m_keyframeDurationPairs[0];
				m_interpolator.resizeKeyframe(_firstPair.m_keyframe->m_transforms);

				PRINT("PLAYED NEW ACTION RESIZE");
			}


			int _endOfCurrentAction = 0;
			std::string _firstActionName = m_timeline.empty() ? "" : m_timeline[0].m_actionName;

			for (std::size_t i = 1; i < m_timeline.size(); ++i){
				if (m_timeline[i].m_actionName == _firstActionName){
					++_endOfCurrentAction;
				} else break;
			}//++_endOfCurrentAction;


			//Remove cancelable animation action keyframes
			int _afterCancelableId = 0;
			for (int i = _endOfCurrentAction; i > 0; --i){
				if (!m_timeline[i].m_cancelable){

					//Do not break => find non cancelable and override
					//TODO check if works correctly
					//Allows crounching when walking so below...
					//if statemet timeline.size() > 2 works correctly
					_afterCancelableId = i;
					//break;
				}
			}

			if (animationAction.m_name == _firstActionName){
				_afterCancelableId = _endOfCurrentAction; // +1;

				//TODO prevent repeating animation
				//TODO prevents crouching when walking
				if (m_timeline.size() > 2){
					return;
				}
			}


			//TODO figure out why not working (adding 1 frame too much?)
			int _repeatFirst = 0;
			int _numActionKeyframes = animationAction.m_keyframeDurationPairs.size() +
				(_repeatFirst ? 1 : 0);


			//If all keyframes were cancelable interpolate from interpolated keyframe
			if (_afterCancelableId == 0){
				_afterCancelableId = 1;

				double _currentTime = static_cast<double>(SDL_GetTicks()) / 1000.0;
				float _duration = animationAction.m_nextActionInterpolationDuration;

				//PRINT("STARTED PLAYING ANIMATION");

				m_timeline.clear();
				m_timeline.emplace_back(animationAction.m_name, 
					SkeletalAnimationKeyframe(m_interpolator.getResult()),
					animationAction.m_cancelable, _duration, _numActionKeyframes, 0);

				//PRINT("SET PREVIOUS AS FIRST");
				m_playingAnimation = false;
			}


			auto &_newKeyframes = animationAction.m_keyframeDurationPairs;
			m_timeline.reserve(_afterCancelableId + _newKeyframes.size() + _repeatFirst);

			for (int i = 0; i < _newKeyframes.size(); ++i){
				m_timeline.emplace_back(animationAction.m_name,
					*_newKeyframes[i].m_keyframe, animationAction.m_cancelable,
					_newKeyframes[i].m_duration, _numActionKeyframes, i);
			}

			//Add first keyframe to the end of the timeline to return to initial positon
			if (_repeatFirst){
				m_timeline.emplace_back(animationAction.m_name, *_newKeyframes[0].m_keyframe,
					animationAction.m_cancelable, _newKeyframes[0].m_duration,
					_numActionKeyframes, _numActionKeyframes - 1);
			}

			updateTimelineDuration();
			playAnimation();

			//PRINT("PLAY ACTION: " << animationAction.m_name << " " << m_timeline.size());
		}


		const bool SkeletalAnimationPlayer::justFinishedPlayingAction() const{
			return m_finishedPlayingCurrentAction;
		}

		const bool SkeletalAnimationPlayer::justFinishedPlayingKeyframe() const{
			return m_justFinishedPlayingKeyframe;
		}

		const bool SkeletalAnimationPlayer::isPlaying() const{
			return m_playingAnimation;
		}

		void SkeletalAnimationPlayer::playAnimation(){
			if (!m_playingAnimation){
				m_playTimer = static_cast<double>(SDL_GetTicks()) / 1000.0;
				m_playingAnimation = true;
			}
		}



		void SkeletalAnimationPlayer::updateTimelineDuration(){
			m_timelineDuration = 0.0f;

			for (auto &_keyframeData : m_timeline){
				m_timelineDuration += _keyframeData.m_keyframe.m_duration;
			}
		}


		std::vector<glm::mat4> SkeletalAnimationPlayer::getInterpolatedTransforms(
			AnimationBoneTree *animationBoneTreeRoot, double timeInSeconds)
		{
			m_finishedPlayingCurrentAction = false;

			// || m_animationTimeline[0].m_keyframe == &m_currentKeyframe
			if (m_timeline.empty() || !m_playingAnimation){
				m_playingAnimation = false;

				return m_interpolator.getTransforms();
			}

			//TODO maybe add back
			/*if (m_timeline.size() < 2){ //m_animationTimeline.empty()
			m_playingAnimation = false;

			return m_finalTransforms;
			}*/


			double _currentTime = static_cast<double>(SDL_GetTicks()) / 1000.0;
			float _keyframeTime = static_cast<float>(_currentTime - m_playTimer);
			_keyframeTime *= 1.0f;

			auto &_currentKeyframeData = m_timeline[0];
			auto &_nextKeyframeData = m_timeline[1];

			/*PRINT(_currentKeyframeData.m_actionName << " " << 
				_currentKeyframeData.m_keyframe->m_name);*/

			float _currentDuration = _currentKeyframeData.m_duration;
			//PRINT(_nextKeyframeData.m_keyframe->m_duration);
			if (_keyframeTime < _currentDuration){
				//If we're not currently switching to the next keyframe we...
				//can be sure we aren't just finishing an animation action
				//m_finishedPlayingCurrentAction = false;
			}

			//Current keyframe has finished playing
			//Find the next keyframe that fits current time

			m_justFinishedPlayingKeyframe = (_keyframeTime >= _currentDuration) && !m_timeline.empty();
			while (_keyframeTime >= _currentDuration && !m_timeline.empty()){
				/*if (_currentKeyframeData.m_keyframeId ==
					_currentKeyframeData.m_numActionKeyframes - 4)
				{
					//Set to true when finishing playing... 
					//last keyframe of an animation action
					m_finishedPlayingCurrentAction = true;
				}*/

				float _nextFrameTime = _keyframeTime - _currentDuration;
				_keyframeTime = _nextFrameTime;
				m_playTimer = _currentTime - _nextFrameTime;

				//TODO make faster by erasing entire action
				m_timeline.erase(m_timeline.begin());

				_currentKeyframeData = m_timeline[0];

				if (m_timeline.size() == 1){
					m_playingAnimation = false;
					_currentKeyframeData = _nextKeyframeData = m_timeline[0];
					m_timeline.erase(m_timeline.begin());
					_keyframeTime = 0.0f;

					return m_interpolator.getTransforms();

				} else{
					_currentKeyframeData = m_timeline[0];
					_nextKeyframeData = m_timeline[1];
				}

				_currentDuration = _currentKeyframeData.m_duration;
			}


			float _startTime = 0.0f;
			float _endTime = _startTime + _currentDuration;
			bool _lastKeyframe = m_timeline.size() <= 2;

			float _timeFactor = Utilities::getTimeFactor(_keyframeTime, _startTime, _endTime);
			_keyframeTime = Utilities::linearInterpolation(_timeFactor, _startTime, _endTime);

			float _factor = _endTime == _startTime ? 1.0f :
				((_keyframeTime - _startTime) / (_endTime - _startTime));
			m_interpolationFactor = _factor;

			//_factor = cubicInterpolation(_factor, 0.0f, 1.0f);

			//TODO specify animation interpolation in config file
			if (_lastKeyframe){
				//m_interpolator.setInterpolationType(Utilities::INTERPOLATION_SPRING);
			} else{
				m_interpolator.setInterpolationType(Utilities::INTERPOLATION_COS);
			}


			m_interpolator.getInterpolatedKeyframe(_factor,
				_currentKeyframeData.m_keyframe, _nextKeyframeData.m_keyframe,
				animationBoneTreeRoot);


			return m_interpolator.getTransforms();
		}


		const SkeletalAnimationKeyframe &SkeletalAnimationPlayer::getCurrentKeyframe() const{
			return m_interpolator.getResult();
		}

		const SkeletalAnimationKeyframe &SkeletalAnimationPlayer::getNextKeyframe() const{
			if (m_timeline.size() >= 2){
				return m_timeline[1].m_keyframe;
			} return getCurrentKeyframe();
		}



		float SkeletalAnimationPlayer::getInterpolationFactor() const{
			return m_interpolationFactor;
		}

		const std::string SkeletalAnimationPlayer::getPlayingAction() const{
			return m_timeline.empty() ? "" : m_timeline[0].m_actionName;
		}	


		void SkeletalAnimationPlayer::updateDurationsForAction(
			const SkeletalAnimationAction &animationAction)
		{
			for (auto &_timelineData : m_timeline){
				if (_timelineData.m_actionName == animationAction.m_name){
					_timelineData.m_duration = animationAction.m_keyframeDurationPairs
						[_timelineData.m_keyframeId].m_duration;
					//PRINT("UPDATED KEYFRAME: " << _timelineData.m_duration);
				}
			}
		}


		void SkeletalAnimationPlayer::clearTimelineToLastKeyframe(){
			m_timeline.resize(2);
		}


		void SkeletalAnimationPlayer::updateNextKeyframeFromFinalTransforms(
			Animation::AnimationSkeleton &skeleton, const std::vector<glm::mat4> &transforms)
		{
			if (m_timeline.size() >= 3){
				m_timeline[2].m_keyframe.setKeyframeTransformsFromFinalTransforms(skeleton, transforms);
			}
		}

		void SkeletalAnimationPlayer::updateCurrentKeyframeFromFinalTransforms(
			Animation::AnimationSkeleton &skeleton, const std::vector<glm::mat4> &transforms)
		{
			if (m_timeline.size() >= 2){
				m_timeline[1].m_keyframe.setKeyframeTransformsFromFinalTransforms(skeleton, transforms);
			}
		}


		void SkeletalAnimationPlayer::updateNextKeyframe(
			const std::string &name, const SkeletalAnimationKeyframe &keyframe)
		{
			for (auto &_timelineData : m_timeline){
				if (_timelineData.m_keyframe.m_name == name){
					_timelineData.m_keyframe = keyframe;
					_timelineData.m_keyframe.m_name = name;
					break;
				}
			}
		}

		void SkeletalAnimationPlayer::updateAllKeyframes(
			const std::string &name, const SkeletalAnimationKeyframe &keyframe)
		{
			for (auto &_timelineData : m_timeline){
				if (_timelineData.m_keyframe.m_name == name){
					_timelineData.m_keyframe = keyframe;
					_timelineData.m_keyframe.m_name = name;
				}
			}
		}



		/*void SkeletalAnimationPlayer::clearTimeline(){
			m_timeline.resize(1);
		}*/



		int getNumBones(const aiNode *node, const aiScene *scene){
			int _numBones = 0;

			for (std::size_t i = 0; i < node->mNumMeshes; ++i){
				const aiMesh *_mesh = scene->mMeshes[node->mMeshes[i]];
				if (_mesh->HasBones()) _numBones += _mesh->mNumBones;
			}

			for (std::size_t i = 0; i < node->mNumChildren; ++i){
				_numBones += getNumBones(node->mChildren[i], scene);
			}

			return _numBones;
		}

		void recursion(const aiNode *node, const aiScene *scene,
			std::unordered_map<std::string, glm::mat4> &offsets)
		{
			for (std::size_t i = 0; i < node->mNumMeshes; ++i){
				const aiMesh *_mesh = scene->mMeshes[node->mMeshes[i]];

				for (std::size_t i = 0; i < _mesh->mNumBones; ++i){
					auto _bone = _mesh->mBones[i];
					std::string _boneName(_bone->mName.data);

					if (offsets.find(_boneName) == offsets.end()){
						offsets[_boneName] = Utilities::convertMat4(_bone->mOffsetMatrix);
					}
				}
			}

			for (std::size_t i = 0; i < node->mNumChildren; ++i){
				recursion(node->mChildren[i], scene, offsets);
			}
		}

		std::vector<glm::mat4> getOffsets(const aiScene *scene,
			std::unordered_map<std::string, unsigned int> &boneMap)
		{
			std::unordered_map<std::string, glm::mat4> _offsets;

			recursion(scene->mRootNode, scene, _offsets);

			std::vector<glm::mat4> _finalOffsets(boneMap.size(), glm::mat4(1.0f));

			for (auto _offset : _offsets){
				_finalOffsets[boneMap[_offset.first]] = _offset.second;
				//Utilities::print(_offset.second);
			}

			return _finalOffsets;
		}

	}
}
