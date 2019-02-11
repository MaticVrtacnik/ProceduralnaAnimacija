#include "RendererSystem.hpp"

#include <chrono>
#include <glm\gtx\transform.hpp>

#include "../Component/ComponentIncluder.hpp"

#include "../World/World.hpp"
#include "../Camera/Camera.hpp"
#include "../GUI/GUI.hpp"
#include "../GUI/GUITab.hpp"
#include "../GUI/GUIToggle.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Utility/RandomUtilities.hpp"
#include "../Renderer/SimpleRenderer.hpp"
#include "../Cubemap/CubemapTexture.hpp"
#include "../Renderer/DeferredRenderer.hpp"
#include "../Texture/Material.hpp"

#include "../Shader/Shader.hpp"
#include "../System/EntityEditorSystem.hpp"

namespace Engine{
	namespace Entities{
		namespace Systems{


			RendererSystem::RendererSystem(){
				addRequired<MatrixComponent>();
				addRequired<MeshComponent>();
				addRequired<CollisionComponent>();
				addRequired<MaterialComponent>();
				addExcluded<SkeletalAnimationComponent>();

				glEnable(GL_DEPTH_TEST);
				glCullFace(GL_BACK);		
				glEnable(GL_CULL_FACE);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			}

			RendererSystem::~RendererSystem(){

			}

			void RendererSystem::init(){
				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				Entity _cubeEntity = getWorld().addEntity("cube");
				_cubeEntity.addComponent<MeshComponent>(_resourceManager.getMesh("primitives", "cube"));
				_cubeEntity.activate();

				
				
				auto &_guiIndex = _variables.m_gui->getIndex();
				//auto &_tab = _guiIndex.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab");
				//auto &_child = _tab.getWindow("quad").getChild<GUI::GUIButton>("post1");
				//_child.setTexture(getWorld().getVariables().m_postProcessing.getLensFlare().getResult());

				
				//auto &_guiIndex = getWorld().getVariables().m_gui->getIndex();
				auto &_quad = _guiIndex.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").getWindow("quad");
			}

			void RendererSystem::generateProbeTextures(){
			}

			void RendererSystem::createProbeTextureArrays(){
				
				
			}

			unsigned int RendererSystem::getNearestProbe(const glm::vec3 &location){
				const float c_maxDistance = 100.0f;
				float _shortestDistance = -1.0f;
				
				//unsigned int _nearestCubemap = getWorld().getSystem<EnvironmentSystem>().getSkybox().getSelectedCubemap();
				unsigned int _nearestCubemap = -1;

				//_nearestCubemap = m_irradianceTextures[0];
	
				/*for (auto _entity : m_environmentalProbes){
					auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

					float _distance = glm::distance(location, _matrixComponent.getLocation());
					if (_distance < c_maxDistance && (_shortestDistance == -1 || _distance < _shortestDistance)){
						_shortestDistance = _distance;

						auto &_probeComponent = _entity.getComponent<ProbeComponent>();
						_nearestCubemap = _probeComponent.getEnvironmentalCubemap();
					}
				}*/

				return _nearestCubemap;
			}
			
