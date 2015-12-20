#include "../include/Rigidbody.h"
#include "../include/WorldObject.h"
#include "../include/World.h"

#include <iostream>

namespace Engine
{
	
	void Rigidbody::update(float dt)
	{
		if (Object.expired())
		{
			return;
		}

		auto obj = Object.lock();
		auto props = obj->getWorld()->getPhysicProperties();
		_velocity = _velocity + props.Gravity * _mass * dt;
		

		if (_friction)
		{
			_velocity.x -= _velocity.x * dt;
		}
		// converting physical velocity to meters
		_lastPosAdded = _velocity / props.PixelToMeter * dt;
		obj->Position = obj->Position + _lastPosAdded;
	}

	void Rigidbody::addVelocity(Point2d velocity)
	{
		_velocity = _velocity + velocity;
	}


	void Rigidbody::onCollisionStart(Point2d collision)
	{
		_friction = true;
		_velocity.y = -_velocity.y/2;
	}


	void Rigidbody::onCollisionEnd()
	{
		_friction = false;
	}

}