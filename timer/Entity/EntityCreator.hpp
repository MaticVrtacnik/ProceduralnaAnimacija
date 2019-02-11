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