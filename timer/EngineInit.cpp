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


#include "Engine.hpp"

#include "Component/ComponentIncluder.hpp"
#include "Entity\EntityCreator.hpp"
#include "Camera/Camera.hpp"
#include "Input/Input.hpp"
#include "Utility\RandomUtilities.hpp"
#include "Utility\MathUtilities.hpp"
#include "Renderer\DeferredRenderer.hpp"


using namespace Physics;


bool Application::loadAlwaysUsedEntities(){
	auto &_variables = m_world.getVariables();
	auto &_resourceManager = m_world.getResourceManager();
	auto &_collision = _variables.m_collision;

	// SUN
	for (auto &_entity : createEntitiesFromMeshFile(m_world, "sphere.3ds", COLLISION_CUBE,
		Location(-170.0f, 250.0f, 350.0f),
		Rotation(0.0f), Scale(1.0f)/*Scale(6.0f)*/))
	{
		auto &_matrixComponent = _entity.getComponent<MatrixComponent>();
		_variables.m_renderer->setSunLocation(_matrixComponent.getLocation());

		float _scale = _matrixComponent.getDistanceFromOrigin() / 45.0f; //ConstantOffset
		_matrixComponent.setScale(_matrixComponent.getScale() * _scale);

		_entity.addComponent<LightComponent>(LIGHT_DIRECTIONAL).m_intensity = 2.0f;
	}//*/


	//TODO remove scale 0
	bool c = false;
	Location _playerLocation = _variables.m_camera->getLocation();
	for (auto &_playerEntity : createEntitiesFromMeshFile(m_world, "protagonist.fbx",
		COLLISION_CUBE, _playerLocation - Location(0.0f, 1.0f, 0.0f), Rotation(0.0f, 0.0f, 0.0f), Scale(0.05f), "player"))
	{
		auto &_playerComponent = _playerEntity.addComponent<PlayerControllerComponent>(*_collision);
		_playerEntity.activate();

		//Prevent multiple entities
		break;
	}

	return true;
}

bool Application::loadVariablyUsedEntities(){
	auto &_variables = m_world.getVariables();
	auto &_resourceManager = m_world.getResourceManager();
	auto &_collision = _variables.m_collision;


	int _tmpC = 0;
	for (auto &_entity : createEntitiesFromMeshFile(m_world, "testRoomLedge.obj",
		COLLISION_COMPLEX, Location(0.0f), Rotation(0.0f), Scale(0.5f)))
	{

	}//*/

	for (auto &_entity : createEntitiesFromMeshFile(m_world, "polls.obj",
		COLLISION_COMPLEX, Location(30.0f, 3.0f, 10.0f), Rotation(0.0f, 180.0f, 0.0f), Scale(1.2f)))
	{
	}//*/

	for (auto &_entity : createEntitiesFromMeshFile(m_world, "obstacle.obj",
		COLLISION_COMPLEX, Location(8.0f, 0.0f, 0.0f), Rotation(0.0f, 180.0f, 0.0f), Scale(0.4f, 0.45f, 0.75f)))
	{
	}//*/

	return true;
}
