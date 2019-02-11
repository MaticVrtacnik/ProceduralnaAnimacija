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