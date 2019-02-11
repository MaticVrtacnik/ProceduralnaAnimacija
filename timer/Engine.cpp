#include "Engine.hpp"

#include "Component/ComponentIncluder.hpp"
#include "Entity\EntityCreator.hpp"
#include "Environment/Skybox.hpp"
#include "Utility/CollisionUtilities.hpp"
#include "Camera/Camera.hpp"
#include "Input/Input.hpp"
#include "Texture\Material.hpp"
#include "Utility\RandomUtilities.hpp"
#include "Utility\MathUtilities.hpp"
#include "GUI\GUI.hpp"
#include "GUI\GUIIncluder.hpp"
#include "Renderer\DeferredRenderer.hpp"
#include "Animation\AnimationSkeleton.hpp"
#include "Timer.hpp"

#include "Utility\AnimationUtilities.hpp"

using namespace Entities;
using namespace Components;       


Application::Application(){
	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);*/

	bool _valid = true;	
	_valid &= loadSystems();
	_valid &= loadResources();
	_valid &= loadEntities();
	_valid &= loadGUIFunctions();
	

	m_world.refresh();
	m_running = _valid;
	m_running = true;
	//TODO

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Application::~Application(){

}


bool Application::loadResources(){
	auto &_variables = m_world.getVariables();
	auto &_resourceManager = m_world.getResourceManager();

	return true;
}

bool Application::loadGUIFunctions(){
	//TODO EXIT
	/*_variables.m_gui.getIndex().getChild<GUI::GUIWindow>("menu").
	getChild<GUI::GUIButton>("Exit").setAction([&running]{
	PRINT("BUREEEEEEEEK");  running = false;
	});*/

	auto &_variables = m_world.getVariables();
	auto &_guiIndex = _variables.m_gui->getIndex();

	//_variables.m_renderer->getFinalFramebuffer().getTexture(0)
	auto &_finalizeSystem = m_world.getSystem<FinalizeSystem>();
	_guiIndex.getChild<GUI::GUIBase>("screen").setTexture(_finalizeSystem.getScreenTexture());

	return true;
}

bool Application::loadSystems(){
	m_world.addSystem(m_initSystem);
	m_world.addSystem(m_guiSystem);
	m_world.addSystem(m_environmentSystem);
	m_world.addSystem(m_collisionSystem);
	m_world.addSystem(m_playerControllerSystem);
	m_world.addSystem(m_editorSystem);	
	m_world.addSystem(m_rendererSystem);
	m_world.addSystem(m_skeletalSytem);
	m_world.addSystem(m_finalizeSystem);

	return true;
}

bool Application::loadEntities(){
	bool _loadedA = loadAlwaysUsedEntities();
	bool _loadedV = loadVariablyUsedEntities();

	m_world.refresh();

	return _loadedA && _loadedV;
}


int Application::run(SDL_Window *window){
	GLuint64 _gpuStartTime, _gpuEndTime;
	unsigned int _startQuery, _endQuery;
	glGenQueries(1, &_startQuery);
	glGenQueries(1, &_endQuery);

	while (m_running)
	{
		//SDL_GL_SetSwapInterval(0);

		//Start timers
		Uint64 _cpuStartTimer = SDL_GetPerformanceCounter();
		glQueryCounter(_startQuery, GL_TIMESTAMP);
		glFinish();

		
		//Update world and systems
		updateWindowResolution();		
		m_world.update(m_fps);
		m_world.refresh();
		handleInput();


		Uint64 _cpuEndTimer = SDL_GetPerformanceCounter();
		double _counterTime = static_cast<double>(_cpuEndTimer - _cpuStartTimer);
		double _cpuFrameTime = _counterTime / SDL_GetPerformanceFrequency();
		float _cpuTime = static_cast<float>(_cpuFrameTime * 1000.0);

		//End timestamp (automatically blocks until results are available)
		glQueryCounter(_endQuery, GL_TIMESTAMP);
		glGetQueryObjectui64v(_startQuery, GL_QUERY_RESULT, &_gpuStartTime);
		glGetQueryObjectui64v(_endQuery, GL_QUERY_RESULT, &_gpuEndTime);
		float _gpuTime = static_cast<float>((_gpuEndTime - _gpuStartTime) / 1000000.0);


		//glFinish(); //Will sync gpu and cpu to remove stuttering

		Timer m_timer; m_timer.startTimer();
		//PRINT("TIMER: " << m_timer.endTimer().getMicroseconds());

		SDL_GL_SwapWindow(window);
		glFinish(); //Will sync gpu and cpu to remove stuttering

		
		

		//End timestamp (automatically blocks until results are available)
		/*glQueryCounter(_endQuery, GL_TIMESTAMP);
		glGetQueryObjectui64v(_startQuery, GL_QUERY_RESULT, &_gpuStartTime);
		glGetQueryObjectui64v(_endQuery, GL_QUERY_RESULT, &_gpuEndTime);
		float _gpuTime = static_cast<float>((_gpuEndTime - _gpuStartTime) / 1000000.0);*/

		m_fps = 1000.0f / glm::max(_cpuTime, _gpuTime);
		//PRINT("FPS: " << m_fps << " | CPU: " << _cpuTime << " GPU: " << _gpuTime);

		updateFpsText(_cpuTime, _gpuTime);
	}

	glDeleteQueries(1, &_startQuery);
	glDeleteQueries(1, &_endQuery);

	return 0;
}