			void RendererSystem::renderShadows(){
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				glm::mat4 _sunViewProjectionMatrix =
					_variables.m_renderer->getSunViewProjectionMatrix();

				//Sun location animation
				//TODO move sun entity
				float _time = SDL_GetTicks() / 1000.0f;
				/*_variables.m_renderer->setSunLocation(glm::vec3(
					-120 + 50.0f * sin(_time / 10.0f), 
					450.0f, 
					350.0f
				));*/

				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glDisable(GL_CULL_FACE);

				//Grass/tree paint particle shadows
				/*auto &_paintParticlesShadowShader = _resourceManager.getShader("shadows/paintParticlesVarianceShadow");
				_paintParticlesShadowShader.bind();
				{
					glEnable(GL_CULL_FACE);

					glm::mat4 _viewMatrix = _camera->getViewMatrix();
					glm::mat4 _projectionMatrix = _camera->getProjectionMatrix();
					glm::mat4 _viewProjectionMatrix = _projectionMatrix * _viewMatrix;

					_paintParticlesShadowShader.setUniform("u_viewMatrix", _viewMatrix);
					_paintParticlesShadowShader.setUniform("u_projectionMatrix", _projectionMatrix);


					Textures::bind2DTexture(_resourceManager.getTextureId("painting/grass_opacity.png"), 0);

					for (auto &_entity : this->m_entities){
						if (!_entity.hasComponent<PaintComponent>()){
							continue; //Skip entities WITHOUT paint component
						}

						auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
						auto &_paintComponent = _entity.getComponent<PaintComponent>();

						glm::mat4 _modelMatrix = _matrixComponent.getMatrix();

						_paintParticlesShadowShader.setUniform("u_worldLocation", _matrixComponent.getLocation());
						_paintParticlesShadowShader.setUniform("u_modelMatrix", _modelMatrix);
						_paintParticlesShadowShader.setUniform("u_modelViewMatrix", _viewMatrix * _modelMatrix);
						_paintParticlesShadowShader.setUniform("u_modelViewProjectionMatrix",
							_viewProjectionMatrix * _modelMatrix);


						glBindVertexArray(_renderer->getScreenQuadVertexArray());
						{
							auto &_locationsBuffer = _paintComponent.m_materialParticleLocationsArrayBuffer;

							_locationsBuffer.bind();
							glEnableVertexAttribArray(1);
							glVertexAttribDivisor(1, 1);
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

							glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _locationsBuffer.m_size);

							glDisableVertexAttribArray(1);
							glVertexAttribDivisor(0, 0);
							glVertexAttribDivisor(1, 0);
						}
					}

					glBindVertexArray(0);
				}
				_paintParticlesShadowShader.unbind();*/


				//auto &_exponentialShadowShader = _resourceManager.getShader("exponentialShadow");
				auto &_varianceShadowShader = _resourceManager.getShader("varianceShadow");			
				_varianceShadowShader.bind();
				{
					for (auto &_entity : m_entities){
						if (_entity.hasComponent<LightComponent>()){
							auto &_lightComponent = _entity.getComponent<LightComponent>();
							if (_lightComponent.getType() == LIGHT_DIRECTIONAL){
								continue;
							}
						} /*else if (_entity.hasComponent<PaintComponent>()){
							continue; //Continue if entity has PaintComponent that requires tessellation
						}*/

						auto &_meshComponent = _entity.getComponent<MeshComponent>();
						auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

						_varianceShadowShader.setUniform("u_modelViewProjectionMatrix",
							_sunViewProjectionMatrix * _matrixComponent.getMatrix());

						_meshComponent.bindVertexArray();
						std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
						glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
					}
				}
				_varianceShadowShader.unbind();

				glBindVertexArray(0);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}

			void RendererSystem::renderOccludedLights(){
				//TODO remove
			}

			void RendererSystem::renderEntitiesTessellated(){
				//TODO
			}

			void RendererSystem::renderWaterEntities(){
				//TODO screenspace reflection
			}

			void RendererSystem::renderPaintDepth(){
			}


			void RendererSystem::preUpdate(float fps){
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				//SHADOW
				const unsigned int _resolution = Graphics::Renderer::c_shadowResolution;
				auto &_variables = getWorld().getVariables();
				auto &_renderer = _variables.m_renderer;

				
				_renderer->getShadowFramebuffer().bind(); 
				{
					glViewport(0, 0, _resolution, _resolution);
					renderShadows();
					glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
				}
				_renderer->getShadowFramebuffer().unbind();
		
				renderPaintDepth();
			}
			
