#pragma once
#include "World.h"
#include "Renderable.h"
#include "Resource.h"

struct Simulation
{
	Simulation(Engine::Point2d windowSize);

	void run(HWND window, std::shared_ptr<bool> run);

	Engine::World& getWorld();
	std::vector< Render::RenderablePtr > getRender();

private:
	Engine::World _world;
	std::vector< Render::RenderablePtr > _renderables;
};