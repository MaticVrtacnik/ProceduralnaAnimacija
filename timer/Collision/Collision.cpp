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


#include "Collision.hpp"

#include <algorithm>
#include <BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "../Utility/CollisionUtilities.hpp"
#include "../Define/PrintDefines.hpp"
#include "../Define/Assert.hpp"


//#define USE_BOUYENCY

namespace Engine{
	namespace Physics{

		btCollisionWorld::ClosestRayResultCallback rayTracingClosest(btDynamicsWorld *world, 
			const glm::vec3 &location, const glm::vec3 &direction, double rayLength,
			short collidesWith)
		{
			ASSERT(world != nullptr, "World is a null pointer!");

			btVector3 _location = convertVec3(location);
			btVector3 _direction = _location + convertVec3(direction * glm::vec3(rayLength));

			btCollisionWorld::ClosestRayResultCallback _callback(_location, _direction);
			_callback.m_collisionFilterMask = collidesWith;

			world->rayTest(_location, _direction, _callback);

			return _callback;
		}

		btCollisionWorld::AllHitsRayResultCallback rayTracingAll(btDynamicsWorld *world,
			const glm::vec3 &location, const glm::vec3 &direction, double rayLength, 
			short collidesWith)
		{
			ASSERT(world != nullptr, "World is a null pointer!");

			btVector3 _location = convertVec3(location);
			btVector3 _direction = convertVec3(direction * glm::vec3(rayLength));

			btCollisionWorld::AllHitsRayResultCallback _callback(_location, _direction);
			_callback.m_collisionFilterMask = collidesWith;

			world->rayTest(_location, _direction, _callback);

			return _callback;
		}



