#include "EntityCreator.hpp"

#include <SDL2\SDL_timer.h>

#include "../Animation/AnimationSkeleton.hpp"
#include "../TransformVariables.hpp"
#include "../World/World.hpp"
#include "../Texture/Material.hpp"
#include "../Collision/Collision.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Component/ComponentIncluder.hpp"


namespace Engine{
	namespace Entities{

		std::vector<Entity> createEntitiesFromMeshFile(World &world, 
			const std::string &path, const Physics::CollisionMeshType &collisionType,
			const Location &location, const Rotation &rotation, const Scale &scale,
			const std::string &name)
		{
			auto &_resourceManager = world.getResourceManager();
			auto &_collision = world.getVariables().m_collision;

			auto _sceneMeshMap = _resourceManager.addSceneMeshes(path);

			std::vector<Entity> _entities;
			_entities.reserve(_sceneMeshMap.size());

			Uint32 _timerStart = SDL_GetTicks();
			std::size_t _numTriangles = 0;

			//std::string::size_type _dotId = path.find('.');
			//std::string _entityName = _dotId == std::string::npos ? path : path.substr(0, _dotId);
			//PRINT(_entityName);

			for (auto &_nameMeshesPair : _sceneMeshMap){
				std::string _entityName = name.empty() ? _nameMeshesPair.first : name;
				//PRINT("NAME: " << _entityName);

				for (auto &_mesh : _nameMeshesPair.second){
					Entity _entity = world.addEntity(_entityName);

					Location _correctedLocation(_mesh->m_correctedLocation * scale + location);
					auto &_meshComponent = _entity.addComponent<MeshComponent>(*_mesh);
					auto &_matrixComponent = _entity.addComponent<MatrixComponent>(
						_correctedLocation, rotation, scale);
					auto &_materialComponent = _entity.addComponent<MaterialComponent>(
						_resourceManager.getMaterialPtr(_mesh->m_material));

					if (_resourceManager.hasSkeleton(path)){
						auto _skeletonPtr = _resourceManager.getSkeletonPtr(path);
						auto &_sac = _entity.addComponent<SkeletalAnimationComponent>(*_skeletonPtr);
						_sac.loadBVHKeyframes(path);

						//Naming action here doesn't matter (if only one is pressent)
						if (_entityName != "ragdoll"){
							//PRINT("NUM ANIMATIONS: " << _skeletonPtr->m_animationActions.size());

							_sac.addAction("action", (*_skeletonPtr->m_animationActions.begin()).second);
							//_sac.addFlags(ANIMATION_FLAG_LOOP);
						}
						//PRINT("SKELETON ENTTIY: " << _entityName)

						_materialComponent.setMaterial(_resourceManager.getMaterialPtr("paper"));
						//_sac.getBoneMatrices(0.0f); //TODO update transform matrices
					}

					//PRINT(path << "- HAS SKELETON: " << _resourceManager.hasSkeleton(path));

					//Ragdoll
					if (_resourceManager.hasSkeleton(path) /*&& _entityName == "ragdoll"*/ /*TODO REMOVE*/){
						PRINT("SKELETON LOADED");

						auto _skeletonPtr = _resourceManager.getSkeletonPtr(path);
						auto &_collisionComponent = _entity.addComponent<CollisionComponent>(_collision, 
							_entity.getId(), *_mesh, _skeletonPtr, _correctedLocation, scale, 1.0f, collisionType, true);
						
						_collisionComponent.setCollisionFilters(
							Physics::GROUP_RAGDOLL, 
							Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC | Physics::GROUP_DYNAMIC_RAGDOLL /*| Physics::GROUP_RAGDOLL*/
							//Physics::GROUP_ALL & ~(Physics::GROUP_EQUIPABLE)
						);



						std::vector<std::string> _collidable = { "L_FOOT", "R_FOOT", "R_L_LEG", "L_L_LEG" , "L_U_LEG", "R_U_LEG"};

						auto &_bodies = _collisionComponent.getBodies();
						auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
						auto &_skeleton = _animationComponent.getSkeleton();

						/*for (auto &_name : _collidable){
							_collision->setBodyCollisionFilter(*_bodies[_skeleton.m_boneMap[_skeleton.m_boneAliases[_name]]], Physics::GROUP_RAGDOLL,
								Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC | Physics::GROUP_DYNAMIC_RAGDOLL | Physics::GROUP_RAGDOLL);
						}*/

						for (int i = 0; i < _bodies.size() - 8; ++i){
							_collision->setBodyCollisionFilter(*_bodies[i], Physics::GROUP_RAGDOLL,
								Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC | Physics::GROUP_DYNAMIC_RAGDOLL | Physics::GROUP_RAGDOLL);
						}

						/*for (int i = 1; i <= 8; ++i){
							_collision->setBodyCollisionFilter(*_bodies[_bodies.size() - i], Physics::GROUP_DYNAMIC_RAGDOLL,
								Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC | Physics::GROUP_RAGDOLL);
						}*/

						//PRINT("NUM BODIES: " << _bodies.size());

						/*for (auto &_body : _bodies){
							_collision->setBodyCollisionFilter(*_body, Physics::GROUP_RAGDOLL,
								Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC | Physics::GROUP_DYNAMIC_RAGDOLL | Physics::GROUP_RAGDOLL);
						}*/
						
						//TODO re-enable
						//Enable collision callback
						//_collisionComponent.setCustomCallback(true);
					} else{
						//TODO set 0.0f weight for non-dynamic(complex) collision bodies

						auto &_collisionComponent = _entity.addComponent<CollisionComponent>(_collision,
							_entity.getId(), *_mesh, _correctedLocation, scale, 0.0f, collisionType, true);
						_collisionComponent.setCollisionFilters(Physics::GROUP_STATIC,
							Physics::GROUP_ALL);

						//PRINT("MATERIAL: " << _mesh->m_material);
						auto &_material = _materialComponent.getMaterial();
						for (auto &_body : _collisionComponent.getBodies()){
							_body->setUserPointer(&world);
							//PRINT("FRICTION: " << _material->m_friction);
							_body->setFriction(_material->m_friction);
							_body->setRestitution(_material->m_restitution);
						}
					}

					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					_collisionComponent.setRotation(rotation);
					_matrixComponent.setRotation(Physics::convertQuat(
						_collisionComponent.getBody()->getWorldTransform().getRotation()));


					auto _boundingBox = _collisionComponent.getBoundingBox();
					int _scaleFactor = static_cast<int>(glm::distance(_boundingBox[0], _boundingBox[1]));
					glm::vec2 _scale = glm::min(glm::vec2(3.0f), glm::vec2(_scaleFactor / 2.0f));
					//PRINT("SCLAE FACTOR: " << _entityName << "  " << _scaleFactor);
					/*auto &_billboardComponent = _entity.addComponent<BillboardComponent>(_entityName, 64);
					_billboardComponent.m_globalScale = _scale; //*/
					

					_entity.activate();
					_entities.push_back(_entity);

					_numTriangles += _mesh->m_indices.size() / 3;
				}
			}


			PRINT("---------------------------------------------------")
				Uint32 _timerDuration = SDL_GetTicks() - _timerStart;
			PRINT("Loaded " << _entities.size() << " entities, " <<
				_numTriangles << " triangles in " << _timerDuration << "ms");
			/*for (auto &_entity : _entities){
				PRINT("- " << _entity.getName());
			}*/

			PRINT("---------------------------------------------------")

			return _entities;
		}


