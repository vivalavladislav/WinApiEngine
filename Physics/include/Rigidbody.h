#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "PhysicsDll.h"

#include "Colliders.h"
#include "Point2d.h"
#include "Colliders.h"
#include <memory>

namespace Engine
{

	/*
		Component which emulates physic body. Mass and velocity properties are presented
		Encapsulates a collider in place of construction for collision haldling

		A gravity influence is handled here
	*/
	struct PHYS_API Rigidbody : public UpdatableComponent, public ColliderListener
	{
		template< class TColliderShape >
		Rigidbody(ObjPtr object, float mass, std::unique_ptr<Collider<TColliderShape>> collider );

		virtual void update(float deltaTime);
		
		virtual void onCollisionStart(Point2d collisionVector);
		virtual void onCollisionEnd();

		void addVelocity( Point2d metersPerSecond );

	private:
		Point2d _velocity;
		float _mass;
		bool _inCollision = false ;
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