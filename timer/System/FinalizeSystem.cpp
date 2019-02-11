#include "FinalizeSystem.hpp"

#include <glm\gtc\type_ptr.hpp>

#include "../World/World.hpp"
#include "../Camera/Camera.hpp"
#include "../Cubemap/CubemapTexture.hpp"
#include "../Renderer/DeferredRenderer.hpp"
#include "../Component/LightComponent.hpp"
#include "../Component/MatrixComponent.hpp"
#include "../PostProcessing/SAO.hpp"
#include "../PostProcessing/HDR.hpp"
#include "../PostProcessing/TextureSizeChanger.hpp"
#include "../PostProcessing/SSLightScattering.hpp"
#include "../PostProcessing/MotionBlur.hpp"
#include "../PostProcessing/GaussianBlur.hpp"
#include "../PostProcessing/CombineEffects.hpp"
#include "../PostProcessing/CombineTextures.hpp"
#include "../GUI/GUI.hpp"
#include "../GUI/GUITab.hpp"
#include "../GUI/GUISlider.hpp"
#include "../GUI/GUIToggle.hpp"
#include "../GUI/GUIWindowStack.hpp"

//TODO REMOVE
#include "RendererSystem.hpp"
#include "EnvironmentSystem.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{
			using namespace Graphics::PostProcessing;


			FinalizeSystem::FinalizeSystem(){
				static std::vector<GLfloat> _vertexData({
					-1.0f, -1.0f, 0.0f,
					1.0f, -1.0f, 0.0f,
					-1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f
				});

				glGenVertexArrays(1, &m_screenQuadVertexArray);
				glBindVertexArray(m_screenQuadVertexArray);

				m_screenQuadBuffer.loadBuffer(_vertexData, GL_ARRAY_BUFFER);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				glBindVertexArray(0);


				glGenQueries(1, &m_timerStartQuery);
				glGenQueries(1, &m_timerEndQuery);
			}

			FinalizeSystem::~FinalizeSystem(){
				glDeleteQueries(1, &m_timerStartQuery);
				glDeleteQueries(1, &m_timerEndQuery);
			}


			void FinalizeSystem::init(){
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				auto &_renderer = _variables.m_renderer;

				unsigned int _shadowRes = _renderer->c_shadowResolution;
				auto _width = _variables.m_screenWidth;
				auto _height = _variables.m_screenHeight;

				
				auto _hdrShader = _resourceManager.getShaderPtr("postprocessing/HDR");
				m_HDR = std::make_unique<HDR>(_hdrShader, _width, _height);

				auto &_gaussianBlurShader = _resourceManager.getShaderPtr("postprocessing/gaussianBlur");
				m_gaussianBlur = std::make_unique<GaussianBlur>(_gaussianBlurShader, _width, _height);
				m_shadowBlur = std::make_unique<GaussianBlur>(_gaussianBlurShader, 
					_shadowRes, _shadowRes, GL_RG32F, GL_RG);
				
				auto _basicShader = _resourceManager.getShaderPtr("postprocessing/basic");
				m_textureSizeChanger = std::make_unique<TextureSizeChanger>(_basicShader, _width, _height);

				auto _scatterShader = _resourceManager.getShaderPtr("postprocessing/lightScattering");
				m_lightScattering = std::make_unique<SSLightScattering>(
					_scatterShader, _gaussianBlurShader, _width, _height);
				m_sunScattering = std::make_unique<SSLightScattering>(
					_scatterShader, _gaussianBlurShader, _width, _height);
				

				//TODO Improve Basic Motion Blur (TODO add skybody?)
				auto _motionBlurShader = _resourceManager.getShaderPtr("postprocessing/motionBlur");
				m_motionBlur = std::make_unique<MotionBlur>(_motionBlurShader, _width, _height);


				//Scallable ambient obscurance 
				auto _linearDepthShader = _resourceManager.getShaderPtr("postprocessing/linearizeDepth");
				auto _saoShader = _resourceManager.getShaderPtr("postprocessing/SAO");
				auto _saoBlurShader = _resourceManager.getShaderPtr("postprocessing/SAOBlur");
				m_SAO = std::make_unique<SAO>(_saoShader, _saoBlurShader, 
					_linearDepthShader, _basicShader, _width, _height);


				//Combine Screenspace Postprocessing Effects
				auto _combineEffShader = _resourceManager.getShaderPtr("postprocessing/combine");
				m_combineEffects = std::make_unique<CombineEffects>(_combineEffShader, _width, _height);


				//Combine Two Textures Into One
				auto _combineShader = _resourceManager.getShaderPtr("postprocessing/combineTextures");
				m_combineTextures = std::make_unique<CombineTextures>(_combineShader, _width, _height);
				m_combineReflections = std::make_unique<CombineTextures>(_combineShader, _width, _height);


				//m_screenTexture = m_FXAA->getResult(0);
				m_screenTexture = m_motionBlur->getResult(0);

				m_bloomTextures.resize(4);
			}

			void FinalizeSystem::setLightUniforms(){
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				auto &_camera = _variables.m_camera;

				auto &_shader = _resourceManager.getShader("postprocessing/deferred");
				_shader.bind();
				{
					glm::mat4 _viewMatrix = _camera->getViewMatrix();
					glm::mat3 _viewRotationMatrix = _camera->getRotationMatrix();

					int _index = 0;
					for (auto &_entity : m_entities){
						if (_index >= 10)break;

						if (_entity.hasComponent<Entities::Components::LightComponent>()){
							auto &_light = _entity.getComponent<Entities::Components::LightComponent>();
							auto &_matrix = _entity.getComponent<Entities::Components::MatrixComponent>();

							_light.setDirection(glm::quat(_viewMatrix * glm::toMat4(_matrix.getRotation())));
							
							//glm::vec3 _lightLocation(_viewMatrix * glm::vec4(-170.0f, 570.0f, 550.0f, 1.0));
							glm::vec3 _lightLocation(_viewMatrix * glm::vec4(_matrix.getLocation(), 1.0));
							//glm::vec3 _lightLocation(_viewRotationMatrix * _matrix.getLocation());
							//Utilities::print(_lightLocation);
							//Utilities::print(_viewMatrix);
							_light.setUniforms(_index, _lightLocation, _shader);

							++_index;
						}
					}

					//Set number of lights at the end of the last iteration
					_shader.setUniform("u_numLights", _index);
				}
				_shader.unbind();
			}

			Entity FinalizeSystem::getSunEntity(){
				for (auto _entity : m_entities){
					if (_entity.hasComponent<Entities::Components::LightComponent>()){
						auto &_lightComponent = _entity.getComponent<Entities::Components::LightComponent>();
						if (_lightComponent.getType() == LIGHT_DIRECTIONAL){
							return _entity;
						}
					}
				}

				return Entity();
			}

			//TODO optimize -> REALLY EXPENSIVE FUNCTION
			void FinalizeSystem::renderDeferred(){
				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				auto &_renderer = _variables.m_renderer;
				auto &_camera = _variables.m_camera;
				glm::mat4 _projectionMatrix = _camera->getProjectionMatrix();
				glm::mat4 _viewMatrix = _camera->getViewMatrix();
				glm::mat4 _viewProjectionMatrix(_projectionMatrix * _viewMatrix);


				glBindVertexArray(m_screenQuadVertexArray);

				glDisable(GL_BLEND);		
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				/*unsigned int _ssaoTexture = m_SSAO->useEffect(_projectionMatrix,
					 _renderer->getLocationTexture(), _renderer->getNormalTexture()
				);//*/

				unsigned int _SAOTexture = m_SAO->useEffect(_projectionMatrix,
					_renderer->getDepthTexture());//*/

				_variables.m_gui->getTabWindow("quad").getChild<GUI::GUIButton>("post1").
					setTexture(_SAOTexture);


				//TODO replace with combineEffects
				/*m_lightScattering->useEffect(_viewProjectionMatrix, m_sunLocation,
					{ _renderer->getOccludedFramebuffer().getTexture(0) }
				);*/

				auto &_gui = _variables.m_gui->getIndex();
				auto &_terrainGUI = _gui.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
					getWindow("environment").getChild<GUI::GUIWindowStack>("stack").getWindow("render");
				auto &_timeSlider = _terrainGUI.getChild<GUI::GUISlider>("time");


				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);

				auto &_finalFramebuffer = _renderer->getFinalFramebuffer();
				_finalFramebuffer.bind();
				{
					glQueryCounter(m_timerStartQuery, GL_TIMESTAMP);

					auto &_deferredShader = _resourceManager.getShader("postprocessing/deferred");
					_deferredShader.bind();
					{
						auto &_skybox = getWorld().getSystem<EnvironmentSystem>().getSkybox();
						glm::mat4 _inverseViewMatrix(glm::inverse(_viewMatrix));
						glm::mat4 _lightMatrix = _renderer->getSunViewProjectionMatrix();

						_deferredShader.setUniform("u_inverseViewMatrix", _inverseViewMatrix);
						_deferredShader.setUniform("u_inverseNormalMatrix", glm::mat3(_inverseViewMatrix));
						_deferredShader.setUniform("u_shadowLightMatrix", _lightMatrix * _inverseViewMatrix);
						
						Textures::bindCubemapTexture(_skybox.getSelectedCubemap(), 0);
						//Textures::bindCubemapTexture(_skybox.getCubemap(2), 0);

						Textures::bind2DTexture(_renderer->getShadowTexture(), 1);

						Textures::bind2DTexture(_renderer->getLocationTexture(), 2);
						Textures::bind2DTexture(_renderer->getNormalTexture(), 3);
						Textures::bind2DTexture(_renderer->getAlbedoTexture(), 4);
						Textures::bind2DTexture(_renderer->getRoughMetalOccluEmissTexture(), 5);
						Textures::bind2DTexture(_SAOTexture, 6);

						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					}
					_deferredShader.unbind();


					glQueryCounter(m_timerEndQuery, GL_TIMESTAMP);
					GLuint64 _gpuStartTime, _gpuEndTime;
					glGetQueryObjectui64v(m_timerStartQuery, GL_QUERY_RESULT, &_gpuStartTime);
					glGetQueryObjectui64v(m_timerEndQuery, GL_QUERY_RESULT, &_gpuEndTime);
					m_lightingTimer = (_gpuEndTime - _gpuStartTime) / 1000000.0;


					glBindVertexArray(0);
					glEnable(GL_DEPTH_TEST);

				}
				_finalFramebuffer.unbind();
			}

			const std::vector<unsigned int> &FinalizeSystem::getBloomTextures() const{
				return m_bloomTextures;
			}

			const unsigned int &FinalizeSystem::getScreenTexture() const{
				return m_screenTexture;
			}


			void FinalizeSystem::preUpdate(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_renderer = _variables.m_renderer;

				glBindVertexArray(m_screenQuadVertexArray);

				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				auto &_shadowFramebuffer = _renderer->getShadowFramebuffer();

				unsigned int _shadowTexture = _shadowFramebuffer.getTexture(0);
				//unsigned int _blurredShadow1 = m_shadowBlur->useEffect(_shadowTexture, 4, 2);
				unsigned int _blurredShadow2 = m_shadowBlur->useEffect(_shadowTexture, 1);

				_shadowFramebuffer.bind();
				_shadowFramebuffer.setTexture(_blurredShadow2);
				//TODO turn into a method
				m_shadowBlur->m_blurPingTextures[0] = _shadowTexture;
				_shadowFramebuffer.unbind();
		

				/*auto &_tab = _variables.m_gui->getIndex().getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab");
				_tab.getWindow("quad").getChild<GUI::GUIButton>("post1").setTexture(_blurredShadow2);*/
		

				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);

				glBindVertexArray(0);
			}


			void FinalizeSystem::update(float fps){
				auto &_resourceManager = getWorld().getResourceManager();
				auto &_variables = getWorld().getVariables();
				auto &_renderer = _variables.m_renderer;
				auto &_camera = _variables.m_camera;

				glm::mat4 _projectionMatrix = _camera->getProjectionMatrix();
				glm::mat4 _viewMatrix = _camera->getViewMatrix();
				glm::mat4 _viewProjectionMatrix(_projectionMatrix * _viewMatrix);

				setLightUniforms();
				renderDeferred();

				glDisable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				glBindVertexArray(m_screenQuadVertexArray);
				{
					/*auto &_skybox = getWorld().getSystem<EnvironmentSystem>().getSkybox();
					m_reflections->useEffect(_projectionMatrix, _viewMatrix,
						_renderer->getLocationTexture(), 
						_renderer->getNormalTexture(),
						_renderer->getFinalFramebuffer().getTexture(0), 
						_renderer->getRoughMetalOccluEmissTexture(),
						_skybox.getSelectedCubemap()
					);*/

					/*_variables.m_gui->getTabWindow("quad").getChild<GUI::GUIButton>("post1").
						//setTexture(_renderer->getOccludedFramebuffer().getTexture(0));
						setTexture(m_reflections->getResult());*/
				}
				glBindVertexArray(0);


				//Setup legacy mode (after all of the system have done updating)
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glMultMatrixf(&_variables.m_camera->getViewMatrix()[0][0]);
			}

			void FinalizeSystem::postUpdate(float fps){
				auto &_variables = getWorld().getVariables();
				auto &_renderer = _variables.m_renderer;
				auto &_camera = _variables.m_camera;
				auto &_rendererGUI = _variables.m_gui->getTabStackWindow("environment", "render");

				//glEnable(GL_BLEND);
				glDisable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				glBindVertexArray(m_screenQuadVertexArray);

				//std::cout << m_sunLocation.y << std::endl;

				glm::mat4 _viewMatrix = _camera->getViewMatrix();
				glm::mat4 _projectionMatrix = _camera->getProjectionMatrix();
				glm::mat4 _viewProjectionMatrix(_projectionMatrix * _viewMatrix);

				unsigned int _occludedLightTexture = _renderer->getOccludedFramebuffer().getTexture(0);
				unsigned int _litTexture = m_combineReflections->useEffect(
					_renderer->getFinalFramebuffer().getTexture(0), /*m_reflections->getResult()*/ 0);
				unsigned int _bloomTexture = _renderer->getFinalFramebuffer().getTexture(1);

				//TODO
				/*	
					morm dodt se bloom na reflection reflectiona
					xd
					an a vids specular zgori
					k se svet
					na vodi je pa sam color
					um mau dol
					xd

					Pa increasam fragcolor za frag + frag * emissive
					Xd na fonu si pa si pism what todo da nam fogetu
				*/


				/*auto &_rendererSystem = getWorld().getSystem<RendererSystem>();
				m_indirectIllumination->useEffect(_camera->getLocation(), _viewMatrix,
				{
				_renderer->getLocationTexture(),
				_renderer->getNormalTexture(),
				//_rendererSystem.m_irradianceTextures[0],
				//_rendererSystem.m_varianceTextures[0],
				_rendererSystem.m_irradianteCubemapArray,
				//_rendererSystem.m_radianceCubemapArray,
				_rendererSystem.m_varianceCubemapArray
				});*/

				//TODO for checking global illumination

				

				/*m_lightScattering->useEffect(_viewProjectionMatrix, m_sunLocation,
					0.975f, 1.0f, 0.6f,
					{ _bloomTexture}
					);*/

				auto _sunEntity = this->getSunEntity();
				if (_sunEntity.isValid()){
					auto &_sunMatrixComponent = _sunEntity.getComponent<MatrixComponent>();
					_renderer->setSunLocation(_sunMatrixComponent.getLocation());

					float _d = glm::distance(_camera->getLocation(), _sunMatrixComponent.getLocation());
					_sunMatrixComponent.setScale(_d / 7.5f); //TODO remove constant

					m_sunScattering->useEffect(_viewMatrix, _projectionMatrix,
						_sunMatrixComponent.getLocation(), _sunMatrixComponent.getScale(),
						0.955f, 1.0f, 20.0f, _occludedLightTexture
					);
				}

				//m_lensFlare->useEffect({ _occludedLightTexture });

				m_combineEffects->useEffect(_viewMatrix,
					_litTexture,
					/*m_lensFlare->getResult()*/ 0,
					m_lightScattering->getResult(),
					m_sunScattering->getResult()
				);

				auto _combinedBloom = m_combineTextures->useEffect(
					m_sunScattering->getResult(), _bloomTexture);
				/*_variables.m_gui->getTabWindow("quad").getChild<GUI::GUIButton>("post1").
				setTexture(_combinedBloom);*/

				{
					glQueryCounter(m_timerStartQuery, GL_TIMESTAMP);

					m_bloomTextures[0] = m_gaussianBlur->useEffect(_combinedBloom, 2, 1);
					m_bloomTextures[1] = m_gaussianBlur->useEffect(m_bloomTextures[0], 2, 2);
					m_bloomTextures[2] = m_gaussianBlur->useEffect(m_bloomTextures[1], 2, 3);
					m_bloomTextures[3] = m_gaussianBlur->useEffect(m_bloomTextures[2], 2, 4);

					glQueryCounter(m_timerEndQuery, GL_TIMESTAMP);
					GLuint64 _gpuStartTime, _gpuEndTime;
					glGetQueryObjectui64v(m_timerStartQuery, GL_QUERY_RESULT, &_gpuStartTime);
					glGetQueryObjectui64v(m_timerEndQuery, GL_QUERY_RESULT, &_gpuEndTime);
					m_bloomTimer = (_gpuEndTime - _gpuStartTime) / 1000000.0;
				}

				


				float _exposureFactor = _rendererGUI.getChild<GUI::GUISlider>("exposure").getFloatValue();
				m_screenTexture = m_HDR->useEffect(fps, _exposureFactor, m_combineEffects->getResult(),
					m_bloomTextures[0], m_bloomTextures[1], m_bloomTextures[2], m_bloomTextures[3]		
				);


				//m_screenTexture = m_grayscale->useEffect({ m_screenTexture });

				/*if (_rendererGUI.getChild<GUI::GUIToggle>("depthOfField").isOn()){
					float _focalDistance = _rendererGUI.getChild<GUI::GUISlider>("focalDistance").getFloatValue();
					float _focalRange = _rendererGUI.getChild<GUI::GUISlider>("focalRange").getFloatValue();

					m_screenTexture = m_depthOfField->useEffect(_focalDistance, _focalRange,
						 m_screenTexture, _renderer->getLocationTexture());
				}*/
				

				//TODO move before lens flare
				m_screenTexture = m_motionBlur->useEffect(fps,
					m_screenTexture, _renderer->getVelocityTexture());

				//m_screenTexture = m_FXAA->useEffect(m_screenTexture, m_SAO->getResult());


				//UPDATE TIMERS
				{
					auto &_gui = _variables.m_gui->getIndex();

					std::string _timerString = std::to_string(m_sunScattering->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("light_scattering").setString(
						"LIGHT SCATTERING: " + _timerString + "ms", GUI::ALIGN_LEFT);

					/*_timerString = std::to_string(m_lensFlare->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("lens_flare").setString(
						"LENS FLARE: " + _timerString + "ms", GUI::ALIGN_LEFT);*/

					_timerString = std::to_string(m_motionBlur->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("motion_blur").setString(
						"MOTION BLUR: " + _timerString + "ms", GUI::ALIGN_LEFT);

					/*_timerString = std::to_string(m_reflections->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("reflections").setString(
						"REFLECTIONS: " + _timerString + "ms", GUI::ALIGN_LEFT);*/

					_timerString = std::to_string(m_SAO->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("ambient_occlusion").setString(
						"AMBIENT OCCLUSION: " + _timerString + "ms", GUI::ALIGN_LEFT);

					_timerString = std::to_string(m_HDR->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("hdr").setString(
						"HDR: " + _timerString + "ms", GUI::ALIGN_LEFT);

					_timerString = std::to_string(m_lightingTimer).substr(0, 6);
					_gui.getChild<GUI::GUIText>("lighting").setString(
						"LIGHTING: " + _timerString + "ms", GUI::ALIGN_LEFT);

					_timerString = std::to_string(m_bloomTimer).substr(0, 6);
					_gui.getChild<GUI::GUIText>("bloom").setString(
						"BLOOM: " + _timerString + "ms", GUI::ALIGN_LEFT);

					/*_timerString = std::to_string(m_FXAA->getEffectDuration()).substr(0, 6);
					_gui.getChild<GUI::GUIText>("fxaa").setString(
						"FXAA: " + _timerString + "ms", GUI::ALIGN_LEFT);*/

				}


				glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);

				glUseProgram(0);
				glBindVertexArray(0);
				
				_renderer->getFinalFramebuffer().unbind();

				glEnable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
			}

		}
	}
}
