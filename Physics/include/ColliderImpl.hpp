#ifndef _COLLIDER_IMPL_H_
#define _COLLIDER_IMPL_H_

namespace Engine
{
	template < class TShape >
	Collider<TShape>::Collider(ObjPtr object, TShape shape)
		: IComponent(object), _shape(move(shape))
	{
	}

	template < class TShape >
	TShape Collider<TShape>::getShape()
	{
		return _shape;
	}

	template < class TShape >
	WorldObjectPtr Collider<TShape>::getObject()
	{
		return Object.lock();
	}

	template < class TShape >
	void Collider<TShape>::onAddedToWorld(World * world)
	{
		if (Object.expired() || Object.lock()->getWorld() != world)
		{
			return;
		}
		world->getCollisionController().addCollider(this);
	}

	template < class TShape >
	void Collider<TShape>::onRemovedFromWorld(World * world)
	{
		if (Object.expired() || Object.lock()->getWorld() != world)
		{
			return;
		}
		world->getCollisionController().addCollider(this);
	}
}

#endif //_COLLIDER_IMPL_H_