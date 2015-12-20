#include "../include/Rigidbody.h"
#include "../include/WorldObject.h"
#include "../include/World.h"

#include <iostream>

namespace Engine
{
	Rigidbody::Rigidbody(ObjPtr obj, float mass )
		: UpdatableComponent(obj), _mass( mass )
	{
	}

	void Rigidbody::update(float dt)
	{
		if (Object.expired())
		{
			return;
		}

		auto obj = Object.lock();
		auto props = obj->getWorld()->getPhysicProperties();
		_velocity = _velocity + props.Gravity * _mass * dt;
		
		// converting physical velocity to meters
		obj->Position = obj->Position + _velocity / props.PixelToMeter * dt;
	}

}