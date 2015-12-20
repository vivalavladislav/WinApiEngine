#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "PhysicsDll.h"

//#include "ObjectComponent.h"
#include "Colliders.h"
#include "Point2d.h"
#include "Colliders.h"
#include <memory>

namespace Engine
{

	struct PHYS_API Rigidbody : public UpdatableComponent, public ColliderListener
	{
		template< class TColliderShape >
		Rigidbody(ObjPtr object, float mass, std::unique_ptr<Collider<TColliderShape>> collider );

		virtual void update(float deltaTime);
		
		virtual void onCollisionStart(Point2d collisionVector);
		virtual void onCollisionEnd();

		void addVelocity( Point2d velocity );

	private:
		Point2d _velocity;
		float _mass;
		Point2d _lastPosAdded;
		bool _friction = false ;
	};

	template< class TColliderShape >
	Rigidbody::Rigidbody(ObjPtr object, float mass, std::unique_ptr<Collider<TColliderShape>> collider)
		: UpdatableComponent(object), _mass(mass)
	{
		collider->setListener(this);

		object.lock()->addComponent(std::move(collider));
	}

}

#endif 