		Collision::Collision(){
			m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
			m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
			m_broadphase = std::make_unique<btDbvtBroadphase>();
			m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
			m_world = std::make_unique<btDiscreteDynamicsWorld>(
				m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfig.get());

			m_world->getSolverInfo().m_numIterations = 10;
			
			m_world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;// ?? WTF ??
			m_world->setGravity(GRAVITY);
			//m_world->setGravity(btVector3(0.0f, 0.0f, 0.0f));

			//m_solver->prepareSolve()

			//TODO maybe remove added from player component
			m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(
				new btGhostPairCallback()
			);



			
			//Init water plane surface at 0.0 height
			m_waterPlane = addPlane(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			m_waterPlane->setRestitution(1.0f);
			m_waterPlane->setUserIndex(11000);
		}

		Collision::~Collision(){
			/*delete m_dispatcher;
			delete m_collisionConfig;
			delete m_solver;
			delete m_broadphase;
			delete m_world;*/

			for (auto &_body : m_bodies){
				removeBody(*_body);
			}

			//TODO REMOVE BODIES
			/*for (auto &_body : m_bodies){
				removeBody(_body);
				}*/
		}


		btDiscreteDynamicsWorld &Collision::getWorld() const{
			return *m_world;
		}

		void Collision::setDefaultValues(btRigidBody &body){
			body.setGravity(GRAVITY);

			body.setLinearFactor(DEFAULT_LINEAR_FACTOR);
			body.setAngularFactor(DEFAULT_ANGULAR_FACTOR);

			body.setFriction(DEFAULT_FRICTION);
			body.setRollingFriction(DEFAULT_ROLLING_FRICTION);
			body.setRestitution(DEFAULT_RESTITUTION);

			body.setDamping(DEFAULT_LINEAR_DAMPING, DEFAULT_ANGULAR_DAMPING);
			body.setSleepingThresholds(
				DEFAULT_LINEAR_SLEEPING_THRESHOLD, DEFAULT_ANGULAR_SLEEPING_THRESHOLD);
		}

		void Collision::setCollisionCallbackFunction(ContactAddedCallback function)
		{
			/*typedef bool CFunction(btManifoldPoint &cp,
				const btCollisionObjectWrapper *obj1, int id1, int index1,
				const btCollisionObjectWrapper *obj2, int id2, int index2);
			*/
			gContactAddedCallback = function;
		}


		btCollisionWorld::ClosestRayResultCallback Collision::rayTracingClosest(
			const glm::vec3 &location, const glm::vec3 &direction, double rayLength, 
			short collidesWith)
		{
			return Physics::rayTracingClosest(m_world.get(), 
				location, direction, rayLength, collidesWith);
		}

		btCollisionWorld::AllHitsRayResultCallback Collision::rayTracingAll(
			const glm::vec3 &location, const glm::vec3 &direction, double rayLength, 
			short collidesWith)
		{
			return Physics::rayTracingAll(m_world.get(), 
				location, direction, rayLength, collidesWith);
		}


		RigidBodyPtr Collision::addBody(btCollisionShape &mesh, const glm::vec3 &location,
			const glm::vec3 &scale, float mass, bool kinematic)
		{
			btTransform _transform;
			_transform.setIdentity();
			_transform.setOrigin(convertVec3(location));
			mesh.setLocalScaling(convertVec3(scale));

			btVector3 _inertia(0.0f, 0.0f, 0.0f);
			if (mass != 0.0f)
				mesh.calculateLocalInertia(mass, _inertia);

			btMotionState* _motion = new btDefaultMotionState(_transform);

			btRigidBody::btRigidBodyConstructionInfo _info(mass, _motion, &mesh, _inertia);

			//TODO debug crashes on make_shared
			std::shared_ptr<btRigidBody> _body(new btRigidBody(_info)); 


			_body->activate(true);
			setDefaultValues(*_body);

			m_world->addRigidBody(_body.get());
			setKinematic(*_body, kinematic, 
				!mass || kinematic ? GROUP_STATIC : GROUP_DYNAMIC, 
				GROUP_ALL
			);

			m_bodies.push_back(_body);
			return _body;
		}

		RigidBodyPtr Collision::addBody(const Meshes::Mesh &mesh, const glm::vec3 &location,
			const glm::vec3 &scale, float mass, CollisionMeshType meshType, bool kinematic)
		{
			ASSERT(mesh != nullptr, "Mesh it a null pointer!");
			ASSERT(!mesh->m_vertexData.empty(), "Mesh has no vertex data!");

			switch (meshType){
			case COLLISION_CUBE:
			{
				glm::vec3 _avg = (mesh.m_min + mesh.m_max) / glm::vec3(2.0f);
				glm::vec3 _correctedLocation = location + _avg;

				glm::vec3 _extent = mesh.m_min - _avg;
				return addBox(_correctedLocation, scale, _extent, mass, kinematic);
			}break;

			case COLLISION_SPHERE:
			{
				glm::vec3 _avg = (mesh.m_min + mesh.m_max) * 0.5f;
				glm::vec3 _correctedLocation = location + _avg;

				float _minDistance = glm::distance(mesh.m_min, _avg);
				float _maxDistance = glm::distance(mesh.m_max, _avg);
				float _radius = glm::max(_minDistance, _maxDistance) * 0.55f;

				float _scale = (scale.x + scale.y + scale.z) * 0.33333f;

				return addSphere(_correctedLocation, _scale, _radius, mass, kinematic);
			}break;

			case COLLISION_SIMPLIFIED:
			{
				return addSimplifiedMesh(mesh, location, scale, mass, kinematic);
			}break;

			case COLLISION_COMPLEX:
			{
				if (mass > 0.0f)WARNING("complex mesh shape cannot be dynamic");
				return addMesh(mesh, location, scale, 0.0f, kinematic);
			}break;
			}

			return nullptr; //should never get here
		}


		btCollisionShape *Collision::getPlaneShape(const glm::vec3 &normal){
			return new btStaticPlaneShape(convertVec3(normal), 0.0f);
		}


		RigidBodyPtr Collision::addPlane(const glm::vec3 &location, const glm::vec3 &normal){
			return addBody(*getPlaneShape(normal), location, glm::vec3(1.0f), 0, false);
		}


		btCollisionShape *Collision::getSphereShape(float radius){
			return new btSphereShape(radius);
		}

		btCollisionShape *Collision::getSphereShape(const Meshes::Mesh &mesh){
			ASSERT(mesh != nullptr, "Mesh it a null pointer!");

			glm::vec3 _avg = (mesh.m_min + mesh.m_max) / glm::vec3(2.0f);
			float _radius = glm::length(mesh.m_max - _avg);
			return getSphereShape(_radius);
		}

		RigidBodyPtr Collision::addSphere(const glm::vec3 &location, 
			float scale, float radius, float mass, bool kinematic)
		{
			return addBody(*getSphereShape(radius), location, 
				glm::vec3(scale), mass, kinematic);
		}


		btCollisionShape *Collision::getCapsuleShape(float radius, float height){
			return new btCapsuleShape(radius, height);
		}
		
		btCollisionShape *Collision::getCapsuleShape(const Meshes::Mesh &mesh){
			ASSERT(mesh != nullptr, "Mesh is a null pointer");

			glm::vec3 _max(glm::max(glm::abs(mesh.m_max), glm::abs(mesh.m_min)));
			float radius = glm::length(glm::vec2(_max.x, _max.z));

			return new btCapsuleShape(radius, mesh.m_max.y - mesh.m_min.y);
		}

		RigidBodyPtr Collision::addCapsule(const glm::vec3 &location, 
			float scale, float radius, float height, float mass, bool kinematic)
		{
			return addBody(*getCapsuleShape(radius, height), 
				location, glm::vec3(scale), mass, kinematic);
		}

		
		btCollisionShape *Collision::getBoxShape(const glm::vec3 &extent){
			return new btBoxShape(convertVec3(glm::abs(extent)));
		}
		
		btCollisionShape *Collision::getBoxShape(const Meshes::Mesh &mesh){
			ASSERT(mesh != nullptr, "Mesh it a null pointer!");

			glm::vec3 _avg = (mesh.m_min + mesh.m_max) / glm::vec3(2.0f);
			glm::vec3 _extent = mesh.m_min - _avg;
			return getBoxShape(_extent);
		}

		RigidBodyPtr Collision::addBox(const glm::vec3 &location, const glm::vec3 &scale,
			const glm::vec3 &extent, float mass, bool kinematic)
		{	
			return addBody(*getBoxShape(extent), location, scale, mass, kinematic);
		}


		btTriangleMesh *Collision::getTriangleMesh(const Meshes::Mesh &mesh){
			btTriangleMesh *_triangleMesh = new btTriangleMesh();

			auto &_indices = mesh.m_indices;
			auto &_vertexData = mesh.m_vertexData;
			ASSERT(_indices.size() % 3 == 0, "Wrong amount of indices!");

			for (std::size_t i = 0; i < _indices.size(); i += 3){
				_triangleMesh->addTriangle(
					convertVec3(_vertexData[_indices[i]].getVertex()),
					convertVec3(_vertexData[_indices[i + 1]].getVertex()),
					convertVec3(_vertexData[_indices[i + 2]].getVertex())
				);
			}

			return _triangleMesh;
		}

		btCollisionShape *Collision::getSimplifiedMeshShape(const Meshes::Mesh &mesh){
			//create a hull approximation
			/*btShapeHull* hull = new btShapeHull(originalConvexShape);
			btScalar margin = originalConvexShape->getMargin();
			hull->buildHull(margin);
			btConvexHullShape* simplifiedConvexShape = new btConvexHullShape(hull->getVertexPointer(), hull->numVertices());*/
			//btTriangleMesh *_triangleMesh = getTriangleMesh(mesh);

			btAlignedObjectArray<btVector3> _vertices;
			for (auto &ind : mesh.m_indices){
				_vertices.push_back(convertVec3(mesh.m_vertexData[ind].getVertex()));
			}

			/*btConvexHullComputer* _convexHullComputer = new btConvexHullComputer();
			Utilities::convexHullMarginMesh(_vertices, 0.1f, 5, true, true, _convexHullComputer);*/

			//FOR RENDER
			/*for (int i = 0; i < convexHullComputer->vertices.size(); i++)
			triangles.push_back(Utilities::convertVec3(convexHullComputer->vertices[i]) + location);*/

			/*btBvhTriangleMeshShape *cMesh = new btBvhTriangleMeshShape(bodyMesh, false);
			btShapeHull* hull = new btShapeHull(cMesh);
			btScalar margin = cMesh->getMargin();
			hull->buildHull(margin);
			btConvexHullShape* collisionMesh = new btConvexHullShape(hull->getVertexPointer(), hull->numVertices());*/

			btConvexHullShape *_mesh = new btConvexHullShape();
			/*for (int i = 0; i < _convexHullComputer->vertices.size(); i++)
				_mesh->addPoint(_convexHullComputer->vertices[i]);*/

			PRINT("NOT IMPLEMENTED");

			return _mesh;
		}

		RigidBodyPtr Collision::addSimplifiedMesh(const Meshes::Mesh &mesh, 
			const glm::vec3 &location, const glm::vec3 &scale, float mass, bool kinematic)
		{
			return addBody(*getSimplifiedMeshShape(mesh), location, scale, mass, kinematic);
		}


		btCollisionShape *Collision::getMeshShape(const Meshes::Mesh &mesh){
			btTriangleMesh *_triangleMesh = getTriangleMesh(mesh);
			//btTriangleInfoMap *_infoMap = new btTriangleInfoMap();
			btBvhTriangleMeshShape *_mesh = new btBvhTriangleMeshShape(_triangleMesh, false);
			//btGenerateInternalEdgeInfo(_mesh, _infoMap);

			return _mesh;
		}

		RigidBodyPtr Collision::addMesh(const Meshes::Mesh &mesh, const glm::vec3 &location, 
			const glm::vec3 &scale, float mass, bool kinematic)
		{
			return addBody(*getMeshShape(mesh), location, scale, mass, kinematic);
		}


		btHeightfieldTerrainShape *Collision::getHeightfieldShape(std::vector<float> &heights){
			ASSERT(!heights.empty(), "No heightmap data provided");

			const int _width = sqrt(heights.size());
			float _minHeight = *std::min_element(heights.begin(), heights.end());
			float _maxHeight = *std::max_element(heights.begin(), heights.end());

			btHeightfieldTerrainShape *_terrain = new btHeightfieldTerrainShape
				(_width, _width, &heights[0], 1.0, _minHeight, _maxHeight, 1, PHY_FLOAT, false);
			_terrain->setUseDiamondSubdivision(true);

			return _terrain;
		}

		RigidBodyPtr Collision::addHeightfield(std::vector<float> &heights, 
			const glm::vec3 &location, const glm::vec3 &scale)
		{
			btHeightfieldTerrainShape *_collisionObject = getHeightfieldShape(heights);

			float _minHeight = *std::min_element(heights.begin(), heights.end());
			float _maxHeight = *std::max_element(heights.begin(), heights.end());
			float _center = (_minHeight + _maxHeight) * 0.5f;

			glm::vec3 _correctedLocation = location + glm::vec3(0.0f, _center, 0.0f);

			return addBody(*_collisionObject, _correctedLocation, scale, 0.0, false);
		}

		/*void Collision::correctHeightfieldLocation(btRigidBody *heightfield, float height){
			ASSERT(heightfield != nullptr, "Heightfield is a null pointer");

			btVector3 _min, _max;

			heightfield->getCollisionShape()->getAabb(btTransform::getIdentity(), _min, _max);

			//btScalar _aabbHeight = (_min.getY() + _max.getY()) * 0.5f;
			btScalar _aabbHeight = abs(_min.getY() + _max.getY());
			btVector3 _correctedHeight(0.0f, _aabbHeight, 0.0f);

			btTransform _transform;
			heightfield->getMotionState()->getWorldTransform(_transform);
			_transform.getOrigin().setY(height);
			heightfield->getMotionState()->setWorldTransform(_transform);

			btTransform &_worldTranform = heightfield->getWorldTransform();
			_worldTranform.getOrigin().setY(height);
		}*/


		void Collision::changeBody(btRigidBody &body, 
			const glm::vec3 &location, const glm::vec3 &scale)
		{
			setBodyLocation(body, location);
			setBodyScale(body, scale);
		}


		void Collision::setBodyLocation(btRigidBody &body, const glm::vec3 &location){
			body.activate(true);

			btVector3 _location = convertVec3(location);

			btTransform _transform = body.getWorldTransform();
			_transform.setOrigin(_location);
			body.setWorldTransform(_transform);//TODO maybe optimize with reference call

			btMotionState *_motionState = body.getMotionState();
			if (_motionState != nullptr){
				_motionState->getWorldTransform(_transform);
				_transform.setOrigin(_location);
				_motionState->setWorldTransform(_transform);
			}
		}

		void Collision::addBodyLocation(btRigidBody &body, const glm::vec3 &location){
			body.activate(true);

			btVector3 _location = convertVec3(location);

			btTransform _transform = body.getWorldTransform();
			_transform.setOrigin(_transform.getOrigin() + _location);
			body.setWorldTransform(_transform); //TODO maybe optimize with reference call

			btMotionState *_motionState = body.getMotionState();
			if (_motionState != nullptr){
				_motionState->getWorldTransform(_transform);
				_transform.setOrigin(_transform.getOrigin() + _location);
				_motionState->setWorldTransform(_transform);
			}
		}


		void Collision::setBodyRotation(btRigidBody &body, const glm::quat &rotation){
			body.activate(true);

			btQuaternion _rotation = convertQuat(rotation);

			btTransform _transform = body.getWorldTransform();
			_transform.setRotation(_rotation);
			body.setWorldTransform(_transform); //TODO maybe optimize with reference call

			btMotionState *_motionState = body.getMotionState();
			if (_motionState != nullptr){
				_motionState->getWorldTransform(_transform);
				_transform.setRotation(_rotation);
				_motionState->setWorldTransform(_transform);
			}
		}

		void Collision::addBodyRotation(btRigidBody &body, const glm::quat &rotation){
			body.activate(true);

			btQuaternion _rotation = convertQuat(rotation);

			btTransform _transform = body.getWorldTransform();
			_transform.setRotation(_rotation * _transform.getRotation());
			body.setWorldTransform(_transform); //TODO maybe optimize with reference call

			btMotionState *_motionState = body.getMotionState();
			if (_motionState != nullptr){
				_motionState->getWorldTransform(_transform);
				_transform.setRotation(_rotation * _transform.getRotation());
				_motionState->setWorldTransform(_transform);
			}
		}


		void Collision::setBodyScale(btRigidBody &body, const glm::vec3 &scale){
			body.getCollisionShape()->setLocalScaling(
				convertVec3(glm::abs(scale) / glm::vec3(2.0f)));

			m_world->updateSingleAabb(&body);
		}

		void Collision::addBodyScale(btRigidBody &body, const glm::vec3 &scale){
			body.getCollisionShape()->setLocalScaling(
				body.getCollisionShape()->getLocalScaling() + convertVec3(scale));

			m_world->updateSingleAabb(&body);
		}


		void Collision::setKinematic(btRigidBody &body, 
			bool kinematic, short collisionGroup, short collidesWith)
		{
			this->removeBodyFromWorld(body);

			int _flags = body.getCollisionFlags();

			if (kinematic) _flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
			else _flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
			
			body.setCollisionFlags(_flags);

			this->addBodyToWorld(body, collisionGroup, collidesWith);
		}

		void Collision::setMass(btRigidBody &body, float mass,
			short collisionGroup, short collidesWith)
		{
			this->removeBodyFromWorld(body);

			body.activate(true);

			btVector3 intertia(0.0f, 0.0f, 0.0f);
			if (mass)body.getCollisionShape()->calculateLocalInertia(mass, intertia);

			body.setMassProps(mass, intertia);
			body.updateInertiaTensor();

			this->addBodyToWorld(body, collisionGroup, collidesWith);
		}


		void Collision::updateCollisionWorld(float fps){

#ifdef USE_BOUYENCY
			btCollisionObjectArray arr = m_world->getCollisionObjectArray();

			for (int i = 0; i < arr.size(); i++){
				btRigidBody *body = btRigidBody::upcast(arr[i]);
				if (!body) continue;

				if (!(body->isStaticObject() || body->isKinematicObject()))
				{
					btTransform tr;
					body->getMotionState()->getWorldTransform(tr);
					btVector3 location = tr.getOrigin();
					float r = body->getCollisionShape()->getLocalScaling().getY() / 2.0f;
					if (location.getY() < -r){
						btVector3 force = body->getLinearVelocity();
						if (force.getY() < 0.0f){
							body->applyCentralImpulse(GRAVITY + btVector3(0.0f, - force.getY(), 0.0f));
						}
						else{
							float y = -location.getY() - r;
							body->setLinearVelocity(btVector3(force.getX(), y, force.getZ()));
						}

						float waterX = 0.0;//0.3
						float waterZ = 0.0;//-0.3
						float f = 1.0f / fps;
						float diff = 1.0f + 5.0f * f;
						float x = force.getX() / diff;
						float z = force.getZ() / diff;
						body->setLinearVelocity(btVector3(x + waterX, body->getLinearVelocity().getY(), z + waterZ));
						body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
					}
				}
			}
#endif

			//std::cout << m_world.get() << std::endl;
			
			/*for (auto &_body : m_bodies){
				std::cout << _body->getUserIndex() << std::endl;
			}*/

			//TODO measure actual fps
			//fps *= 0.5f;
			float _fixedTimeStep = 1.0f / fps;

			m_world->stepSimulation(1.0f / fps, 2, _fixedTimeStep);
		}

		void Collision::updateSingleBody(btRigidBody &body){
			m_world->updateSingleAabb(&body);
		}


		void Collision::enableGravity(){
			m_world->setGravity(GRAVITY);
		}

		void Collision::disableGravity(){
			m_world->setGravity(btVector3(0.0f, 0.0f, 0.0f));
		}


		void Collision::removeBody(btRigidBody &body){
			//Remove local instance
			//TODO maybe too slow 
			/*for (auto &_body : m_bodies){
				if (_body.get() == (&body)){
					_body.reset();
					break;
				}
			}*/


			btMotionState* _motionState = body.getMotionState();
			btCollisionShape* _shape = body.getCollisionShape();
			//	world->removeCollisionObject(body);

			m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), [&body]
				(RigidBodyPtr &ptr) -> bool{ return ptr.get() == &body; }
			));

