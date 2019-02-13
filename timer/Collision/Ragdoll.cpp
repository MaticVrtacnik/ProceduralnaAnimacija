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


#include "Ragdoll.hpp"

#include <assimp\scene.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>

#include "Collision.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Animation/AnimationSkeleton.hpp"
#include "../Define/Defines.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Physics{
		namespace Ragdoll{
			using namespace Animation;


			const aiNode *findSkeletonRoot(
				const std::unordered_map<std::string, unsigned int> &boneMap, 
				const aiNode *node)
			{			
				if (boneMap.find(node->mName.data) != boneMap.end()){
					return node;
				}

				for (std::size_t i = 0; node->mNumChildren; ++i){
					const aiNode *_node = findSkeletonRoot(boneMap, node->mChildren[i]);
					if (_node != nullptr) return _node;				
				}

				return nullptr;
			}


			bool addRagdollNode(
				std::vector<RigidBodyPtr> &bodies,
				AnimationSkeleton &skeleton,
				Collision &collision,
				const aiNode *child,
				btRigidBody *parentBody,
				float radius, float height)
			{
				/*ASSERT(parentBody != nullptr, "Parent body is a null pointer");
				ASSERT(child != nullptr, "Child is a null pointer");

				auto &_boneMap = skeleton.m_boneMap;
				auto &_boneTransforms = skeleton.m_boneTransforms;
				std::string _childName(child->mName.data);

				if (_boneMap.find(_childName) != _boneMap.end()){
					auto &_childTransform = _boneTransforms[_boneMap[_childName]].m_transform;
					glm::vec3 _childLocation = Utilities::getLocationFromMatrix(_childTransform) * 2.0f;
					auto _childBody = collision.addCapsule(_childLocation, 1.0f, radius, height, 1.0f, true);

					std::cout << "PARENT: " << parentBody << " CHILD: " << _childBody.get() << std::endl;

					btTransform _localA;
					_localA.setIdentity();
					_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
					_localA.setOrigin(btVector3(0.0f, radius, 0.0f));

					btTransform _localB;
					_localB.setIdentity();
					_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
					_localB.setOrigin(btVector3(0.0f, -radius, 0.0f));

					auto _constraint = new btConeTwistConstraint(
						*parentBody, *_childBody.get(), _localA, _localB);
					
					//_constraint->setLimit(-Utilities::PI / 4.0f, Utilities::PI / 2.0f);

					collision.getWorld().addConstraint(_constraint, true);
					bodies.push_back(std::move(_childBody));

					return true;
				}*/

				return false;
			}


			std::vector<RigidBodyPtr> createRagdoll(Collision &collision, 
				AnimationSkeleton &skeleton, const glm::vec3 &location, float scale)
			{
				std::vector<RigidBodyPtr> bodies;
				ASSERT(0, "Implement createRagdoll function");

				return bodies;

				/*std::vector<RigidBodyPtr> bodies;

				const aiNode *_rootNode = findSkeletonRoot(skeleton.m_boneMap, skeleton.m_scene->mRootNode);
				//_rootNode = _rootNode->mParent;
				std::string _rootName(_rootNode->mName.data);
				std::cout << "ROOT NAME: " << _rootName << std::endl;

				float _height = 4.0f; // abs((_parentLocation - _childLocation).y);
				const float _radius = _height / 4.0f; //TODO calculate from mesh
				glm::vec3 _location = Utilities::getLocationFromMatrix(
					skeleton.m_boneTransforms[skeleton.m_boneMap[_rootName]].m_transform);

				
				bodies.push_back(collision.addCapsule(
					_location, 1.0f, _radius, _height, 1.0f, true));

								
				std::vector<const aiNode*> _nodeStack{ _rootNode };
				const aiNode* _parent = nullptr;

				while (!_nodeStack.empty()){
					const aiNode *_node = _nodeStack.back();

					/*if (_parent != nullptr){
						std::cout << "PARENT: " << std::string(_parent->mName.data) << 
							" CHILD: " << std::string(_node->mName.data) << std::endl;
					}*/

					/*if (_node->mNumChildren != 0){
						_parent = _node;

						_nodeStack.reserve(_node->mNumChildren);

						//if (_parent != nullptr) std::cout << "PARENT: " << std::string(_parent->mName.data) << "HAS: " << std::endl;

						for (std::size_t i = 0, _boneCount = 1; i < _node->mNumChildren; ++i){
							aiNode *_child = _node->mChildren[i];
							auto _parentBody = bodies[bodies.size() - _boneCount].get();

							std::cout << "PARENT BODY: " << _parentBody << std::endl;
							
							if (addRagdollNode(bodies, skeleton, collision, 
								_child, _parentBody, _radius, _height)){
								_boneCount++;
							}

							_nodeStack.push_back(_child);
						}
					}
					else{
						_nodeStack.pop_back();
						while (!_nodeStack.empty() && _nodeStack.back() == _parent){
							_parent = _nodeStack.back()->mParent;
							_nodeStack.pop_back();					
						}
					}			
				}


				for (auto &_body : bodies){	
					//_body->setRestitution(1.0f);
					_body->setDamping(0.05f, 0.85f);
					_body->setDeactivationTime(0.8f);
					_body->setSleepingThresholds(1.6f, 2.5f);
					_body->setUserIndex(111111);				
				}

				std::cout << "DATA SIZE: " << bodies.size() << std::endl;
				return bodies;*/
			}


			std::vector<btConvexHullShape*> getBoneBoundingConvexHullShapes(const Meshes::Mesh &mesh, 
				Animation::AnimationSkeleton &skeleton, float scale)
			{		
				std::vector<std::vector<btScalar>> _tmpMeshPointsArray(skeleton.m_boneMap.size());

				auto &_vertexData = mesh.m_vertexData;
				auto &_boneData = skeleton.m_vertexBoneData;

				for (auto i = 0; i < _vertexData.size(); ++i){
					auto &_vertex = _vertexData[i].getVertex() * scale;
					auto &_boneInfo = _boneData[i];

					for (std::size_t j = 0; j < Animation::c_numBonesPerVertex; ++j){
						if (_boneInfo.m_weights[j] > 0.0f){
							unsigned int _boneId = _boneInfo.m_indices[j];
							auto &_tmpMeshPoints = _tmpMeshPointsArray[_boneId];

							_tmpMeshPoints.push_back(_vertex.x);
							_tmpMeshPoints.push_back(_vertex.y);
							_tmpMeshPoints.push_back(_vertex.z);

							break;
						}
					}
				}
				//PRINT("HULL SIZE: " << _tmpMeshPointsArray.size());

				std::vector<btConvexHullShape*> _convexHullShapes(_tmpMeshPointsArray.size(), nullptr);
				for (auto i = 0; i < _tmpMeshPointsArray.size(); ++i){
					const auto &_tmpMeshPoints = _tmpMeshPointsArray[i];
					int _numPoints = _tmpMeshPoints.size() / 4 - 1; //Proper
					//int _numPoints = _tmpMeshPoints.size() / 10; //Estimation

					//TODO add glm precision stride as last parameter sizeof(glm::type)
					btConvexHullShape _convexHullShape(_tmpMeshPoints.data(), _numPoints);
					_convexHullShape.setMargin(0);  // this is to compensate for a bug in bullet

					btShapeHull* _hull = new btShapeHull(&_convexHullShape);
					_hull->buildHull(0);    // note: parameter is ignored by buildHull

					_convexHullShapes[i] = new btConvexHullShape(
						(const btScalar*) _hull->getVertexPointer(), _hull->numVertices(), sizeof(btVector3));
					delete _hull;

				}

				return _convexHullShapes;
			}

			std::vector<btCollisionShape*> getBoneBoundingBoxShapes(const Meshes::Mesh &mesh,
				Animation::AnimationSkeleton &skeleton, float scale, std::vector<glm::mat2x3> *boundingBoxes)
			{
				auto _boundingBoxes = getBoneBoundingBoxes(mesh, skeleton, scale);
				
				std::vector<btCollisionShape*> _boxShapes(_boundingBoxes.size(), nullptr);
				for (int i = 0; i < _boxShapes.size(); ++i){
					auto &_boundingBox = _boundingBoxes[i];
					glm::vec3 _extents(glm::abs(_boundingBox[1] - _boundingBox[0]) * 0.5f);
					glm::vec3 _location = (_boundingBox[1] + _boundingBox[0]) / glm::vec3(2.0f);

					_extents = glm::vec3(0.21f);

					/*float _max = glm::max(_extents.x, glm::max(_extents.y, _extents.z));
					if (_max == _extents.x){
						_extents = glm::vec3(_extents.x, 0.1f, 0.1f);
					} else if (_max == _extents.y){
						_extents = glm::vec3(0.1f, _extents.y, 0.1f);
					} else{
						_extents = glm::vec3(0.1f, 0.1f, _extents.z);
					}*/

					//_boundingBox[0] = _location - _extents;
					//_boundingBox[1] = _location + _extents;

					_boxShapes[i] = new btBoxShape(Physics::convertVec3(_extents));
				}

				//Set bounding boxes argument
				*boundingBoxes = _boundingBoxes;

				return _boxShapes;
			}

			std::vector<glm::mat2x3> getBoneBoundingBoxes(const Meshes::Mesh &mesh,
				Animation::AnimationSkeleton &skeleton, float scale)
			{
				std::vector<glm::mat2x3> _boundingBoxes(skeleton.m_boneMap.size(),
					glm::mat2x3(glm::vec3(WORLD_SIZE), glm::vec3(-WORLD_SIZE)));

				auto &_vertexData = mesh.m_vertexData;
				auto &_boneData = skeleton.m_vertexBoneData;

				for (std::size_t i = 0; i < _vertexData.size(); ++i){
					auto &_vertex = _vertexData[i].getVertex() * scale;
					auto &_boneInfo = _boneData[i];

					for (std::size_t j = 0; j < Animation::c_numBonesPerVertex; ++j){
						if (_boneInfo.m_weights[j] > 0.0f){

							unsigned int _boneId = _boneInfo.m_indices[j];
							auto &_boundingBox = _boundingBoxes[_boneId];

							_boundingBox[0] = glm::min(_boundingBox[0], _vertex);
							_boundingBox[1] = glm::max(_boundingBox[1], _vertex);

							//TODO remove?
							break;
						}
					}
				}

				PRINT("SCALE: " << scale);

				return _boundingBoxes;
			}

			
			std::vector<glm::mat2x3> createHumanRagdoll(
				Collision &collision, 
				const Meshes::Mesh &mesh,
				Animation::AnimationSkeleton &skeleton, 				
				std::vector<Physics::RigidBodyPtr> &bodies,
				std::unordered_map<std::string, Physics::ConstraintPtr> &constraints,
				const glm::vec3& location, 
				float scale,
				bool kinematic)
			{
				bodies.resize(skeleton.m_boneMap.size());

				//const bool kinematic = false;
				//const float c_ragdollWeight = 1.0f;
				const float c_ragdollWeight = 0.0f;

				//const auto _boundingBoxes = getBoneBoundingBoxes(mesh, skeleton, scale);

				std::vector<glm::mat2x3> _boundingBoxes;
				//auto _collisionShapes = getBoneBoundingConvexHullShapes(mesh, skeleton, scale);
				auto _collisionShapes = getBoneBoundingBoxShapes(mesh, skeleton, scale, &_boundingBoxes);


				std::vector<Location> _locations;
				_locations.reserve(_boundingBoxes.size());
				for (auto &_boundingBox : _boundingBoxes){
					_locations.push_back((_boundingBox[1] + _boundingBox[0]) * 0.5f);
					//Utilities::print(_locations.back());
				}

				//auto _boundingBoxes = std::vector<glm::mat2x3>(_collisionShapes.size(), glm::mat2x3(0.0));
				auto _boxExtents = std::vector<glm::vec3>(_collisionShapes.size(), glm::vec3(0.0));
				for (auto i = 0; i < _collisionShapes.size(); ++i){
					//glm::mat2x3 _boundingBox = Physics::getBoundingBox(_collisionShapes[i]);
					glm::mat2x3 _boundingBox = _boundingBoxes[i];
					//_boundingBoxes[i] = _boundingBox;

					_boxExtents[i] = glm::abs(_boundingBox[1] - _boundingBox[0]) * 0.5f;
				}

				//const float c_boneWeight = c_ragdollWeight / _boundingBoxes.size();
				const float c_boneWeight = 0.0f;

				/*for (int i = 0; i < _capsuleScales.size(); ++i){
					_collisionShapes[i] = collision.getBoxShape(glm::vec3(
						_capsuleScales[i].x, _capsuleScales[i].y, _capsuleScales[i].x) * 0.5f
					);
				}*/

				//19 bones - TODO load from config file with joint angles
				auto HIP = skeleton.getBoneId("HIP");
				auto ABDOMEN = skeleton.getBoneId("ABDOMEN");
				auto CHEST = skeleton.getBoneId("CHEST");
				auto NECK = skeleton.getBoneId("NECK");
				auto HEAD = skeleton.getBoneId("HEAD");
				auto R_COLLAR = skeleton.getBoneId("R_COLLAR");
				auto R_U_ARM = skeleton.getBoneId("R_U_ARM");
				auto R_L_ARM = skeleton.getBoneId("R_L_ARM");
				auto R_HAND = skeleton.getBoneId("R_HAND");
				auto L_COLLAR = skeleton.getBoneId("L_COLLAR");
				auto L_U_ARM = skeleton.getBoneId("L_U_ARM");
				auto L_L_ARM = skeleton.getBoneId("L_L_ARM");
				auto L_HAND = skeleton.getBoneId("L_HAND");
				auto R_U_LEG = skeleton.getBoneId("R_U_LEG");
				auto R_L_LEG = skeleton.getBoneId("R_L_LEG");
				auto R_FOOT = skeleton.getBoneId("R_FOOT");
				auto L_U_LEG = skeleton.getBoneId("L_U_LEG");
				auto L_L_LEG = skeleton.getBoneId("L_L_LEG");
				auto L_FOOT = skeleton.getBoneId("L_FOOT");
				

				scale = 1.0f;

				/*PRINT("LOCATIONS::!!");
				for (auto &_l : _locations){
					Utilities::print(_l);
				}*/

				//BODY CENTER
				/*********************************************************************************************************/
				bodies[HIP] = collision.addBody(*_collisionShapes[HIP],
					location + _locations[HIP] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[ABDOMEN] = collision.addBody(*_collisionShapes[ABDOMEN],
					location + _locations[ABDOMEN] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[CHEST] = collision.addBody(*_collisionShapes[CHEST],
					location + _locations[CHEST] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[NECK] = collision.addBody(*_collisionShapes[NECK],
					location + _locations[NECK] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[HEAD] = collision.addBody(*_collisionShapes[HEAD],
					location + _locations[HEAD] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				//LEFT LEG
				/*********************************************************************************************************/
				bodies[L_U_LEG] = collision.addBody(*_collisionShapes[L_U_LEG],
					location + _locations[L_U_LEG] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[L_L_LEG] = collision.addBody(*_collisionShapes[L_L_LEG],
					location + _locations[L_L_LEG] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[L_FOOT] = collision.addBody(*_collisionShapes[L_FOOT],
					location + _locations[L_FOOT] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_LEFT_FOOT]->getWorldTransform().getBasis().setEulerZYX(Utilities::PI / 2.0f, 0.0f, 0.0f);

				//RIGHT LEG
				/*********************************************************************************************************/
				bodies[R_U_LEG] = collision.addBody(*_collisionShapes[R_U_LEG],
					location + _locations[R_U_LEG] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[R_L_LEG] = collision.addBody(*_collisionShapes[R_L_LEG],
					location + _locations[R_L_LEG] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[R_FOOT] = collision.addBody(*_collisionShapes[R_FOOT],
					location + _locations[R_FOOT] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_RIGHT_FOOT]->getWorldTransform().getBasis().setEulerZYX(Utilities::PI / 2.0f, 0.0f, 0.0f);

				//LEFT ARM
				/*********************************************************************************************************/
				bodies[L_COLLAR] = collision.addBody(*_collisionShapes[L_COLLAR],
					location + _locations[L_COLLAR] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[L_U_ARM] = collision.addBody(*_collisionShapes[L_U_ARM],
					location + _locations[L_U_ARM] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_LEFT_UPPER_ARM]->getWorldTransform().getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);

				bodies[L_L_ARM] = collision.addBody(*_collisionShapes[L_L_ARM],
					location + _locations[L_L_ARM] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_LEFT_LOWER_ARM]->getWorldTransform().getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);

				bodies[L_HAND] = collision.addBody(*_collisionShapes[L_HAND],
					location + _locations[L_HAND] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_LEFT_HAND]->getWorldTransform().getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);

				//RIGHT ARM
				/*********************************************************************************************************/
				bodies[R_COLLAR] = collision.addBody(*_collisionShapes[R_COLLAR],
					location + _locations[R_COLLAR] * scale, glm::vec3(scale), c_boneWeight, kinematic);

				bodies[R_U_ARM] = collision.addBody(*_collisionShapes[R_U_ARM],
					location + _locations[R_U_ARM] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_RIGHT_UPPER_ARM]->getWorldTransform().getBasis().setEulerZYX(0.0f, 0.0f, -Utilities::PI / 2.0f);

				bodies[R_L_ARM] = collision.addBody(*_collisionShapes[R_L_ARM],
					location + _locations[R_L_ARM] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_RIGHT_LOWER_ARM]->getWorldTransform().getBasis().setEulerZYX(0.0f, 0.0f, -Utilities::PI / 2.0f);

				bodies[R_HAND] = collision.addBody(*_collisionShapes[R_HAND],
					location + _locations[R_HAND] * scale, glm::vec3(scale), c_boneWeight, kinematic);
				//bodies[BODYPART_RIGHT_HAND]->getWorldTransform().getBasis().setEulerZYX(0.0f, 0.0f, -Utilities::PI / 2.0f);

				//OTHER BONES WITHOUT KNOWN ALIASES
				/*********************************************************************************************************/
				
				float angleFactor = 360.0f / 8.0f;
				std::vector<float> _angles = {90.0f, 225.0f, 180.0f, 315.0f, 0.0f, 45.0f, 135.0f, 270.0f};

				int boneCounter = 0;
				for (auto &_boneMap : skeleton.m_boneMap){
					//If we dont find a bone in known aliases bone map we add a default body
					if (!skeleton.hasAliasForBone(_boneMap.first)){
						//auto _defaultShape = collision.getBoxShape(glm::vec3(1.0f) * scale);

						PRINT("BONE: " << _boneMap.first << " |ANGLE: " << _angles[boneCounter]);

						//Utilities::print(skeleton.m_boneTransforms[_boneMap.second].m_originalOffset);

						//TODO instead of _collisionShapes[id] use a default body
						/*bodies[_boneMap.second] = collision.addBox(
							location + _locations[_boneMap.second] * scale * 0.05f, 
							glm::vec3(scale * 0.05f),
							glm::vec3(0.15f), 1.0f, false);*/

						/*bodies[_boneMap.second] = collision.addBody(*_collisionShapes[_boneMap.second],
						location + _locations[_boneMap.second] * scale * 0.05f, glm::vec3(scale * 0.05f), 1.0f, false);*/

						bodies[_boneMap.second] = collision.addBody(
							//*collision.getBoxShape(glm::vec3(0.15f, 0.5f, 0.15f)),
							*collision.getCapsuleShape(0.15f, 0.6f),
							location, glm::vec3(1.0), 1.0f, false
						);

						
						collision.setBodyCollisionFilter(*bodies[_boneMap.second], 
							Physics::GROUP_DYNAMIC_RAGDOLL, Physics::GROUP_DYNAMIC | Physics::GROUP_STATIC | Physics::GROUP_RAGDOLL);
						bodies[_boneMap.second]->forceActivationState(DISABLE_DEACTIVATION);

						

						/*bodies[_boneMap.second]->setCollisionFlags(bodies[_boneMap.second]->getCollisionFlags() |
							btCollisionObject::CF_NO_CONTACT_RESPONSE);*/

						glm::mat4 _parentTransform = glm::translate(glm::vec3(0.0f, 0.0f, 0.5f));
						glm::mat4 _childTransform = glm::translate(glm::vec3(0.0f, -0.75f, 0.0f));

						_childTransform *= glm::toMat4(Rotation(90.0f, 0.0f, 0.0f));

						Rotation _parentRotation(0.0f);
						_parentRotation = _parentRotation * Rotation(90.0f, 0.0f, 0.0f);
						_parentRotation = _parentRotation * Rotation(0.0f, 90 + _angles[boneCounter], 0.0f);
						_parentRotation = _parentRotation * Rotation(-60.0f, 0.0f, 0.0f);
						_parentTransform *= glm::toMat4(_parentRotation);

						//Translation from center
						_parentTransform *= glm::translate(0.25f, 0.0f, 0.0f);
						//_parentTransform *= glm::translate(1.0f, 0.0f, 0.0f);


						auto &_twistConstraint = collision.addConeTwistConstraint(
							"JOINT" + std::to_string(_boneMap.second), 
							*bodies[HIP], *bodies[_boneMap.second],
							_parentTransform, _childTransform,
							{ glm::vec2(2.0f, 10.0f), 20.0f } //first parameter is roll
						);//*/


						//_twistConstraint.updateRHS()

						/*collision.addHingeConstraint(
							"JOINT" + std::to_string(_boneMap.second), *bodies[_boneMap.second], *bodies[HIP],
							glm::vec3(-90.0, 0.0f, 0.0f), glm::vec3(0.0f, 2.0, 0.0f),
							glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0, 0.0f),
							glm::vec2(-50.0f, 50.0f)
						);//*/

						++boneCounter;
					}
				}


				/*********************************************************************************************************/

				/*for (std::size_t i = 0; i < bodies.size(); ++i){
					auto &_body = bodies[i];

					glm::vec3 _bbScale = glm::abs(_boundingBoxes[i][1] - _boundingBoxes[i][0]) * 0.5f;
					float scale = glm::dot(glm::vec3(0.33333f), _bbScale);

					_body->setCcdSweptSphereRadius(0.2f * scale);
					//_body->setCcdMotionThreshold(scale);
					//_body->setHitFraction(1.0f * scale);
					_body->setHitFraction(1.0f);

					_body->setRestitution(0.8f); //ORIGINAL 1.0f
					_body->setDamping(0.05f, 0.85f);
					_body->setFriction(0.5f);
					_body->setDeactivationTime(0.8f);
					_body->setSleepingThresholds(0.5f, 0.5f);
					_body->setUserIndex(111111);
				}*/
				

				btTransform _localA, _localB;

				/*********************************************************************************************************/
				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f); 
				_localA.setOrigin(btVector3(0.0f, _boxExtents[HIP].x, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f); 
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[ABDOMEN].x, 0.0f) * scale);
				auto hipAbdomenConstraint = std::make_shared<btHingeConstraint>(
					*bodies[HIP], *bodies[ABDOMEN], _localA, _localB);
				hipAbdomenConstraint->setLimit(-Utilities::PI / 2.0f, Utilities::PI / 4.0f);
				constraints["HIP-ABDOMEN"] = hipAbdomenConstraint;
				collision.getWorld().addConstraint(constraints["HIP-ABDOMEN"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f); 
				_localA.setOrigin(btVector3(0.0f, _boxExtents[ABDOMEN].x, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f); 
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[CHEST].x, 0.0f) * scale);
				auto abdomenChestConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[ABDOMEN], *bodies[CHEST], _localA, _localB);
				abdomenChestConstraint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, Utilities::PI / 2.0f);
				constraints["ABDOMEN-CHEST"] = abdomenChestConstraint;
				collision.getWorld().addConstraint(constraints["ABDOMEN-CHEST"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[CHEST].x, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[NECK].x, 0.0f) * scale);
				auto chestNeckConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[CHEST], *bodies[NECK], _localA, _localB);
				chestNeckConstraint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, Utilities::PI / 2.0f);
				constraints["CHEST-NECK"] = chestNeckConstraint;
				collision.getWorld().addConstraint(constraints["CHEST-NECK"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[NECK].x, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[HEAD].x, 0.0f) * scale);
				auto neckHeadConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[NECK], *bodies[HEAD], _localA, _localB);
				neckHeadConstraint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, Utilities::PI / 2.0f);
				constraints["NECK-HEAD"] = neckHeadConstraint;
				collision.getWorld().addConstraint(constraints["NECK-HEAD"].get(), true);


				/*********************************************************************************************************/
				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, -Utilities::PI / 4.0f * 5.0f);
				_localA.setOrigin(btVector3(-_boxExtents[L_U_LEG].x, -_boxExtents[HIP].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, -Utilities::PI / 4.0f * 5.0f);
				_localB.setOrigin(btVector3(0.0f, _boxExtents[L_U_LEG].y, 0.0f) * scale);
				auto hipLeftLowerLegConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[HIP], *bodies[L_U_LEG], _localA, _localB);
				hipLeftLowerLegConstraint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, 0.0f, 0.9f, 0.1f, 1.0f);
				constraints["HIP-L_U_LEG"] = hipLeftLowerLegConstraint;
				collision.getWorld().addConstraint(constraints["HIP-L_U_LEG"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, -_boxExtents[L_U_LEG].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, _boxExtents[L_L_LEG].y, 0.0f) * scale);
				auto leftUpperLegLowerLegContraint = std::make_shared<btHingeConstraint>(
					*bodies[L_U_LEG], *bodies[L_L_LEG], _localA, _localB);
				leftUpperLegLowerLegContraint->setLimit(-Utilities::PI * 3.0f / 4.0f, 0.0f, 0.9f, 0.1f, 1.0f);
				constraints["L_U_LEG-L_L_LEG"] = leftUpperLegLowerLegContraint;
				collision.getWorld().addConstraint(constraints["L_U_LEG-L_L_LEG"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, -_boxExtents[L_L_LEG].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, _boxExtents[L_FOOT].y, 0.0f) * scale);
				auto leftLegFootContraint = std::make_shared<btHingeConstraint>(
					*bodies[L_L_LEG], *bodies[L_FOOT], _localA, _localB);
				leftLegFootContraint->setLimit(-Utilities::PI / 2.0f, Utilities::PI / 4.0f, 0.9f, 0.1f, 1.0f);
				constraints["L_L_LEG-L_FOOT"] = leftLegFootContraint;
				collision.getWorld().addConstraint(constraints["L_L_LEG-L_FOOT"].get(), true);


				/*********************************************************************************************************/
				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 4.0f);
				_localA.setOrigin(btVector3(_boxExtents[R_U_LEG].x, -_boxExtents[HIP].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 4.0f);
				_localB.setOrigin(btVector3(0.0f, _boxExtents[R_U_LEG].y, 0.0f) * scale);
				auto hipRightUpperLegConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[HIP], *bodies[R_U_LEG], _localA, _localB);
				hipRightUpperLegConstraint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, 0.0f);
				constraints["HIP-R_U_LEG"] = hipRightUpperLegConstraint;
				collision.getWorld().addConstraint(constraints["HIP-R_U_LEG"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, -_boxExtents[R_U_LEG].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, _boxExtents[R_L_LEG].y, 0.0f) * scale);
				auto rightUpperLegLowerLegContraint = std::make_shared<btHingeConstraint>(
					*bodies[R_U_LEG], *bodies[R_L_LEG], _localA, _localB);
				rightUpperLegLowerLegContraint->setLimit(-Utilities::PI * 3.0f / 4.0f, 0);
				constraints["R_U_LEG-R_L_LEG"] = rightUpperLegLowerLegContraint;
				collision.getWorld().addConstraint(constraints["R_U_LEG-R_L_LEG"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, -_boxExtents[R_L_LEG].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, _boxExtents[R_FOOT].y, 0.0f) * scale);
				auto rightLegFootContraint = std::make_shared<btHingeConstraint>(
					*bodies[R_L_LEG], *bodies[R_FOOT], _localA, _localB);
				rightLegFootContraint->setLimit(-Utilities::PI / 2.0f, Utilities::PI / 4.0f);
				constraints["R_L_LEG-R_FOOT"] = rightLegFootContraint;
				collision.getWorld().addConstraint(constraints["R_L_LEG-R_FOOT"].get(), true);


				/*********************************************************************************************************/
				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI);
				_localA.setOrigin(btVector3(_boxExtents[CHEST].x, 0.0f, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f); 
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[L_COLLAR].y, 0.0f) * scale);
				auto chestLeftCollarConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[CHEST], *bodies[L_COLLAR], _localA, _localB);
				chestLeftCollarConstraint->setLimit(Utilities::PI / 2.0f, Utilities::PI / 2.0f, 0.0f);
				//spineLeftUpperArmContraint->setLimit(Utilities::PI, Utilities::PI, 0.0f);
				constraints["CHEST-L_COLLAR"] = chestLeftCollarConstraint;
				collision.getWorld().addConstraint(constraints["CHEST-L_COLLAR"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[L_COLLAR].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[L_U_ARM].y, 0.0f) * scale);
				auto leftCollarArmConstraint = std::make_shared<btHingeConstraint>(
					*bodies[L_COLLAR], *bodies[L_U_ARM], _localA, _localB);
				leftCollarArmConstraint->setLimit(0.0f, Utilities::PI / 2.0f);
				//hingeC->setLimit((-Utilities::PI / 2.0f), (0));
				constraints["L_COLLAR-L_U_ARM"] = leftCollarArmConstraint;
				collision.getWorld().addConstraint(constraints["L_COLLAR-L_U_ARM"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[L_U_ARM].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[L_L_ARM].y, 0.0f) * scale);
				auto leftUpperArmLowerArmContraint = std::make_shared<btHingeConstraint>(
					*bodies[L_U_ARM], *bodies[L_L_ARM], _localA, _localB);
				leftUpperArmLowerArmContraint->setLimit(0.0f, Utilities::PI / 2.0f);
				//hingeC->setLimit((-Utilities::PI / 2.0f), (0));
				constraints["L_U_ARM-L_L_ARM"] = leftUpperArmLowerArmContraint;
				collision.getWorld().addConstraint(constraints["L_U_ARM-L_L_ARM"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[L_L_ARM].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[L_HAND].y, 0.0f) * scale);
				auto leftArmHandConstaint = std::make_shared<btConeTwistConstraint>(
					*bodies[L_L_ARM], *bodies[L_HAND], _localA, _localB);
				leftArmHandConstaint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, 0.0f);
				constraints["L_L_ARM-L_HAND"] = leftArmHandConstaint;
				collision.getWorld().addConstraint(constraints["L_L_ARM-L_HAND"].get(), true);


				/*********************************************************************************************************/
				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, 0.0f, -Utilities::PI);
				_localA.setOrigin(btVector3(-_boxExtents[CHEST].x, 0.0f, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, 0.0f, Utilities::PI / 2.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[R_COLLAR].y, 0.0f) * scale);
				auto chestRightCollarConstraint = std::make_shared<btConeTwistConstraint>(
					*bodies[CHEST], *bodies[R_COLLAR], _localA, _localB);
				chestRightCollarConstraint->setLimit(Utilities::PI / 2.0f, Utilities::PI / 2.0f, 0.0f);
				constraints["CHEST-R_COLLAR"] = chestRightCollarConstraint;
				collision.getWorld().addConstraint(constraints["CHEST-R_COLLAR"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[R_COLLAR].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[R_U_ARM].y, 0.0f) * scale);
				auto rightCollarArmConstraint = std::make_shared<btHingeConstraint>(
					*bodies[R_COLLAR], *bodies[R_U_ARM], _localA, _localB);
				rightCollarArmConstraint->setLimit(0.0f, Utilities::PI / 2.0f);
				//hingeC->setLimit((-Utilities::PI / 2.0f), (0));
				constraints["R_COLLAR-R_U_ARM"] = rightCollarArmConstraint;
				collision.getWorld().addConstraint(constraints["R_COLLAR-R_U_ARM"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f); 
				_localA.setOrigin(btVector3(0.0f, _boxExtents[R_U_ARM].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f); 
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[R_L_ARM].y, 0.0f) * scale);
				auto rightUpperArmLowerArmConstraint = std::make_shared<btHingeConstraint>(
					*bodies[R_U_ARM], *bodies[R_L_ARM], _localA, _localB);
				rightUpperArmLowerArmConstraint->setLimit(0.0f, Utilities::PI / 2.0f);
				//hingeC->setLimit((-Utilities::PI / 2.0f), (0));
				constraints["R_U_ARM-R_L_ARM"] = rightUpperArmLowerArmConstraint;
				collision.getWorld().addConstraint(constraints["R_U_ARM-R_L_ARM"].get(), true);


				_localA.setIdentity(); _localB.setIdentity();
				_localA.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localA.setOrigin(btVector3(0.0f, _boxExtents[R_L_ARM].y, 0.0f) * scale);
				_localB.getBasis().setEulerZYX(0.0f, Utilities::PI / 2.0f, 0.0f);
				_localB.setOrigin(btVector3(0.0f, -_boxExtents[R_HAND].y, 0.0f) * scale);
				auto rightArmHandConstaint = std::make_shared<btConeTwistConstraint>(
					*bodies[R_L_ARM], *bodies[R_HAND], _localA, _localB);
				rightArmHandConstaint->setLimit(Utilities::PI / 4.0f, Utilities::PI / 4.0f, 0.0f);
				constraints["R_L_ARM-R_HAND"] = rightArmHandConstaint;
				collision.getWorld().addConstraint(constraints["R_L_ARM-R_HAND"].get(), true);

				/*********************************************************************************************************/

				return _boundingBoxes;
			}

		}
	}
}
