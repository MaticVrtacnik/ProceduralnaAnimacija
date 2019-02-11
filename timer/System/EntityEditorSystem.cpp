#include "EntityEditorSystem.hpp"

#include <queue>
#include <glm\gtc\type_ptr.hpp>

#include "../World/World.hpp"
#include "../Camera/Camera.hpp"
#include "../Renderer/DeferredRenderer.hpp"
#include "../Texture/Material.hpp"
#include "../Collision/Collision.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Input/Input.hpp"
#include "../Animation/AnimationSkeleton.hpp"
#include "../Animation/SkeletalAnimationPlayer.hpp"

#include "../Component/ComponentIncluder.hpp"
#include "../Renderer/SimpleRenderer.hpp"
#include "../Define/PrintDefines.hpp"
#include "../System/RendererSystem.hpp"
#include "../Utility/RandomUtilities.hpp"

#include "../GUI/GUI.hpp"
#include "../GUI/GUITab.hpp"
#include "../GUI/GUITextBox.hpp"

#include "GUISystem.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			EntityEditorSystem::EntityEditorSystem(){
				addRequired<Components::MatrixComponent>();
				addRequired<Components::CollisionComponent>();

				m_selectedEntity = nullptr;
				m_editingMode = EDITING_TRANSLATE;
			
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			EntityEditorSystem::~EntityEditorSystem(){

			}



			void EntityEditorSystem::init(){
				auto &_variables = getWorld().getVariables();
				float _screenWidth = static_cast<float>(_variables.m_screenWidth);
				float _screenHeight = static_cast<float>(_variables.m_screenHeight);
				float _aspectRatio = _screenWidth / _screenHeight;

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(90.0f, _aspectRatio, 0.1f, 10000.0f);
				glMatrixMode(GL_MODELVIEW);

				//setSelectedEntity(nullptr);


				//Init add/delete button actions for entity components
				initComponentGUIAction<MaterialComponent>("material");
				initComponentGUIAction<LightComponent>("light");
				initComponentGUIAction<PlayerControllerComponent>("player", *_variables.m_collision);


				auto &_stack = _variables.m_gui->getIndex().getChild<GUI::GUIWindow>("tab").
					getChild<GUI::GUITab>("tab").getWindow("components").getChild<GUI::GUIWindowStack>("stack");

			}

			void EntityEditorSystem::onEntityAdded(Entity &entity){
				m_selectedEntity = nullptr;
				m_selectedEntities.clear();

				//Set collision body properties from material
				if (entity.hasComponent<MaterialComponent>()){
					auto &_collisionComponent = entity.getComponent<CollisionComponent>();
					auto &_materialComponent = entity.getComponent<MaterialComponent>();
					const auto &_material = _materialComponent.getMaterial();

					for (auto &_body : _collisionComponent.getBodies()){
						_body->setFriction(_material->m_friction);
						_body->setRestitution(_material->m_restitution);
					}
				}
			}

			void EntityEditorSystem::onEntityRemoved(Entity &entity){
				auto &_variables = getWorld().getVariables();

				//TODO maybe remove already in CollisionSystem
				/*auto &_collisionComponent = entity.getComponent<CollisionComponent>();
				for (auto &_body : _collisionComponent.getBodies()){
					//TODO fix crashes on swap
					_variables.m_collision->removeBody(*_body);
				}*/
			}



			void EntityEditorSystem::calculateOffset(){
				if (m_selectedEntity == nullptr) return;

				auto &_variables = getWorld().getVariables();
				glm::vec3 _mouseVector = _variables.m_mouseVector;
				glm::vec3 _cameraLocation = _variables.m_camera->getLocation();

				m_previousLocation = m_selectedEntity->getComponent<MatrixComponent>().getLocation();

				if (m_pickingX && m_pickingY && m_pickingZ){

				}
				else if (m_pickingX && m_pickingY){
					float _centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);

					float _positionChangeX = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					m_startingLocation.x = _positionChangeX - m_previousLocation.x;

						float _positionChangeY = (_mouseVector.y * _centerDistance) + _cameraLocation.y;
					m_startingLocation.y = _positionChangeY - m_previousLocation.y;
				} 
				else if (m_pickingX && m_pickingZ){
					float _centerDistance = abs((_cameraLocation.y - m_previousLocation.y) / _mouseVector.y);

					float _positionChangeX = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					m_startingLocation.x = _positionChangeX - m_previousLocation.x;

						float _positionChangeZ = (_mouseVector.z * _centerDistance) + _cameraLocation.z;
					m_startingLocation.z = _positionChangeZ - m_previousLocation.z;
				} 
				else if (m_pickingY && m_pickingZ){
					float _centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);

					float _positionChangeY = (_mouseVector.y * _centerDistance) + _cameraLocation.y;
					m_startingLocation.y = _positionChangeY - m_previousLocation.y;

					float _positionChangeZ = (_mouseVector.z * _centerDistance) + _cameraLocation.z;
					m_startingLocation.z = _positionChangeZ - m_previousLocation.z;
				}
				else if (m_pickingX){
					float _centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);
					float _positionChange = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					m_startingLocation.x = _positionChange - m_previousLocation.x;
				}
				else if (m_pickingY){
					float _centerDistance = 0.0f;
					//If x has a smaller dot product of mouse vector and perpendicular plane
					if (glm::abs(_mouseVector.x) > glm::abs(_mouseVector.z)){
						_centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);
					} else{
						_centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);
					}
	
					float _positionChange = ((_mouseVector.y) * _centerDistance) + _cameraLocation.y;
					m_startingLocation.y = _positionChange - m_previousLocation.y;
				}
				else if (m_pickingZ){
					float _centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);
					float _positionChange = ((_mouseVector.z) * _centerDistance) + _cameraLocation.z;
					m_startingLocation.z = _positionChange - m_previousLocation.z;
				}
			}

			void EntityEditorSystem::editTranslation(){
				if (m_selectedEntity == nullptr || m_editingMode != EDITING_TRANSLATE){
					return;
				}

				auto &_variables = getWorld().getVariables();
				glm::vec3 _cameraLocation = _variables.m_camera->getLocation();
				glm::vec3 _mouseVector = _variables.m_mouseVector;

				glm::vec3 _change(0.0f);


				if (m_pickingX && m_pickingY && m_pickingZ){
					/*float _centerDistance = 0.0f;

					//If x has a smaller dot product of mouse vector and perpendicular plane
					if (glm::abs(_mouseVector.x) < glm::abs(_mouseVector.z)){
						_centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);
					} else{
						_centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);
					}

					float _positionChangeX = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					_change.x = _positionChangeX - m_previousLocation.x - m_startingLocation.x;

					float _positionChangeY = (_mouseVector.y * _centerDistance) + _cameraLocation.y;
					_change.y = _positionChangeY - m_previousLocation.y - m_startingLocation.y;

					float _positionChangeZ = (_mouseVector.z * _centerDistance) + _cameraLocation.z;
					_change.z = _positionChangeZ - m_previousLocation.z - m_startingLocation.z;*/
				} else if (m_pickingX && m_pickingY){
					float _centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);

					float _positionChangeX = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					_change.x = _positionChangeX - m_previousLocation.x - m_startingLocation.x;

					float _positionChangeY = (_mouseVector.y * _centerDistance) + _cameraLocation.y;
					_change.y = _positionChangeY - m_previousLocation.y - m_startingLocation.y;
				}
				else if (m_pickingX && m_pickingZ){
					float _centerDistance = abs((_cameraLocation.y - m_previousLocation.y) / _mouseVector.y);

					float _positionChangeX = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					_change.x = _positionChangeX - m_previousLocation.x - m_startingLocation.x;

					float _positionChangeZ = (_mouseVector.z * _centerDistance) + _cameraLocation.z;
					_change.z = _positionChangeZ - m_previousLocation.z - m_startingLocation.z;
				} 
				else if (m_pickingY && m_pickingZ){
					float _centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);

					float _positionChangeY = (_mouseVector.y * _centerDistance) + _cameraLocation.y;
					_change.y = _positionChangeY - m_previousLocation.y - m_startingLocation.y;

					float _positionChangeZ = (_mouseVector.z * _centerDistance) + _cameraLocation.z;
					_change.z = _positionChangeZ - m_previousLocation.z - m_startingLocation.z;
				}
				else if (m_pickingX){
					float _centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);
					float _positionChange = (_mouseVector.x * _centerDistance) + _cameraLocation.x;
					_change.x = _positionChange - m_previousLocation.x - m_startingLocation.x;
				}
				else if (m_pickingY){
					float _centerDistance = 0.0f;
					//If x has a smaller dot product of mouse vector and perpendicular plane
					if (glm::abs(_mouseVector.x) > glm::abs(_mouseVector.z)){
						_centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);
					} else{
						_centerDistance = abs((_cameraLocation.z - m_previousLocation.z) / _mouseVector.z);
					}

					float _positionChange = (_mouseVector.y * _centerDistance) + _cameraLocation.y;
					_change.y = _positionChange - m_previousLocation.y - m_startingLocation.y;
				}
				else if (m_pickingZ){
					float _centerDistance = abs((_cameraLocation.x - m_previousLocation.x) / _mouseVector.x);
					float _positionChange = (_mouseVector.z * _centerDistance) + _cameraLocation.z;
					_change.z = _positionChange - m_previousLocation.z - m_startingLocation.z;
				}
				

				if (glm::length(_change) != 0.0f)
				{
					auto &_collisionComponent = m_selectedEntity->getComponent<CollisionComponent>();
					glm::mat2x3 _boundingBox = _collisionComponent.getBoundingBox();

					bool _positiveDirection = glm::dot(_change, glm::vec3(1.0f)) >= 0.0f;
					glm::vec3 _normalizedDirection = glm::normalize(_change);

					//Max vec3 of the boundingBox is [1] and min is [0]
					glm::vec3 _rayLocation = m_previousLocation;
					_rayLocation += glm::abs(_normalizedDirection) * _boundingBox[_positiveDirection ? 1 : 0];


					//Snap to nearest surface
					const float c_snappingDistance = 0.5f;
					auto _snapToRay = _variables.m_collision->rayTracingClosest(
						_rayLocation, _normalizedDirection, c_snappingDistance);

					//Snap if left CTRL is being held
					if (_snapToRay.hasHit() && _variables.m_input->isButtonHeld(KEYBOARD_BUTTON(LCTRL))){
						Location _hitLocation = Physics::convertVec3(_snapToRay.m_hitPointWorld);
						_change = _hitLocation - _rayLocation;
						PRINT("SNAPPING TO GRID");
					}


					auto &_collision = getWorld().getVariables().m_collision;
					for (auto &_entity : m_selectedEntities){
						auto &_matrixComponent = _entity->getComponent<MatrixComponent>();
						auto &_collisionComponent = _entity->getComponent<CollisionComponent>();

						_collisionComponent.increaseLocation(_change);
						_matrixComponent.setLocation(Physics::convertVec3(
							_collisionComponent.getBody()->getWorldTransform().getOrigin()));
						_matrixComponent.updateStartTransforms();


						//Update sun location TODO change structure for multiple shadowed lights
						if (_entity->hasComponent<LightComponent>()){
							auto &_lightComponent = _entity->getComponent<LightComponent>();
							if (_lightComponent.getType() == LIGHT_DIRECTIONAL){

								//TODO add back
								_variables.m_renderer->setSunLocation(_matrixComponent.getLocation());
							}
						}
					}

					auto &_matrixComponent = m_selectedEntity->getComponent<MatrixComponent>();
					m_previousLocation = _matrixComponent.getLocation();


					auto &_matrixGUI = getWorld().getVariables().m_gui->getIndex().
						getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
						getWindow("components").getChild<GUI::GUIWindowStack>("stack").getWindow("matrix");

					const auto &_location = _matrixComponent.getLocation();
					_matrixGUI.getChild<GUI::GUITextBox>("locationX").setValue(_location.x);
					_matrixGUI.getChild<GUI::GUITextBox>("locationY").setValue(_location.y);
					_matrixGUI.getChild<GUI::GUITextBox>("locationZ").setValue(_location.z);
				}
			}

			void EntityEditorSystem::editRotation(){
				if (m_selectedEntity == nullptr || m_editingMode != EDITING_ROTATE){
					return;
				}

				auto &_variables = getWorld().getVariables();
				

				int _mouseX, _mouseY;
				SDL_GetRelativeMouseState(&_mouseX, &_mouseY);

				const float c_mouseSpeed = 0.35f;
				float _mouseLocationChange = static_cast<float>(_mouseX - _mouseY) * c_mouseSpeed;

				//If at least two picking directions are picked then rotate around camera->location vector
				bool _pickingAll = m_pickingX ? (m_pickingY || m_pickingZ) : (m_pickingY && m_pickingZ);

				Rotation _newRotation(0.0f);
				if (_pickingAll){
					auto &_matrixComponent = m_selectedEntity->getComponent<MatrixComponent>();
					glm::vec3 _cameraLocation = _variables.m_camera->getLocation();

					//TODO fix
					glm::vec3 _rotationVector = glm::normalize(_cameraLocation - _matrixComponent.getLocation());
					_newRotation = Rotation(_mouseLocationChange * _variables.m_camera->getVector());
				} else{
					_newRotation = Rotation(
						m_pickingX ? _mouseLocationChange : 0.0f,
						m_pickingY ? _mouseLocationChange : 0.0f,
						m_pickingZ ? _mouseLocationChange : 0.0f
					);
				}


				if (_mouseLocationChange != 0.0f)
				{
					auto &_collision = getWorld().getVariables().m_collision;
					for (auto &_entity : m_selectedEntities){
						auto &_matrixComponent = _entity->getComponent<MatrixComponent>();
						auto &_collisionComponent = _entity->getComponent<CollisionComponent>();

						_collisionComponent.increaseRotation(_newRotation);
						_matrixComponent.setRotation(Physics::convertQuat(
							_collisionComponent.getBody()->getWorldTransform().getRotation()));
						_matrixComponent.updateStartTransforms();
					}


					auto &_matrixGUI = getWorld().getVariables().m_gui->getIndex().
						getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
						getWindow("components").getChild<GUI::GUIWindowStack>("stack").getWindow("matrix");
					auto &_matrixComponent = m_selectedEntity->getComponent<MatrixComponent>();

					glm::vec3 _eulerAngles = glm::eulerAngles(_matrixComponent.getRotation());
					_matrixGUI.getChild<GUI::GUITextBox>("rotationX").setValue(_eulerAngles.x);
					_matrixGUI.getChild<GUI::GUITextBox>("rotationY").setValue(_eulerAngles.y);
					_matrixGUI.getChild<GUI::GUITextBox>("rotationZ").setValue(_eulerAngles.z);
				}
			}

			void EntityEditorSystem::editScaleScreen(){
				if (m_selectedEntity == nullptr || m_editingMode != EDITING_SCALE){
					return;
				}

				auto &_variables = getWorld().getVariables();
				auto _cameraLocation = _variables.m_camera->getLocation();
				auto _cameraVector = _variables.m_camera->getVector();


				int _mouseX, _mouseY;
				SDL_GetRelativeMouseState(&_mouseX, &_mouseY);
							
				auto &_entityLocation = m_selectedEntity->getComponent<MatrixComponent>().getLocation();
				float c_mouseSpeed = sqrtf(glm::distance(_cameraLocation, _entityLocation) / 10000.0f);

				//TODO maybe distance from center
				//glm::vec2 _screenCenter(_variables.m_screenWidth * 0.5f, _variables.m_screenHeight * 0.5f);		
				float _mouseLocationChange = static_cast<float>(_mouseX - _mouseY) * c_mouseSpeed;
				glm::vec3 _change(0.0f);
				
				//If at least two picking directions are picked then change all axis scale
				bool _pickingAll = m_pickingX ? (m_pickingY || m_pickingZ) : (m_pickingY && m_pickingZ);

				if (_pickingAll){
					_change = glm::vec3(_mouseLocationChange);
				} else{
					_change.x = m_pickingX ? _mouseLocationChange : 0.0f;
					_change.y = m_pickingY ? _mouseLocationChange : 0.0f;
					_change.z = m_pickingZ ? _mouseLocationChange : 0.0f;
				}

				
				glm::bvec3 _camPositive(_cameraVector.x > 0.0f, _cameraVector.y > 0.0f, _cameraVector.z > 0.0f);	


				/*if ((!_camPositive.x && _camPositive.z) || (_camPositive.x && !_camPositive.z)){
					_change.x = -_change.x;
				}if ((!_camPositive.x && _camPositive.z) || (_camPositive.x && _camPositive.z)){
					_change.z = -_change.z;
				}if (_camPositive.y){
					_change.y = -_change.y;
				}*/


				if (glm::length(_change) != 0.0f)
				{
					auto &_collision = getWorld().getVariables().m_collision;
					for (auto &_entity : m_selectedEntities){
						auto &_matrixComponent = _entity->getComponent<MatrixComponent>();
						auto &_collisionComponent = _entity->getComponent<CollisionComponent>();
						const auto &_body = _collisionComponent.getBody();

						glm::vec3 _rotatedScale = _change;
						if (!_pickingAll){
							glm::mat3 _rotationMatrix = glm::mat3(glm::toMat4(Physics::convertQuat(
								_body->getWorldTransform().getRotation())));
							_rotatedScale = glm::transpose(_rotationMatrix) * _change;
						}

						auto _newScale = _matrixComponent.getScale() + _rotatedScale;
						_collision->setBodyScale(*_body, 2.0f * _newScale);
						_matrixComponent.setScale(_newScale);
						_matrixComponent.updateStartTransforms();
					}

		
					auto &_matrixGUI = getWorld().getVariables().m_gui->getIndex().
						getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
						getWindow("components").getChild<GUI::GUIWindowStack>("stack").getWindow("matrix");

					const auto &_scale = m_selectedEntity->getComponent<MatrixComponent>().getScale();
					_matrixGUI.getChild<GUI::GUITextBox>("scaleX").setValue(_scale.x);
					_matrixGUI.getChild<GUI::GUITextBox>("scaleY").setValue(_scale.y);
					_matrixGUI.getChild<GUI::GUITextBox>("scaleZ").setValue(_scale.z);		
				}	
			}

			void EntityEditorSystem::editScaleWorld(){
				//TODO ?
			}



			void EntityEditorSystem::setSelectedEntity(Entity *entity){
				m_selectedEntity = entity;
					
				auto &_GUISystem = getWorld().getSystem<GUISystem>();
				_GUISystem.setGUIValuesFromEntity(m_selectedEntity != nullptr ? *m_selectedEntity : Entity());
			}
		
			void EntityEditorSystem::rayPicking(){
				auto &_variables = getWorld().getVariables();
				auto &_input = _variables.m_input;

				glm::vec3 _cameraLocation = _variables.m_camera->getLocation();
				glm::vec3 _mouseVector = _variables.m_mouseVector;

				//return;
				if (_input->m_mouseRightDown)
				{
					auto _rayTracingResult = _variables.m_collision->rayTracingAll(_cameraLocation, _mouseVector, 5000.0);

					Entity *_pickedEntity = nullptr;
					if (_rayTracingResult.hasHit()){
						std::vector<int> _indices(_rayTracingResult.m_collisionObjects.size());
						std::iota(_indices.begin(), _indices.end(), 0);

						//Sort according to distance from camera
						std::sort(_indices.begin(), _indices.end(), [&_cameraLocation, &_rayTracingResult]
							(const int &a, const int &b) ->bool
						{
							float _distanceA = glm::distance(_cameraLocation,
								Physics::convertVec3(_rayTracingResult.m_hitPointWorld[a]));
							float _distanceB = glm::distance(_cameraLocation,
								Physics::convertVec3(_rayTracingResult.m_hitPointWorld[b]));

							return _distanceA < _distanceB;
						});


						int _currentPickedId = -1;
						for (std::size_t i = 0; i < _indices.size(); ++i){
							auto _entityId = _rayTracingResult.m_collisionObjects[_indices[i]]->getUserIndex();
							if (m_selectedEntity == nullptr || _entityId == m_selectedEntity->getId()){
								_currentPickedId = i;
								break;
							}
						}

						if (_currentPickedId == _indices.size() - 1){
							_currentPickedId = -1;
						}


						int _nextValidId = _currentPickedId;
						for (std::size_t i = 1; i <= _indices.size(); ++i){
							int _nextId = _indices[(_currentPickedId + i) % _indices.size()];

							auto _collisionObject = _rayTracingResult.m_collisionObjects[_nextId];
							int _entityId = _collisionObject->getUserIndex();

							if (_entityId < MAX_ENTITIES && _entityId > 0){
								auto _value = std::find_if(m_entities.begin(), m_entities.end(),
									[&_entityId](const Entity &entity) -> bool{ return entity.getId() == _entityId; });

								if (_value != m_entities.end()){
									_pickedEntity = &*_value;
									break;
								}
							}
						}

					}

					if (_pickedEntity != nullptr){
						bool _entityAlreadyAdded = m_selectedEntities.find(_pickedEntity) != m_selectedEntities.end();

						if (_input->isButtonHeld(KEYBOARD_BUTTON(LCTRL)))
						{
							if (_entityAlreadyAdded){
								m_selectedEntities.erase(_pickedEntity);

								if (m_selectedEntities.size() > 0)
									setSelectedEntity(_pickedEntity);
								else setSelectedEntity(nullptr);
							} else{
								m_selectedEntities.insert(_pickedEntity);
								setSelectedEntity(_pickedEntity);
							}
						} else{
							if (_entityAlreadyAdded){
								setSelectedEntity(_pickedEntity);
							} else{
								m_selectedEntities.clear();
								m_selectedEntities.insert(_pickedEntity);
								setSelectedEntity(_pickedEntity);
							}
						}

					}

				}
				else if (_input->m_mouseLeftDown)
				{
					if (m_selectedEntity != nullptr){
						auto &_camera = _variables.m_camera;
						auto &_matrixComponent = m_selectedEntity->getComponent<MatrixComponent>();

						glm::vec3 _location = _matrixComponent.getLocation();
						glm::vec3 _cameraLocation = _camera->getLocation();
						glm::mat4 _viewProjectionMatrix = _camera->getProjectionMatrix() * _camera->getViewMatrix();

						float _screenWidth = static_cast<float>(_variables.m_screenWidth);
						float _screenHeight = static_cast<float>(_variables.m_screenHeight);
						glm::vec2 _screenResolution = glm::vec2(_screenWidth, _screenHeight);

						auto f_getScreenLocation = [&_screenResolution, &_viewProjectionMatrix](const glm::vec3 &location){
							glm::vec4 _perpective = _viewProjectionMatrix * glm::vec4(location, 1.0f);
							glm::vec3 _NDC = glm::vec3(_perpective) / _perpective.w;
							glm::vec2 _screenLocation = (glm::vec2(_NDC) * 0.5f + 0.5f) * _screenResolution;
							return _screenLocation;
						};


						int _mouseX, _mouseY;
						SDL_GetMouseState(&_mouseX, &_mouseY);
						_mouseY = _variables.m_screenHeight - _mouseY;

						//TODO calculate height to width ratio from arrow 3d model
						const float c_distance = glm::distance(_camera->getLocation(), _location) / 8.0f;
						const float c_offset = 2.0f * c_distance;
						const float c_width = _screenHeight / 70.0f;

						glm::vec2 _screenLocation = f_getScreenLocation(_location);
						glm::vec2 _screenLocationX = f_getScreenLocation(_location + glm::vec3(c_offset, 0.0f, 0.0f));
						glm::vec2 _screenLocationY = f_getScreenLocation(_location + glm::vec3(0.0f, c_offset, 0.0f));
						glm::vec2 _screenLocationZ = f_getScreenLocation(_location + glm::vec3(0.0f, 0.0f, c_offset));
						glm::vec3 _mouseVector = _variables.m_mouseVector;

						
						glm::vec2 _startX = _mouseVector.z < 0.0f ? _screenLocation : _screenLocationX;
						glm::vec2 _endX = _mouseVector.z < 0.0f ? _screenLocationX : _screenLocation;
						glm::vec2 _startZ = _mouseVector.x < 0.0f ? _screenLocationZ : _screenLocation;
						glm::vec2 _endZ = _mouseVector.x < 0.0f ? _screenLocation : _screenLocationZ;

						//inside X
						if (_mouseX > _startX.x && _mouseX < _endX.x){
							float _factorY = (_mouseX - _startX.x) / (_endX.x - _startX.x);
							float _currentHeight = _startX.y + (_endX.y - _startX.y) * _factorY;

							if (_mouseY > (_currentHeight - c_width) && _mouseY < (_currentHeight + c_width)){
								m_pickingX = true; //inside Y
							}
						}

						//inside X
						if (_mouseX > _startZ.x && _mouseX < _endZ.x){
							float _factorY = (_mouseX - _startZ.x) / (_endZ.x - _startZ.x);
							float _currentHeight = _startZ.y + (_endZ.y - _startZ.y) * _factorY;
				
							if (_mouseY > (_currentHeight - c_width) && _mouseY < (_currentHeight + c_width)){
								m_pickingZ = true; //inside Y
							}
						}

						//inside Y
						if (_mouseY > _screenLocation.y && _mouseY < _screenLocationY.y){
							float _factorX = (_mouseY - _screenLocation.y) / (_screenLocationY.y - _screenLocation.y);
							float _currentHeight = _screenLocation.x + (_screenLocationY.x - _screenLocation.x) * _factorX;
				
							if (_mouseX > (_currentHeight - c_width) && _mouseX < (_currentHeight + c_width)){
								m_pickingY = true;	//inside X	
							}
						}


						//Move mouse to center of the screen only if picking any axis
						if (m_pickingX || m_pickingY || m_pickingZ){
							if (m_editingMode == EDITING_TRANSLATE){
								calculateOffset();
							} else{
								//Clear relative mouse state ?? i think
								int _mouseTmp; SDL_GetRelativeMouseState(&_mouseTmp, &_mouseTmp);
								SDL_WarpMouseInWindow(NULL, _variables.m_screenWidth / 2, _variables.m_screenHeight / 2);
							}
						}			
					}		
				} 
				else if(!_input->m_mouseLeftHold){
					m_pickingX = m_pickingY = m_pickingZ = false;
				}

			}

			void EntityEditorSystem::handleButtonInput(){
				auto &_input = getWorld().getVariables().m_input;

				switch (_input->m_buttonLastDown){
					case SDLK_1: m_editingMode = EDITING_TRANSLATE; break;
					case SDLK_2: m_editingMode = EDITING_ROTATE; break;
					case SDLK_3: m_editingMode = EDITING_SCALE; break;

					case SDLK_a:
					if (_input->m_keymod == SDL_Keymod::KMOD_LCTRL){
						m_selectedEntities.clear();
						setSelectedEntity(&m_entities[0]);

						for (auto &_entity : m_entities)
							m_selectedEntities.insert(&_entity);
					}

					break;

					case SDLK_DELETE:
					for (auto _entity : m_selectedEntities){
						getWorld().removeEntity(*_entity);

						auto _body = _entity->getComponent<CollisionComponent>().getBody();
						getWorld().getVariables().m_collision->removeBody(*_body);
					}

					getWorld().refresh();
					m_selectedEntities.clear();
					setSelectedEntity(nullptr);
					break;

					default:break;
				}
			}



			void EntityEditorSystem::preUpdate(float fps){
				auto &_variables = getWorld().getVariables();

				//match matrix to collision
				for (auto &_entity : m_entities){
					//if (_varibles.m_editing)break;

					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

					//&& _collisionComponent.isKinematic() //setOnlyLocation() below;
					bool _hasAnimation = _entity.hasComponent<SkeletalAnimationComponent>();
					if (_hasAnimation && _collisionComponent.isKinematic()){
						continue;
					}


					auto &_body = _collisionComponent.getBody();
					//TODO add isStaticObject
					if (!_body->isKinematicObject()){
						btTransform _transform = _body->getWorldTransform();
						_body->getMotionState()->getWorldTransform(_transform);
						_matrixComponent.setLocation(Physics::convertVec3(_transform.getOrigin()));
						if (!_hasAnimation){
							_matrixComponent.setRotation(Physics::convertQuat(_transform.getRotation()));
						}
					}

				}
			}

			void EntityEditorSystem::update(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_input = _variables.m_input;

				if (_variables.m_editing)
				{
					//Set entity values from GUI
					if (m_selectedEntity != nullptr){
						auto &_GUISystem = getWorld().getSystem<GUISystem>();
						_GUISystem.setEntityValuesFromGUI(*m_selectedEntity, m_selectedEntities);
					}

					//PRINT(_variables.m_gui.isBeingUsed())
					if (!_variables.m_gui->isBeingUsed()){
						rayPicking();
						handleButtonInput();
					
						if (_input->m_mouseLeftHold){
							editTranslation();				
						}
					}


					bool _mouseEditing = (m_editingMode == EDITING_ROTATE) || (m_editingMode == EDITING_SCALE);
					bool _movingCamera = _variables.m_camera->isMouseIn();
					if (_mouseEditing && !_movingCamera){
						bool _pickingAny = m_pickingX || m_pickingY || m_pickingZ;

						//Set relative mouse only when editing any axis
						if (_input->m_mouseLeftDown && _pickingAny){
							SDL_SetRelativeMouseMode(SDL_TRUE);
						} else if (!_input->m_mouseLeftHold){
							SDL_SetRelativeMouseMode(SDL_FALSE);
						}

						if (_input->m_mouseLeftHold){		
							editRotation();
							editScaleScreen();
						}
					} else if(!_movingCamera){
						SDL_SetRelativeMouseMode(SDL_FALSE);
					}
					
				}
				else{
					if (m_selectedEntity != nullptr){
						m_selectedEntity = nullptr;
						m_selectedEntities.clear();
					}
				}

			}

			void EntityEditorSystem::postUpdate(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_rendererSystem = getWorld().getSystem<RendererSystem>();
				auto &_finalFramebuffer = _variables.m_renderer->getFinalFramebuffer();
				auto &_GFrambuffer = _variables.m_renderer->getGFramebuffer();

				_finalFramebuffer.bind();
				_finalFramebuffer.setDrawBuffers({ 0 });
				_finalFramebuffer.setDepthTexture(_GFrambuffer.getDepthTexture());
				{
					glEnable(GL_BLEND);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
					glDepthMask(GL_FALSE);

					//TODO render lines with shader
					glUseProgram(0);
					renderEditorMeshes(); 
					glColor4f(1, 1, 1, 1);
					glLineWidth(1.0f);

					//glEnable(GL_DEPTH_TEST);

					glUseProgram(0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					auto &_collision = _variables.m_collision;
					for (auto &_body : _collision->getAllBodies()){
						//m_collisionRenderer.drawBody(_body.get());			
					}
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

					/*for (auto &_entity : m_entities){
						auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
						auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
						//if (!_collisionComponent.isRagdoll())continue;

						for (auto _body : _collisionComponent.getBodies()){
							//glm::vec3 _randColor = Utilities::randomVec3(0.0f, 1.0f, _entity.getId());
							//Utilities::print(_randColor);
							//_rendererSystem.renderCollisionBody(_body.get(), glm::vec4(1.0f, 0.0f, 0.5f, 0.5f));
						}

						//TODO DRAW SKELETON JOINTS WITHOUT DEPRECATED FUNCTIONS
						if (_entity.hasComponent<SkeletalAnimationComponent>()){
							auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();

							auto &_skeleton = _animationComponent.getSkeleton();
							auto &_boneTransforms = _animationComponent.getTransforms();
							auto &_hipTransform = _matrixComponent.getMatrix() * 
								_skeleton.m_globalInverseTransform * _boneTransforms[_skeleton.m_boneMap["hip"]];

							for (std::size_t i = 0; i < _boneTransforms.size(); i++){
								_rendererSystem.renderPrimitive("cube", _matrixComponent.getMatrix() * 
									_skeleton.m_globalInverseTransform * _boneTransforms[i], 
									glm::vec4(0.0f, 0.0f, 1.5f, 1.0f));

								_rendererSystem.renderPrimitive("cube", _matrixComponent.getMatrix() * 
									_skeleton.m_globalInverseTransform * _boneTransforms[i + 1], 
									glm::vec4(0.0f, 0.0f, 1.5f, 1.0f));
							}
						}

					}//*/

					glDepthMask(GL_TRUE);
					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH_TEST);
					glDisable(GL_BLEND);
				}
				_finalFramebuffer.setDepthTexture(0);
				_finalFramebuffer.setDrawBuffers();
				_finalFramebuffer.unbind();
			}



			void EntityEditorSystem::renderEditorMeshes(){
				//return;

				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;

				glm::vec3 _cameraLocation = _camera->getLocation();
				glm::mat4 _viewProjectionMatrix = _camera->getProjectionMatrix() * _camera->getViewMatrix();

				float _screenWidth = static_cast<float>(_variables.m_screenWidth);
				float _screenHeight = static_cast<float>(_variables.m_screenHeight);
				glm::vec2 _screenResolution = glm::vec2(_screenWidth, _screenHeight);

				auto f_getScreenLocation = [&_screenResolution, &_viewProjectionMatrix](const glm::vec3 &location){
					glm::vec4 _perpective = _viewProjectionMatrix * glm::vec4(location, 1.0f);
					glm::vec3 _NDC = glm::vec3(_perpective) / _perpective.w;
					glm::vec2 _screenLocation = (glm::vec2(_NDC) * 0.5f + 0.5f) * _screenResolution;
					return _screenLocation;
				};


				for (auto &_entity : m_selectedEntities){
					auto &_collision = _entity->getComponent<CollisionComponent>();
					auto &_matrix = _entity->getComponent<MatrixComponent>();
					auto _body = _collision.getBody();

					if (_entity->hasComponent<LightComponent>()){
						glColor4f(1, 1, 1, 1);
						glLineWidth(5.0f);
						Meshes::drawVector(_matrix.getLocation(),_entity->getComponent
							<LightComponent>().getDirection() * 100.0f);
					}


					/*btVector3 _btMin, _btMax;
					btTransform _transform = _body->getWorldTransform();
					_body->getCollisionShape()->getAabb(_transform, _btMin, _btMax);

					glm::vec3 _min(Physics::convertVec3(_btMin) * 1.05f);
					glm::vec3 _max(Physics::convertVec3(_btMax) * 1.05f);*/

					glm::mat2x3 _boundingBox = _collision.getBoundingBox();
					glm::vec3 _min(_boundingBox[0] * 1.05f);
					glm::vec3 _max(_boundingBox[1] * 1.05f);

					glm::vec3 _scale = glm::abs(_max - _min) * 0.5f;
					glm::vec3 _center = (_min + _max) / 2.0f;

					glm::vec3 _bodyCenter = Physics::convertVec3(_body->getWorldTransform().getOrigin());
					glm::vec3 _correctedDistance = _bodyCenter - _center;
					_min += _correctedDistance;
					_max += _correctedDistance;

					glEnable(GL_BLEND);
					glEnable(GL_DEPTH_TEST);

				

					glEnable(GL_BLEND);
					glColor4f(1.0, 1.0, 1.0, 1.0);

					glm::vec3 _location;
					if (m_selectedEntity != nullptr){
						_location = m_selectedEntity->getComponent<MatrixComponent>().getLocation();
					}

					glLineWidth(1.0);
					glPushMatrix();
					{
						glBegin(GL_LINES);

						//FRONT FACE
						glVertex3f(_min.x, _min.y, _max.z); glVertex3f(_min.x, _max.y, _max.z);
						glVertex3f(_min.x, _max.y, _max.z); glVertex3f(_max.x, _max.y, _max.z);
						glVertex3f(_max.x, _max.y, _max.z); glVertex3f(_max.x, _min.y, _max.z);
						glVertex3f(_max.x, _min.y, _max.z); glVertex3f(_min.x, _min.y, _max.z);

						//BACK FACE
						glVertex3f(_min.x, _min.y, _min.z); glVertex3f(_min.x, _max.y, _min.z);
						glVertex3f(_min.x, _max.y, _min.z); glVertex3f(_max.x, _max.y, _min.z);
						glVertex3f(_max.x, _max.y, _min.z); glVertex3f(_max.x, _min.y, _min.z);
						glVertex3f(_max.x, _min.y, _min.z); glVertex3f(_min.x, _min.y, _min.z);

						//UPPER FACE
						glVertex3f(_min.x, _max.y, _max.z); glVertex3f(_min.x, _max.y, _min.z);
						glVertex3f(_max.x, _max.y, _max.z); glVertex3f(_max.x, _max.y, _min.z);

						//BOTTOM FACE
						glVertex3f(_min.x, _min.y, _max.z); glVertex3f(_min.x, _min.y, _min.z);
						glVertex3f(_max.x, _min.y, _max.z); glVertex3f(_max.x, _min.y, _min.z);	

						glEnd();
					}
					glPopMatrix();
					

					const float c_distance = glm::distance(_camera->getLocation(), _location) / 8.0f;
					glm::mat4 _rotationMatrix = glm::mat4(1.0f); // glm::toMat4(_matrix.getRotation());
					glm::vec3 _locationX = glm::vec3(_rotationMatrix * glm::vec4(2.0 * c_distance, 0.0f, 0.0f, 1.0f)) + _location;
					glm::vec3 _locationY = glm::vec3(_rotationMatrix * glm::vec4(0.0f, 2.0 * c_distance, 0.0f, 1.0f)) + _location;
					glm::vec3 _locationZ = glm::vec3(_rotationMatrix * glm::vec4(0.0f, 0.0f, 2.0 * c_distance, 1.0f)) + _location;
	
					const glm::vec3 _axisXColor = glm::vec3(0.86f, 0.0f, 0.0f);
					const glm::vec3 _axisYColor = glm::vec3(0.0f, 0.86f, 0.0f);
					const glm::vec3 _axisZColor = glm::vec3(0.0f, 0.0f, 0.86f);
					glm::vec3 _colorX = m_pickingX ? glm::vec3(1.0f, 1.0f, 0.0f) : _axisXColor;
					glm::vec3 _colorY = m_pickingY ? glm::vec3(1.0f, 1.0f, 0.0f) : _axisYColor;
					glm::vec3 _colorZ = m_pickingZ ? glm::vec3(1.0f, 1.0f, 0.0f) : _axisZColor;


					glLineWidth(3.0);
					glPushMatrix();
					{
						if (!m_pickingX && !m_pickingY && !m_pickingZ)
						{
							const float c_doubleAxisScale = c_distance * 0.5f;

							// DRAW DOUBLE AXIS EDIT QUADS
							glBegin(GL_LINES);
								glColor3f(_colorX.x, _colorX.y, _colorX.z);
								glVertex3f(_location.x + c_doubleAxisScale, _location.y, _location.z);
								glVertex3f(_location.x + c_doubleAxisScale, _location.y + c_doubleAxisScale, _location.z);			
								glColor3f(_colorY.x, _colorY.y, _colorY.z);
								glVertex3f(_location.x, _location.y + c_doubleAxisScale, _location.z);
								glVertex3f(_location.x + c_doubleAxisScale, _location.y + c_doubleAxisScale, _location.z);
							glEnd();

							glBegin(GL_LINES);
								glColor3f(_colorX.x, _colorX.y, _colorX.z);
								glVertex3f(_location.x + c_doubleAxisScale, _location.y, _location.z);
								glVertex3f(_location.x + c_doubleAxisScale, _location.y, _location.z + c_doubleAxisScale);
								glColor3f(_colorZ.x, _colorZ.y, _colorZ.z);
								glVertex3f(_location.x, _location.y, _location.z + c_doubleAxisScale);
								glVertex3f(_location.x + c_doubleAxisScale, _location.y, _location.z + c_doubleAxisScale);
							glEnd();
								
							glBegin(GL_LINES);
								glColor3f(_colorY.x, _colorY.y, _colorY.z);
								glVertex3f(_location.x, _location.y + c_doubleAxisScale, _location.z);
								glVertex3f(_location.x, _location.y + c_doubleAxisScale, _location.z + c_doubleAxisScale);
								glColor3f(_colorZ.x, _colorZ.y, _colorZ.z);
								glVertex3f(_location.x, _location.y, _location.z + c_doubleAxisScale);
								glVertex3f(_location.x, _location.y + c_doubleAxisScale, _location.z + c_doubleAxisScale);
							glEnd();

							// X-AXIS ARROW
							glBegin(GL_TRIANGLE_FAN);			
								glColor3f(_colorX.x, _colorX.y, _colorX.z);
								glVertex3f(_locationX.x, _locationX.y, _locationX.z);
								for (float angle = 0.0; angle <= 360.0; angle += 30.0){
									float _z = cos(glm::radians(angle)) * c_distance * 0.1f;
									float _y = sin(glm::radians(angle)) * c_distance * 0.1f;
									glVertex3f(_locationX.x - c_distance * 0.35f, _locationX.y + _y, _locationX.z + _z);
								}
							glEnd();

							// Y-AXIS ARROW
							glBegin(GL_TRIANGLE_FAN);
								glColor3f(_colorY.x, _colorY.y, _colorY.z); 
								glVertex3f(_locationY.x, _locationY.y, _locationY.z);
								for (float angle = 0.0; angle <= 360.0; angle += 30.0){
									float _x = cos(glm::radians(angle)) * c_distance * 0.1f;
									float _z = sin(glm::radians(angle)) * c_distance * 0.1f;
									glVertex3f(_locationY.x + _x, _locationY.y - c_distance * 0.35f, _locationY.z + _z);
								}
							glEnd();

							// Z-AXIS ARROW
							glBegin(GL_TRIANGLE_FAN);
								glColor3f(_colorZ.x, _colorZ.y, _colorZ.z);
								glVertex3f(_locationZ.x, _locationZ.y, _locationZ.z);
								for (float angle = 0.0; angle <= 360.0; angle += 30.0){
									float _x = cos(glm::radians(angle)) * c_distance * 0.1f;
									float _y = sin(glm::radians(angle)) * c_distance * 0.1f;
									glVertex3f(_locationZ.x + _x, _locationZ.y + _y, _locationZ.z - c_distance * 0.35f);
								}
							glEnd();
						}


						bool _pickingAny = m_pickingX || m_pickingY || m_pickingZ;
						bool _pickingAll = m_pickingX && m_pickingY && m_pickingZ;

						if (m_editingMode == EDITING_TRANSLATE)
						{
							const float c_planeSize = WORLD_SIZE;
							glEnable(GL_DEPTH_TEST);

							// DRAW DOUBLE PICKING PLANE XY
							if (m_pickingX && m_pickingY && !_pickingAll){
								glBegin(GL_QUADS);
									glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
									glVertex3f(_location.x - c_planeSize, _location.y - c_planeSize, _location.z);
									glVertex3f(_location.x - c_planeSize, _location.y + c_planeSize, _location.z);
									glVertex3f(_location.x + c_planeSize, _location.y + c_planeSize, _location.z);
									glVertex3f(_location.x + c_planeSize, _location.y - c_planeSize, _location.z);
								glEnd();
							}

							// DRAW DOUBLE PICKING PLANE XZ
							if (m_pickingX && m_pickingZ && !_pickingAll){
								glBegin(GL_QUADS);
									glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
									glVertex3f(_location.x - c_planeSize, _location.y, _location.z - c_planeSize);
									glVertex3f(_location.x - c_planeSize, _location.y, _location.z + c_planeSize);
									glVertex3f(_location.x + c_planeSize, _location.y, _location.z + c_planeSize);
									glVertex3f(_location.x + c_planeSize, _location.y, _location.z - c_planeSize);
								glEnd();
							}

							// DRAW DOUBLE PICKING PLANE YZ
							if (m_pickingY && m_pickingZ && !_pickingAll){
								glBegin(GL_QUADS);
									glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
									glVertex3f(_location.x, _location.y - c_planeSize, _location.z - c_planeSize);
									glVertex3f(_location.x, _location.y + c_planeSize, _location.z - c_planeSize);
									glVertex3f(_location.x, _location.y + c_planeSize, _location.z + c_planeSize);
									glVertex3f(_location.x, _location.y - c_planeSize, _location.z + c_planeSize);
								glEnd();
							}

							glDisable(GL_DEPTH_TEST);
							glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						}


						bool _atLeastTwo = m_pickingX ? (m_pickingY || m_pickingZ) : (m_pickingY && m_pickingZ);
						bool _rotateOrScale = m_editingMode == EDITING_ROTATE || m_editingMode == EDITING_SCALE;
						bool _showAllAxis = _atLeastTwo && _rotateOrScale;

						if (m_pickingX || _showAllAxis){
							glBegin(GL_LINES);
								glColor3f(_axisXColor.x, _axisXColor.y, _axisXColor.z);
								glVertex3f(_location.x - WORLD_SIZE, _location.y, _location.z);
								glVertex3f(_location.x + WORLD_SIZE, _location.y, _location.z);
							glEnd();
						}

						if ((m_pickingX || !_pickingAny) && !_showAllAxis){
							glBegin(GL_LINES);
								glColor3f(_colorX.x, _colorX.y, _colorX.z);
								glVertex3f(_location.x, _location.y, _location.z);
								glVertex3f(_locationX.x - c_distance * 0.25f, _locationX.y, _locationX.z);
							glEnd();
						}
						

						if (m_pickingY || _showAllAxis){
							glBegin(GL_LINES);
								glColor3f(_axisYColor.x, _axisYColor.y, _axisYColor.z);
								glVertex3f(_location.x, _location.y - WORLD_SIZE, _location.z);
								glVertex3f(_location.x, _location.y + WORLD_SIZE, _location.z);
							glEnd();
						}

						if ((m_pickingY || !_pickingAny) && !_showAllAxis){
							glBegin(GL_LINES);
								glColor3f(_colorY.x, _colorY.y, _colorY.z);
								glVertex3f(_location.x, _location.y, _location.z);
								glVertex3f(_locationY.x, _locationY.y - c_distance * 0.25f, _locationY.z);
							glEnd();
						}


						if (m_pickingZ || _showAllAxis){
							glBegin(GL_LINES);
								glColor3f(_axisZColor.x, _axisZColor.y, _axisZColor.z);
								glVertex3f(_location.x, _location.y, _location.z - WORLD_SIZE);
								glVertex3f(_location.x, _location.y, _location.z + WORLD_SIZE);
							glEnd();
						}

						if ((m_pickingZ || !_pickingAny) && !_showAllAxis){
							glBegin(GL_LINES);
								glColor3f(_colorZ.x, _colorZ.y, _colorZ.z);
								glVertex3f(_location.x, _location.y, _location.z);
								glVertex3f(_locationZ.x, _locationZ.y, _locationZ.z - c_distance * 0.25f);
							glEnd();
						}

						glLineWidth(1.0);
						glColor4f(1.0, 1.0, 1.0, 1.0);
					}
					glPopMatrix();

					glDisable(GL_BLEND);
					glEnable(GL_DEPTH_TEST);				
				}
			

				for (auto &_entity : m_entities){
					if (!_entity.hasComponent<SkeletalAnimationComponent>() || 
						_entity.getComponent<SkeletalAnimationComponent>().getTransforms().empty())
					{
						continue;
					}

					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
					auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
					auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();
					
					auto &_skeleton = _animationComponent.getSkeleton();
					auto &_boneTransforms = _animationComponent.getTransforms();

					glm::mat4 _modelMatrix = _matrixComponent.getMatrix() * _skeleton.m_globalInverseTransform;
					glm::vec4 _zeroPoint = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


					glLineWidth(3.0f);
					glPointSize(10.0f);

					std::queue<AnimationBoneTree*> _queue;
					_queue.push(_skeleton.m_animationBoneTreeRoot.get());
					while (!_queue.empty()){
						//break;

						auto _node = _queue.front();
						_queue.pop();

						_zeroPoint = _boneTransforms[_node->m_boneId][3];

						//Render animation skeleton
						glm::vec3 _p1(_modelMatrix /** _boneTransforms[_node->m_boneId]*/ * _zeroPoint);
						glm::vec3 _p11(_modelMatrix * _boneTransforms[_node->m_boneId] * glm::vec4(0, 0, 0, 1));


						glm::mat3 _rotation(_boneTransforms[_node->m_boneId]);

						glm::vec3 _r1 = glm::normalize(_rotation[1]) * 0.33f;
						glm::vec3 _r2 = _p1 + _r1;
	
						glBegin(GL_POINTS); 
							glColor3f(0, 0, 1);
							glVertex3f(_p1.x, _p1.y, _p1.z); 
							glColor3f(1, 0, 0);
							glVertex3f(_p11.x, _p11.y, _p11.z);
						glEnd();

						glBegin(GL_LINES);
						//Joint rotation orietation 
						glColor3f(0, 1, 0);
						glVertex3f(_p1.x, _p1.y, _p1.z);
						glVertex3f(_r2.x, _r2.y, _r2.z);
						glEnd();

						for (auto &_child : _node->m_children){
							//PRINT(_child->m_name);
							_queue.push(_child.get());

							_zeroPoint = _boneTransforms[_child->m_boneId][3];
							glm::vec3 _p2(_modelMatrix /** _boneTransforms[_child->m_boneId]*/ * _zeroPoint);

							glBegin(GL_LINES);
								//Connection between joints
								glColor3f(1, 1, 1);
								glVertex3f(_p1.x, _p1.y, _p1.z);
								glVertex3f(_p2.x, _p2.y, _p2.z);

								
								
							glEnd();	

							//Reset color
							glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						}
					}

					glPointSize(25.0f);
					glColor3f(0, 1, 0);

					unsigned int _rootId = _skeleton.m_animationBoneTreeRoot->m_boneId;
					glm::vec3 _newLocation(_modelMatrix * _zeroPoint);
					//_newLocation -= glm::vec3(-0.0432967f, 2.06322f, -0.195435f);

					glBegin(GL_POINTS);
					//glVertex3f(_newLocation.x, _newLocation.y, _newLocation.z);
					glEnd();

					glColor3f(1, 1, 1);


					//Utilities::print(_newLocation);
					if (_animationComponent.getAnimationPlayer().justFinishedPlayingAction()){
						//PRINT("SET LOC");					
						//Location _change = _newLocation - _matrixComponent.getLocation();

						//_matrixComponent.setLocation(_newLocation);
						Utilities::print(_matrixComponent.getLocation());
						//_collisionComponent.setLocation(_change);
					}

					//_matrixComponent.setRotation(Rotation(AXIS_Y, _camera->getYaw()));

					//Utilities::print(_matrixComponent.getLocation());
					
					//_collisionComponent.setLocation(_newLocation);

					//Location _p2(_matrixComponent.getLocation());
					

					glLineWidth(5.0f);


					/*glBegin(GL_LINES);
					glVertex3f(_p1.x, _p1.y, _p1.z);
					glVertex3f(_p2.x, _p2.y, _p2.z);
					glEnd();*/

					/*glBegin(GL_QUADS);
					glVertex3f(0.0f, -10.0f, 0.0f);
					glVertex3f(0.0f, 10.0f, 0.0f);
					glVertex3f(0.0f, 10.0f, 30.0f);
					glVertex3f(0.0f, -10.0f, 30.0f);
					glEnd();*/

					glPointSize(1.0f);
					glLineWidth(1.0f);



				}

			}

		}
	}
}
