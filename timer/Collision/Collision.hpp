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


#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <LinearMath/btConvexHull.h>

#include <memory>
#include <glm/gtx/quaternion.hpp>
#include "../Mesh/Mesh.hpp"


namespace Engine{
	namespace Physics{

		const btVector3 GRAVITY(0.0f, -9.8f, 0.0f);

		const btVector3	DEFAULT_LINEAR_FACTOR(1.0f, 1.0f, 1.0f);
		const btVector3	DEFAULT_ANGULAR_FACTOR(1.0f, 1.0f, 1.0f);

		const btScalar  DEFAULT_FRICTION = 0.8f;
		const btScalar	DEFAULT_ROLLING_FRICTION = 0.5f;
		const btScalar	DEFAULT_RESTITUTION = 0.2f;

		const btScalar  DEFAULT_LINEAR_DAMPING = 0.0f;
		const btScalar  DEFAULT_ANGULAR_DAMPING = 0.0f;
		const btScalar	DEFAULT_LINEAR_SLEEPING_THRESHOLD = 0.8f;
		const btScalar	DEFAULT_ANGULAR_SLEEPING_THRESHOLD = 1.0f;


		enum CollisionGroupType{
			GROUP_ALL = -1,
			GROUP_NOTHING = 0x00,
			GROUP_STATIC = 0x01, //TODO change when collision component updated
			GROUP_RAGDOLL = 0x02,
			GROUP_DYNAMIC = 0x04, //TODO change when collision component updated
			GROUP_CHARACTER_CONTROLLER = 0x08,
			GROUP_EQUIPABLE = 0x10,
			GROUP_GHOST = 0x20,
			GROUP_DYNAMIC_RAGDOLL = 0x40
		};

		enum CollisionMeshType{
			COLLISION_SPHERE = 0,
			COLLISION_CAPSULE = 1, //TODO
			COLLISION_CUBE = 2,
			COLLISION_SIMPLIFIED = 3,
			COLLISION_COMPLEX = 4
		};

		enum CollisionConstraintType{
			CONSTRAINT_POINT2POINT,
			CONSTRAINT_HINGE,
			CONSTRAINT_CONE_TWIST,
			CONSTRAINT_SLIDER,
			CONSTRAINT_6_DEGREES,
		};


		struct ConeTwistConstraintParameters{
		public:
			glm::vec2 m_swingLimitsRadians = glm::vec2(0.0f);
			float m_twistLimitRadians = 0.0f;
			glm::vec3 m_defaultRotations = glm::vec3(0.0f);
			float m_softness = 0.9f;
			float m_biasFactor = 0.3f;
			float m_bounceFactor = 1.0f;

		public:
			ConeTwistConstraintParameters(){

			}

			ConeTwistConstraintParameters(
				const glm::vec2 &swingLimits, float twistLimit,
				const glm::vec3 &defaultRotations = glm::vec3(0.0f),
				float softness = 0.9f, float biasFactor = 0.3f, float bounceFactor = 1.0f
			) :
				m_swingLimitsRadians(glm::radians(swingLimits)), 
				m_twistLimitRadians(glm::radians(twistLimit)), 
				m_defaultRotations(defaultRotations),
				m_softness(softness),
				m_biasFactor(biasFactor), 
				m_bounceFactor(bounceFactor)
			{
			}
		};



		btCollisionWorld::ClosestRayResultCallback rayTracingClosest(btDynamicsWorld *world, 
			const glm::vec3 &location, const glm::vec3 &direction, double rayLength, 
			short collidesWith = -1);

		btCollisionWorld::AllHitsRayResultCallback rayTracingAll(btDynamicsWorld *world, 
			const glm::vec3 &location, const glm::vec3 &direction, double rayLength,
			short collidesWith = -1);


		using RigidBodyPtr = std::shared_ptr<btRigidBody>;
		using ConstraintPtr = std::shared_ptr<btTypedConstraint>;

		class Collision{
		private: //ESSENTIALS
			std::unique_ptr<btDiscreteDynamicsWorld> m_world;
			std::unique_ptr<btDispatcher> m_dispatcher;
			std::unique_ptr<btBroadphaseInterface> m_broadphase;
			std::unique_ptr<btConstraintSolver> m_solver;
			std::unique_ptr<btCollisionConfiguration> m_collisionConfig;

		private: //MEMBERS
			RigidBodyPtr m_waterPlane = nullptr;
			std::vector<RigidBodyPtr> m_bodies;
			std::vector<ConstraintPtr> m_constraints; //TODO

			btCollisionWorld::ClosestRayResultCallback m_closestRayResult(btVector3());
			btCollisionWorld::AllHitsRayResultCallback m_allRayResults(btVector3());

		private: //METHODS
			void setDefaultValues(btRigidBody &body);

			btTriangleMesh *getTriangleMesh(const Meshes::Mesh &mesh);


		public:
			Collision();

			~Collision();


			btDiscreteDynamicsWorld &getWorld() const;


			void setBodyCollisionFilter(btRigidBody &body, short collisionGroup, short collidesWith);

			void setCollisionCallbackFunction(ContactAddedCallback function);


			btCollisionWorld::ClosestRayResultCallback rayTracingClosest(const glm::vec3 &location, 
				const glm::vec3 &direction, double rayLength = 999999999.0, short collidesWith = -1);

			btCollisionWorld::AllHitsRayResultCallback rayTracingAll(const glm::vec3 &location, 
				const glm::vec3 &direction, double rayLength = 999999999.0, short collidesWith = -1);



