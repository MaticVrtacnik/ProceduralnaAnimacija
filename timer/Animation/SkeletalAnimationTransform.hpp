#ifndef SKELETAL_ANIMATION_TRANSFORM_HPP
#define SKELETAL_ANIMATION_TRANSFORM_HPP

#include <string>

#include "../TransformVariables.hpp"


namespace Engine{
	namespace Animation{

		struct SkeletalAnimationTransform{
		public:
			std::string m_name = "";
			unsigned int m_boneId = 0;

			Location m_location = Location(0.0f);
			Rotation m_rotation = Rotation(0.0f);
			Scale m_scale = Scale(1.0f);

		public:
			SkeletalAnimationTransform();

			SkeletalAnimationTransform(
				const Location &location,
				const Rotation &rotation = Rotation(0.0f),
				const Scale &scale = Scale(1.0f)
			);

			SkeletalAnimationTransform(
				const std::string &name,
				const Location &location = Location(0.0f),
				const Rotation &rotation = Rotation(0.0f),
				const Scale &scale = Scale(1.0f)
			);

			~SkeletalAnimationTransform();


			const glm::mat4 getTransform() const;

		};

	}
}

#endif //SKELETAL_ANIMATION_TRANSFORM_HPP