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

	template < class TShape >
	void Collider<TShape>::setListener(ColliderListener* listener)
	{
		_listener = listener;
	}


	template< class TLShape, class TRShape >
	void CollisionController::trackCollisionState(Collider<TLShape>* lhs, Collider<TRShape>* rhs,
		std::set< std::pair<Collider<TLShape>*, Collider<TRShape>*> >& trackingSet,
		bool inCollision)
	{
		auto pair = std::make_pair(lhs, rhs);
		if (inCollision)
		{
			// this collision was already detected
			if (trackingSet.find(pair) != trackingSet.end())
			{
				if ( lhs->_listener != nullptr )
					lhs->_listener->onStayInCollision( Point2d::Zero );
				if ( rhs->_listener != nullptr )
					rhs->_listener->onStayInCollision( Point2d::Zero );
			}
			else // new collision
			{
				trackingSet.insert( pair );

				if (lhs->_listener != nullptr)
					lhs->_listener->onCollisionStart(Point2d::Zero);
				if (rhs->_listener != nullptr)
					rhs->_listener->onCollisionStart(Point2d::Zero);
			}
		}
		else 
		{
			auto iter = trackingSet.find(pair);
			
			// collision was detected but now it's over
			if (iter != trackingSet.end())
			{
				if (lhs->_listener != nullptr)
					lhs->_listener->onCollisionEnd();
				if (rhs->_listener != nullptr)
					rhs->_listener->onCollisionEnd();

				trackingSet.erase( iter );
			}
		}
	}
}

#endif //_COLLIDER_IMPL_H_