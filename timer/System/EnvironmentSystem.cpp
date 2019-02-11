#include "EnvironmentSystem.hpp"

#include "../World/World.hpp"
#include "../Camera/Camera.hpp"
#include "../Renderer/DeferredRenderer.hpp"
#include "../Input/Input.hpp"
#include "../Cubemap/CubemapTexture.hpp"

#include "../Texture/Material.hpp"
#include "../Component/MatrixComponent.hpp"
#include "../Component/LightComponent.hpp"
#include "../Component/CollisionComponent.hpp"
#include "../Component/MaterialComponent.hpp"
#include "../System/EntityEditorSystem.hpp"

#include "../Utility/MathUtilities.hpp"
#include "../Utility/CollisionUtilities.hpp"

#include "../GUI/GUI.hpp"
#include "../GUI/GUIIncluder.hpp"

#include "EnvironmentSystem.hpp"

#include "FinalizeSystem.hpp"

#define LOD_DISTANCE0 300.0f
#define DYNAMIC_SHADOWS


namespace Engine{
	namespace Entities{
		namespace Systems{


			EnvironmentSystem::EnvironmentSystem(){
				addRequired<MatrixComponent>();
				addRequired<MeshComponent>();
				addRequired<CollisionComponent>();
				addRequired<MaterialComponent>();
			}

			EnvironmentSystem::~EnvironmentSystem(){
			}

			void EnvironmentSystem::init(){
				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				auto &_guiIndex = _variables.m_gui->getIndex();
				auto &_tab = _guiIndex.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab");
				auto &_stack = _tab.getWindow("environment").getChild<GUI::GUIWindowStack>("stack");
				auto &_terrainGUI = _stack.getWindow("terrain");

				auto &_child = _tab.getWindow("quad").getChild<GUI::GUIButton>("post1");
				_child.setTexture(_variables.m_renderer->getShadowTexture());

				updateShadows();
			}

			void EnvironmentSystem::reload(){
				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				updateShadows();
			}

			void EnvironmentSystem::updateSunLocation(){
				for (auto &_entity : this->m_entities){
					if (_entity.hasComponent<LightComponent>()){
						if (_entity.getComponent<LightComponent>().getType() == LIGHT_DIRECTIONAL)
						{
							auto &_matrixComponent = _entity.getComponent<MatrixComponent>();

							auto &_variables = getWorld().getVariables();
							glm::vec3 _cameraLocation = _variables.m_camera->getLocation();
							glm::vec3 _sunLocation = _variables.m_renderer->getSunLocation();
							//_matrixComponent.setLocation(_cameraLocation + _sunLocation);

							break;
						}
					}
				}
			}


			void EnvironmentSystem::updateShadows(){
				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				auto &_camera = _variables.m_camera;
				auto &_shadowFramebuffer = _variables.m_renderer->getShadowFramebuffer();

				const unsigned int _resolution = Graphics::Renderer::c_shadowResolution;
				glViewport(0, 0, _resolution, _resolution);

				_shadowFramebuffer.bind();

				glDisable(GL_CULL_FACE);
				//glCullFace(GL_FRONT);

				auto &_renderer = _variables.m_renderer;
				glm::mat4 _sunViewProjectionMatrix = _renderer->getSunViewProjectionMatrix();
				glm::mat4 _sunViewMatrix = _renderer->getSunViewMatrix();

				glm::mat4 _projectionMatrix(_camera->getProjectionMatrix());
				glm::mat4 _viewMatrix(_camera->getViewMatrix());

				Textures::unbind2DTexture();
				glBindVertexArray(0);

				_shadowFramebuffer.unbind();
				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);

				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}

			void EnvironmentSystem::preUpdate(float fps){
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				auto & _renderer = _variables.m_renderer;

				//updateSunLocation();
				//updateWater();

#ifdef DYNAMIC_SHADOWS
				//updateShadows();

#endif //DYNAMIC_SHADOWS

				glDisable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
			}

			void EnvironmentSystem::update(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_input = _variables.m_input;
				auto &_gui = _variables.m_gui->getIndex();

				if (_input->m_buttonLastDown == SDLK_RIGHT){
					m_skybox.setNextCubemap();
				} else if (_input->m_buttonLastDown == SDLK_LEFT){
					m_skybox.setPreviousCubemap();
				}
			}

			void EnvironmentSystem::postUpdate(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_GFramebuffer = _variables.m_renderer->getGFramebuffer();

				auto &_finalFramebuffer = _variables.m_renderer->getFinalFramebuffer();
				_finalFramebuffer.bind();
				{
					glDepthMask(GL_TRUE);
					glEnable(GL_DEPTH_TEST);

					unsigned int _screenDepth = _finalFramebuffer.getDepthTexture();
					_finalFramebuffer.setDepthTexture(_GFramebuffer.getDepthTexture());

					//TODO not working correctly because were not drawing into Gbuffer locationDepthBuffer
					drawSkybox();

					_finalFramebuffer.setDepthTexture(_screenDepth);

					glDepthMask(GL_TRUE);
					glEnable(GL_DEPTH_TEST);
				}
				_finalFramebuffer.unbind();
			}
			

			void EnvironmentSystem::drawSkybox(){
				auto &_variables = getWorld().getVariables();
				float _screenWidth = static_cast<float>(_variables.m_screenWidth);
				float _screenHeight = static_cast<float>(_variables.m_screenHeight);

				auto &_resourceManager = getWorld().getResourceManager();
				auto &_skyboxShader = _resourceManager.getShader<Shaders::Shader>("skybox");
				_skyboxShader.bind();
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);

					auto _rotationMatrix = glm::mat4(_variables.m_camera->getRotationMatrix());
					glm::mat4 _projectionMatrix(glm::perspective(_variables.m_fieldOfView,
						_screenWidth / _screenHeight, 1.0f, 1000000.0f));
					_skyboxShader.setUniform("u_viewProjectionMatrix",
						_projectionMatrix * _rotationMatrix);

					glBindVertexArray(m_skybox.m_vertexArray);
					{
						//Textures::bindCubemapTexture(m_cubemaps[1], 0); //sky
						Textures::bindCubemapTexture(m_skybox.getSelectedCubemap(), 0);
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
					glBindVertexArray(0);

					//glEnable(GL_DEPTH_TEST);
					//glDepthMask(GL_TRUE);
					glCullFace(GL_BACK);
				}
				_skyboxShader.unbind();
			}

			Environment::Skybox &EnvironmentSystem::getSkybox(){
				return m_skybox;
			}

			void EnvironmentSystem::onEntityAdded(Entities::Entity &entity){

			}

			void EnvironmentSystem::onEntityRemoved(Entities::Entity &entity){

			}
		
		}
	}
}
