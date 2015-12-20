#include "../include/WorldObject.h"
#include "../include/World.h"
#include "../include/ObjectComponent.h"


namespace Engine
{
	WorldObject::WorldObject()
	{
	}

	World* WorldObject::getWorld()
	{
		return _world;
	}

	void WorldObject::onChildAdded(WorldObjectPtr child)
	{
		child->onAddedToWord( _world );
	}

	void WorldObject::onChildRemoved(WorldObjectPtr child)
	{
		child->onRemovedFromWorld( _world );
	}

	void WorldObject::onAddedToWord(World* world)
	{
		if (_world != nullptr)
		{
			return;
		}

		_world = world;
		
		for (auto& component : _components)
		{
			component->onAddedToWorld( _world );
		}

		for (auto child : getChildren())
		{
			child->onAddedToWord(world);
		}
	}

	void WorldObject::onRemovedFromWorld(World* world)
	{
		if (world != _world)
		{
			return;
		}

		_world = nullptr;
		for (auto& component : _components)
		{
			component->onRemovedFromWorld( _world );
		}

		for (auto child : getChildren())
		{
			child->onRemovedFromWorld( world );
		}
	}

	void WorldObject::addComponent(ComponentPtr component)
	{
		if (_world != nullptr)
		{
			component->onAddedToWorld(_world);
		}

		_components.push_back( move(component) );
	}

	void WorldObject::removeComponent(IComponent* component)
	{
		auto iter = find_if(_components.begin(), _components.end(), [component](const ComponentPtr& inVector)
		{
			return inVector.get() == component;
		});

		if (iter != _components.end())
		{
			if (_world != nullptr)
			{
				iter->get()->onRemovedFromWorld(_world);
			}
			_components.erase(iter);
		}
	}
}