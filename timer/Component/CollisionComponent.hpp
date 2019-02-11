#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include <unordered_map>

#include "Component.hpp"
#include "../Collision/Collision.hpp"
#include "../TransformVariables.hpp"


namespace Engine{ namespace Animation{
	struct AnimationSkeleton;
} }


namespace Engine{
	namespace Entities{
		namespace Components{

			class CollisionComponent : public Component{
			private:
				std::shared_ptr<Physics::Collision> m_collision = nullptr;
				std::vector<Physics::RigidBodyPtr> m_bodies;
				std::unordered_map<std::string, Physics::ConstraintPtr> m_constraints;


				float m_mass = 0.0f;
				bool m_kinematic = true;
				short m_collisionGroup = Physics::GROUP_STATIC;
				short m_collidesWith = Physics::GROUP_ALL;

				Physics::CollisionMeshType m_meshType = Physics::COLLISION_CUBE;

				//empty if bodies are not part of a ragdoll
				std::vector<glm::mat2x3> m_ragdollBoundingBoxes;
				

			public:
				CollisionComponent();

				CollisionComponent(std::shared_ptr<Physics::Collision> collision, INT_TYPE entityId,
					const Meshes::Mesh &mesh, const glm::vec3 &location, const glm::vec3 &scale,
					float mass, Physics::CollisionMeshType meshType, bool kinematic);

				CollisionComponent(std::shared_ptr<Physics::Collision> collision, INT_TYPE entityId,
					const Meshes::Mesh &mesh, std::shared_ptr<Animation::AnimationSkeleton> skeleton,
					const glm::vec3 &location, const glm::vec3 &scale,
					float mass, Physics::CollisionMeshType collisionType, bool kinematic);

				/*CollisionComponent(INT_TYPE entityId, const std::vector<glm::vec3> &vertices,
					const std::vector<unsigned int> &indices, int type);

				CollisionComponent(INT_TYPE entityId, const std::vector<glm::vec3> &vertices);
				
				CollisionComponent(INT_TYPE entityId, std::vector<glm::vec3> &&vertices,
					std::vector<unsigned int> &&indices);		

				CollisionComponent(INT_TYPE entityId, std::vector<glm::vec3> &&vertices);
				
				CollisionComponent(INT_TYPE entityId, const glm::vec3 &min, const glm::vec3 &max, int type);
				
				CollisionComponent(INT_TYPE entityId, glm::vec3 &&min, glm::vec3 &&max, int type);	*/

				~CollisionComponent();



				Physics::Collision &getCollision() const;

				void removeBodies();

				const Physics::RigidBodyPtr &getBody(std::size_t id = 0) const;

				const std::vector<Physics::RigidBodyPtr> &getBodies() const;

				btRigidBody &addBody(INT_TYPE entityId, const Meshes::Mesh &mesh,
					const glm::vec3 &location, const glm::quat &rotation, const glm::vec3 &scale);

				void addHingeConstraint(
					const std::string &name, btRigidBody &otherBody,
					const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
					const glm::vec3 &rotation2, const glm::vec3 &locationFactors2, 
					const glm::vec2 &limits, float defaultRotation = 0.0f, 
					float softness = 0.9f, float biasFactor = 0.3f, float bounceFactor = 1.0f);

				void addConeTwistConstraint(
					const std::string &name, btRigidBody &otherBody,
					const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
					const glm::vec3 &rotation2, const glm::vec3 &locationFactors2,
					const glm::vec2 &swingLimits, float twistLimit, const glm::vec3 &defaultRotations = glm::vec3(0.0f),
					float softness = 0.9f, float biasFactor = 0.3f, float bounceFactor = 1.0f);
				
				void addHingeConstraint(
					const std::string &name, const glm::vec3 &rotation1, const glm::vec3 &locationFactors1, 
					const glm::vec2 &limits, float defaultRotation = 0.0f, 
					float biasFactor = 0.3f, float bounceFactor = 1.0f);


				btTypedConstraint &addFixedConstraint(
					const std::string &name, btRigidBody &thisBody, const Location &thisLocation);
				
				/*void addConstraint(Physics::Collision &collision, btRigidBody *otherBody,
					const glm::vec3 &rotation1, float locationFactor1, const glm::vec3 &rotation2, 
					float locationFactor2, Physics::CollisionConstraintType constraintType,
					float biasFactor = 0.3f, float relaxationFactor = 1.0f);*/
				
				void addConstraint(const std::string &name, 
					const glm::vec3 &rotation1, Physics::CollisionConstraintType constraintType);
		
				const std::vector<Physics::ConstraintPtr> getConstraints() const;

				const std::unordered_map<std::string, Physics::ConstraintPtr> &getConstraintHashmap() const;

				const btTypedConstraint *getConstraint(const std::string &name) const;


				Physics::CollisionMeshType getMeshType() const;

				void setMeshType(Physics::CollisionMeshType meshType, const Meshes::Mesh &mesh);
				

				void addCollisionFlags(int flags, std::size_t bodyId = -1);

				void removeCollisionFlags(int flags, std::size_t bodyId = -1);

				void setCustomCallback(bool on);


				bool isStaticObject() const;

				bool isKinematic() const;

				void setKinematic(bool kinematic);
				
				void setKinematicVariable(bool kinematic);

			
				float getMass() const;

				void setMassVariable(float mass);

				void setMass(float mass);			


				short getCollisionGroup() const;

				short getCollidesWith() const;

				void setCollisionGroup(short collisionGroup);

				void setCollidesWithFilter(short collidesWith);

				void setCollisionFilterVariables(short collisionGroup, short collidesWith);

				void setCollisionFilters(short collisionGroup, short collidesWith);


				bool isRagdoll() const;

				const std::vector<glm::mat2x3> &getRagdollBoundingBoxes() const;


				Location getLocation() const;

				void setLocation(const glm::vec3 &location);

				void setLocation(float x, float y, float z);

				void increaseLocation(const glm::vec3 &location);

				void increaseLocation(float x, float y, float z);


				//TODO add everything
				void setRotation(const Rotation &rotation);

				void increaseRotation(const Rotation &rotation);

				void increaseRotation(const glm::vec3 &angles);

				void increaseRotation(float x, float y, float z);

		
				glm::mat2x3 getBoundingBox() const;

			};
		}
	}
}

#endif //COLLISION_COMPONENT_H