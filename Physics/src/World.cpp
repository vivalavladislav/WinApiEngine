#include "../include/World.h"

namespace Engine
{
	World::World(PhysicProperties properties)
		: _props(properties), _collision( this )
	{
	}

	PhysicProperties World::getPhysicProperties()
	{
		return _props;
	}

	TimeUpdater& World::getUpdater()
	{
		return _updater;
	}

	CollisionController& World::getCollisionController()
	{
		return _collision;
	}

	void World::onChildAdded(WorldObjectPtr child)
	{
		child->onAddedToWord( this );
	}

	void World::onChildRemoved(WorldObjectPtr child)
	{
		child->onRemovedFromWorld( this );
	}
}