			void RendererSystem::update(float fps){
				if (m_entities.empty()){
					return;
				}

				//TODO do something with this
				//generateProbeTextures();


				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;
				auto &_renderer = _variables.m_renderer;

				glm::mat4 _viewMatrix = _camera->getViewMatrix();
				glm::mat4 _inverseViewMatrix = glm::inverse(_viewMatrix);
				glm::mat4 _projectionMatrix = _camera->getProjectionMatrix();


				auto &_gui = _variables.m_gui->getIndex();
				auto &_terrainGUI = _gui.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
					getWindow("environment").getChild<GUI::GUIWindowStack>("stack").getWindow("terrain");

				bool _wireframe = _terrainGUI.getChild<GUI::GUIToggle>("wireframe").isOn();
				if (_wireframe)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				_renderer->getGFramebuffer().bind();
				{
					//TODO order entities by texture type
					//auto _usedTexture = m_entities.front().getComponent<MeshComponent>().getMesh()->m_textures;
					//if (getWorld().getVariables().m_input.m_buttonDown == SDLK_LEFT)

					glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);

					glDisable(GL_BLEND);
					glEnable(GL_DEPTH_TEST);
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					//glDisable(GL_CULL_FACE);


					glDisable(GL_CULL_FACE);
					glEnable(GL_CULL_FACE);

					//Draw normal deferred entities
					auto &_shader = _resourceManager.getShader("deferred");
					_shader.bind();
					{
						auto _paperMaterial = MaterialComponent(_resourceManager.getMaterialPtr("paper"));
						//auto _paperMaterial = MaterialComponent(_resourceManager.getMaterialPtr("tiles/dark"));
						//auto _paperMaterial = MaterialComponent(_resourceManager.getMaterialPtr("road/asphalt_old"));

						_shader.setUniform("u_projectionMatrix", _projectionMatrix);
						_shader.setUniform("u_inverseViewMatrix", _inverseViewMatrix);
						_shader.setUniform("u_cameraLocation", _camera->getThirdPersonLocation());

						for (auto &_entity : this->m_entities){

							auto &_meshComponent = _entity.getComponent<MeshComponent>();
							auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
							auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
							auto &_materialComponent = _entity.getComponent<MaterialComponent>();
							auto &_material = _materialComponent.getMaterial();

							if (_entity.hasComponent<LightComponent>()){
								const auto &_lightComponent = _entity.getComponent<LightComponent>();
								//DONT DRAW SUN TODO ADD SUN ENTITY TO RENDERER CLASS
								if (_lightComponent.getType() == LIGHT_DIRECTIONAL)continue;

								_shader.setUniform("u_color", _lightComponent.m_color * _lightComponent.m_intensity);
							} else _shader.setUniform("u_color", _materialComponent.getColor());

							//TODO remove random color
							_shader.setUniform("u_color", Utilities::randomVec3(0.0f, 1.0f, _entity.getId()));


							glm::mat4 _modelViewMatrix = _viewMatrix * _matrixComponent.getMatrix();
							glm::mat3 _normalMatrix = glm::transpose(glm::inverse(glm::mat3(_modelViewMatrix)));

							_shader.setUniform("u_modelViewMatrix", _modelViewMatrix);
							_shader.setUniform("u_prevModelViewProjectionMatrix", _projectionMatrix *
								_camera->getPreviousViewMatrix() * _matrixComponent.getPreviousMatrix());
							_shader.setUniform("u_normalMatrix", _normalMatrix);
							_shader.setUniform("u_inverseNormalMatrix", glm::inverse(_normalMatrix));
							_shader.setUniform("u_material", glm::vec4(0.0f));
							_shader.setUniform("u_displacementScale", _material->m_displacementScale);

							_meshComponent.bindVertexArray();
							{
								Textures::bind2DTexture(_materialComponent.getAlbedoTexture(), 0);
								Textures::bind2DTexture(_materialComponent.getNormalTexture(), 1);
								Textures::bind2DTexture(_materialComponent.getRoughnessTexture(), 2);
								Textures::bind2DTexture(_materialComponent.getMetallicTexture(), 3);
								Textures::bind2DTexture(_materialComponent.getOcclusionTexture(), 4);
								Textures::bind2DTexture(_materialComponent.getHeightTexture(), 5);//*/

								//std::cout << _entity.getName() << " || "; Utilities::print(_matrixComponent.getLocation());
								/*Textures::bind2DTexture(_paperMaterial.getAlbedoTexture(), 0);
								Textures::bind2DTexture(_paperMaterial.getNormalTexture(), 1);
								Textures::bind2DTexture(_paperMaterial.getRoughnessTexture(), 2);
								Textures::bind2DTexture(_paperMaterial.getMetallicTexture(), 3);
								Textures::bind2DTexture(_paperMaterial.getOcclusionTexture(), 4);
								Textures::bind2DTexture(_paperMaterial.getHeightTexture(), 5);//*/


								//Set stencil values to 1 if object is static to prevent decals from being draw on dynamic objects
								GLint _stencilValue = _collisionComponent.isStaticObject() ? 1 : 0;
								glStencilFunc(GL_ALWAYS, _stencilValue, 0xFF);

								std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
								glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
							}
						}
					}
					_shader.unbind();

					glStencilMask(0x00);
					Textures::unbind2DTexture();
					glBindVertexArray(0);
					glDisable(GL_STENCIL_TEST);
				}
				_renderer->getGFramebuffer().unbind();
				