void Application::updateWindowResolution(){
	//TODO RESIZABLE WINDOW
	auto &_variables = m_world.getVariables();
	//SDL_GetWindowSize(window, &_variables.m_screenWidth, &_variables.m_screenHeight);
	glViewport(0, 0, _variables.m_screenWidth, _variables.m_screenHeight);
}

void Application::handleInput(){
	auto &_resourceManager = m_world.getResourceManager();
	auto &_variables = m_world.getVariables();
	auto &_collision = *_variables.m_collision;
	auto &_camera = *_variables.m_camera;
	auto &_input = *_variables.m_input;


	if (_input.m_applicationExit){
		if (_variables.m_editing); // m_running = false;
		else _variables.m_editing = true;
	}//*/


	if (/*!_variables.m_editing &&*/ _input.isButtonDown(KEYBOARD_BUTTON(UP))){
		Location _location = _camera.getLocation() + _variables.m_mouseVector * 1.0f;

		//auto _bodyEntities = m_editorSystem.createMeshEntities("body.dae", 
		//	MeshComponent::MESH_BUILDING, Physics::COLLISION_CUBE, _location);
		//auto &_npc = _bodyEntities[0].addComponent<NPCControllerComponent>(*_collision);
		//

		for (auto &_entity : createEntitiesFromMeshFile(m_world, "banana.obj",
			Physics::COLLISION_CUBE, _location, Rotation(0.0f), Scale(0.5f)))
		{
			auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
			_collisionComponent.setKinematic(false);
			_collisionComponent.setMass(1.0f);
			_collisionComponent.setCollisionFilters(Physics::GROUP_DYNAMIC, Physics::GROUP_STATIC);

			//Enable custom callback
			_collisionComponent.setCustomCallback(true);


			/*auto &_particleComponent = _entity.addComponent<ParticleComponent>();
			for (auto &_particleContainer : _particleComponent.getParticleContainers()){
				_particleContainer->setLife(1.2f);
				_particleContainer->setMaxParticleCount(160);
			}*/
		}//*/

		/*for (auto &_entity : createEntitiesFromMeshFile(m_world, "sphere.obj",
		COLLISION_SPHERE, _location, Rotation(0.0f), Scale(0.333f)))
		{
		auto &_mc = _entity.getComponent<MaterialComponent>();
		_mc.setMaterial(_resourceManager.getMaterialPtr("synthetic/rubber"));

		glm::vec3 _randomColor(Utilities::randomVec3(0.2f, 0.9f));
		auto &_lc = _entity.addComponent<LightComponent>(LIGHT_POINT);
		_lc.m_intensity = 15.0f;
		_lc.m_quadricAttenuation = 0.1f;
		_lc.m_linearAttenuation = 10.0f;
		_lc.m_color = glm::vec3(1.0, 0.3, 0.1);

		auto &_collision = *_variables.m_collision;
		auto &_cc = _entity.getComponent<CollisionComponent>();
		_cc.setKinematic(_collision, false);
		_cc.setMass(_collision, 5.0f);
		_cc.getBody()->setFriction(0.4f);
		_cc.getBody()->setRollingFriction(0.4f);
		_cc.getBody()->setRestitution(0.6f);

		auto _direction = Physics::convertVec3(_variables.m_camera->getVector());
		_cc.getBody()->setLinearVelocity(_direction * THROW_FORCE * 0.5f);

		auto &_particleComponent = _entity.addComponent<ParticleComponent>();
		for (auto &_particleContainer : _particleComponent.getParticleContainers()){
		_particleContainer->setLife(1.2f);
		_particleContainer->setMaxParticleCount(160);

		_particleContainer->removeFlags(Particles::PF_TRIGGER);
		if (_entity.getId() % 2 == 0){
		_particleContainer->removeFlags(Particles::PF_TRIGGER);
		}
		if (_entity.getId() % 3 == 0){
		//_particleContainer->m_additiveBlending = false;
		}
		}

		//Enviornmental probe
		//_entities[0].addComponent<ProbeComponent>(Graphics::generateCubemap(m_world, _location));
		//_cubeEntity.activate();
		}//*/
	}//*/


	if (_input.m_mouseLeftDoubleClick && !_variables.m_editing){
		glm::vec3 _randomColor = Utilities::randomVec3(0.1f, 0.9f);
		addDecalAtClickLocation(_randomColor, 10.0f);
	}//*/

	/*if (_input.m_mouseLeftHold && !_variables.m_editing){
		auto _raycast = _collision.rayTracingClosest(
			_camera.getLocation(), _variables.m_mouseVector, 100.0f);
		if (_raycast.hasHit()){
			glm::vec3 _location = Physics::convertVec3(_raycast.m_hitPointWorld);
			glm::vec4 _locationScale(_location, Utilities::randomFloat(0.8f, 1.0f));
			m_environmentSystem.addGrassLocation(_locationScale);
		}
	}//*/


	/*if (_input.isButtonDown(KEYBOARD_BUTTON(P))){
		for (auto &_knifeEntity : m_world.getEntities("brush")){
			if (_knifeEntity.isValid()){
				auto &_knifeCollisionComponent = _knifeEntity.getComponent<CollisionComponent>();
				_knifeCollisionComponent.setKinematic(_collision, !_knifeCollisionComponent.isKinematic());
				_knifeCollisionComponent.getBody()->activate(true);
			}
		}

		for (auto &_entity : m_world.getEntities("player")){
			auto &_collisionComponent = _entity.getComponent<CollisionComponent>();
			auto &_animationComponent = _entity.getComponent<SkeletalAnimationComponent>();

			//_collisionComponent.setKinematic(_collision, false);
			//_collisionComponent.setKinematic(true);

			_collisionComponent.setKinematic(_collision, !_collisionComponent.isKinematic());

			//auto &_bodies = _collisionComponent.getBodies();
			//auto &_boneMap = _animationComponent.getSkeleton().m_boneMap;
			//_collision.setKinematic(_bodies[_boneMap["right_foot"]].get(), false);
			//_collision.setKinematic(_bodies[_boneMap["left_foot"]].get(), false);


			_animationComponent.playAction("walk");
		}
	}//*/


	if (_input.isButtonDown(KEYBOARD_BUTTON(R))){
		_resourceManager.reloadShaders();
		_resourceManager.reloadMaterialConfigs();
	}//*/

}

