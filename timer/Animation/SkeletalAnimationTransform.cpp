#include "SkeletalAnimationTransform.hpp"

#include <glm\gtx\transform.hpp>


namespace Engine{
	namespace Animation{

		SkeletalAnimationTransform::SkeletalAnimationTransform(){

		}

		SkeletalAnimationTransform::SkeletalAnimationTransform(
			const Location &location, const Rotation &rotation, const Scale &scale
		) :
			m_location(location), m_rotation(rotation), m_scale(scale)
		{
		}

		SkeletalAnimationTransform::SkeletalAnimationTransform(const std::string &name, 
			const Location &location, const Rotation &rotation, const Scale &scale
		) :
			SkeletalAnimationTransform(location, rotation, scale)
		{
			m_name = name;
		}

		SkeletalAnimationTransform::~SkeletalAnimationTransform(){

		}


		const glm::mat4 SkeletalAnimationTransform::getTransform() const{
			return glm::translate(m_location) * glm::toMat4(m_rotation) * glm::scale(m_scale);
		}

	}
}
