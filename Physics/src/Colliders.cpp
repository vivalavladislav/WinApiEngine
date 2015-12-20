#include "../include/Colliders.h"
#include "../include/World.h"


using namespace std;

namespace Engine
{
	CollisionController::CollisionController(World* world)
		: _world( world )
	{
		auto func = bind(&CollisionController::update, this, placeholders::_1);
		_updateCallback = make_shared< UpdateF >( func );
		_world->getUpdater().registerUpdate( _updateCallback );
	}

	CollisionController::~CollisionController()
	{
		_world->getUpdater().unregisterUpdate(_updateCallback);
	}


	void CollisionController::update(float dt)
	{
		for (auto iter = _cirles.begin(); iter != _cirles.end(); ++iter)
		{
			for (auto sq = _squares.begin(); sq != _squares.end(); ++sq)
			{
				checkCollision((*iter)->getObject(), (*sq)->getObject(), (*iter)->getShape(), (*sq)->getShape());
			}
		}
	}

	bool CollisionController::checkCollision(WorldObjectPtr, WorldObjectPtr, Circle, Square)
	{
		return false;
	}

	bool CollisionController::checkCollision(WorldObjectPtr, WorldObjectPtr, Circle, Circle)
	{
		return false;
	}

	bool CollisionController::checkCollision(WorldObjectPtr, WorldObjectPtr, Square, Square)
	{
		return false;
	}
}