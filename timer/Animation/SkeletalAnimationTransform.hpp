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