				if (_wireframe)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			void RendererSystem::postUpdate(float fps){
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				auto &_renderer = _variables.m_renderer;

				auto &_occludedFramebuffer = _renderer->getOccludedFramebuffer();
				_occludedFramebuffer.bind();
				{
					_occludedFramebuffer.setDepthTexture(_renderer->getGFramebuffer().getDepthTexture());
					/*_variables.m_gui->getTabWindow("quad").getChild<GUI::GUIButton>("post3").
						setTexture(_occludedFramebuffer.getTexture(0));*/

					//glViewport(0, 0, _variables.m_screenWidth / 2, _variables.m_screenHeight / 2);
					glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
					//glViewport(-200, -200, _variables.m_screenWidth / 2 + 200, _variables.m_screenHeight / 2 + 200);

					glDisable(GL_CULL_FACE);
					glEnable(GL_DEPTH_TEST);
					glCullFace(GL_BACK);

					
					auto &_colorShader = _resourceManager.getShader<Shaders::Shader>("color");
					_colorShader.bind();
					{
						auto &_camera = _variables.m_camera;
						glm::mat4 _viewProjectionMatrix = 
							_camera->getProjectionMatrix() * _camera->getViewMatrix();

						for (auto &_entity : this->m_entities){
							if (!_entity.hasComponent<LightComponent>()){
								continue; //Continue if there is no light component
							}
							
							auto &_lightComponent = _entity.getComponent<LightComponent>();
							if (_lightComponent.getType() == LIGHT_DIRECTIONAL)
							{
								//TODO change sun scale dependant on distance
								auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
								auto &_meshComponent = _entity.getComponent<MeshComponent>();

								_colorShader.setUniform("u_modelViewProjectionMatrix",
									_viewProjectionMatrix * _matrixComponent.getMatrix());
								_colorShader.setUniform("u_color", _lightComponent.m_color);
								
								_meshComponent.bindVertexArray();
								{
									std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
									glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
								}

								break; //Allow only one sun
							}					
						}

						glBindVertexArray(0);
					}
					_colorShader.unbind();
					
					glEnable(GL_CULL_FACE);
					glEnable(GL_DEPTH_TEST);
				}		
				_occludedFramebuffer.unbind();

				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
			}

			void RendererSystem::onEntityAdded(Entity &entity){

			}

			void RendererSystem::onEntityRemoved(Entity &entity){

			}



			void RendererSystem::renderPrimitive(const std::string &name, 
				const glm::mat4 &modelMatrix, const glm::vec4 color)
			{
				auto &_resourceManager = getWorld().getResourceManager();
				if (!_resourceManager.hasMesh("primitives", name)){
					DEBUG_PRINT("Unable to render primitive '" << name << "'");
					return;
				}

				Entity _entity = getWorld().getEntity(name);
				if (!_entity.isValid()){
					DEBUG_PRINT("Invalid entity '" << name << "'");
					return;
				}

				glEnable(GL_BLEND);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;
				auto &_meshComponent = _entity.getComponent<MeshComponent>();

				auto &_shader = _resourceManager.getShader<Shaders::Shader>("basic");
				_shader.bind();
				{
					_shader.setUniform("u_modelViewProjectionMatrix",
						_camera->getProjectionMatrix() * _camera->getViewMatrix() * modelMatrix);
					_shader.setUniform("u_color", color);

					_meshComponent.bindVertexArray();
					{
						std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
						glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
					}
					glBindVertexArray(0);
				}
				_shader.unbind();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDisable(GL_BLEND);
			}