		Entity createDecalEntity(World &world, const glm::vec3 &color,
			const Location &location, const Rotation &rotation, const Scale &scale)
		{
			Scale _scale(scale);
			auto &_variables = world.getVariables();
			auto &_resouceManager = world.getResourceManager();
			auto &_mesh = _resouceManager.getMesh("primitives", "cube");

			auto _raycast = _variables.m_collision->rayTracingClosest(
				location, glm::vec3(0.0f, -1.0f, 0.0f), 1000.0f);
			if (_raycast.hasHit())
			{
				glm::vec3 _hitLocation = Physics::convertVec3(_raycast.m_hitPointWorld);

				auto _entity = world.addEntity("decal");
				{
					auto &_matrixComponent = _entity.addComponent<MatrixComponent>(
						_hitLocation, rotation, _scale);

					auto &_collisionComponent = _entity.addComponent<CollisionComponent>(
						_variables.m_collision, _entity.getId(), _mesh, _matrixComponent.getLocation(),
						_scale * 0.5f, 0.0f, Physics::COLLISION_CUBE, true);
					_collisionComponent.setRotation(_matrixComponent.getRotation());

					_collisionComponent.addCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);			
					_collisionComponent.setCollisionFilters(
						Physics::GROUP_GHOST, Physics::GROUP_NOTHING);


					auto &_materialComponent = _entity.addComponent<MaterialComponent>(
						_resouceManager.getMaterialPtr("paper"), color);
				}
				_entity.activate();

				return _entity;

			} else{
				return Entity();
			}	
		}

	}
}
