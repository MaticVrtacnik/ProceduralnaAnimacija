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


#ifndef ENTITY_CREATOR_HPP
#define ENTITY_CREATOR_HPP

#include <vector>

#include "../TransformVariables.hpp"


namespace Engine{ namespace Entities{
	class World;
	class Entity;
} }

namespace Engine{ namespace Physics{
	enum CollisionMeshType;
} }


namespace Engine{
	namespace Entities{

		//TODO maybe move to resouce manager?
		std::vector<Entity> createEntitiesFromMeshFile(
			World &world,
			const std::string &path,
			const Physics::CollisionMeshType &collisionType,
			const Location &location = Location(0.0f),
			const Rotation &rotation = Rotation(0.0f),
			const Scale &scale = Scale(1.0f),
			const std::string &name = ""
		);

		Entity createDecalEntity(
			World &world,
			const glm::vec3 &color,
			const Location &location = Location(0.0f),
			const Rotation &rotation = Rotation(0.0f),
			const Scale &scale = Scale(1.0f)
		);

	}
}

#endif //ENTITY_CREATOR_HPP