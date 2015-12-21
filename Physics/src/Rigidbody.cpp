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

		if (!_inCollision)
		{
			// use gravity when object isn't colliding
			_velocity = _velocity + props.Gravity * _mass * dt;
		}

		if (_inCollision)
		{
			// friction 
			_velocity.x -= _velocity.x * dt;
		}

		// converting meters to pixels
		obj->Position = obj->Position + _velocity / props.PixelToMeter * dt;;
	}

	void Rigidbody::addVelocity(Point2d metersPerSecond)
	{
		_velocity = _velocity + metersPerSecond;
	}


	void Rigidbody::onCollisionStart(Point2d collision)
	{
		_inCollision = true;
		if (abs(_velocity.y) > 0.02f)
			_velocity.y = -_velocity.y / 2;
		else
			_velocity.y = 0.f;
	}


	void Rigidbody::onCollisionEnd()
	{
		_inCollision = false;
	}

}