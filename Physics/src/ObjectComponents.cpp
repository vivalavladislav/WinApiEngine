#include "../include/ObjectComponent.h"
#include "../include/World.h"

using namespace std;


namespace Engine
{

	IComponent::IComponent( ObjPtr object )
		: Object( object )
	{
	}
	
	IComponent::~IComponent()
	{
	}

	UpdatableComponent::UpdatableComponent(ObjPtr obj)
	: IComponent( obj )
	{
	}

	UpdatableComponent::~UpdatableComponent()
	{
	}

	void UpdatableComponent::onAddedToWorld(World* world)
	{
		if (Object.expired() || Object.lock()->getWorld() != world)
		{
			return;
		}

		auto func = bind(&UpdatableComponent::update, this, placeholders::_1);
		_updateCallback = make_shared<UpdateF>(func);
		world->getUpdater().registerUpdate(_updateCallback);
	}

	void UpdatableComponent::onRemovedFromWorld(World* world)
	{
		if (Object.expired() || Object.lock()->getWorld() != world)
		{
			return;
		}

		world->getUpdater().unregisterUpdate(_updateCallback);
	}

	MoveUp::MoveUp(ObjPtr obj)
	: UpdatableComponent( obj )
	{
	}

	void MoveUp::update(float dt)
	{
		auto posDelta = dt * 50;
		Object.lock()->Position = Object.lock()->Position + Point2d(0, posDelta);
	}
}