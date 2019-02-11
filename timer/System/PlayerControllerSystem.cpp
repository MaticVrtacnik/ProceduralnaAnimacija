#include "PlayerControllerSystem.hpp"

#include <glm\gtx\transform.hpp>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "../Animation/AnimationSkeleton.hpp"
#include "../Animation/SkeletalAnimationPlayer.hpp"
#include "../Animation/SkeletalAnimationTransform.hpp"
#include "../Camera/Camera.hpp"
#include "../Component/CollisionComponent.hpp"
#include "../Component/MatrixComponent.hpp"
#include "../Component/PlayerControllerComponent.hpp"
#include "../Component/SkeletalAnimationComponent.hpp"
#include "../Component/MaterialComponent.hpp"
#include "../Texture/Material.hpp"
#include "../GUI/GUIWindow.hpp"
#include "../GUI/GUITab.hpp"
#include "../GUI/GUIWindowStack.hpp"
#include "../GUI/GUIDropdown.hpp"
#include "../GUI/GUISlider.hpp"
#include "../GUI/GUI.hpp"
#include "../Renderer/DeferredRenderer.hpp"
#include "../Entity/EntityCreator.hpp"
#include "../Utility/AnimationUtilities.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/RandomUtilities.hpp"
#include "../Utility/ColorUtilities.hpp"
#include "../Utility/InterpolationUtilities.hpp"
#include "../System/RendererSystem.hpp"
#include "../World/World.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			using namespace Controller;
			using namespace Physics;


			PlayerControllerSystem::PlayerControllerSystem(){
				//addExcluded<CollisionComponent>();
				addRequired<PlayerControllerComponent>();
				addRequired<MatrixComponent>();
			}

			PlayerControllerSystem::~PlayerControllerSystem(){

			}


			void PlayerControllerSystem::init(){
				auto &_world = getWorld();
				auto &_variables = _world.getVariables();
				auto &_gui = _variables.m_gui->getIndex();
				auto &_tools = _gui.getChild<GUI::GUIWindow>("tools");

				_tools.getChild<GUI::GUIButton>("player").setAction([&_world]{
					auto &_variables = _world.getVariables();
					auto &_collision = _variables.m_collision;
					bool &_editing = _variables.m_editing;
					_editing = !_editing;

					/*if (_editing){
						for (auto &_entity : _world.getEntities()){
							if (_entity.hasComponent<CollisionComponent>()){
								auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
								short _collisionGroup = _collisionComponent.getCollisionGroup();
								short _collidesWith = _collisionComponent.getCollidesWith();

								for (auto _body : _collisionComponent.getBodies()){
									//TODO maybe change collision filters
									_collision->setKinematic(*_body, true, _collisionGroup, _collidesWith);
									_collision->setMass(*_body, 0.0f, _collisionGroup, _collidesWith);
								}
							}
						}
					}
					else{
						for (auto &_entity : _world.getEntities()){
							if (_entity.hasComponent<CollisionComponent>()){
								auto &_cc = _entity.getComponent<CollisionComponent>();
								_cc.setKinematic(*_collision, _cc.isKinematic());
								_cc.setMass(*_collision, _cc.getMass());
							}
						}
					}*/
				});


				//Set action for player color editor
				auto &_colorEditor = _gui.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab")
					.getWindow("environment").getChild<GUI::GUIWindowStack>("stack").getWindow("colorEditor");

				auto &_clothingDropdown = _colorEditor.getChild<GUI::GUIDropdown>("clothing");
				_clothingDropdown.setOnChangeAction([this, &_colorEditor](std::string newValue){
					if (m_clothingColors.find(newValue) == m_clothingColors.end()){
						//Unable to find clothing color -> default to white
						m_clothingColors[newValue] = glm::vec3(1.0f, 1.0f, 1.0f);
					}

					glm::vec3 _clothingColor = m_clothingColors[newValue];
					_colorEditor.getChild<GUI::GUISlider>("colorR").setValue(_clothingColor.r);
					_colorEditor.getChild<GUI::GUISlider>("colorG").setValue(_clothingColor.g);
					_colorEditor.getChild<GUI::GUISlider>("colorB").setValue(_clothingColor.b);
				});


				//TODO only working when manually changing color (dragging slider)
				auto &_resetButton = _colorEditor.getChild<GUI::GUIButton>("reset");
				_resetButton.setAction([this, &_colorEditor]
				{
					auto &_variables = getWorld().getVariables();
					auto &_resourceManager = getWorld().getResourceManager();

					m_originalClothingTexture = _resourceManager.getTextureId(
						//"../materials/protagonist/albedoCOLORS.png");
						"../materials/protagonist/albedoHSV.png");
					

					auto &_copyShader = _resourceManager.getShader("copy");
					m_clothingRecollorFramebuffer.bind();
					_copyShader.bind();
					{
						glViewport(0, 0, 4096, 4096);
						glBindVertexArray(_variables.m_renderer->getScreenQuadVertexArray());

						Textures::bind2DTexture(m_originalClothingTexture, 0);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

						glBindVertexArray(0);
						glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
					}
					_copyShader.unbind();
					m_clothingRecollorFramebuffer.unbind();

					//TODO move to function
					if (!m_entities.empty()){
						auto &_entity = m_entities.back();
						auto &_materialComponent = _entity.getComponent<MaterialComponent>();

						_materialComponent.getMaterial()->setTextureId(
							m_clothingRecollorFramebuffer.getTexture(0), Textures::TEXTURE_ALBEDO);

						auto &_textureGui = _colorEditor.getChild<GUI::GUIBase>("texture");
						_textureGui.setTexture(m_clothingRecollorFramebuffer.getTexture(0));
					}


					setDefaultClothingColors();

					//Remove all set colors
					/*m_clothingColors.clear();
					_colorEditor.getChild<GUI::GUISlider>("colorR").setValue(0.0f);
					_colorEditor.getChild<GUI::GUISlider>("colorG").setValue(0.0f);
					_colorEditor.getChild<GUI::GUISlider>("colorB").setValue(0.0f);*/
				});


				//Init framebuffer for recolloring player's clothing
				m_clothingRecollorFramebuffer.init(
					c_clothingMapResolution, c_clothingMapResolution, false);
				m_clothingRecollorFramebuffer.addTexture(Textures::createEmptyTexture(
					c_clothingMapResolution, c_clothingMapResolution,
					false, GL_RGBA, GL_NEAREST, GL_RGBA, GL_UNSIGNED_INT));

				//Init original colors texture
				_resetButton.getAction()();
			}


			void PlayerControllerSystem::correctCollisionConstraints(Entity &ragdollEntity, 
				const std::vector<SkeletalAnimationTransform> &boneTransforms,
				const glm::mat4 &inverseModelMatrix)
			{
				if (!ragdollEntity.isValid()){
					return;
				}

				auto &_collision = getWorld().getVariables().m_collision;
				auto &_animationComponent = ragdollEntity.getComponent<SkeletalAnimationComponent>();
				//auto &_collisionComponent = ragdollEntity.getComponent<CollisionComponent>();
				auto &_matrixComponent = ragdollEntity.getComponent<MatrixComponent>();

				/*auto &_bodies = _collisionComponent.getBodies();
				short _collisionGroup = _collisionComponent.getCollisionGroup();
				short _collidesWith = _collisionComponent.getCollidesWith();*/

				/*for (auto &_body : _bodies){
					_collision->setKinematic(*_body, false, _collisionGroup, _collidesWith);
				}*/


				auto _transforms = _animationComponent.getTransforms();
				if (_transforms.empty())return;

				for (auto &_transform : boneTransforms){
					unsigned int _boneId = _animationComponent.getBoneId(_transform.m_name);
					
					//Utilities::print(_transforms[_boneId]);

					_transforms[_boneId] = inverseModelMatrix * glm::translate(_transform.m_location) *
						glm::toMat4(_transform.m_rotation) * glm::scale(_matrixComponent.getScale());


					//_collision->setKinematic(*_body, true, _collisionGroup, _collidesWith);
					//_collision->setMass(*_body, 0.0f, _collisionGroup, _collidesWith);
					
					//if (!_collisionComponent.isKinematic()){
						//_collision->addBodyLocation(*_body, glm::vec3(0.0f, 0.005f, 0.0f));
					//_collision->setBodyLocation(*_body, _transform.m_location);

					//TODO add bool members to SkeletalTransform
					if (_transform.m_rotation != Rotation(0.0f)){
						//_collision->setBodyRotation(*_body, _transform.m_rotation);
					}
					//
					//_collision->setBodyScale(*_body, _transform.m_scale);

					//}
				}

				_animationComponent.setTransforms(_transforms);



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


			bool PlayerControllerSystem::controlMovement(glm::vec3 *movementDirection, 
				float *speed, const float yaw, bool canRun)
			{
				if (m_entities.empty())return false;

				if (movementDirection == nullptr || speed == nullptr){
					return false;
				}

				auto &_variables = getWorld().getVariables();
				auto &_input = _variables.m_input;

				auto &_playerEntity = m_entities.front();
				auto &_playerComponent = _playerEntity.getComponent<PlayerControllerComponent>();


				if (_input->isButtonHeld(SDL_SCANCODE_W)){
					float s = sin(glm::radians(yaw));
					float c = cos(glm::radians(yaw));
					*movementDirection -= glm::vec3(s, 0.0f, c);
				}

				if (_input->isButtonHeld(SDL_SCANCODE_A)){
					float s = sin(glm::radians(yaw + 90.0f));
					float c = cos(glm::radians(yaw + 90.0f));
					*movementDirection -= glm::vec3(s, 0.0f, c);
				}

				if (_input->isButtonHeld(SDL_SCANCODE_S)){

					float s = sin(glm::radians(yaw + 180.0f));
					float c = cos(glm::radians(yaw + 180.0f));
					*movementDirection -= glm::vec3(s, 0.0f, c);
				}

				if (_input->isButtonHeld(SDL_SCANCODE_D)){
					float s = sin(glm::radians(yaw + 270.0f));
					float c = cos(glm::radians(yaw + 270.0f));
					*movementDirection -= glm::vec3(s, 0.0f, c);
				}

				bool _stoppedRunning = _input->isButtonUp(KEYBOARD_BUTTON(LSHIFT));
				if (_stoppedRunning || !_input->isWalking() || !canRun){
					_playerComponent.m_accelerationFactor = 1.0f;
				}

				//Increase speed if running
				if (_input->isButtonHeld(SDL_SCANCODE_LSHIFT) && canRun){
					_playerComponent.m_accelerationFactor = glm::min(_playerComponent.m_maxAccelerationFactor,
						_playerComponent.m_accelerationFactor + _playerComponent.m_accelerationFactor * 0.08f);
						//m_accelerationFactor + 0.01f);

					*speed *= 1.0 + _playerComponent.m_accelerationFactor;
				}


				if (_input->isWalking()){
					*movementDirection = glm::normalize(*movementDirection);
				}

				return _input->isWalking();
			}



			void PlayerControllerSystem::preUpdate(float fps){
				if (m_entities.empty()){
					return;
				}

				m_debugLines.clear();

				//Clothing color editor handling
				updatePlayerColors();


				auto &_variables = getWorld().getVariables();
				auto &_entity = m_entities.front();

				auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
				auto &_playerComponent = _entity.getComponent<PlayerControllerComponent>();

				Location _cameraOffset(0.0f, _playerComponent.getCurrentHeight(), 0.0f);
				//_variables.m_camera->setLocation(_playerComponent.getLocation() + _cameraOffset);
				//_matrixComponent.setLocation(_playerComponent.getLocation());


				_playerComponent.testContacts();
			
				//Increase fov at higher speeds
				auto &_camera = _variables.m_camera;
				float _previousFov = _camera->getFov();
				float _newFov = DEFAULT_FOV + glm::length(_playerComponent.getLinearVelocity()) * 10.0f;
				//_camera->setFov(glm::mix(_previousFov, _newFov, 0.1f));



				//PRINT(_camera->getFov());

	
				/*if (_variables.m_input->m_buttonDown == SDLK_c){
					_playerComponent.m_state = CONTROLLER_CROUCH;
				}*/

				//TODO remove from player (camera zoomout on death)
				/*auto _bodyEntity = getWorld().getEntity("body");
				if (_bodyEntity.isValid()){
					auto &_collisionComponent = _bodyEntity.getComponent<CollisionComponent>();
					glm::mat2x3 _boundingBox = _collisionComponent.getBoundingBox();
					glm::vec3 _scale = glm::abs(_boundingBox[1] - _boundingBox[0]);

					auto &_entity = m_entities.front();
					auto &_playerComponent = _entity.getComponent<PlayerControllerComponent>();

					auto _collisionShape = _playerComponent.getBody()->getCollisionShape();
					auto _localScale = Physics::convertVec3(_collisionShape->getLocalScaling());
					_collisionShape->setLocalScaling(Physics::convertVec3(_scale / _localScale));
				}*/
			}

			void PlayerControllerSystem::update(float fps){
				if (m_entities.empty()){
					//dont update player if not in player mode
					return;
				}

				fps = 60.0f;
				
				
				auto &_entity = m_entities.front();
				auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
				auto &_playerComponent = _entity.getComponent<PlayerControllerComponent>();
				auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();

				//TODO use in fall animation
				//PRINT("VELOCITY: " << _playerComponent.getVerticalVelocity());


				//TODO correct invalid #IO values
				/*if (!_animationComponent.getTransforms().empty()){
					Utilities::print(_matrixComponent.getMatrix());
				}*/

				auto &_variables = getWorld().getVariables();
				if (_variables.m_editing){				
					_playerComponent.setLocation(_variables.m_camera->getLocation());
					//positive because raycast required opposite
					m_previousRaycastNormal = glm::vec3(0.0f, 1.0f, 0.0f);
					return;
				}

				auto &_collision = _variables.m_collision;
				auto &_input = _variables.m_input;
				auto &_camera = _variables.m_camera;
				

				float _yaw = _variables.m_camera->getYaw();

				glm::vec3 _movementDirection(0.0f, -0.00001f, 0.0f);
				float _movementSpeed = c_playerWalkingSpeed * (1.0f / fps);


				_playerComponent.getBody()->getWorldTransform().setRotation(
					btQuaternion(btVector3(0.0f, 1.0f, 0.0f), glm::radians(_yaw + 180.0f)));


				//PRINT("STATE: " << _playerComponent.m_state);

				bool _justLanded = _playerComponent.canJump() && !m_previousCanJump;
				float _movementBounceOffset = 0.0f;
				
				if (_playerComponent.m_state == CONTROLLER_CROUCH){
					bool _moving = controlMovement(
						&_movementDirection, &_movementSpeed, _yaw, false);

					if (_input->isWalking()){
						
					} else{
						//TODO Invert lean for slowing down (lean back)
						//_movementLean = -glm::length(_playerComponent.getWalkDirectionChange()) * 1000.0f;
					}

					

					if (_moving){//TODO add constant for crouch walk factor
						_playerComponent.setInterpolatedFactorWalkDirection(
							_movementDirection * _movementSpeed * 0.75f);
						_animationComponent.playAction("boxing");

						glm::vec3 _eulerAngles(Utilities::
							getEulerRotationFromVector(_movementDirection));

						float _mod = fmod(_eulerAngles.y - _playerComponent.m_latestYRotation, 360.0f);
						float _angle = fmod(_mod + 540.0f, 360.0f) - 180.0f;

						float _currentYRotation = _playerComponent.m_latestYRotation;
						_playerComponent.m_latestYRotation = Utilities::limitAngle(glm::mix(
							_playerComponent.m_latestYRotation,
							_playerComponent.m_latestYRotation + _angle, 
							0.1f));

						float _YRotationChange = _playerComponent.m_latestYRotation - _currentYRotation;
						PRINT(_YRotationChange);

						//Same as just multiplying by 0.9f (reduce velocity lean when crouching)
						_playerComponent.m_latestLeanAngle = glm::mix(_playerComponent.m_latestLeanAngle, 0.0f, 0.1f);
						_matrixComponent.setRotation(_playerComponent.m_latestLeanAngle,
							_playerComponent.m_latestYRotation + 180.0f, 0.0f);

						_playerComponent.m_latestZRotation = glm::mix(
							_playerComponent.m_latestZRotation, _YRotationChange, 0.1f);
					} 
					else{
						_playerComponent.setInterpolatedFactorWalkDirection(
							glm::vec3(0.0f, -0.0001f, 0.0f));
						_animationComponent.playAction("crouch");
					}

					if (_input->isButtonDown(KEYBOARD_BUTTON(C))){
						_playerComponent.m_state = CONTROLLER_STAND;
						_animationComponent.playAction("stand");
						_playerComponent.m_accelerationFactor = 1.0f;
					}
				}
				else if (_playerComponent.m_state == CONTROLLER_MOVING || 
					_playerComponent.m_state == CONTROLLER_STAND)
				{
					bool _moving = controlMovement(&_movementDirection, &_movementSpeed, _yaw);

					glm::vec3 _movementVelocity = _movementDirection * _movementSpeed;
					if (!_justLanded && _playerComponent.canJump()){
						//_movementVelocity += glm::vec3(0.0f, 10.0f, 0.0f);
					}


					const float c_maxFrictionInterpolationFactor = 0.17f;
					const float c_minFrictionInterpolationFactor = 0.02f;
					float _frictionInterpolationFactor = c_maxFrictionInterpolationFactor;

					if (_playerComponent.canJump()){
						//Get the friction of the underlying floor to set the interpolaction factor				
						Entity _entityUnder = getEntityUnderPlayer(_entity);

						if (_entityUnder.isValid() && _entityUnder.hasComponent<MaterialComponent>()){
							auto &_materialComponent = _entityUnder.getComponent<MaterialComponent>();
							float _friction = _materialComponent.getMaterial()->m_friction;

							//PRINT("FRICTION: " << _friction);
							//_friction = 0.5f;

							//Calculate the new friction walking interpolation factor
							_frictionInterpolationFactor = glm::mix(c_minFrictionInterpolationFactor,
								c_maxFrictionInterpolationFactor, _friction);
							//PRINT("_frictionInterpolationFactor: " << _frictionInterpolationFactor);
						}
					}

					
					//PLAYER HAS JUST LANDED (play landing absorption animation)
					if (_justLanded){
						_playerComponent.m_state = CONTROLLER_STAND;

						//If there is a positive fall factor (absorbing fall)
						float _fallFactor = setFallAbsorptionKeyframes(
							glm::max(0.0f, -m_previousVerticalVelocity));
						if (_fallFactor > 0.0f){
							//TODO re-enable
							_animationComponent.playAction("land");
						}
					}
					//PLAYER IS CROUCHED AND MOVING ON THE GROUND (can't crouch while in the air)
					else if (_input->isButtonDown(KEYBOARD_BUTTON(C)) && _playerComponent.canJump())
					{
						//Can't crouch while in the air
						_playerComponent.m_state = CONTROLLER_CROUCH;
						_animationComponent.playAction("crouch");
						_playerComponent.m_accelerationFactor = 1.0f;
					}
					//PLAYER IS MOVING ON THE GROUND (NOT CROUCHED)
					else if (_moving && _playerComponent.canJump())
					{
						_playerComponent.m_state = CONTROLLER_MOVING;
						_animationComponent.playAction("walk");


						_playerComponent.setInterpolatedFactorWalkDirection(
							_movementVelocity, _frictionInterpolationFactor);

						glm::vec3 _eulerAngles(Utilities::
							getEulerRotationFromVector(_movementDirection));
						float _mod = fmod(_eulerAngles.y - _playerComponent.m_latestYRotation, 360.0f);
						float _angle = fmod(_mod + 540.0f, 360.0f) - 180.0f;

						const float c_newRotationFactor = 0.065f;
						float _currentYRotation = _playerComponent.m_latestYRotation;
						//const float c_newRotationFactor = _frictionInterpolationFactor * 0.5f;
						_playerComponent.m_latestYRotation = Utilities::limitAngle(glm::mix(
							_playerComponent.m_latestYRotation,
							_playerComponent.m_latestYRotation + _angle, 
							c_newRotationFactor));

						float _YRotationChange = _playerComponent.m_latestYRotation - _currentYRotation;
						_playerComponent.m_latestZRotation = glm::mix(
							_playerComponent.m_latestZRotation, _YRotationChange, 0.1f);

						//Animation playing factor (state of the walking cycle)
						float _factor = _animationComponent.getAnimationPlayer().getInterpolationFactor();
						_movementBounceOffset = sin(M_PI * _factor) * 0.1f; //0.1f
						//PRINT(_movementBounceOffset);


						bool _leanBack = false; //TODO remove
						//bool _leanBack = !_moving || !_playerComponent.canJump();

						glm::vec3 _newWalkDirection = _playerComponent.getWalkDirection();
						float _velocity = glm::length(glm::vec2(_newWalkDirection.x, _newWalkDirection.z));
						float _newLeanAngle = (_leanBack ? -_velocity : _velocity) * 60.0f;
						_playerComponent.m_latestLeanAngle = glm::mix(
							_playerComponent.m_latestLeanAngle, _newLeanAngle, 0.2f);

						setLocomotionKeyframes(_velocity, 0.0f);
					} 
					//PLAYER IS IN THE AIR
					else if (!_playerComponent.canJump())
					{ 
						_playerComponent.m_state = CONTROLLER_MOVING;
						_animationComponent.playAction("stand");

						//Set interpolation factor to minimum when in the air
						_playerComponent.setInterpolatedFactorWalkDirection(
							_movementVelocity, c_minFrictionInterpolationFactor);

						if (_moving){ //Rotate the body only if moving
							glm::vec3 _eulerAngles(Utilities::
								getEulerRotationFromVector(_movementDirection));
							float _mod = fmod(_eulerAngles.y - _playerComponent.m_latestYRotation, 360.0f);
							float _angle = fmod(_mod + 540.0f, 360.0f) - 180.0f;

							const float c_newRotationFactor = 0.02f;
							float _currentYRotation = _playerComponent.m_latestYRotation;
							_playerComponent.m_latestYRotation = Utilities::limitAngle(glm::mix(
								_playerComponent.m_latestYRotation,
								_playerComponent.m_latestYRotation + _angle, 
								c_newRotationFactor));				

							float _YRotationChange = _playerComponent.m_latestYRotation - _currentYRotation;
							_playerComponent.m_latestZRotation = glm::mix(
								_playerComponent.m_latestZRotation, _YRotationChange, 0.1f);
						}	

						//Predict landing angle
						glm::vec3 _movementDirection = _playerComponent.getNormalizedLinearVelocity();

						//If falling
						if (true || _movementDirection.y < 0.0f){
							const float c_playerViewDistance = 20.0f;
							auto _raycast = _collision->rayTracingClosest(
								_playerComponent.getLocation(), 
								_movementDirection,
								c_playerViewDistance, 
								Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC);

							if (_raycast.hasHit()){
								glm::vec3 _hitNormal(glm::normalize(Physics::convertVec3(_raycast.m_hitNormalWorld)));
								glm::vec2 _direction2d(_hitNormal.x, _hitNormal.z);

								//Prepare for bounce only for bouncy surfaces


								//TODO remove or not 
								//Rotating player along surface

								//Ignore small length close to [0, 1, 0] normal
								/*if (_validEntity && glm::length(_direction2d) > 0.2f){
									glm::vec2 _normalizedDirection2d = glm::normalize(_direction2d);

									float _angle = atan2f(_normalizedDirection2d.x, _normalizedDirection2d.y);
									_playerComponent.m_targetBounceYRotation = fmod(glm::degrees(_angle) + 180.0f, 360.0f);

									_playerComponent.m_latestYRotation = glm::mix(
										_playerComponent.m_latestYRotation, _playerComponent.m_targetBounceYRotation, 0.2f);

									//TODO interpolate
									//_playerComponent.m_latestYRotation = _playerComponent.m_targetBounceYRotation;
									//PRINT(_playerComponent.m_latestYRotation);
								}*/


								//TODO remove or not 
								//Rotating player along surface
								/*const glm::vec3 _upVector(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
								const glm::vec3 _crossVectors(glm::normalize(glm::vec3(1.0f) - _upVector));							
								float _dotUp = glm::dot(_upVector, _hitNormal);
								float _dotCross = glm::dot(_crossVectors, _hitNormal);

								float _sign = glm::sign(_dotCross) *
									glm::sign(_playerComponent.m_targetBounceYRotation - 180.0f);
								float _leanAngle = glm::degrees(glm::acos(_dotUp)) * _sign;

								_playerComponent.m_latestLeanAngle = glm::mix(
									_playerComponent.m_latestLeanAngle, _leanAngle, 0.05f);*/

								/*glm::vec3 _eulerAngles = Utilities::getEulerRotationFromVector(_hitNormal);

								
								Utilities::print(_eulerAngles);*/

								//PRINT(_leanAngle << " " << glm::sign(_dotCross));

							}
						}

						

						//_playerComponent.m_targetBounceYRotation

					} 
					//PLAYER IS NOT MOVING (STANDING STILL)
					else
					{
						_playerComponent.setInterpolatedFactorWalkDirection(
							glm::vec3(0.0f, -0.0001f, 0.0f), _frictionInterpolationFactor);

						_playerComponent.m_state = CONTROLLER_STAND;
						_animationComponent.playAction("stand");

						//Stand up straight - no forward lean when standing still
						_playerComponent.m_latestLeanAngle = glm::mix(
							_playerComponent.m_latestLeanAngle, 0.0f, 0.1f);
					}

					
					

						
					/*if (_moving){
						glm::vec3 _eulerAngles(Utilities::
							getEulerRotationFromVector(_movementDirection));

						float _mod = fmod(_eulerAngles.y - m_latestYRotation, 360.0f);
						//float _mod = fmod(_yaw - m_latestYRotation, 360.0f);
						float _angle = fmod(_mod + 540.0f, 360.0f) - 180.0f;
						
						const float c_newRotationFactor = 0.065f; // 0.1f
						m_latestYRotation = Utilities::limitAngle(glm::mix(m_latestYRotation,
							m_latestYRotation + _angle, c_newRotationFactor));

					} else{
						//TODO Invert lean for slowing down (lean back)
						//_movementLean = -glm::length(_playerComponent.getWalkDirectionChange()) * 1000.0f;
					}*/

					


					//_matrixComponent.setRotation(_movementLean, 0.0f, 0.0f);
					_matrixComponent.setRotation(_playerComponent.m_latestLeanAngle,
						Utilities::limitAngle(_playerComponent.m_latestYRotation + 180.0f), 0.0f);

					/*_matrixComponent.setRotation(_matrixComponent.getRotation() * 
						Rotation(AXIS_Z, -_playerComponent.m_latestZRotation * 2.0f));*/

					//PRINT("Z: " << _playerComponent.m_latestZRotation)
				} 


				bool _validState = _playerComponent.m_state == CONTROLLER_MOVING ||
					_playerComponent.m_state == CONTROLLER_STAND;
				if (_validState)
				{
					if (_input->isButtonDown(KEYBOARD_BUTTON(SPACE)) &&
						_playerComponent.canJump() && !_justLanded)
					{
						//m_accelerationFactor = 1.0f; //TODO maybe remove reset acceleration
						_playerComponent.jump();
						_playerComponent.enableGravity();

						_animationComponent.getAnimationPlayer().clearTimeline();
						_animationComponent.playAction("jump");

						_playerComponent.m_state = CONTROLLER_MOVING;
					} else{
						_playerComponent.enableGravity();
						_playerComponent.m_state = CONTROLLER_STAND;
					}
				}
				
			
				//TODO change when changing player width 0.6 = 2.25f
				//-0.2f
				//2.0f
				_matrixComponent.setLocation(_playerComponent.getLocation() -
					Location(0.0f, _playerComponent.getCurrentHeight() * 0.8f, 0.0f) + Location(0.0f, _movementBounceOffset, 0.0f));
				//_matrixComponent.setRotation(Rotation(AXIS_Y, _camera->getYaw()));

				Location _cameraOffset(0.0f, _playerComponent.getCurrentHeight() * 1.5f, 0.0f);
				_variables.m_camera->setLocation(_playerComponent.getLocation() + _cameraOffset);
				_variables.m_camera->transform(!_variables.m_editing);


				mousePicking(_entity);


				m_previousVerticalVelocity = _playerComponent.getVerticalVelocity();
				m_previousCanJump = _playerComponent.canJump();
			}

			void PlayerControllerSystem::postUpdate(float fps){
				//return;

				if (m_entities.empty()){
					return;
				}



				auto &_variables = getWorld().getVariables();
				auto &_rendererSystem = getWorld().getSystem<RendererSystem>();
				auto &_framebuffer = _variables.m_renderer->getFinalFramebuffer();

				auto &_entity = m_entities.front();
				auto &_playerComponent = _entity.getComponent<PlayerControllerComponent>();
				auto &_controllerComponent = _entity.getComponent<PlayerControllerComponent>();

				//Utilities::print(_playerComponent.getLinearVelocity());
				_playerComponent.setPreviousLinearVelocity(_playerComponent.getLinearVelocity());


				//Update previous move direction (used for landing force)
				

				return; //Prevent drawing debug geometry

				_framebuffer.bind();
				{
					glEnable(GL_BLEND);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
			
					auto _body = _controllerComponent.getBody();

					btTransform _transform = _body->getWorldTransform();
					glm::vec3 _location(Physics::convertVec3(_transform.getOrigin()));
					glm::quat _rotation(Physics::convertQuat(_transform.getRotation()));
					glm::vec3 _scale(Physics::convertVec3(_body->getCollisionShape()->getLocalScaling()));

					/*_rendererSystem.renderCollisionShape(_body->getCollisionShape(),
						_location, _rotation, _scale * 0.5f, glm::vec4(0.0f, 1.0f, 0.5f, 1.0f));//*/
					
					/*auto &_body = m_ledgeGrabber.m_ledgeTopSphereBody;
					if (_body != nullptr){
						btTransform _transform = _body->getWorldTransform();
						glm::vec3 _location(Physics::convertVec3(_transform.getOrigin()));

						//Utilities::print(_location);
						_rendererSystem.renderCollisionShape(_body->getCollisionShape(),
							_location, 0.0f, 0.5f, glm::vec4(0.0f, 1.0f, 0.5f, 1.0f));
					}*/


					/*if (m_shimmySphereBody != nullptr){
						btTransform _transform = m_shimmySphereBody->getWorldTransform();
						glm::vec3 _location(Physics::convertVec3(_transform.getOrigin()));
						glm::quat _rotation(Physics::convertQuat(_transform.getRotation()));
						glm::vec3 _scale(Physics::convertVec3(
							m_shimmySphereBody->getCollisionShape()->getLocalScaling()));
						
						_rendererSystem.renderCollisionShape(m_shimmySphereBody->getCollisionShape(),
							_location, _rotation, _scale, glm::vec4(0.0f, 1.0f, 0.5f, 1.0f));
					}*/
					

					glUseProgram(0);
					glLineWidth(5.0f);
					glBegin(GL_LINES);
					{
						glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

						for (auto &_line : m_debugLines){
							glm::vec3 _from = _line.first, _to = _line.second;
							
							glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
							glVertex3f(_from.x, _from.y, _from.z);
							glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
							glVertex3f(_to.x, _to.y, _to.z);
						}

						glColor4f(0.0f, 1.0f, 1.0f, 1.0f);

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					}
					glEnd();

					//Movement direction
					glBegin(GL_LINES);
					{
						auto _dir = _playerComponent.getNormalizedLinearVelocity() * 20.0f;
						auto _loc = _playerComponent.getLocation();

						glVertex3f(_loc.x, _loc.y, _loc.z);
						glVertex3f(_loc.x + _dir.x, _loc.y + _dir.y, _loc.z + _dir.z);
					}
					glEnd();
				
					glColor3f(1, 1, 1);

					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH_TEST);
					glDisable(GL_BLEND);
				}
				_framebuffer.unbind();
			}


			void PlayerControllerSystem::mousePicking(const Entity &playerEntity)
			{
#ifdef MOUSE_PICKING

				auto &_variables = getWorld().getVariables();
				auto &_input = _variables.m_input;
				auto &_collision = _variables.m_collision;

				auto &_playerComponent = playerEntity.getComponent<PlayerControllerComponent>();
				auto &_mousePicker = _playerComponent.m_mousePicker;

				if (_input->m_mouseLeftDown){
					bool _pickedBody = _mousePicker.rayPicking(getWorld(),
						_variables.m_camera->getLocation(), _variables.m_mouseVector,
						_input->isButtonHeld(KEYBOARD_BUTTON(LSHIFT)));

					if (_pickedBody && _mousePicker.getPickedBody() != nullptr)
					{
						//if successfully picked a new not null body
						//PRINT("BODY INDEX: " << ->getUserIndex());	

						auto _pickedBody = _mousePicker.getPickedBody();
						auto _pickedEntity = getWorld().getEntity(_pickedBody->getUserIndex());
						if (_pickedEntity.isValid()){
							auto _entityName = _pickedEntity.getName();
							if (false /*false && (_entityName == "cube1" || _entityName == "cube2"*/){
								PRINT(_entityName);
								_mousePicker.cleanPicking(&_collision->getWorld());

								auto &_matrixComponent = _pickedEntity.getComponent<MatrixComponent>();
								glm::vec3 _randomColor = Utilities::randomVec3(0.0f, 0.8f);
								glm::vec3 _grassColor = glm::vec3(0.002f, 0.456f, 0.007f);

								Scale _decalScale = _matrixComponent.getScale() * glm::vec3(15.0f, 0.25f, 15.0f) * 8.0f;

								//if (!_pickedEntity.hasComponent<ParticleComponent>()){
									auto _decalEntity = createDecalEntity(getWorld(),
										_grassColor, _matrixComponent.getLocation(),
										//_matrixComponent.getRotation() 
										Rotation(0.0f), _decalScale);

									_decalEntity.removeComponent<DecalComponent>();

									auto &_decalMatrixComponent = _decalEntity.getComponent<MatrixComponent>();
									auto _decalLocation = _decalMatrixComponent.getLocation();

									/*auto f_removeFromCollisionWorld = [&](const std::string &entityName){
									for (auto &_entity : getWorld().getEntities(entityName)){
									auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
									_collision->removeBodyFromWorld(_collisionComponent.getBody().get());
									}
									};
									auto f_addToCollisionWorld = [&](const std::string &entityName){
									for (auto &_entity : getWorld().getEntities(entityName)){
									auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
									_collision->removeBodyFromWorld(_collisionComponent.getBody().get());
									}
									};*/


									const int c_numIterations = 200;
									const float _sizeStep = _decalScale.x / c_numIterations;
									const float _scaleX = _decalScale.x * 0.5f, _scaleZ = _decalScale.z * 0.5f;
									const float _distanceToCorner = glm::length(glm::vec2(_scaleX, _scaleZ));

									Uint64 _startTimer = SDL_GetPerformanceCounter();
									std::vector<glm::vec4> _grassLocations;

									for (float z = -_scaleZ; z <= _scaleZ; z += _sizeStep){
										for (float x = -_scaleX; x <= _scaleX; x += _sizeStep){
											const float _grassScale = Utilities::randomFloat(0.25f, 2.0f);
											const float _randomRange = _grassScale * 8.0f;
											const float _offsetX = Utilities::randomFloat(-_randomRange, _randomRange) * 0.5f;
											const float _offsetZ = Utilities::randomFloat(-_randomRange, _randomRange) * 0.5f;
											Location _offset = Location(_offsetX, 0.0f, _offsetZ);
											glm::vec3 _offsetFromCenter = Location(x, _decalScale.y, z) + _offset;

											float _distanceFromCenter = glm::length(glm::vec3(_offsetFromCenter));
											float _continue = Utilities::randomFloat(0.0f, _distanceFromCenter);
											//PRINT("DISTNACE: " << _continue << " " << _distanceToCorner * 0.5f)

											//if (_continue > (_distanceToCorner * 0.25f))continue;


											//TODO if performance slow transfer to compute shader
											/*auto _grassRaycastAll = _collision->rayTracingAll(_decalLocation + _offsetFromCenter,
												glm::vec3(0.0f, -1.0f, 0.0f), _decalScale.y);*/

											Location _location = _offsetFromCenter;
											_location.y = 0.0f;
											_grassLocations.emplace_back(_location, _grassScale);

											//if (_grassRaycastAll.hasHit()){
												/*for (int i = 0; i < _grassRaycastAll.m_hitPointWorld.size(); ++i){
													auto _collisionId = _grassRaycastAll.m_collisionObjects[i]->getUserIndex();
													bool _idInRange = _collisionId >= 0 && _collisionId < MAX_ENTITIES;
													auto _entity = getWorld().getEntity(_collisionId);

													if (_entity.isValid()){
														auto _entityName = getWorld().getEntityName(_collisionId);
														bool _isDecal = _idInRange && _entity.hasComponent<DecalComponent>();
														bool _isEnemyCube = _entityName == "cube1" || _entityName == "cube2";
														bool _validNormal = _grassRaycastAll.m_hitNormalWorld[i].normalize().getY() > 0.8f;

														if (!_isDecal && !_isEnemyCube && _validNormal){
															Location _hitLocation = Physics::convertVec3(
																_grassRaycastAll.m_hitPointWorld[i]);

															_grassLocations.emplace_back(_hitLocation, _grassScale);
														}

													}
												}*/

											//} else continue;
										}
									}


									double _counterTime = static_cast<double>(SDL_GetPerformanceCounter() - _startTimer);
									float _time = static_cast<float>((_counterTime / SDL_GetPerformanceFrequency()) * 1000.0);
									PRINT("ADDING TOOK: " << _time);

									//*/

									/*std::vector<glm::vec4> _grassLocations;
									for (int i = 0; i < 400; ++i){
									float _randX = Utilities::randomFloat(-_decalScale.x, _decalScale.x) * 0.5f;
									float _randZ = Utilities::randomFloat(-_decalScale.z, _decalScale.z) * 0.5f;
									glm::vec3 _offset = glm::vec3(_randX, 0.0f, _randZ);

									float _distanceFromCenter = glm::length(_offset);
									float _continue = Utilities::randomFloat(0.0f, _distanceFromCenter);
									if (_continue > _decalScale.x * 0.25f)continue;


									const float _grassScale = Utilities::randomFloat(0.5f, 1.5f);
									_grassLocations.emplace_back((_decalLocation + _offset), _grassScale);
									}*/

									//TODO remove for adding grass locations
									auto &_environmentSystem = getWorld().getSystem<EnvironmentSystem>();
									_environmentSystem.addGrassLocations(_grassLocations);

									auto &_particleComponent = _pickedEntity.addComponent<ParticleComponent>();
									for (auto &_particleContainer : _particleComponent.getParticleContainers()){
										_particleContainer->addFlags(Particles::PF_TRIGGER);
										_particleContainer->setLife(1.2f);
										_particleContainer->m_gravity = glm::vec3(0.0f, -50.8f, 0.0f);
										_particleContainer->m_speed = 6.0f;
										_particleContainer->m_spread.y = 6.0f;
										_particleContainer->m_additiveBlending = false;
										_particleContainer->setMaxParticleCount(2000);
									}

									_pickedEntity.activate();

								//}

							}
						}
					}
				} else if (!_input->m_mouseLeftHold){
					_mousePicker.cleanPicking(&_collision->getWorld());
				}

				if (_input->m_mouseLeftHold){
					btRigidBody *_pickedBody = _mousePicker.getPickedBody();
					if (_pickedBody)
					{
						_mousePicker.updatePicking(
							_variables.m_camera->getLocation(), _variables.m_mouseVector);

#ifdef THROW_FORCE
						if (_input->isButtonDown(KEYBOARD_BUTTON(T))){ //THROW PICKED OBJECT
							float _newThrowForce = THROW_FORCE;

							auto &_pickedEntity = getWorld().getEntity(_pickedBody->getUserIndex());
							if (_pickedEntity.isValid()){
								auto &_collisionComponent =
									_pickedEntity.getComponent<CollisionComponent>();

								if (_collisionComponent.isRagdoll()){
									_newThrowForce *= 6.0f;
								}
							}


							_pickedBody->setLinearVelocity(
								convertVec3(_variables.m_mouseVector * _newThrowForce));
							_mousePicker.cleanPicking(&_collision->getWorld());
						}
#endif //THROW_FORCE

					}
				}

#endif //MOUSE_PICKING
			}


			Entity PlayerControllerSystem::getEntityUnderPlayer(const Entity &playerEntity){
				auto &_variables = getWorld().getVariables();
				auto &_collision = _variables.m_collision;

				auto &_matrixComponent = playerEntity.getComponent<MatrixComponent>();
				auto &_playerComponent = playerEntity.getComponent<PlayerControllerComponent>();

				auto _raycast = _collision->rayTracingClosest(_playerComponent.getLocation(),
					glm::vec3(0.0f, -1.0f, 0.0f), _playerComponent.getHeight(),
					GROUP_STATIC | GROUP_DYNAMIC
				);

				if (_raycast.hasHit()){
					unsigned int _entityId = _raycast.m_collisionObject->getUserIndex();
					bool _validIdRange = _entityId >= 0 && _entityId < MAX_ENTITIES;

					return _validIdRange ? getWorld().getEntity(_entityId) : Entity();
				}
				else{
					return Entity();
				}
			}


			float PlayerControllerSystem::setFallAbsorptionKeyframes(float fallSpeed){
				if (m_entities.empty()/* || fallSpeed < 0.001f*/){
					return 0.0f;
				}

				auto &_entity = m_entities.front();
				auto &_playerComponent = _entity.getComponent<PlayerControllerComponent>();
				auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
				auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
				auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

				auto _landAction = _animationComponent.getAction("land");
				auto _fallAbsorptionKeyframe = _animationComponent.getKeyframe("fall_absorption");
				auto _fallAbsorptionHighest = _animationComponent.getKeyframe("fall_absorption_highest");
				auto _fallAbsorptionLowest = _animationComponent.getKeyframe("fall_absorption_lowest");
				

				//PRINT(velocity);
				if (_landAction && _fallAbsorptionHighest && _fallAbsorptionLowest)
				{
					m_interpolator.resizeKeyframe(_fallAbsorptionHighest->m_transforms);

					float _factor = fallSpeed / glm::abs(_playerComponent.c_playerGravityY);
					_factor = _factor < 0.2f ? 0.0f : glm::min(1.0f, glm::pow(_factor, 1.0f));//0.4?
					//_factor = 0.0f;
					//PRINT("FACTOR: " << _factor);

					auto _boneTreeRoot = _animationComponent.getSkeleton().
						m_animationBoneTreeRoot.get();

					if (_factor >= 1.0f){
						*_fallAbsorptionKeyframe = *_fallAbsorptionLowest;
					} else if (_factor <= 0.0f){
						*_fallAbsorptionKeyframe = *_fallAbsorptionHighest;
					} else{
						m_interpolator.setInterpolationType(Utilities::INTERPOLATION_COS);
						*_fallAbsorptionKeyframe = m_interpolator.getInterpolatedKeyframe(
							_factor, *_fallAbsorptionHighest, *_fallAbsorptionLowest, _boneTreeRoot);
					}

					return _factor;
				} 
				else{
					return 0.0f;
				}
			}


			void PlayerControllerSystem::setLocomotionKeyframes(
				float velocity, const float yaw, const float fps)
			{
				if (m_entities.empty() || velocity < 0.001f){
					return;
				}

				auto &_entity = m_entities.front();
				auto &_playerComponent = _entity.getComponent<PlayerControllerComponent>();
				auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
				auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
				auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

				auto _walkSlowestRight = _animationComponent.getKeyframe("walk_slowest_right");
				auto _runFastestRight = _animationComponent.getKeyframe("run_fastest_right");
				bool _validRight = _walkSlowestRight && _runFastestRight;

				auto _walkSlowestLeft = _animationComponent.getKeyframe("walk_slowest_left");
				auto _runFastestLeft = _animationComponent.getKeyframe("run_fastest_left");
				bool _validLeft = _walkSlowestLeft && _runFastestLeft;


				//PRINT(velocity);
				if (_validRight && _validLeft)
				{
					const float _keyframeTime = 1.0f / fps;
					const float _minVelocity = c_playerWalkingSpeed * _keyframeTime;
					const float _maxVelocity = c_playerWalkingSpeed * 
						(1.0f + _playerComponent.m_maxAccelerationFactor) * _keyframeTime;
					const float _ratio = _maxVelocity / _minVelocity * 0.5f;

					auto _walkAction = _animationComponent.getAction("walk");
					auto _walkRightKeyframe = _animationComponent.getKeyframe("walk_right");
					auto _walkLeftKeyframe = _animationComponent.getKeyframe("walk_left");

					if (_walkAction){
						m_interpolator.resizeKeyframe(_walkSlowestRight->m_transforms);

						float _factor = (velocity - _minVelocity) / (_maxVelocity - _minVelocity);
						_factor = glm::clamp(_factor, 0.0f, 1.0f);

						auto _boneTreeRoot = _animationComponent.getSkeleton().m_animationBoneTreeRoot.get();
						m_interpolator.setInterpolationType(Utilities::INTERPOLATION_COS);
						//m_interpolator.setInterpolationType(INTERPOLATION_LINEAR);

						*_walkRightKeyframe = m_interpolator.getInterpolatedKeyframe(
							_factor, *_walkSlowestRight, *_runFastestRight, _boneTreeRoot);
						_walkRightKeyframe->m_name = "walk_right";
						*_walkLeftKeyframe = m_interpolator.getInterpolatedKeyframe(
							_factor, *_walkSlowestLeft, *_runFastestLeft, _boneTreeRoot);
						_walkLeftKeyframe->m_name = "walk_left";
						

						//TODO update without ruining IK
						/*_animationComponent.getAnimationPlayer().updateAllKeyframes("walk_right", *_walkRightKeyframe);
						_animationComponent.getAnimationPlayer().updateAllKeyframes("walk_left", *_walkLeftKeyframe);*/


						//TODO remove testing
						/*auto _rootNode = _animationComponent.getSkeleton().m_animationBoneTreeRoot.get();

						std::vector<glm::mat4> _transforms(_walkLeftKeyframe->m_transforms.size());
						_walkLeftKeyframe->getFinalTransforms(_rootNode, &_transforms);

						_walkRightKeyframe->setKeyframeTransformsFromFinalTransforms(
							_animationComponent.getSkeleton().m_animationBoneTreeRoot.get(), _transforms);//*/


						/*unsigned int _hipBoneId = _animationComponent.getBoneId("HIP");
						Rotation _newRotation(0.0f, 0.0f, 180.0 + yaw);

						auto &_rightTransform = _walkRightKeyframe->m_transforms[_hipBoneId];
						_rightTransform.m_rotation = _newRotation * _rightTransform.m_rotation;
						auto &_leftTransform = _walkLeftKeyframe->m_transforms[_hipBoneId];
						_leftTransform.m_rotation = _newRotation * _leftTransform.m_rotation;*/

						//TODO load from file or save in animation the original duration
						const float _walkCycleDuration = 0.4f; 
						const float _runCycleDuration = _walkCycleDuration / 
							(1.0f + _playerComponent.m_accelerationFactor * 0.6f);
						float _newCycleDuration = Utilities::linearInterpolation(
							_factor, _walkCycleDuration, _runCycleDuration);

						//_newCycleDuration = 0.3f;
						
						_walkAction->setKeyframeDuration(0, _newCycleDuration);
						_walkAction->setKeyframeDuration(1, _newCycleDuration);
						_animationComponent.updateDurationsForAction("walk");
					}
				}

			}

			void PlayerControllerSystem::updatePlayerColors(){
				
			}

			void PlayerControllerSystem::setDefaultClothingColors(){

			}


			void PlayerControllerSystem::onEntityAdded(Entity &entity){
				auto _body = entity.getComponent<PlayerControllerComponent>().getBody();
				_body->setUserPointer(m_world);
				_body->setUserIndex(entity.getId());

				//make sure we only have one player controller
				if (m_entities.size() > 1){
					m_entities.begin()->removeComponent<PlayerControllerComponent>();
					m_entities.begin()->activate();
					getWorld().refresh();
				}
			}

		}
	}
}
