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


#include "InitSystem.hpp"

#include "../World/World.hpp"
#include "../Camera/Camera.hpp"
#include "../Collision/Collision.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Input/Input.hpp"
#include "../Renderer/DeferredRenderer.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{

			InitSystem::InitSystem(){

			}

			InitSystem::~InitSystem(){

			}


			void InitSystem::init(){

			}


			void InitSystem::controllCamera(){
				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;
				auto &_input = _variables.m_input;

				/*if (!_variables.m_editing){
				_camera->setMouseIn(true);
				_camera->control(5.0f);
				}*/

				/*if (!_variables.m_editing){
				_camera->transform(true);
				}*/
			}

			void InitSystem::preUpdate(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_collision = _variables.m_collision;
				auto &_camera = _variables.m_camera;
				auto &_renderer = _variables.m_renderer;

			
				if (!_variables.m_editing){
					glm::vec3 _offset = _camera->getThirdPersonOffset();
					glm::vec3 _vectorToOffset = glm::normalize(_offset);
					float _distanceToCamera = _camera->getMaxThirdPersonDistance();

					//PRINT(_distanceToCamera << " " << _camera->getThirdPersonDistance());

					auto _raycast = _collision->rayTracingClosest(
						_camera->getLocation() - _camera->getVector() * 0.5f, //Offset ray location a little
						_vectorToOffset, 
						_distanceToCamera,
						Physics::GROUP_STATIC | Physics::GROUP_DYNAMIC
					);

					if (_raycast.hasHit()){
						auto _collisionObject = _raycast.m_collisionObject;
						int _entityId = _collisionObject->getUserIndex();
						std::string _entityName = getWorld().getEntityName(_entityId);

						glm::vec3 _hitLocation = Physics::convertVec3(_raycast.m_hitPointWorld);
						glm::vec3 _hitNormal = Physics::convertVec3(_raycast.m_hitNormalWorld);

						glm::vec3 _newVector = _hitLocation - _camera->getLocation();
						//TODO offset third person camera location according to surface normal (prevent clipping)
						float _newDistance = glm::length(_newVector); // -1.0f;
						_newDistance = glm::max(0.0f, _newDistance);

						//If previous distance was shorter then interpolate 
						float _previousDistance = _camera->getThirdPersonDistance();
						if (_previousDistance < _newDistance){
							m_currentCameraDistance = glm::mix(
								m_currentCameraDistance,_newDistance, 0.05f);
						} else{
							m_currentCameraDistance = _newDistance;
						}

						//PRINT(_entityName << " " << SDL_GetTicks());
					} else{
						//m_currentCameraDistance = _camera->m_maxThirdPersonDistance;
						m_currentCameraDistance = glm::mix(m_currentCameraDistance,
							_camera->m_maxThirdPersonDistance, 0.05f);
						//_camera->restoreThirdPersonDistance();
					}

					//Update distance
					//TODO re-enable
					//_camera->setThirdPersonDistance(m_currentCameraDistance);
					
				}

				//Setup legacy mode
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glMultMatrixf(&_variables.m_camera->getViewMatrix()[0][0]);
			}

			void InitSystem::update(float fps){

			}

			void InitSystem::postUpdate(float fps){

			}

		}
	}
}
