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


#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SDL2\SDL.h>
#include "World/World.hpp"
#include "System/SystemIncluder.hpp"


using namespace Engine;
using namespace Entities;
using namespace Systems;


class Application{
private:
	World m_world;
	InitSystem m_initSystem;
	GUISystem m_guiSystem;
	SkeletalAnimationRendererSystem m_skeletalSytem;
	RendererSystem m_rendererSystem;
	EntityEditorSystem m_editorSystem;
	PlayerControllerSystem m_playerControllerSystem;
	CollisionSystem m_collisionSystem;
	EnvironmentSystem m_environmentSystem;
	FinalizeSystem m_finalizeSystem;

private:
	Uint32 m_updateFpsTimer = 0;
	bool m_running = true;
	float m_fps = DEFAULT_FPS;

private:
	bool loadResources();

	bool loadGUIFunctions();

	bool loadSystems();

	bool loadEntities();

private:
	bool loadAlwaysUsedEntities();

	bool loadVariablyUsedEntities();

private:
	void updateWindowResolution();

	void handleInput();

	void updateFpsText(float cpuTime, float gpuTime);

	void addDecalAtClickLocation(const glm::vec3 &color, float scale);

public:
	Application();

	~Application();


	int run(SDL_Window *window);

};

#endif //ENGINE_HPP