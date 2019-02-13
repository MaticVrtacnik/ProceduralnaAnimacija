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

#ifndef TRANSFORM_VARIABLES_HPP
#define TRANSFORM_VARIABLES_HPP

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Engine{

	enum Axis{
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_Z = 2
	};


	struct Location : glm::vec3
	{
		Location() : glm::vec3(0.0){};

		Location(float x) : glm::vec3(x){};

		Location(float x, float y, float z) : glm::vec3(x, y, z){};

		Location(const glm::vec3 &vec) : glm::vec3(vec){};

		Location(glm::vec3 &&vec) : glm::vec3(std::move(vec)){};

	};


	struct Rotation : glm::quat{
	private:
		void normalize() {
			float d = glm::sqrt(x * x + y * y + z * z + w * w);
			if (d != 0.0f){
				x /= d;
				y /= d;
				z /= d;
				w /= d;
			}
		}

	public:
		Rotation() : glm::quat(glm::vec3(0.0)){ };

		Rotation(float angles) : glm::quat(glm::vec3(glm::radians(angles))){ normalize(); };

		Rotation(const glm::vec3 &angles) : glm::quat(glm::radians(angles)){ normalize(); };

		Rotation(glm::vec3 &&angles) : glm::quat(glm::radians(std::move(angles))){ normalize(); };

		Rotation(float x, float y, float z) : glm::quat(glm::radians(glm::vec3(x, y, z))){ normalize(); };

		Rotation(float x, float y, float z, float w) : glm::quat(w, x, y, z){ normalize(); };

		Rotation(float angle, const glm::vec3 &vec) : glm::quat(glm::radians(angle), vec){ normalize(); };

		Rotation(float angle, glm::vec3 &&vec) : glm::quat(glm::radians(angle), std::move(vec)){ normalize(); };

		Rotation(Axis axis, float angle) : Rotation(axis == AXIS_X ? angle : 0.0f, 
			axis == AXIS_Y ? angle : 0.0f, axis == AXIS_Z ? angle : 0.0f)
		{
			normalize();
		};

		Rotation(const glm::quat &quat) : glm::quat(quat){ normalize(); };

		Rotation(glm::quat &&quat) : glm::quat(std::move(quat)){ normalize(); };

	};


	struct Scale : glm::vec3
	{
		Scale() : glm::vec3(1.0){};

		Scale(float x) : glm::vec3(x){};

		Scale(float x, float y, float z) : glm::vec3(x, y, z){};

		Scale(const glm::vec3 &vec) : glm::vec3(vec){};

		Scale(glm::vec3 &&vec) : glm::vec3(std::move(vec)){};

	};

}

#endif //TRANSFORM_VARIABLES_HPP