#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "PhysicsDll.h"

#include "ObjectComponent.h"
#include "Point2d.h"
#include "Colliders.h"

namespace Engine
{

	struct PHYS_API Rigidbody : public UpdatableComponent
	{
		Rigidbody(ObjPtr object, float mass );

		void update(float deltaTime);

	private:
		Point2d _velocity;
		float _mass;
	};

}

#endif 