			RigidBodyPtr addBody(btCollisionShape &collisionShape, const glm::vec3 &location,
				const glm::vec3 &scale, float mass, bool kinematic);


			//
			// GENERAL BODY SHAPE
			//
			RigidBodyPtr addBody(const Meshes::Mesh &mesh, const glm::vec3 &location, 
				const glm::vec3 &scale, float mass, CollisionMeshType meshType, bool kinematic);


			//
			// PLANE SHAPE
			//
			btCollisionShape *getPlaneShape(const glm::vec3 &normal);

			RigidBodyPtr addPlane(const glm::vec3 &location, const glm::vec3 &normal);


			//
			// SPHERE SHAPE
			//
			btCollisionShape *getSphereShape(float radius);

			btCollisionShape *getSphereShape(const Meshes::Mesh &mesh);

			RigidBodyPtr addSphere(const glm::vec3 &location, float scale,
				float radius, float mass, bool kinematic);


			//
			// CAPSULE SHAPE
			//
			btCollisionShape *getCapsuleShape(float radius, float height);

			btCollisionShape *getCapsuleShape(const Meshes::Mesh &mesh);

			RigidBodyPtr addCapsule(const glm::vec3 &location, float scale, 
				float radius, float height, float mass, bool kinematic);


			//
			// BOX SHAPE
			//
			btCollisionShape *getBoxShape(const glm::vec3 &extent);

			btCollisionShape *getBoxShape(const Meshes::Mesh &mesh);

			RigidBodyPtr addBox(const glm::vec3 &location, const glm::vec3 &scale,
				const glm::vec3 &extent, float mass, bool kinematic);


			//
			// SIMPLIFIED MESH SHAPE
			//
			btCollisionShape *getSimplifiedMeshShape(const Meshes::Mesh &mesh);

			RigidBodyPtr addSimplifiedMesh(const Meshes::Mesh &mesh, const glm::vec3 &location,
				const glm::vec3 &scale, float mass, bool kinematic);

			//
			// COMPLEX MESH SHAPE
			//
			btCollisionShape *getMeshShape(const Meshes::Mesh &mesh);

			RigidBodyPtr addMesh(const Meshes::Mesh &mesh, const glm::vec3 &location,
				const glm::vec3 &scale, float mass, bool kinematic);


			//
			// TERRAIN HEIGHTMAP SHAPE
			//
			btHeightfieldTerrainShape *getHeightfieldShape(std::vector<float> &heights);

			RigidBodyPtr addHeightfield(std::vector<float> &heights,
				const glm::vec3 &location, const glm::vec3 &scale);

			//void correctHeightfieldLocation(btRigidBody *heightfield, float height);

			//
			// UTILITIES REQUIRING COLLISION WORLD
			// 
			void changeBody(btRigidBody &body, const glm::vec3 &location, const glm::vec3 &scale);


			void setBodyLocation(btRigidBody &body, const glm::vec3 &location);

			void addBodyLocation(btRigidBody &body, const glm::vec3 &location);
			
			void setBodyRotation(btRigidBody &body, const glm::quat &rotation);

			void addBodyRotation(btRigidBody &body, const glm::quat &rotation);
			
			void setBodyScale(btRigidBody &body, const glm::vec3 &scale);

			void addBodyScale(btRigidBody &body, const glm::vec3 &scale);


			void setKinematic(btRigidBody &body, bool kinematic,
				short collisionGroup, short collidesWith);
			
			void setMass(btRigidBody &body, float mass, 
				short collisionGroup, short collidesWith);


			void updateCollisionWorld(float fps = 60.0f);
			
			void updateSingleBody(btRigidBody &body);
					

			void removeBodyFromWorld(btRigidBody &body);

			void addBodyToWorld(btRigidBody &body, 
				short collisionGroup, short collidesWith);


			void removeBody(btRigidBody &body);


			void enableGravity();

			void disableGravity();

			const std::vector<RigidBodyPtr> getAllBodies() const;



			btTransform getLocalConstraintTransform(btRigidBody &body,
				const glm::vec3 &rotation1, const glm::vec3 &locationFactors1);
			
			void addConstraint(ConstraintPtr constraint);

			
			btHingeConstraint& addHingeConstraint(
				const std::string &name, btRigidBody &body1, btRigidBody &body2,
				const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
				const glm::vec3 &rotation2, const glm::vec3 &locationFactors2,
				const glm::vec2 &limits, float defaultRotation = 0.0f,
				float softness = 0.9f, float biasFactor = 0.3f, float bounceFactor = 1.0f);


			btGeneric6DofSpring2Constraint &addConeTwistConstraint(
				const std::string &name,
				btRigidBody &body1,
				btRigidBody &body2,
				const btTransform &parentTransform,
				const btTransform &childTransform,
				ConeTwistConstraintParameters constraintParameters
			);

			btGeneric6DofSpring2Constraint &addConeTwistConstraint(
				const std::string &name, 
				btRigidBody &body1, 
				btRigidBody &body2,
				const glm::mat4 &parentTransform, 
				const glm::mat4 &childTransform,
				ConeTwistConstraintParameters constraintParameters
			);

			btGeneric6DofSpring2Constraint& addConeTwistConstraint(
				const std::string &name, 
				btRigidBody &body1, 
				btRigidBody &body2,
				const glm::vec3 &rotation1, 
				const glm::vec3 &locationFactors1,
				const glm::vec3 &rotation2, 
				const glm::vec3 &locationFactors2,
				ConeTwistConstraintParameters constraintParameters
			);

		};

	}
}

#endif //COLLISION_HPP