			//TODO fix memory leak
			//m_world->removeRigidBody(&body);
			if (_shape != nullptr) delete _shape;
			if (_motionState != nullptr) delete _motionState;
			
			
			//body = nullptr;
		}

		void Collision::removeBodyFromWorld(btRigidBody &body){
			m_world->removeRigidBody(&body);
		}

		void Collision::addBodyToWorld(btRigidBody &body,
			short collisionGroup, short collidesWith)
		{
			m_world->addRigidBody(&body, collisionGroup, collidesWith);
		}


		void Collision::setBodyCollisionFilter(btRigidBody &body, 
			short collisionGroup, short collidesWith)
		{
			m_world->removeRigidBody(&body);
			m_world->addRigidBody(&body, collisionGroup, collidesWith);
		}


		const std::vector<RigidBodyPtr> Collision::getAllBodies() const{
			return m_bodies;
		}



		btTransform Collision::getLocalConstraintTransform(btRigidBody &body,
			const glm::vec3 &rotation, const glm::vec3 &locationFactors)
		{
			glm::vec3 _bodyScale = Physics::getBodyScale(body);

			btTransform _transform;
			_transform.setIdentity();

			glm::vec3 _radiansRotation = glm::radians(rotation);
			_transform.getBasis().setEulerZYX(_radiansRotation.x, _radiansRotation.y, _radiansRotation.z);
			_transform.setOrigin(btVector3(_bodyScale.x * locationFactors.x,
				_bodyScale.y * locationFactors.y, _bodyScale.z * locationFactors.z));

			return _transform;
		}

		void Collision::addConstraint(ConstraintPtr constraint){
			m_constraints.push_back(constraint);
		}


		btGeneric6DofSpring2Constraint &Collision::addConeTwistConstraint(
			const std::string &name, btRigidBody &parentBody, btRigidBody &childBody,
			const btTransform &parentTransform, const btTransform &childTransform,
			ConeTwistConstraintParameters constraintParameters)
		{
			/*auto _coneTwistConstraint = std::make_shared<btConeTwistConstraint>(
				parentBody, childBody, parentTransform, childTransform);*/
			
			auto _coneTwistConstraint = std::make_shared<btGeneric6DofSpring2Constraint>(
				parentBody, childBody, parentTransform, childTransform);


			/*_coneTwistConstraint->setLimit(
				constraintParameters.m_swingLimitsRadians.x, 
				constraintParameters.m_swingLimitsRadians.y,
				constraintParameters.m_twistLimitRadians,
				constraintParameters.m_softness, 
				constraintParameters.m_biasFactor, 
				constraintParameters.m_bounceFactor
			);*/

			/*_coneTwistConstraint->setLimit(
				constraintParameters.m_swingLimitsRadians.x,
				constraintParameters.m_swingLimitsRadians.y,
				constraintParameters.m_twistLimitRadians
			);*/

			_coneTwistConstraint->setLimit(0, 0, 0);
			_coneTwistConstraint->setLimit(1, 0, 0);
			_coneTwistConstraint->setLimit(2, 0, 0);
			_coneTwistConstraint->setLimit(3, -0.5, 0.35);
			_coneTwistConstraint->setLimit(4, -0.1, 0.1);
			//_coneTwistConstraint->setLimit(4, 0, 0);
			_coneTwistConstraint->setLimit(5, 0, 0);

			_coneTwistConstraint->enableSpring(3, true);
			_coneTwistConstraint->setStiffness(3, 50.0f);
			//_coneTwistConstraint->setBounce(3, 10.0f);
			_coneTwistConstraint->setDamping(3, 5.0f);
			_coneTwistConstraint->setEquilibriumPoint(3, 0);

			_coneTwistConstraint->enableSpring(4, true);
			_coneTwistConstraint->setStiffness(4, 50.0f);
			//_coneTwistConstraint->setBounce(3, 1.0f);

			_coneTwistConstraint->setDamping(4, 5.0f);
			_coneTwistConstraint->setEquilibriumPoint(4, 0);

			/*_coneTwistConstraint->setLinearLowerLimit(btVector3(1.0f, 1.0f, 1.0f));
			_coneTwistConstraint->setLinearUpperLimit(btVector3(0.0f, 0.0f, 0.0f));*/

			// Enabling the spring behavior for they y-coordinate (index = 1)
			/*_coneTwistConstraint->enableSpring(0, true);
			_coneTwistConstraint->enableSpring(1, true);
			_coneTwistConstraint->enableSpring(2, true);
			_coneTwistConstraint->setStiffness(1, 35.0f);
			_coneTwistConstraint->setDamping(1, 0.5f);
			_coneTwistConstraint->setEquilibriumPoint();*/

			//_coneTwistConstraint->setSt

			this->getWorld().addConstraint(_coneTwistConstraint.get(), true);
			this->addConstraint(_coneTwistConstraint);

			//TODO set body to default position
			//body1.getWorldTransform().setRotation(Physics::convertQuat(Rotation(defaultRotations)));

			return *_coneTwistConstraint;
		}

		btGeneric6DofSpring2Constraint &Collision::addConeTwistConstraint(
			const std::string &name, btRigidBody &parentBody, btRigidBody &childBody,
			const glm::mat4 &parentTransform, const glm::mat4 &childTransform,
			ConeTwistConstraintParameters constraintParameters)
		{
			return addConeTwistConstraint(name, parentBody, childBody,
				Physics::convertMat4(parentTransform), 
				Physics::convertMat4(childTransform), 
				constraintParameters
			);
		}

		btGeneric6DofSpring2Constraint &Collision::addConeTwistConstraint(
			const std::string &name, btRigidBody &body1, btRigidBody &body2,
			const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
			const glm::vec3 &rotation2, const glm::vec3 &locationFactors2,
			ConeTwistConstraintParameters constraintParameters)
		{
			btTransform _localTransformA = this->getLocalConstraintTransform(body2, rotation2, locationFactors2);
			btTransform _localTransformB = this->getLocalConstraintTransform(body1, rotation1, locationFactors1);

			//TODO switch body1, body2 => parent,child
			return addConeTwistConstraint(name, body2, body1, 
				_localTransformA, _localTransformB, constraintParameters);
		}

		btHingeConstraint& Collision::addHingeConstraint(
			const std::string &name, btRigidBody &body1, btRigidBody &body2,
			const glm::vec3 &rotation1, const glm::vec3 &locationFactors1,
			const glm::vec3 &rotation2, const glm::vec3 &locationFactors2,
			const glm::vec2 &limits, float defaultRotation,
			float softness, float biasFactor, float bounceFactor)
		{
			btTransform _localTransformA = this->getLocalConstraintTransform(body2, rotation2, locationFactors2);
			btTransform _localTransformB = this->getLocalConstraintTransform(body1, rotation1, locationFactors1);
			auto _hingeConstraint = std::make_shared<btHingeConstraint>(
				body2, body1, _localTransformA, _localTransformB);

			glm::vec2 _rLimits = glm::radians(limits);
			_hingeConstraint->setLimit(_rLimits.x, _rLimits.y, softness, biasFactor, bounceFactor);

			this->getWorld().addConstraint(_hingeConstraint.get(), true);
			this->addConstraint(_hingeConstraint);

			//TODO set body to default position
			//body1.getWorldTransform().setRotation(Physics::convertQuat(Rotation(AXIS_Y, defaultRotation)));

			return *_hingeConstraint;
		}


	}
}