			void RendererSystem::renderPrimitive(
				const std::string &name,
				const glm::vec3 &location, 
				const glm::quat &rotation,
				const glm::vec3 &scale,
				const glm::vec4 color)
			{
				auto &_resourceManager = getWorld().getResourceManager();
				if (!_resourceManager.hasMesh("primitives", name)){
					DEBUG_PRINT("Unable to render primitive '" << name << "'");
					return;
				}

				Entity _entity = getWorld().getEntity(name);
				if (!_entity.isValid()){
					DEBUG_PRINT("Invalid entity '" << name << "'");
					return;
				}

				glEnable(GL_BLEND);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;
				auto &_meshComponent = _entity.getComponent<MeshComponent>();

				auto &_shader = _resourceManager.getShader<Shaders::Shader>("basic");
				_shader.bind();
				{
					glm::mat4 _modelMatrix(1.0);
					_modelMatrix *= glm::translate(location);
					_modelMatrix *= glm::toMat4(rotation);
					_modelMatrix *= glm::scale(scale);

					_shader.setUniform("u_modelViewProjectionMatrix",
						_camera->getProjectionMatrix() * _camera->getViewMatrix() * _modelMatrix);
					_shader.setUniform("u_color", color);
					
					_meshComponent.bindVertexArray();
					{
						std::size_t _polyCount = _meshComponent.getMesh().m_indices.size();
						glDrawElements(GL_TRIANGLES, _polyCount, GL_UNSIGNED_INT, 0);
					}
					glBindVertexArray(0);
				}
				_shader.unbind();	

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDisable(GL_BLEND);
			}


			void RendererSystem::renderCollisionBody(btRigidBody *body, const glm::vec4 color){
				if (body == nullptr){
					DEBUG_PRINT("Render collision body is a null pointer!");
					return;
				}

				btTransform _transform = body->getWorldTransform();
				glm::vec3 _location(Physics::convertVec3(_transform.getOrigin()));
				glm::quat _rotation(Physics::convertQuat(_transform.getRotation()));
				glm::vec3 _scale(Physics::convertVec3(body->getCollisionShape()->getLocalScaling()));

				renderCollisionShape(body->getCollisionShape(), _location, _rotation, _scale, color);
			}

			void RendererSystem::renderCollisionShape(btCollisionShape *shape, const Location &location,
				const Rotation &rotation, const Scale &scale, const glm::vec4 color)
			{
				if (shape == nullptr){
					DEBUG_PRINT("Render collision shape is a null pointer!");
					return;
				}

				switch (shape->getShapeType()){
					case BOX_SHAPE_PROXYTYPE:
					{
						btBoxShape *_shape = static_cast<btBoxShape*>(shape);
						glm::vec3 _extent = Physics::convertVec3(_shape->getHalfExtentsWithoutMargin());
						Scale _scale = scale * _extent * 2.0f;

						renderPrimitive("cube", location, rotation, _scale, color);

						break;
					}
					/*case STATIC_PLANE_PROXYTYPE:
					{
						renderPrimitive("cube", _location, _rotation, _scale, color);
						break;
						}*/
						/*case SPHERE_SHAPE_PROXYTYPE:
						{
						float _radius = static_cast<btSphereShape*>
						(body->getCollisionShape())->getRadius();

						renderPrimitive("cube", _location, _rotation, _scale, color);
						break;
					}*/
					case CAPSULE_SHAPE_PROXYTYPE:
					{
						btCapsuleShape *_capsule = static_cast<btCapsuleShape*>(shape);
						const float _capsuleRadius = _capsule->getRadius();
						const float _capsuleHeightScale = (_capsule->getHalfHeight()) / _capsuleRadius;
						Scale _scale = scale * Scale(1.0f, _capsuleHeightScale, 1.0f);

						renderPrimitive("cube", location, rotation, _scale, color);

						break;
					}
					case TRIANGLE_MESH_SHAPE_PROXYTYPE:
					{
						//DEBUG_PRINT("TODO: IMPLEMENT TRIANGLE MESH RENDERING!");
						break;
					}
					default: break;
				}
			}

		}
	}
}
