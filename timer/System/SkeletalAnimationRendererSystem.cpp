#include "SkeletalAnimationRendererSystem.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\transform.hpp>

#include "../Animation/AnimationSkeleton.hpp"
#include "../Animation/SkeletalAnimationPlayer.hpp"
#include "../Animation/SkeletalAnimationAction.hpp"
#include "../Input/Input.hpp"
#include "../Camera/Camera.hpp"
#include "../Collision/Ragdoll.hpp"
#include "../Collision/Collision.hpp"
#include "../Component/CollisionComponent.hpp"
#include "../Component/MeshComponent.hpp"
#include "../Component/MatrixComponent.hpp"
#include "../Component/MaterialComponent.hpp"
#include "../Component/PlayerControllerComponent.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Utility/AnimationUtilities.hpp"
#include "../Renderer/DeferredRenderer.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Texture/Texture.hpp"
#include "../World/World.hpp"


#include "../Texture/Material.hpp"

#include "../Animation/InverseKinematics/FABRIK.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			KinematicCollision::KinematicCollision(){

			}

			KinematicCollision::~KinematicCollision(){
				if (m_dynamicsWorld){
					for (int i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i--){
						m_dynamicsWorld->removeConstraint(m_dynamicsWorld->getConstraint(i));
					}

					for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--){
						btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
						m_dynamicsWorld->removeCollisionObject(obj);
					}
				}

				m_dynamicsWorld.release();
				m_dynamicsWorld = nullptr;

				m_solver.release();
				m_solver = nullptr;

				m_broadphase.release();
				m_broadphase = nullptr;

				m_dispatcher.release();
				m_dispatcher = nullptr;

				m_collisionConfiguration.release();
				m_collisionConfiguration = nullptr;
			}


			void KinematicCollision::init(){
				m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
				m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
				m_broadphase = std::make_unique<btDbvtBroadphase>();
				m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

				m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(
					m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get());

				m_dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
				//m_dynamicsWorld->setGravity(Physics::GRAVITY);
			}



			SkeletalAnimationRendererSystem::SkeletalAnimationRendererSystem() :
				m_ikSolver(std::make_unique<Animation::FABRIK>())
			{
				addRequired<SkeletalAnimationComponent>();
				addRequired<MeshComponent>();
				addRequired<CollisionComponent>();
				addRequired<MaterialComponent>();
			}

			SkeletalAnimationRendererSystem::~SkeletalAnimationRendererSystem(){
				//g_thread.join();
			}



			void SkeletalAnimationRendererSystem::init(){
				m_kinematicCollision.init();

				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_shader = _resourceManager.getShader("skeletalAnimation");

				m_boneUniformLocations.resize(Animation::c_maxBones);
				for (std::size_t i = 0; i < m_boneUniformLocations.size(); ++i){
					std::string _name = "u_bones[" + std::to_string(i) + "]";
					m_boneUniformLocations[i] = glGetUniformLocation(_shader.getProgram(), _name.c_str());
				}

				auto &_collision = _variables.m_collision;
				m_body1 = _collision->addBox(glm::vec3(0.0), glm::vec3(1.0f), glm::vec3(0.15f), 0.0f, true);
				_collision->setBodyCollisionFilter(*m_body1, Physics::GROUP_GHOST, Physics::GROUP_NOTHING);

				m_body2 = _collision->addBox(glm::vec3(0.0), glm::vec3(1.0f), glm::vec3(0.15f), 1.0f, false);
				_collision->setBodyCollisionFilter(*m_body2, Physics::GROUP_GHOST, Physics::GROUP_NOTHING);

				m_body2->forceActivationState(DISABLE_DEACTIVATION);

				/*CollisionComponent::addHingeConstraint(*_collision, "test", *m_body2, *m_body1,
					glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.0f, 0.5f, 0.0f),
					glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.0f, -0.5f, 0.0f),
					glm::vec2(-135.0f, 135.0f), 0.0f);//*/

				_collision->addConeTwistConstraint("test", *m_body2, *m_body1,
					glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.0f, 0.5f, 0.0f),
					glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.0f, -0.5f, 0.0f),
					{ glm::vec2(10.0f, 10.0f), 0.0f });//*/
			}

			void SkeletalAnimationRendererSystem::reloadAnimationConfigs(){
				for (auto &_entity : m_entities){
					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
					_animationComponent.reloadAnimationActionConfig();
				}
			}



			void SkeletalAnimationRendererSystem::drawShadows(){		
				auto &_variables = getWorld().getVariables();
				auto &_shadowFramebuffer = _variables.m_renderer->getShadowFramebuffer();

				const unsigned int _resolution = Graphics::Renderer::c_shadowResolution;
				glViewport(0, 0, _resolution, _resolution);

				_shadowFramebuffer.bind();
				{
					glEnable(GL_CULL_FACE);	
					glCullFace(GL_BACK);
					glDisable(GL_CULL_FACE);

					auto &_resourceManager = getWorld().getResourceManager();
					auto &_shadowShader = _resourceManager.getShader("skeletalAnimationShadow");
					_shadowShader.bind();
					{
						auto &_camera = _variables.m_camera;
						auto &_sunViewProjectionMatrix = _variables.m_renderer->getSunViewProjectionMatrix();

						for (auto &_entity : m_entities){
							auto &_meshComponent = _entity.getComponent<MeshComponent>();
							auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
							auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
							auto &_collisionComponent = _entity.getComponent<CollisionComponent>();

							//Don't let camera attached entities cast shadows
							if (!_matrixComponent.m_cameraTransform){
								continue;
							}


							const auto &_offsetBoneTransforms = _animationComponent.getOffsetTransforms();
							for (std::size_t i = 0; i < _offsetBoneTransforms.size(); ++i){
								std::string _name = "u_bones[" + std::to_string(i) + "]";
								_shadowShader.setUniform(_name.c_str(), _offsetBoneTransforms[i]);

								//TOOD will not work on shader
								//glUniformMatrix4fv(m_boneUniformLocations[i], 1, GL_FALSE, &_boneTransforms[i][0][0]);
							}


							glm::mat4 _modelMatrix = _matrixComponent.getMatrix();

							//If we rotate the entity with the camera (first person arms for example)
							/*if (!_matrixComponent.m_cameraTransform){
								glm::mat4 _cameraTransform = glm::translate(_camera->getLocation());
								_cameraTransform *= glm::rotate(_camera->getYaw(), 0.0f, 1.0f, 0.0f);
								//_cameraTransform *= glm::mat4(_camera->getRotationMatrix());
								_modelMatrix = _cameraTransform * _modelMatrix;
							}*/

							//Utilities::print(_matrixComponent.getLocation());
							_shadowShader.setUniform("u_modelViewProjectionMatrix",
								_sunViewProjectionMatrix * _modelMatrix);


							_meshComponent.bindVertexArray();
							std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
							glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
						}

						glBindVertexArray(0);
					}
					_shadowShader.unbind();

					glCullFace(GL_BACK);
					glEnable(GL_CULL_FACE);
				}
				_shadowFramebuffer.unbind();

				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);		
			}

			void SkeletalAnimationRendererSystem::drawOccluded(){			
				auto &_variables = getWorld().getVariables();
				glViewport(0, 0, _variables.m_screenWidth / 2, _variables.m_screenHeight / 2);

				auto &_occludedFramebuffer = _variables.m_renderer->getOccludedFramebuffer();
				/*_occludedFramebuffer.bind();
				{
					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH_TEST);
					glCullFace(GL_BACK);
					//glDisable(GL_DEPTH_TEST);

					auto &_resourceManager = getWorld().getResourceManager();
					auto &_skeletalAnimationColorShader = _resourceManager.getShader("skeletalAnimationColor");
					_skeletalAnimationColorShader.bind();

					glm::mat4 _viewMatrix = _variables.m_camera->getViewMatrix();
					glm::mat4 _projectionMatrix = _variables.m_camera->getProjectionMatrix();
					glm::mat4 _viewProjectionMatrix = _projectionMatrix * _viewMatrix;

					for (auto &_entity : m_entities){
						auto &_meshComponent = _entity.getComponent<MeshComponent>();
						auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
						auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
						auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();


						//update shader bone transform matrices
						const auto &_boneTransforms = _animationComponent.getOffsetTransforms();
						for (std::size_t i = 0; i < _boneTransforms.size(); ++i){
							std::string _name = "u_bones[" + std::to_string(i) + "]";
							_skeletalAnimationColorShader.setUniform(_name.c_str(), _boneTransforms[i]);

							//will not work on shader
							//glUniformMatrix4fv(m_boneUniformLocations[i], 1, GL_FALSE, &_boneTransforms[i][0][0]);
						}


						glm::mat4 _modelMatrix(1.0f);
						if (_collisionComponent.isRagdoll()){
							if (_collisionComponent.isKinematic()){
								_modelMatrix *= glm::translate(_matrixComponent.getLocation());
								_modelMatrix *= glm::toMat4(_matrixComponent.getRotation());
							}
						} else{
							_modelMatrix = _matrixComponent.getMatrix();
						}

						//TODO maybe
						if (_matrixComponent.m_cameraTransform){
							//_modelViewMatrix = _viewMatrix * _modelViewMatrix;
						}

						_skeletalAnimationColorShader.setUniform("u_color", glm::vec3(0.0f));
						_skeletalAnimationColorShader.setUniform("u_modelViewProjectionMatrix",
							_viewProjectionMatrix * _modelMatrix);

						_meshComponent.bindVertexArray();
						std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
						glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
					}

					glBindVertexArray(0);
					_skeletalAnimationColorShader.unbind();
				}
				_occludedFramebuffer.unbind();*/

				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
			}



			void SkeletalAnimationRendererSystem::equipKnife(){
				if (m_entities.empty())return;

				auto &_variables = getWorld().getVariables();
				auto &_input = _variables.m_input;

				auto &_entity = m_entities.back();
				auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
				auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
				auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

				const auto &_transforms = _animationComponent.getOffsetTransforms();

				//PRINT("KNIFE");

				if (/*_collisionComponent.isRagdoll() &&*/ _collisionComponent.isKinematic())
				{
					std::vector<unsigned int> _equipableBoneIds = { 
						_animationComponent.getBoneId("CHEST"),
						/*_animationComponent.getBoneId("L_HAND"),
						_animationComponent.getBoneId("R_FOOT"),
						_animationComponent.getBoneId("L_FOOT")*/
					};
	

					auto &_knifeEntities = getWorld().getEntities("brush");
					for (std::size_t i = 0; i < _knifeEntities.size() && i < _equipableBoneIds.size(); ++i)
					{
						auto &_knifeEntity = _knifeEntities[i];
						auto &_knifeCollisionComponent = _knifeEntity.getComponent<CollisionComponent>();

						if (_knifeCollisionComponent.isKinematic()){

							auto &_knifeMatrixComponent = _knifeEntity.getComponent<MatrixComponent>();

							auto &_skeleton = _animationComponent.getSkeleton();
							auto _boneId = _equipableBoneIds[i];
							auto _handTransform = _matrixComponent.getMatrix() * _transforms[_boneId];


							auto &_ragdollBoundingBoxes = _collisionComponent.getRagdollBoundingBoxes();
							auto &_handBoundingBox = _ragdollBoundingBoxes[_boneId];
							glm::vec3 _handScale = !_collisionComponent.isRagdoll() ? glm::vec3(1.0f) :
								glm::abs(_handBoundingBox[1] - _handBoundingBox[0]) * 0.5f;

							auto &_knifeMesh = _knifeEntity.getComponent<MeshComponent>().getMesh();
							glm::vec3 _knifeScale = glm::abs(_knifeMesh.m_max - _knifeMesh.m_min) * 0.5f;
							_knifeScale = _knifeScale * _knifeMatrixComponent.getScale();


							auto &_inverseOffset = _skeleton.m_boneTransforms[_boneId].m_originalInverseOffset;
							glm::vec3 _location = glm::vec3(_inverseOffset[3]);


							glm::vec3 _knifeOffset = glm::vec3(0.0f, -_knifeScale.y, _knifeScale.z) * 0.6f;
							glm::vec3 _handOffset = glm::vec3(-_handScale.x, 0.0f, 0.0f);
							_handTransform *= glm::translate(_location);
							
							
							//_handTransform *= glm::translate(_location);
							//_handTransform *= _inverseOffset;

							if (i % 2 == 1){
								_handTransform = _handTransform * glm::rotate(180.0f, 0.0f, 0.0f, 1.0f) *
									glm::translate(_knifeOffset + _handOffset);
							} else{
								_handTransform *= glm::translate(_knifeOffset + _handOffset);
							}

							glm::vec3 _kLoc = glm::vec3(_handTransform[3]);
							glm::quat _kRot = glm::quat_cast(glm::mat3(_handTransform));

							//PRINT("SET KNIFE FOR BONE ID: " << _boneId);
							_knifeMatrixComponent.setLocation(_kLoc);
							_knifeMatrixComponent.setRotation(_kRot);
							_knifeCollisionComponent.setLocation(_kLoc);
							_knifeCollisionComponent.setRotation(_kRot);
						}
					}

					

					if (_input->m_buttonLastDown == SDLK_k){
						auto &_collision = *_variables.m_collision;

						for (auto &_knifeEntity : getWorld().getEntities("brush")){
							auto &_knifeCollisionComponent = _knifeEntity.getComponent<CollisionComponent>();

							//change knife kinematics and set it to active so gravity works
							bool _isKinematic = _knifeCollisionComponent.isKinematic();
							_knifeCollisionComponent.setKinematic(!_isKinematic);

							auto _body = _knifeCollisionComponent.getBody();
							_body->activate(true);

							glm::vec3 _linearVelocity = Physics::convertVec3(_body->getLinearVelocity());

							float _bodyMass = _knifeCollisionComponent.getMass();
							float _multiplier = _bodyMass == 0.0f ? 0.0f : 100.0f / _bodyMass;

							if (glm::length(_linearVelocity) > _multiplier){
								_linearVelocity = glm::normalize(_linearVelocity) * _multiplier;
								_body->setLinearVelocity(Physics::convertVec3(_linearVelocity));
							}
						}
					}

				}
			}


			bool SkeletalAnimationRendererSystem::getRayClosestData(
				const glm::vec3 &rayStartLocation, glm::vec3 &location, glm::vec3 &normal)
			{
				auto &_world = getWorld();
				auto &_collision = _world.getVariables().m_collision;
				auto _rightFootRay = _collision->rayTracingAll(
					rayStartLocation, glm::vec3(0.0f, -1.0f, 0.0f), 15.0);

				if (_rightFootRay.hasHit())
				{
					int _hitBodyId = -1;

					for (int i = 0; i < _rightFootRay.m_hitFractions.size(); ++i){
						auto _collisionObject = _rightFootRay.m_collisionObjects[i];

						int _entityId = _collisionObject->getUserIndex();
						bool _validEntity = _entityId < MAX_ENTITIES && _entityId >= 0;
						if (_validEntity){
							auto _entity = _world.getEntity(_entityId);

							bool _isRagdoll = _entity.hasComponent<CollisionComponent>() ?
								_entity.getComponent<CollisionComponent>().isRagdoll() : false;

							_validEntity = _validEntity && !_isRagdoll;
						}
			

						auto &_hitFractions = _rightFootRay.m_hitFractions;
						bool _isClosestHit = _hitBodyId < 0 || _hitFractions[_hitBodyId] >= _hitFractions[i];

						int _shapeType = _collisionObject->getCollisionShape()->getShapeType();
						bool _validShapeType = _shapeType != CAPSULE_SHAPE_PROXYTYPE;

						if (_validEntity && _isClosestHit && _validShapeType){
							_hitBodyId = i;
						}
					}

					bool _hitValidBody = _hitBodyId != -1;

					if (_hitValidBody){
						auto _collisionObject = _rightFootRay.m_collisionObjects[_hitBodyId];
						int _entityId = _collisionObject->getUserIndex();
						std::string _entityName = getWorld().getEntityName(_entityId);

						//PRINT(_entityId << " " << _entityName);

						location = Physics::convertVec3(
							_rightFootRay.m_hitPointWorld[_hitBodyId]);	
						normal = Physics::convertVec3(
							_rightFootRay.m_hitNormalWorld[_hitBodyId]);

						//PRINT(_rightFootRay.m_hitFractions[_hitBodyId]);

					} else{
						location = glm::vec3(0.0f, 0.0f, 0.0f);
						normal = glm::vec3(0.0f, 1.0f, 0.0f);
					}


					return _hitValidBody;
				}

				return false;
			}


			void SkeletalAnimationRendererSystem::correctBoneTransforms(){
				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;

				for (auto &_entity : m_entities){
					if (!_entity.getComponent<CollisionComponent>().isKinematic()){
						continue;
					}

					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

					auto _offsetTransforms = _animationComponent.getOffsetTransforms();
					auto _transforms = _animationComponent.getTransforms();
					auto &_skeleton = _animationComponent.getSkeleton();
					auto &_bodies = _collisionComponent.getBodies();


					unsigned int _headId = _skeleton.m_boneMap["head"];
					auto _headOffset = _skeleton.m_boneTransforms[_headId].m_originalOffset;

					glm::vec3 _headLocation = Physics::convertVec3(
						_bodies[_headId]->getWorldTransform().getOrigin());
					glm::mat4 _headTransform = Utilities::getLookAtRotation(_camera->getLocation(), _headLocation) *
						glm::transpose(glm::toMat4(_matrixComponent.getRotation()));
					_headTransform = glm::mat4(1.0f);


					auto &_boundingBoxes = _collisionComponent.getRagdollBoundingBoxes();

					auto f_getUpdatedBoneTransform = [this, &_transforms, &_offsetTransforms, 
						&_matrixComponent, &_skeleton, &_boundingBoxes, &_bodies](unsigned int boneId)
					{
						auto _transform = _transforms[boneId];
						auto _boundingBox = _boundingBoxes[boneId];
						auto _offset = _skeleton.m_boneTransforms[boneId].m_originalOffset;

						//Location _localLocation = (_boundingBox[0] + _boundingBox[1]) * 0.5f;
						Location _localLocation = glm::vec3(_transforms[boneId][3]);
						Location _location = glm::vec3(_matrixComponent.getMatrix() * glm::vec4(_localLocation, 1.0f));

						//add raycast offset so ray start is always above the ground
						Scale _scale = Scale(glm::abs(_boundingBox[1] - _boundingBox[0]) * 0.5f);
						_location = _location + Location(0.0f, _scale.y * 2.0f, 0.0f);
						//_location = _location + Location(0.0f, _scale.y * 2.0f, _scale.z * 0.5f);

						/*auto &_body = _bodies[boneId];
						_location = Physics::convertVec3(_body->getWorldTransform().getOrigin());
						_location = _location + Location(0.0f, _scale.y * 2.0f, 0.0f);*/

						glm::vec3 _hitLocation, _hitNormal;
						glm::mat4 _boneTransform(1.0f);


						m_debugLines.push_back(std::make_pair(_location, _location + Location(0.0f, -2.0f, 0.0f)));

						if (getRayClosestData(_location, _hitLocation, _hitNormal))
						{
							glm::vec3 _eulerRotation = Utilities::getEulerRotationFromVector(_hitNormal);
							_eulerRotation = glm::vec3(_eulerRotation.x - 90.0f, 0.0f, 0.0f);

							glm::mat4 _rotationMatrix = glm::toMat4(Rotation(_eulerRotation));
							//glm::mat4 _rotation = glm::toMat4(Rotation(glm::vec3(45.0f, 0.0f, 0.0f)));


							glm::vec3 _localHitLocation = _hitLocation - _localLocation - _matrixComponent.getLocation();
							//Utilities::print(_localHitLocation);

							//Set X and Z axis to 0.0 to make sure foot only moves verticly
							_localHitLocation.x = _localHitLocation.z = 0.0f;

							//Make sure foot is always above the ground 
							//_localHitLocation.y += _scale.y * 1.5f;

							_boneTransform = glm::translate(_localHitLocation) * _rotationMatrix *
								glm::translate(0.0f, _scale.y * 2.0f, 0.0f);
						}

						return _boneTransform;
					};


					unsigned int _rightFootId = _skeleton.m_boneMap["right_foot"];
					glm::mat4 _rightFootOffset = _skeleton.m_boneTransforms[_rightFootId].m_originalOffset;
					glm::mat4 _rightFootTransform = f_getUpdatedBoneTransform(_rightFootId);

					unsigned int _leftFootId = _skeleton.m_boneMap["left_foot"];
					glm::mat4 _leftFootOffset = _skeleton.m_boneTransforms[_leftFootId].m_originalOffset;
					glm::mat4 _leftFootTransform = f_getUpdatedBoneTransform(_leftFootId);


					_offsetTransforms[_rightFootId] = glm::translate(glm::vec3(_transforms[_rightFootId][3])) *
						_rightFootTransform * _rightFootOffset;

					_offsetTransforms[_leftFootId] = glm::translate(glm::vec3(_transforms[_leftFootId][3])) *
						_leftFootTransform * _leftFootOffset;

					_offsetTransforms[_headId] = glm::translate(glm::vec3(_transforms[_headId][3])) *
						_headTransform * _headOffset;
					//_offsetTransforms[_headId] = _transforms[_headId] * _headTransform * _headOffset;


					//Utilities::print(glm::vec3(_transform[_leftFootId][3]));

					//TODO
					_animationComponent.updateOffsetTransforms();
					//_animationComponent.setOffsetTransforms(_offsetTransforms);
				}

			}


			void SkeletalAnimationRendererSystem::updateBoneTransforms(){
				auto &_input = getWorld().getVariables().m_input;
				double _animationTime = static_cast<double>(SDL_GetTicks()) / 1000.0;

				for (auto &_entity : m_entities){
					/*if (_entity.hasComponent<NPCControllerComponent>()){
					auto &_NPCControllerComponent = _entity.getComponent<NPCControllerComponent>();
					if (_NPCControllerComponent.isRecording())continue;
					}*/

					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();

					/*if (_animationComponent.isTypeLooping())
					{
						PRINT("LOOPING " << SDL_GetTicks());
						_animationComponent.playAction("action");
						//PRINT("PLAYING DEFAULT ACTION FOR: " << _entity.getName());
					}*/


					if (_collisionComponent.isKinematic() || !_collisionComponent.isRagdoll())
					{
						//TODO ARMS MOVE
						/*if (_input->m_mouseLeftHold){
						_animationComponent.playAction("hold");
						}else _animationComponent.playAction("rest");*/

						/*if (_input->isButtonHeld(SDL_SCANCODE_UP)){
						_animationComponent.playAction("point");
						}else
						_animationComponent.playAction("rest");*/


						if(_animationComponent.isTypeLooping())
						{
							
							//_animationComponent.playAction();
							//PRINT("PLAYING DEFAULT ACTION FOR: " << _entity.getName());
						} 
						else
						{

							//_animationComponent.playAction("walk");
							

							//if (!_variables.m_editing){
							/*if (_controllerState == CONTROLLER_SHIMMY &&_input->m_mouseRightHold){
								_animationComponent.playAction("shimmy_hold");
							} else if (_input->m_buttonLastDown == SDLK_c){
								_crouched = !_crouched;
								_animationComponent.playAction(_crouched ? "crouch" : "stand");
							} else if (_input->m_buttonLastDown == SDLK_SPACE){ // && _NPCConmponent.canJump()*
								_animationComponent.playAction("jump");
								_crouched = false;
							} else if (_input->m_buttonLastDown == SDLK_COMMA){
								_animationComponent.playAction("kick");
							} else if (_input->isWalking() || _input->isButtonHeld(KEYBOARD_BUTTON(J))){
								_animationComponent.playAction(_crouched ? "crouched_walk" : "walk");
								//PRINT("WALKING " << SDL_GetTicks())
							} else if (_input->m_buttonLastDown == SDLK_m){
								_animationComponent.playAction("complex");
							} else if (_input->isButtonHeld(KEYBOARD_BUTTON(LEFT))){
								_animationComponent.playAction("shimmy_left");
							} else if (_input->isButtonHeld(KEYBOARD_BUTTON(RIGHT))){
								//_animationComponent.playAction("shimmy_right");
							} else{
								_animationComponent.playAction("stand");
							}//*/
						}

							//}			
						//}

						//Update matrices with interpolation from animation player timeline
						_animationComponent.updateBoneTransforms(_animationTime);

					}
				}
			}


			void SkeletalAnimationRendererSystem::generateBoneRotationsFromLocations(
				const Animation::AnimationBoneTree *node,
				Animation::AnimationSkeleton &skeleton,
				std::vector<glm::mat4> &transforms)
			{
				if (node == nullptr){
					return;
				}

				unsigned int _boneId = node->m_boneId;
				glm::vec3 _offsetLocation(skeleton.m_boneTransforms[_boneId].m_inverseBvhOffset[3]);
				glm::vec3 _newLocation(transforms[_boneId][3]);


				glm::mat4 _newTransform = glm::mat4(glm::mat3(transforms[_boneId])); //Extract rotation		

				for (auto &_child : node->m_children){
					auto _childBoneId = _child->m_boneId;

					glm::vec3 _childOffsetLocation(skeleton.m_boneTransforms[_childBoneId].m_inverseBvhOffset[3]);
					glm::vec3 _offsetVector = glm::normalize(_childOffsetLocation - _offsetLocation);

					glm::vec3 _childNewLocation(transforms[_childBoneId][3]);
					glm::vec3 _newVector = glm::normalize(_childNewLocation - _newLocation);

					if (glm::distance(_newVector, _offsetVector) > 0.01f){
						_newTransform = glm::toMat4(Utilities::getQuatRotationFromVector(_newVector, -_offsetVector));
					}

					generateBoneRotationsFromLocations(_child.get(), skeleton, transforms);
				}

				//_newTransform = glm::mat4(1.0f);
				_newTransform[3] = glm::vec4(_newLocation, 1.0f); //Set location
				transforms[_boneId] = _newTransform;
			}

			void SkeletalAnimationRendererSystem::updateBoneCollision(){
				auto &_variables = getWorld().getVariables();
				//auto &_camera = _variables.m_camera;

				for (auto &_entity : m_entities){
					/*if (_entity.hasComponent<NPCControllerComponent>()){
					auto &_NPCControllerComponent = _entity.getComponent<NPCControllerComponent>();
					if (_NPCControllerComponent.isRecording())continue;
					}*/

					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

					
					//TODO make a mixed function that does both		
					auto _transforms = Utilities::setCollisionBodyTransformsFromBones(_entity); //For kinematic bodies

					//Update skirt
					Utilities::setBoneTransformsFromCollisionBodies(_entity); //For dynamic bones




					//Utilities::print(_transforms[_animationComponent.getBoneId("HIP")]);

					/*if (_collisionComponent.isKinematic() || !_collisionComponent.isRagdoll())
					{
						if (_collisionComponent.isRagdoll()){
							Utilities::setCollisionBodyTransformsFromBones(_entity);
						}
					} 
					else{
						Utilities::setBoneTransformsFromCollisionBodies(_entity); //For dynamic bones
					}//*/	


				}

			}


			void SkeletalAnimationRendererSystem::correctCollisionConstraints(){
				auto &_collision = getWorld().getVariables().m_collision;

				for (auto &_entity : m_entities){
					//continue;

					/*if (!_entity.getComponent<CollisionComponent>().isKinematic()){
						continue;
					}*/

					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();


					auto &_boneMap = _animationComponent.getSkeleton().m_boneMap;
					std::vector<unsigned int> _exeptions = {
						_boneMap["right_foot"], _boneMap["left_foot"]
					};

					auto f_isException = [&_exeptions](unsigned int id){
						for (auto _exeptionId : _exeptions){
							if (id == _exeptionId)return true;
						}return false;
					};



					auto &_bodies = _collisionComponent.getBodies();
					//auto &_thisWorld = _collision->getWorld();
					auto &_ikWorld = m_kinematicCollision.m_dynamicsWorld;


					for (auto &_constraint : _collisionComponent.getConstraints()){
						//_thisWorld->removeConstraint(_constraint.get());
					}

					for (int i = 0; i < _bodies.size(); ++i){
						auto &_body = _bodies[i];
						bool _exception = f_isException(i);

						//_thisWorld->removeRigidBody(_body.get());

						/*if (_exception){
							_body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
						} else _body->setCollisionFlags(_body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
						*/
						//_ikWorld->addRigidBody(_body.get());


						//if (f_isExeption(i))continue;

						if (i != _boneMap["right_lower_leg"] || i != _boneMap["left_lower_leg"]){
							//continue;
						}


						short _collisionGroup = _collisionComponent.getCollisionGroup();
						short _collidesWith = _collisionComponent.getCollidesWith();
						_collision->setKinematic(*_body, _exception, _collisionGroup, _collidesWith);

						if (!_collisionComponent.isKinematic() && _exception){
							_collision->addBodyLocation(*_body, glm::vec3(0.005f, 0.0f, 0.0f));
						}


						//_collision->setKinematic(_body.get(), false);
						//_body->setGravity(btVector3(0.0f, 0.0f, 0.0f));

						//TODO update
					}

					/*for (auto &_constraint : _collisionComponent.getConstraints()){
						_ikWorld->addConstraint(_constraint.get(), true);
					}*/



					//float _delta = 1.0f / 60.0f;
					/*float _delta = 1.0f / 60.0f;
					for (int i = 0; i < 30; i++)
						m_kinematicCollision.m_dynamicsWorld->stepSimulation(_delta, 2, _delta);
						*/



					//_collision->getWorld()->getConstraintSolver();
					//_collision->updateCollisionWorld();

					/*unsigned int _leftKneeId = Physics::Ragdoll::JOINT_LEFT_KNEE;
					auto _constraint = static_cast<btHingeConstraint*>(
					_collisionComponent.getConstraints()[_leftKneeId].get());
					*/

					/*{
					unsigned int _leftKneeId = Physics::Ragdoll::JOINT_LEFT_KNEE;
					auto _constraint = static_cast<btHingeConstraint*>(
					_collisionComponent.getConstraints()[_leftKneeId].get());


					if (_input->m_buttonHold[SDL_SCANCODE_B]){
					_collision->updateCollisionWorld();
					//_constraint->updateRHS(1.0 / 60.0f);
					//_constraint->enableAngularMotor(true, 0.5f, 10.0f);
					//_constraint->setMotorTarget(90.0f, 0.1f);
					} else{
					//_constraint->enableAngularMotor(false, 0.0f, 0.0f);
					}
					}*/




					for (auto &_constraint : _collisionComponent.getConstraints()){
						//_ikWorld->removeConstraint(_constraint.get());
					}

					//_collisionComponent.setKinematic(true);
					for (auto &_body : _bodies){
						//_ikWorld->removeRigidBody(_body.get());
						//_thisWorld->addRigidBody(_body.get());

						//TODO optimize by not readding the body again to set kinematic
						//_collision->setKinematic(_body.get(), true);
					}

					for (auto &_constraint : _collisionComponent.getConstraints()){
						//_thisWorld->addConstraint(_constraint.get(), true);
					}


					/*auto &_ragdollBoundingBoxes = _collisionComponent.getRagdollBoundingBoxes();

					std::vector<glm::mat4> _transforms;
					_transforms.resize(_bodies.size(), glm::mat4(1.0f));

					for (int i = 0; i < _bodies.size(); ++i){
					glm::mat4 _transform(1.0f);
					_bodies[i]->getWorldTransform().getOpenGLMatrix(glm::value_ptr(_transform));

					if (i >= 3 && i < 6)
					_transform *= glm::rotate(90.0f, 0.0f, 0.0f, 1.0f);
					else if (i >= 6 && i < 9)
					_transform *= glm::rotate(-90.0f, 0.0f, 0.0f, 1.0f);

					auto &_ragdollOffset = _ragdollBoundingBoxes[i];
					glm::vec3 _location = (_ragdollOffset[0] + _ragdollOffset[1]) * 0.5f;

					_transform *= glm::translate(-_location);
					_transforms[i] = _transform;
					}

					_animationComponent.setOffsetTransforms(_transforms);*/
				}
			}

			void SkeletalAnimationRendererSystem::crouchToAvoidObstacles(){

			}



			void SkeletalAnimationRendererSystem::preUpdate(float fps){
				m_debugLines.clear();


				equipKnife();
				drawShadows();
			}

			void SkeletalAnimationRendererSystem::update(float fps){

				auto &_variables = getWorld().getVariables();
				auto &_collision = _variables.m_collision;
				auto &_input = _variables.m_input;

				if (_input->m_buttonLastDown == SDLK_n){
					/*auto &_skeleton = _animationComponent.getSkeleton();

					for (auto &_body : _collisionComponent.getBodies()){
					_body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
					}*/

					/*for (auto _str : { "left_lower_leg", "left_foot" }){
					auto &_body = _collisionComponent.getBodies()[_skeleton.m_boneMap[_str]];
					_collision->setKinematic(_body.get(), !_body->isKinematicObject());

					}*/
				}


				auto &_playerEntity = getWorld().getEntity("player");
				auto &_playerController = _playerEntity.getComponent<PlayerControllerComponent>();
				auto &_playerMatrixComponent = _playerEntity.getComponent<MatrixComponent>();
				auto &_playerCollisionComponent = _playerEntity.getComponent<CollisionComponent>();
				auto &_playerAnimationComponent = _playerEntity.getComponent<SkeletalAnimationComponent>();

				auto &_nextTimelineKeyframe = _playerAnimationComponent.getAnimationPlayer().getNextKeyframe();
				if (_nextTimelineKeyframe.m_name == "walk_right" || _nextTimelineKeyframe.m_name == "walk_left" || 
					_nextTimelineKeyframe.m_name == "stand" || _nextTimelineKeyframe.m_name == "crouch")
				{
					auto &_skeleton = _playerAnimationComponent.getSkeleton();
					auto _nextKeyframe = _playerAnimationComponent.getKeyframe(_nextTimelineKeyframe.m_name);

					if (_nextKeyframe != nullptr/* && _playerAnimationComponent.getAnimationPlayer().justFinishedPlayingKeyframe()*/ /*&& SDL_GetTicks() - gg_timer > 1000*/)
					{
						//gg_timer = SDL_GetTicks();


						std::vector<glm::mat4> _transforms(_nextKeyframe->m_transforms.size(), glm::mat4(1.0f));
						_nextKeyframe->getFinalTransforms(_skeleton.m_animationBoneTreeRoot.get(), &_transforms);

						auto _originalTransforms = _transforms;
						std::unordered_map<std::string, glm::vec3> _endEffectorLocations;


						/*for (auto &_t : _transforms){
						Utilities::print(_t);
						}*/

						//PRINT(_nextKeyframe->m_name);
						
						glm::mat4 _modelMatrix = _playerMatrixComponent.getMatrix(); // glm::translate(_playerMatrixComponent.getLocation());


						glm::mat4 _animationCurrentModelMatrix = _modelMatrix * _skeleton.m_globalInverseTransform;
						glm::mat4 _inverseAnimationCurrentModelMatrix = glm::inverse(_animationCurrentModelMatrix);

						glm::vec3 _nextKeyframeLocation = _playerController.getLinearVelocity() * fps  * 1.0f;
						glm::mat4 _animationNextModelMatrix = glm::translate(_nextKeyframeLocation) *
							_modelMatrix * _skeleton.m_globalInverseTransform;
						glm::mat4 _inverseAnimationNextModelMatrix = glm::inverse(_animationNextModelMatrix);

						_animationNextModelMatrix = _animationCurrentModelMatrix;
						_inverseAnimationNextModelMatrix = _inverseAnimationCurrentModelMatrix;



						const float c_rayLength = 3.0f;
						const unsigned int c_maxChainLength = 2;
		
						auto &_boundingBoxes = _playerCollisionComponent.getRagdollBoundingBoxes();
						auto &_headBoundingBox = _boundingBoxes[_skeleton.getBoneId("HEAD")];
						const float _headOffset = (_headBoundingBox[1] - _headBoundingBox[0]).y + 0.2f;


						glm::vec3 _currentHeadLocationWorld(_animationCurrentModelMatrix * _transforms[_skeleton.getBoneId("HEAD")][3]);
						glm::vec3 _nextHeadLocationWorld(_animationNextModelMatrix * _transforms[_skeleton.getBoneId("HEAD")][3]);

						glm::vec3 _newCurrentHeadLocation = _currentHeadLocationWorld;
						glm::vec3 _newNextHeadLocation = _currentHeadLocationWorld; //TODO set as next



						auto _headCurrentLocationRaycast = _collision->rayTracingClosest(
							_currentHeadLocationWorld - glm::vec3(0.0, c_rayLength * 0.11115f, 0.0f),
							glm::vec3(0.0f, 1.0f, 0.0f), c_rayLength, Physics::GROUP_STATIC);
						if (_headCurrentLocationRaycast.hasHit())
						{
							glm::vec3 _hitLocationWorld = Physics::convertVec3(_headCurrentLocationRaycast.m_hitPointWorld);
							_hitLocationWorld.y -= _headOffset;

							

							//If new locations is closer to the head location
							if (_newCurrentHeadLocation.y > _hitLocationWorld.y){
								_newCurrentHeadLocation = _hitLocationWorld;
							}
						}

						auto _headNextLocationRaycast = _collision->rayTracingClosest(
							_nextHeadLocationWorld - glm::vec3(0.0, c_rayLength * 0.11115f, 0.0f),
							glm::vec3(0.0f, 1.0f, 0.0f), c_rayLength, Physics::GROUP_STATIC);
						if (_headNextLocationRaycast.hasHit())
						{
							glm::vec3 _hitLocationWorld = Physics::convertVec3(_headCurrentLocationRaycast.m_hitPointWorld);
							_hitLocationWorld.y -= _headOffset;

							//If new locations is closer to the head location
							if (_newNextHeadLocation.y > _hitLocationWorld.y){
								_newNextHeadLocation = _hitLocationWorld;
							}
						}


						//TODO change interpolation speed for different raycast locations
						//If the one hit that is at current location use next keyframe [1]
						//If the one hit that is further awaym use the next next keyframe [2]

						if (_headCurrentLocationRaycast.hasHit() || _headNextLocationRaycast.hasHit()){
							//If current location is lower (then use this one, else interpolate to the next location)
							bool _currentLowerThanNext = _newCurrentHeadLocation.y < _newNextHeadLocation.y;
							_currentLowerThanNext = false;

							glm::vec3 _headLocalAnimationLocation = _currentLowerThanNext ? _newCurrentHeadLocation : _newNextHeadLocation;

							float _headDisplacement = 0.0f;
							//if (_currentLowerThanNext){
								_headDisplacement = _newCurrentHeadLocation.y > _currentHeadLocationWorld.y ?
									0.0f : _currentHeadLocationWorld.y - _newCurrentHeadLocation.y;
							//} else{
							//	_headDisplacement = _newNextHeadLocation.y > _nextHeadLocationWorld.y ?
							//		0.0f : _nextHeadLocationWorld.y - _newNextHeadLocation.y;
							//}
							
							_headLocalAnimationLocation -= glm::vec3(0.0f, _headDisplacement * 0.5f, 0.0f);

							//_hitLocationWorld.y = glm::min(_hitLocationWorld.y - 1.5f, _headLocationWorld.y); //Limit to head height

							glm::mat4 _animationModelMatrix = _currentLowerThanNext ?
								_animationCurrentModelMatrix : _animationNextModelMatrix;
							glm::mat4 _inverseAnimationModelMatrix = _currentLowerThanNext ? 
								_inverseAnimationCurrentModelMatrix : _inverseAnimationNextModelMatrix;


	
							glm::vec3 _hitLocalLocation(_inverseAnimationModelMatrix * glm::vec4(_headLocalAnimationLocation, 1.0f));

							_endEffectorLocations["HEAD"] = _hitLocalLocation;
							//m_ikSolver->solve(_skeleton, _transforms, "HEAD", _hitLocalLocation, 5);


							Utilities::setTransformRotation(_skeleton, &_transforms, "HEAD", Rotation(45.0f * _headDisplacement, 0.0f, 0.0f));

							glm::vec3 _hipLocationWorld(_animationModelMatrix * _transforms[_skeleton.getBoneId("HIP")][3]);
							_hipLocationWorld.y -= _headDisplacement * 0.5f;

							glm::vec3 _hipLocationLocal(_inverseAnimationModelMatrix * glm::vec4(_hipLocationWorld, 1.0f));
							m_ikSolver->solve(_skeleton, _transforms, "HIP", _hipLocationLocal, 1);
						}


						//first... right hand, second... left hand
						std::pair<Location, Location> _nearHandCollisionPoints = 
							m_nearCollisionGrabber.getNearbyHandCollisionPoints(*_collision, _playerEntity);
						//PRINT("COUNT: " << _nearCollisionPoints.size());

						glm::vec3 _rightHandLocation = _nearHandCollisionPoints.first;
						glm::vec3 _leftHandLocation = _nearHandCollisionPoints.second;


						//RIGHT HAND
						if (glm::length(_rightHandLocation) > 0.0f){
							glm::vec3 _localLocation(_inverseAnimationCurrentModelMatrix * glm::vec4(_rightHandLocation, 1.0f));

							_endEffectorLocations["L_HAND"] = _localLocation;
							//m_ikSolver->solve(_skeleton, _transforms, "L_HAND", _localLocation, 3);


							Utilities::setTransformRotation(_skeleton, &_transforms, "L_HAND", Rotation(0.0f, 0.0f, 90.0f));
						}

						//LEFT HAND
						if (glm::length(_leftHandLocation) > 0.0f){
							glm::vec3 _localLocation(_inverseAnimationCurrentModelMatrix * glm::vec4(_leftHandLocation, 1.0f));

							_endEffectorLocations["R_HAND"] = _localLocation;
							//m_ikSolver->solve(_skeleton, _transforms, "R_HAND", _localLocation, 3);

							Utilities::setTransformRotation(_skeleton, &_transforms, "R_HAND", Rotation(0.0f, 0.0f, -90.0f));
						}



						{
							glm::vec3 _footWorldLocation(_animationNextModelMatrix * _transforms[_skeleton.getBoneId("L_FOOT")][3]);
							auto _raycast = _collision->rayTracingClosest(
							_footWorldLocation + glm::vec3(0.0, c_rayLength * 0.5f, 0.0f),
							glm::vec3(0.0f, -1.0f, 0.0f),
							c_rayLength,
							Physics::GROUP_STATIC
							);

							if (_raycast.hasHit()){
							glm::vec3 _hitLocationWorld = Physics::convertVec3(_raycast.m_hitPointWorld);
							_hitLocationWorld.y += 0.1f;

							glm::vec3 _hitLocalLocation;
							if (_nextTimelineKeyframe.m_name != "walk_right")
								_hitLocalLocation = glm::vec3(_inverseAnimationNextModelMatrix * glm::vec4(_hitLocationWorld, 1.0f));
							else
								_hitLocalLocation = glm::vec3(_inverseAnimationNextModelMatrix * glm::vec4(_hitLocationWorld + glm::vec3(0.0f, 0.5f, 0.0f), 1.0f));
						

							_endEffectorLocations["L_FOOT"] = _hitLocalLocation;
							//m_ikSolver->solve(_skeleton, _transforms, "L_FOOT", _hitLocalLocation, c_maxChainLength);

							if (_nextTimelineKeyframe.m_name == "walk_right"){
								Utilities::setTransformRotation(_skeleton, &_transforms, "L_FOOT", Rotation(40.0f, 0.0f, 0.0f)); 
							} else Utilities::setTransformRotation(_skeleton, &_transforms, "L_FOOT", Rotation(-15.0f, 0.0f, 0.0f));

							} 
						}//*/

						{
							glm::vec3 _footWorldLocation(_animationNextModelMatrix * _transforms[_skeleton.getBoneId("R_FOOT")][3]);

							auto _raycast = _collision->rayTracingClosest(
								_footWorldLocation + glm::vec3(0.0, c_rayLength * 0.5f, 0.0f),
								glm::vec3(0.0f, -1.0f, 0.0f),
								c_rayLength,
								Physics::GROUP_STATIC
							);

							if (_raycast.hasHit()){
								glm::vec3 _hitLocationWorld = Physics::convertVec3(_raycast.m_hitPointWorld);
								_hitLocationWorld.y += 0.1f;

								glm::vec3 _hitLocalLocation;
								if (_nextTimelineKeyframe.m_name != "walk_left")
									_hitLocalLocation = glm::vec3(_inverseAnimationNextModelMatrix * glm::vec4(_hitLocationWorld, 1.0f));
								else
									_hitLocalLocation = glm::vec3(_inverseAnimationNextModelMatrix * glm::vec4(_hitLocationWorld + glm::vec3(0.0f, 0.5f, 0.0f), 1.0f));

								_endEffectorLocations["R_FOOT"] = _hitLocalLocation;

								if (_nextTimelineKeyframe.m_name == "walk_left"){
									Utilities::setTransformRotation(_skeleton, &_transforms, "R_FOOT", Rotation(40.0f, 0.0f, 0.0f));
								} else Utilities::setTransformRotation(_skeleton, &_transforms, "R_FOOT", Rotation(-15.0f, 0.0f, 0.0f));
							}
						}//*/




						//Update locations using multiple end-effector IK					
						m_ikSolver->solveMultiple(_skeleton, _transforms, _endEffectorLocations);

						//Generate rotations from calculated locations
						generateBoneRotationsFromLocations(_skeleton.m_animationBoneTreeRoot.get(), _skeleton, _transforms);

						const bool c_smoothTransition = false;

						
						if (c_smoothTransition){
							//Will smoothly interpolate into pose transforms next keyframe
							_playerAnimationComponent.getAnimationPlayer().updateNextKeyframeFromFinalTransforms(_skeleton, _transforms);
						} else{
							//Will directly use the generated pose transforms
							_playerAnimationComponent.getAnimationPlayer().updateCurrentKeyframeFromFinalTransforms(_skeleton, _transforms);
						}
					}
				}

				//Update transforms from the animation player interpolated timeline
				updateBoneTransforms();
				

				for (auto &_entity : m_entities){
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

					updateBoneCollision();

				}


				glEnable(GL_DEPTH);
				glDisable(GL_CULL_FACE);
				//glEnable(GL_CULL_FACE);

				//auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				if (_variables.m_input->m_buttonLastDown == SDLK_r){
					//reloadAnimationConfigs();
				}


				auto &_camera = _variables.m_camera;
				glm::mat4 _viewMatrix = _camera->getViewMatrix();
				glm::mat3 _inverseViewMatrixRotation = glm::inverse(glm::mat3(_viewMatrix));
				glm::mat4 _projectionMatrix = _camera->getProjectionMatrix();
				glm::mat4 _lightViewProjectionMatrix = _variables.m_renderer->getSunViewProjectionMatrix();


				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
				auto &_GFramebuffer = _variables.m_renderer->getGFramebuffer();
				_GFramebuffer.bind();

				auto &_skeletalAnimationShader = _resourceManager.getShader("skeletalAnimation");
				_skeletalAnimationShader.bind();

				/*
				for (int i = 0; i < Meshes::Animation::c_maxBones; ++i){
					std::string _name = "u_bones[" + std::to_string(i) + "]";
					_skeletalAnimationShader.setUniform(_name.c_str(), glm::mat4(1.0f));
				}
				*/

				_skeletalAnimationShader.setUniform("u_viewMatrix", _viewMatrix);
				//_shadersetUniform("clipPlane", clipPlane);
				_skeletalAnimationShader.setUniform("u_projectionMatrix", _projectionMatrix);

				//PRINT("ANIMATED: " << m_entities.size());

				for (auto &_entity : m_entities){
					/*if (_entity.hasComponent<NPCControllerComponent>()){
						auto &_NPCControllerComponent = _entity.getComponent<NPCControllerComponent>();
						if (_NPCControllerComponent.isRecording())continue;
					}*/
					
					//HIDE for first person
					if (_camera->getThirdPersonDistance() < 1.5f)continue;

					//PRINT(_entity.getName());

					auto &_meshComponent = _entity.getComponent<MeshComponent>();
					auto &_materialComponent = _entity.getComponent<MaterialComponent>();
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();

					//PRINTING OFFSET MATRICES
					auto &_skeleton = _animationComponent.getSkeleton();
					for (auto &_transform : _skeleton.m_boneTransforms){
						const auto &_m = _transform.m_originalOffset;
						//Utilities::print(glm::vec3(_m[0][0], _m[1][1], _m[2][2]));
					}


					//update shader bone transform matrices
					const auto &_boneTransforms = _animationComponent.getOffsetTransforms();
					//PRINT("NUM_TRANSFORMS: " << _boneTransforms.size());

					for (std::size_t i = 0; i < _boneTransforms.size(); ++i){
						std::string _name = "u_bones[" + std::to_string(i) + "]";
						_skeletalAnimationShader.setUniform(_name.c_str(), _boneTransforms[i]);

						//will not work on shader
						//glUniformMatrix4fv(m_boneUniformLocations[i], 1, GL_FALSE, &_boneTransforms[i][0][0]);
					}

					

					const auto &_previousBoneTransforms = _animationComponent.getPreviousOffsetTransforms();
					for (std::size_t i = 0; i < _previousBoneTransforms.size(); ++i){
						std::string _name = "u_prevBones[" + std::to_string(i) + "]";
						_skeletalAnimationShader.setUniform(_name.c_str(), _previousBoneTransforms[i]);

						//will not work on shader reload
						//glUniformMatrix4fv(m_boneUniformLocations[i], 1, GL_FALSE, &_boneTransforms[i][0][0]);
					}

					//update previous offsets for next frame
					_animationComponent.setPreviousOffsetTransforms(_boneTransforms);


					glm::mat4 _modelViewMatrix = _matrixComponent.getMatrix();
					glm::mat4 _previousModelViewMatrix = _matrixComponent.getPreviousMatrix();
					if (_matrixComponent.m_cameraTransform){
						//glEnable(GL_DEPTH_TEST);
						//glDepthMask(GL_TRUE);

						_modelViewMatrix = _viewMatrix * _modelViewMatrix;

						if (_entity.getName() == "player"){
							_previousModelViewMatrix = _camera->getViewMatrix() * _previousModelViewMatrix;

							/*auto &_skeleton = _animationComponent.getSkeleton();
							Location _hipLocation = glm::vec3(_animationComponent
								.getOffsetTransforms()[_skeleton.m_boneMap["hip"]][3]);*/

							//_matrixComponent.setLocation(_hipLocation);
							//_entity.getComponent<PlayerControllerComponent>().setLocation(_hipLocation);						

						} else{
							_previousModelViewMatrix = _camera->getPreviousViewMatrix() * _previousModelViewMatrix;
						}
						//_previousModelViewMatrix = _camera->getPreviousViewMatrix() * _modelMatrix;
						//TODO camera motion blur doenst look correct if camera is moving with the object

						//_previousModelViewMatrix = _viewMatrix * _modelMatrix;
					} else{
						//glDisable(GL_DEPTH_TEST);
						//glDepthMask(GL_FALSE);
					}


					_skeletalAnimationShader.setUniform("u_color", _materialComponent.getColor());
					_skeletalAnimationShader.setUniform("u_scale", _matrixComponent.getScale());
					_skeletalAnimationShader.setUniform("u_modelViewMatrix", _modelViewMatrix);
			
					_skeletalAnimationShader.setUniform("u_prevModelViewProjectionMatrix",
						_projectionMatrix * _previousModelViewMatrix);
					_skeletalAnimationShader.setUniform("u_modelViewProjectionMatrix",
						_projectionMatrix * _modelViewMatrix);

					
					_meshComponent.bindVertexArray();
					{
						auto *_material = _materialComponent.getMaterial().get();

						_material = _resourceManager.getMaterialPtr("paper").get();

						Textures::bind2DTexture(_material->getAlbedoTexture(), 0);
						Textures::bind2DTexture(_material->getNormalTexture(), 1);
						Textures::bind2DTexture(_material->getRoughnessTexture(), 2);
						Textures::bind2DTexture(_material->getMetallicTexture(), 3);
						Textures::bind2DTexture(_material->getOcclusionTexture(), 4);
						Textures::bind2DTexture(_material->getHeightTexture(), 5);

						
						std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
						glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
					}		
				}

				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);

				glBindVertexArray(0);
				Textures::unbind2DTexture();

				_GFramebuffer.unbind();
				_skeletalAnimationShader.unbind();
			}

			void SkeletalAnimationRendererSystem::postUpdate(float fps){
				drawOccluded();

				auto &_variables = getWorld().getVariables();
				auto &_finalFramebuffer = _variables.m_renderer->getFinalFramebuffer();

				_finalFramebuffer.bind();
				{
					glUseProgram(0);			

					glLineWidth(5.0f);
					glBegin(GL_LINES);
					{
						glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

						for (auto &_debugLine : m_nearCollisionGrabber.getDebugLines()){
							glm::vec3 _from = _debugLine.first;
							glVertex3f(_from.x, _from.y, _from.z);

							glm::vec3 _to = _debugLine.second;
							glVertex3f(_to.x, _to.y, _to.z);
						}


						glColor4f(0.0f, 1.0f, 0.0f, 1.0f);


						/*auto &_playerEntity = getWorld().getEntity("player");
						auto &_matrixComponent = _playerEntity.getComponent<MatrixComponent>();
						auto &_playerComponent = _playerEntity.getComponent<PlayerControllerComponent>();

						glm::vec3 _side = _variables.m_camera->getSideVector();

						_side = Utilities::getVectorFromRotation(Rotation(0.0f, _playerComponent.m_latestYRotation, 0.0f), 
							glm::vec3(1.0f, 0.0f, 0.0f));


						glm::vec3 _from = _matrixComponent.getLocation();
						glVertex3f(_from.x, _from.y, _from.z);

						glm::vec3 _to = _from + _side;
						glVertex3f(_to.x, _to.y, _to.z);

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);*/
					}
					glEnd();
				}
				_finalFramebuffer.unbind();
			}



			void SkeletalAnimationRendererSystem::onEntityAdded(Entity &entity){
				auto &_animationComponent = entity.getComponent<SkeletalAnimationComponent>();
				auto &_meshComponent = entity.getComponent<MeshComponent>();
				auto &_collisionComponent = entity.getComponent<CollisionComponent>();

				//to set initial state
				//_animationComponent.playAction("walk");


				auto &_resourceManager = getWorld().getResourceManager();
				auto &_shader = _resourceManager.getShader<Shaders::Shader>("skeletalAnimation");

				auto &_variables = getWorld().getVariables();
				auto &_collision = _variables.m_collision;

				if (_collisionComponent.isRagdoll()){
					auto &_world = m_kinematicCollision.m_dynamicsWorld;

					for (auto &_body : _collisionComponent.getBodies()){
						//_world->addRigidBody(_body.get());
					}
				
					for (auto &_constraint : _collisionComponent.getConstraints()){
						//_world->addConstraint(_constraint.get());
					}
				}


				//TODO check if is ragdoll legit			


				//TODO BVH
				/*for (auto &_array : m_skeletons){
				if (!_array.empty()){
				auto &_sac = _array[0].getComponent<SkeletalAnimationComponent>();
				if (_entitySkeleton == _sac.getSkeleton()){

				auto _entityItr = std::find_if(_array.begin(), _array.end(),
				[&_meshComponent](const Entity &e) -> bool{
				return _meshComponent.getMesh() ==
				e.getComponent<MeshComponent>().getMesh();
				});

				_animationComponent.setOffset(std::accumulate(_array.begin(), _entityItr, 0,
				[](unsigned int c, const Entity &e) -> unsigned int{
				auto _mesh = e.getComponent<MeshComponent>().getMesh();
				return c + _mesh->m_vertexData.size();
				}));

				_array.push_back(entity);
				_found = true;

				break;
				}
				}
				}*/

				/*if (!_found){
				m_skeletons.push_back(std::vector<Entity>({ entity }));
				}*/

				//TODO SORT TEXTURES

				/*std::sort(m_entities.begin(), m_entities.end(),
				[](const Entity &e1, const Entity &e2) -> bool {
				auto &_sac1 = e1.getComponent<SkeletalAnimationComponent>();
				auto &_sac2 = e2.getComponent<SkeletalAnimationComponent>();
				return _sac1.getSkeleton() < _sac2.getSkeleton();
				});*/

				//TODO remove
				/*for (auto &_vertexData : _meshComponent.getMesh()->m_vertexData){
				Utilities::print(_vertexData.m_uv);
				}*/

				_meshComponent.bindVertexArray();
				_animationComponent.getBuffer().bind();

				const GLsizei _stride = sizeof(Animation::AnimationSkeleton::VertexBoneData);
				const unsigned int _offset = _animationComponent.m_offset;

				glEnableVertexAttribArray(4);
				glVertexAttribIPointer(4, 4, GL_INT, _stride,
					reinterpret_cast<const GLvoid*>(_offset * _stride));

				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, _stride,
					reinterpret_cast<const GLvoid*>(_offset * _stride + sizeof(glm::vec4)));

				glBindVertexArray(0);

			}

			void SkeletalAnimationRendererSystem::onEntityRemoved(Entity &entity){
				auto &_animationComponent = entity.getComponent<SkeletalAnimationComponent>();
				auto _entitySkeleton = _animationComponent.getSkeleton();


				//TODO BVH 
				/*for (auto &_array : m_skeletons){
				if (!_array.empty()){
				auto &_sac = _array[0].getComponent<SkeletalAnimationComponent>();
				if (_entitySkeleton == _sac.getSkeleton()){
				_array.erase(std::remove(_array.begin(), _array.end(), entity), _array.end());
				}
				}
				}*/

			}

		}
	}
}