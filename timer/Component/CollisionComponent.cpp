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


#include "CollisionComponent.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "../Mesh/Mesh.hpp"
#include "../Animation/AnimationSkeleton.hpp"
#include "../Collision/Ragdoll.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			CollisionComponent::CollisionComponent(){

			}

			CollisionComponent::CollisionComponent(std::shared_ptr<Physics::Collision> collision, INT_TYPE entityId,
				const Meshes::Mesh &mesh, const glm::vec3 &location, const glm::vec3 &scale,
				float mass, Physics::CollisionMeshType meshType, bool kinematic
			) :
				m_collision(collision)
			{
				m_meshType = meshType;
				m_mass = mass < 0.0f ? 0.0f : mass;
				m_kinematic = kinematic;

				m_bodies.push_back(std::move(m_collision->addBody(
					mesh, location, scale, m_mass, m_meshType, m_kinematic)));
				m_bodies.back()->setUserIndex(entityId);
				m_bodies.back()->getCollisionShape()->setUserIndex(entityId);
			}

			CollisionComponent::CollisionComponent(std::shared_ptr<Physics::Collision> collision, INT_TYPE entityId,
				const Meshes::Mesh &mesh, std::shared_ptr<Animation::AnimationSkeleton> skeleton,
				const glm::vec3 &location, const glm::vec3 &scale,
				float mass, Physics::CollisionMeshType collisionType, bool kinematic
			) :
				m_collision(collision), 
				m_meshType(Physics::COLLISION_CUBE)
			{
				setMassVariable(mass);
				setKinematicVariable(kinematic);


				float _scale = glm::dot(glm::vec3(0.33333f), scale);
				m_ragdollBoundingBoxes = Physics::Ragdoll::createHumanRagdoll(
					*collision, mesh, *skeleton, m_bodies, m_constraints, location, _scale, kinematic);

				for (auto &_body : m_bodies){
					_body->setUserIndex(entityId);
				}


				//m_bodies = Physics::Ragdoll::createRagdoll(collision, skeleton, location);

				/*for (auto &_transform : skeleton->m_boneTransforms){
				m_bodies.push_back(std::move(collision.addBody(
				mesh, location, glm::vec3(1, 1, 1), m_mass, m_meshType, m_kinematic)));

				auto _wt = m_bodies.back()->getWorldTransform();
				_wt.setFromOpenGLMatrix(glm::value_ptr( _transform.m_transform));

				m_bodies.back()->setWorldTransform(_wt);

				m_bodies.back()->setUserIndex(entityId);
				}		*/

			}

			/*CollisionComponent::CollisionComponent(INT_TYPE entityId, 
				const std::vector<glm::vec3> &vertices,
				const std::vector<unsigned int> &indices, 
				int type)
			{
				std::vector<glm::vec3> _vertices;
				_vertices.reserve(indices.size());

				for (auto ind : indices){
					_vertices.push_back(vertices[ind]);
				}

				//TODO add to collision world
				ASSERT(0, "");
			}

			CollisionComponent::CollisionComponent(INT_TYPE entityId,
				const std::vector<glm::vec3> &vertices)
			{
				//TOOD add to collision world

				ASSERT(0, "");
			}

			CollisionComponent::CollisionComponent(INT_TYPE entityId,
				std::vector<glm::vec3> &&vertices,
				std::vector<unsigned int> &&indices)
			{
				//TODO add to collision world
				ASSERT(0, "");
			}

			CollisionComponent::CollisionComponent(INT_TYPE entityId,
				std::vector<glm::vec3> &&vertices)
			{
				//std::move
				//TODO add to collision world
				ASSERT(0, "");
			}

			CollisionComponent::CollisionComponent(INT_TYPE entityId,
				const glm::vec3 &min, const glm::vec3 &max, int type)
			{
				//if (type == m)
				ASSERT(0, "");
			}

			CollisionComponent::CollisionComponent(INT_TYPE entityId,
				glm::vec3 &&min, glm::vec3 &&max, int type)
			{
				ASSERT(0, "");
			}*/

			CollisionComponent::~CollisionComponent(){
				std::cout << "Deleted collision component!" << std::endl;

				//Cleanup
				removeBodies();
			}



			Physics::Collision &CollisionComponent::getCollision() const{
				ASSERT(m_collision != nullptr, "Collision is a nullptr");

				return *m_collision;
			}


			void CollisionComponent::removeBodies(){
				auto &_collision = getCollision();

				for (auto &_body : m_bodies){
					_collision.removeBody(*_body);
					_body.reset();
				}

				m_bodies.clear();
			}

			const Physics::RigidBodyPtr &CollisionComponent::getBody(std::size_t id) const{
				ASSERT(id < m_bodies.size(), "Collision body id out of range");

				return m_bodies[id];
			}

			const std::vector<Physics::RigidBodyPtr> &CollisionComponent::getBodies() const{
				return m_bodies;
			}

			btRigidBody &CollisionComponent::addBody(INT_TYPE entityId, const Meshes::Mesh &mesh, 
				const glm::vec3 &location, const glm::quat &rotation, const glm::vec3 &scale)
			{
				m_bodies.push_back(std::move(m_collision->addBody(
					mesh, location, scale, m_mass, m_meshType, m_kinematic)));

				m_bodies.back()->setUserIndex(entityId);
				m_bodies.back()->getCollisionShape()->setUserIndex(entityId);

				return *m_bodies.back();
			}




			void CollisionComponent::addHingeConstraint(
				const std::string &name, btRigidBody &otherBody, 
				const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
				const glm::vec3 &rotation2, const glm::vec3 &locationFactors2,
				const glm::vec2 &limits, float defaultRotation, 
				float softness, float biasFactor, float bounceFactor)
			{
				m_collision->addHingeConstraint(name, *getBody(), otherBody,
					rotation1, locationFactors1, rotation2, locationFactors2,
					limits, defaultRotation, 
					softness, biasFactor, bounceFactor);
			}

			void CollisionComponent::addConeTwistConstraint(
				const std::string &name, btRigidBody &otherBody,
				const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
				const glm::vec3 &rotation2, const glm::vec3 &locationFactors2,
				const glm::vec2 &swingLimits, float twistLimit, const glm::vec3 &defaultRotations,
				float softness, float biasFactor, float bounceFactor)
			{
				m_collision->addConeTwistConstraint(name, *getBody(), otherBody,
					rotation1, locationFactors1, rotation2, locationFactors2,
					{ //Argument list
						swingLimits, 
						twistLimit, 
						defaultRotations,
						softness, 
						biasFactor, 
						bounceFactor 
					}
				);
			}


			void CollisionComponent::addHingeConstraint(
				const std::string &name, const glm::vec3 &rotation1, const glm::vec3 &locationFactors1, 
				const glm::vec2 &limits, float defaultRotation, float biasFactor, float bounceFactor)
			{
				auto &_body = getBody();
				glm::vec3 _bodyScale = Physics::getBodyScale(*_body);

				btTransform _localTransform;
				{
					_localTransform.setIdentity();
					glm::vec3 _rr1 = glm::radians(rotation1);
					_localTransform.getBasis().setEulerZYX(_rr1.x, _rr1.y, _rr1.z);
					_localTransform.setOrigin(btVector3(_bodyScale.x * locationFactors1.x,
						_bodyScale.y * locationFactors1.y, _bodyScale.z * locationFactors1.z));
				}

				auto _hingeConstraint = std::make_shared<btHingeConstraint>(*_body, _localTransform);

				glm::vec2 _rLimits = glm::radians(limits);
				_hingeConstraint->setLimit(_rLimits.x, _rLimits.y, 0.9f, biasFactor, bounceFactor);

				m_collision->getWorld().addConstraint(_hingeConstraint.get(), true);
				m_constraints[name] = _hingeConstraint;

				//set body to default position
				auto _worldTransform = _body->getWorldTransform();
				float _defaultRadRot = glm::radians(defaultRotation);
				_worldTransform.setRotation(btQuaternion(0.0f, 0.0f, _defaultRadRot));
				//_defaultRadRot, _defaultRadRot, _defaultRadRot));
				_body->setWorldTransform(_worldTransform);
			}


			btTypedConstraint &CollisionComponent::addFixedConstraint(
				const std::string &name, btRigidBody &thisBody, const Location &thisLocation)
			{
				auto _collisionWorld = m_collision->getWorld();

				btTransform _transform;
				_transform.setIdentity();
				_transform.setOrigin(Physics::convertVec3(thisLocation));

				//create fully limited 6dof constraint
				auto _fixedContraint = std::make_shared<btGeneric6DofSpring2Constraint>(thisBody, _transform);		
				_fixedContraint->setAngularLowerLimit(btVector3(0, 0, 0));
				_fixedContraint->setAngularUpperLimit(btVector3(0, 0, 0));
				_fixedContraint->setLinearLowerLimit(btVector3(0, 0, 0));
				_fixedContraint->setLinearUpperLimit(btVector3(0, 0, 0));
				
				if (m_constraints.find(name) != m_constraints.end()){
					_collisionWorld.removeConstraint(_fixedContraint.get());
					m_constraints.erase(name);
				}

				//TODO check if already exists (probably delete if found)
				_collisionWorld.addConstraint(_fixedContraint.get(), true);
				m_constraints[name] = _fixedContraint;

				return *m_constraints[name];
			}


			/*void CollisionComponent::addConstraint(
				Physics::Collision &collision, btRigidBody *otherBody,
				const glm::vec3 &rotation1, float locationFactor1, 
				const glm::vec3 &rotation2, float locationFactor2,
				Physics::CollisionConstraintType constraintType, 
				float biasFactor = 0.3f, float relaxationFactor = 1.0f)
			{
				switch (constraintType){
					case Physics::CONSTRAINT_POINT2POINT:
					{
					PRINT("TODO POINT2POINT CONSTRAING");
					}break;

					case Physics::CONSTRAINT_HINGE:
					{


					}break;

					case Physics::CONSTRAINT_CONE_TWIST:
					{
					PRINT("TODO CONE TWIST CONSTRAING");
					}break;

					case Physics::CONSTRAINT_SLIDER:
					{
					PRINT("TODO SLIDER CONSTRAING");
					}break;

					case Physics::CONSTRAINT_6_DEGREES:
					{
					PRINT("TODO 6DEGREE CONSTRAING");
					}break;

					default:
					{
					ASSERT(0, "Uknown constraint type");
					break;
					}

				}
			}*/

			void CollisionComponent::addConstraint(const std::string &name,
				const glm::vec3 &rotation1, Physics::CollisionConstraintType constraintType)
			{
				//TODO single constraint body
			}

			const std::unordered_map<std::string, Physics::ConstraintPtr> 
				&CollisionComponent::getConstraintHashmap() const
			{
				return m_constraints;
			}

			const std::vector<Physics::ConstraintPtr> CollisionComponent::getConstraints() const{
				std::vector<Physics::ConstraintPtr> _constraints;
				_constraints.reserve(m_constraints.size());

				for (auto &_pair : m_constraints){
					_constraints.push_back(_pair.second);
				}

				return _constraints;
			}

			const btTypedConstraint *CollisionComponent::getConstraint(const std::string &name) const{
				auto _itr = m_constraints.find(name);

				if (_itr != m_constraints.end()){
					return (*_itr).second.get();
				} else return nullptr;
			}


			void CollisionComponent::setMeshType(
				Physics::CollisionMeshType meshType, const Meshes::Mesh &mesh)
			{
				m_meshType = meshType;
			
				auto &_collision = getCollision();
				auto &_body = getBody();
				_collision.removeBodyFromWorld(*_body);

				auto _collisionShape = getBody()->getCollisionShape();
				ASSERT(_collisionShape != nullptr,
					"Body collision shape is null pointer");

				auto _scale = _collisionShape->getLocalScaling();
				delete _collisionShape;
				_body->activate(true);

				btCollisionShape *_shape = nullptr;

				//TODO
				switch (meshType){
					case Physics::COLLISION_SPHERE:
					_shape = _collision.getSphereShape(mesh);
					break;

					case Physics::COLLISION_CUBE:
					_shape = _collision.getBoxShape(mesh);
					break;

					case Physics::COLLISION_SIMPLIFIED:
					_shape = _collision.getSimplifiedMeshShape(mesh);
					break;

					case Physics::COLLISION_COMPLEX:
					_body->setMassProps(0, btVector3(0.0f, 0.0f, 0.0f));
					_shape = _collision.getMeshShape(mesh);
					break;

					default: ASSERT(0, "Shouldn't get here");
				}


				_shape->setLocalScaling(_scale);
				_body->setCollisionShape(_shape);

				_collision.addBodyToWorld(*_body, m_collisionGroup, m_collidesWith);
			}

			Physics::CollisionMeshType CollisionComponent::getMeshType() const{
				return m_meshType;
			}



			void CollisionComponent::addCollisionFlags(int flags, std::size_t bodyId){
				if (bodyId == -1){
					for (auto &_body : m_bodies){
						_body->setCollisionFlags(_body->getCollisionFlags() | flags);
					}
				} else{
					ASSERT(bodyId < m_bodies.size(), "Collision bodyId out of range");
					m_bodies[bodyId]->setCollisionFlags(
						m_bodies[bodyId]->getCollisionFlags() | flags);
				}
			}

			void CollisionComponent::removeCollisionFlags(int flags, std::size_t bodyId){
				if (bodyId == -1){
					for (auto &_body : m_bodies){
						_body->setCollisionFlags(_body->getCollisionFlags() & (~flags));
					}
				} else{
					ASSERT(bodyId < m_bodies.size(), "Collision bodyId out of range");
					m_bodies[bodyId]->setCollisionFlags(
						m_bodies[bodyId]->getCollisionFlags() & (~flags));
				}
			}

			void CollisionComponent::setCustomCallback(bool on){
				if (on)addCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
				else removeCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			}



			bool CollisionComponent::isStaticObject() const{
				return getBody()->isStaticObject();
			}

			bool CollisionComponent::isKinematic() const{
				return m_kinematic;
			}

			void CollisionComponent::setKinematic(bool kinematic){
				setKinematicVariable(kinematic);

				auto &_collision = getCollision();
				for (auto &_body : m_bodies){
					_collision.setKinematic(*_body, m_kinematic, m_collisionGroup, m_collidesWith);
					_collision.setBodyCollisionFilter(*_body, m_collisionGroup, m_collidesWith);
				}

				if (!m_kinematic){
					for (auto &_body : m_bodies){
						_body->activate(true);
					}
				}

				//TODO test how ragdoll reacts without disabling constraints
				for (auto &_constraint : m_constraints){
					_constraint.second->setEnabled(!kinematic);
					//_constraint.second->setEnabled(true);
				}
			}

			void CollisionComponent::setKinematicVariable(bool kinematic){
				m_kinematic = kinematic;
			}


	
			float CollisionComponent::getMass() const{
				return m_mass;
			}

			void CollisionComponent::setMassVariable(float mass){
				m_mass = mass < 0.0f ? 0.0f : mass;

				//Can't have dynamic complex bodies
				if (m_meshType == Physics::COLLISION_COMPLEX){
					m_mass = 0.0f;
				}
			}

			void CollisionComponent::setMass(float mass){
				setMassVariable(mass);

				auto &_collision = getCollision();
				for (auto &_body : m_bodies){
					_collision.setMass(*_body, m_mass, m_collisionGroup, m_collidesWith);
					//collision.setBodyCollisionFilter(*_body, m_collisionGroup, m_collidesWith);
				}
			}


			short CollisionComponent::getCollisionGroup() const{
				return m_collisionGroup;
			}

			short CollisionComponent::getCollidesWith() const{
				return m_collidesWith;
			}

			void CollisionComponent::setCollisionGroup(short collisionGroup){
				m_collisionGroup = collisionGroup;
			}

			void CollisionComponent::setCollidesWithFilter(short collidesWith){
				m_collidesWith = collidesWith;
			}

			void CollisionComponent::setCollisionFilterVariables(short collisionGroup, short collidesWith){
				setCollisionGroup(collisionGroup);
				setCollidesWithFilter(collidesWith);
			}

			void CollisionComponent::setCollisionFilters(short collisionGroup, short collidesWith)
			{
				setCollisionFilterVariables(collisionGroup, collidesWith);

				auto &_collision = getCollision();
				for (auto &_body : m_bodies){
					_collision.setBodyCollisionFilter(*_body, collisionGroup, collidesWith);
				}
			}


			bool CollisionComponent::isRagdoll() const{
				return !m_ragdollBoundingBoxes.empty();
			}

			const std::vector<glm::mat2x3> &CollisionComponent::getRagdollBoundingBoxes() const{
				return m_ragdollBoundingBoxes;
			}


			Location CollisionComponent::getLocation() const{
				return Physics::convertVec3(getBody()->getWorldTransform().getOrigin());
			}

			void CollisionComponent::setLocation(const glm::vec3 &location){
				btVector3 _location = Physics::convertVec3(location);

				for (auto &_body : m_bodies){
					/*btTransform _transform = _body->getCenterOfMassTransform();
					_transform.setOrigin(_location);
					_body->setCenterOfMassTransform(_transform);*/

					btTransform _transform;
					_body->getMotionState()->getWorldTransform(_transform);
					_transform.setOrigin(_location);
					_body->getMotionState()->setWorldTransform(_transform);

					_transform = _body->getWorldTransform();
					_transform.setOrigin(_location);
					_body->setWorldTransform(_transform);
				}
			}

			void CollisionComponent::setLocation(float x, float y, float z){
				setLocation(glm::vec3(x, y, z));
			}

			void CollisionComponent::increaseLocation(const glm::vec3 &location){
				btVector3 _location = Physics::convertVec3(location);

				for (auto &_body : m_bodies){
					btTransform _transform;
					_body->getMotionState()->getWorldTransform(_transform);
					_transform.setOrigin(_transform.getOrigin() + _location);
					_body->getMotionState()->setWorldTransform(_transform);

					_transform = _body->getWorldTransform();
					_transform.setOrigin(_transform.getOrigin() + _location);
					_body->setWorldTransform(_transform);

					_body->activate(true);
				}
			}

			void CollisionComponent::increaseLocation(float x, float y, float z){
				increaseLocation(glm::vec3(x, y, z));
			}



			//TODO add everything
			void CollisionComponent::setRotation(const Rotation &rotation){
				auto _rotation = Physics::convertQuat(rotation);

				for (auto &_body : m_bodies){
					/*btTransform _transform = _body->getCenterOfMassTransform();
					_transform.setRotation(_rotation);
					_body->setCenterOfMassTransform(_transform);*/

					btTransform _transform;
					_body->getMotionState()->getWorldTransform(_transform);
					_transform.setRotation(_rotation);
					_body->getMotionState()->setWorldTransform(_transform);

					_transform = _body->getWorldTransform();
					_transform.setRotation(_rotation);
					_body->setWorldTransform(_transform);
				}
			}

			void CollisionComponent::increaseRotation(const Rotation &rotation){
				btQuaternion _rotation = Physics::convertQuat(rotation);

				for (auto &_body : m_bodies){
					btTransform _transform;
					_body->getMotionState()->getWorldTransform(_transform);
					_transform.setRotation(_rotation * _transform.getRotation());
					_body->getMotionState()->setWorldTransform(_transform);

					_transform = _body->getWorldTransform();
					_transform.setRotation(_rotation * _transform.getRotation());
					_body->setWorldTransform(_transform);
				}
			}

			void CollisionComponent::increaseRotation(const glm::vec3 &angles){
				increaseRotation(Rotation(angles));
			}

			void CollisionComponent::increaseRotation(float x, float y, float z){
				increaseRotation(Rotation(x, y, z));
			}



			glm::mat2x3 CollisionComponent::getBoundingBox() const{
				glm::mat2x3 _boundingBox(glm::vec3(WORLD_SIZE), glm::vec3(-WORLD_SIZE));
				
				for (auto &_body : m_bodies){
					btVector3 _min, _max;
					_body->getAabb(_min, _max);

					_boundingBox[0] = glm::min(_boundingBox[0], Physics::convertVec3(_min));
					_boundingBox[1] = glm::max(_boundingBox[1], Physics::convertVec3(_max));
				}

				return _boundingBox;
			}

		}
	}
}
