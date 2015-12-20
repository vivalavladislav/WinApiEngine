#include "../include/World.h"

namespace Engine
{
	World::World(PhysicProperties properties)
		: _props( properties )
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

	void World::onChildAdded(WorldObjectPtr child)
	{
		child->onAddedToWord( this );
	}

	void World::onChildRemoved(WorldObjectPtr child)
	{
		child->onRemovedFromWorld( this );
	}
}