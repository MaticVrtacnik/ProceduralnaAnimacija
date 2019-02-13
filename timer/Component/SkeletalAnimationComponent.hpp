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


#ifndef SKELETAL_ANIMATION_COMPONENT_HPP
#define SKELETAL_ANIMATION_COMPONENT_HPP

#include <memory>
#include <unordered_map>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

#include "../Component/Component.hpp"
#include "../Buffer/ArrayBuffer.hpp"



struct aiNode;
struct aiScene;
struct aiNodeAnim;
struct aiAnimation;

namespace Engine{ namespace Animation{ 
	struct BoneTree;
	struct AnimationSkeleton;
	struct SkeletalAnimationAction;
	struct SkeletalAnimationKeyframe;
	struct SkeletalAnimationTransform;

	class SkeletalAnimationPlayer;
} }


namespace Engine{
	namespace Entities{
		namespace Components{
			using namespace Animation;

			const float c_defaultTicksPerSecond = 60;

			enum AnimationType{
				ANIMATION_KEYFRAME,
				ANIMATION_MULTI_KEYFRAME,
				ANIMATION_CONTINUOUS,
				
				//TODO maybe remove (sliding door,...)
				ANIMATION_SINGLE_TRANSFORM
			};

			enum AnimationFlags{
				ANIMATION_FLAG_KEYFRAME = 0x01,
				ANIMATION_FLAG_SEPARATE_FROM_MESH = 0x02,
				ANIMATION_FLAG_LOOP = 0x04,
				ANIMATION_FLAG_RETURN_TO_START = 0x08
			};


			class SkeletalAnimationComponent : public Component{
			public:
				unsigned int m_offset = 0;

			private:	
				//Directory of the loaded animation data file (collada, fbx, ...)
				std::string m_fileDirectory = "";

				std::unique_ptr<SkeletalAnimationPlayer> m_animationPlayer = nullptr;
				std::unordered_map<std::string, std::shared_ptr<SkeletalAnimationKeyframe>> m_keyframes;
				std::unordered_map<std::string, SkeletalAnimationAction> m_animationActions;

				Buffers::ArrayBuffer m_boneBuffer;
				std::shared_ptr<AnimationSkeleton> m_skeleton = nullptr;
				

				std::vector<glm::mat4> m_transforms;
				std::vector<glm::mat4> m_offsetTransforms;

				//for motion blur
				std::vector<glm::mat4> m_previousOffsetTransforms;

				//Type
				int m_flags = 0;

			private:
				void changeOffsets(const aiNode *node, const aiScene *scene);

			private:
				glm::vec3 getInterpolatedLocation(float animationTime, const aiNodeAnim *animationNode);

				glm::quat getInterpolatedRotation(float animationTime, const aiNodeAnim *animationNode);

				glm::vec3 getInterpolatedScale(float animationTime, const aiNodeAnim *animationNode);

				int setChildTransforms(float animationTime, const aiNode *node, const aiAnimation *animation,
					const glm::mat4 &parentTransform, int depth = 0);

			private:
				bool loadBVHKeyframe(const std::string &path, const std::string &name = "");

				bool loadBVHAction(const std::string &path, const std::string &name = "",
					bool cancelable = false, float nextActionInterpolationDuration = 0.3f);

				bool loadAnimationActionConfig(const std::string &path);

				bool loadBVHAnimationActionConfig(const std::string &dir);

				bool hasKeyframe(const std::string &name);

				bool createAnimationAction(const std::string &actionName,
					const std::vector<std::pair<std::string, float>> &keyframeNamesTimes,
					bool cancelable = false, float nextActionInterpolationDuration = 0.3f);

			public:
				SkeletalAnimationComponent(AnimationSkeleton &skeleton);

				~SkeletalAnimationComponent();
			

				//loads initial keyframes from config file
				void loadBVHKeyframes(const std::string &path);

				void loadKeyframes(const std::string &path);

				void reloadAnimationActionConfig();


				const Buffers::ArrayBuffer &getBuffer() const;

				std::size_t getNumBones() const;

				AnimationSkeleton &getSkeleton() const;

				std::vector<glm::mat4> getDefaultBoneMatrices();

				
				void addAction(const std::string &name, const SkeletalAnimationAction &action);

				void playAction(const std::string &name = "");

				const std::string getPlayingAction() const;

				void updateBoneTransforms(double timeInSeconds);


				void updateOffsetTransforms();

				std::vector<glm::mat4> &getTransforms();

				const std::vector<glm::mat4> &getTransforms() const;

				void setTransforms(const std::vector<glm::mat4> &transforms);

				const std::vector<glm::mat4> &getOffsetTransforms() const;

				const std::vector<glm::mat4> &getPreviousOffsetTransforms() const;

				void setPreviousOffsetTransforms(const std::vector<glm::mat4> &transforms);

				unsigned int getBoneId(const std::string &boneAlias) const;


				void setKeyframeDuration(const std::string &actionName, 
					const std::string &keyframeName, float duration);


				SkeletalAnimationPlayer &getAnimationPlayer() const;


				void addFlags(int flags);

				void removeFlags(int flags);

				const bool isTypeLooping() const;


				public:
					SkeletalAnimationKeyframe *getKeyframe(const std::string &name);

					SkeletalAnimationAction *getAction(const std::string &name);

					void updateDurationsForAction(const std::string &actionName);

			};

		}
	}
}

#endif //SKELETAL_ANIMATION_COMPONENT_HPP