void Application::updateFpsText(float cpuTime, float gpuTime){
	auto &_variables = m_world.getVariables();

	//TODO HORRIBLE PERFORMANCE
	if (SDL_GetTicks() - m_updateFpsTimer > 100){
		m_updateFpsTimer = SDL_GetTicks();

		auto _cLoc = _variables.m_camera->getLocation();
		std::string _cameraLocString(std::to_string(_cLoc.x) + ", " +
			std::to_string(_cLoc.y) + ", " + std::to_string(_cLoc.z));
		std::string _cameraString = "Camera Location : " + _cameraLocString;

		std::string _cpuString = "CPU: " + std::to_string(cpuTime).substr(0, 6) + "ms";
		std::string _gpuString = "GPU: " + std::to_string(gpuTime).substr(0, 6) + "ms";

		_variables.m_gui->getIndex().getChild<GUI::GUIText>("fps").setString(
			_cpuString + " " + _gpuString + "  || " + _cameraString, GUI::ALIGN_LEFT);
	}
}


void Application::addDecalAtClickLocation(const glm::vec3 &color, float scale){
	auto &_variables = m_world.getVariables();
	auto &_resouceManager = m_world.getResourceManager();

	auto _ray = _variables.m_collision->rayTracingAll(
		_variables.m_camera->getLocation(), _variables.m_mouseVector, 1000.0f);

	if (_ray.hasHit())
	{
		auto &_mesh = _resouceManager.getMesh("primitives", "cube");

		const btCollisionObject *_collisionObject = nullptr;
		Location _hitPointWorld(0.0f);
		glm::vec3 _hitNormal(0.0f, 1.0f, 0.0f);
		btScalar _shortestDistance = WORLD_SIZE;

		for (int i = 0; i < _ray.m_hitPointWorld.size(); ++i){
			if (_ray.m_hitFractions[i] < _shortestDistance){
				INT_TYPE _entityId = _ray.m_collisionObjects[i]->getUserIndex();
				auto &_entity = m_world.getEntity(_entityId);

				if (_entity.isValid() && _entityId < MAX_ENTITIES){
					//Make sure the entity is static because dynamic objects do no receive decals
					bool _staticObject = _entity.hasComponent<CollisionComponent>() &&
						_entity.getComponent<CollisionComponent>().isStaticObject();
				}
			}
		}

		if (_collisionObject != nullptr)
		{
			/*glm::vec3 _randomColor = Utilities::randomVec3(0.0f, 1.0f);
			auto _decalEntity = createDecalEntity(m_world, _randomColor, _hitPointWorld,
				Utilities::getQuatRotationFromVector(-_hitNormal), Scale(scale));*/

			/*auto _hitEntity = m_world.getEntity(_collisionObject->getUserIndex());
			auto &_hitEntityHierarchyComponent = _hitEntity.addComponent<HierarchyComponent>();
			_decalEntity.addComponent<HierarchyComponent>(_hitEntity);
			_hitEntityHierarchyComponent.addChild(_decalEntity);*/
		}

	}
}
