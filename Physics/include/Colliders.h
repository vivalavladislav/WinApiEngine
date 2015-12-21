#ifndef _COLLIDERS_H_
#define _COLLIDERS_H_

#include "PhysicsDll.h"
#include "Shapes.h"
#include "WorldObject.h"
#include "ObjectComponent.h"
#include <memory>
#include <set>
#include <map>

namespace Engine
{
	
	/*
		Event must be used in place, but for sake of development's speed delegates are used 
	*/
	struct ColliderListener
	{
		virtual void onCollisionStart(Point2d collisionVector){};
		virtual void onCollisionEnd(){};
		virtual void onStayInCollision(Point2d collisionVector){};

		virtual ~ColliderListener()
		{
		}
	};

	struct World;
	struct CollisionController;

	/*
		Colliders were designed to be tightly coupled with a their shape for compile-time collision detection procedures
		But that produced highly complex Collision Controller code which must be refactored 
	*/
	template < class TShape >
	struct Collider : public IComponent
	{
		friend struct CollisionController;
		Collider(ObjPtr obj, TShape shape );
		
		TShape getShape();
		WorldObjectPtr getObject();

		void setListener( ColliderListener* listener );

		virtual void onAddedToWorld(World *);
		virtual void onRemovedFromWorld(World*);

	private:
		TShape _shape;
		ColliderListener* _listener = nullptr;
	};


	struct PHYS_API CollisionController
	{
		CollisionController( World* );
		~CollisionController();
		
		void update(float dt);

		// adds compile time support for shape
		// a set of colliders with the shape is added and a set of tracking collisions
		// 
		// problem : for a new shape support there should be manually added a number of procedures to check a collision with other shape
		// another problem: detection loop must be updated with a new code to check collisions 
		// feature : this kind of design excluded casts and provided compile-time safety for collision support
#define SHAPE_SUPPORT(Shape, Name, TrackingName)\
	private:\
	std::set< std::pair< Collider<Shape>*, Collider<Shape>* > > TrackingName;\
	std::set< Collider< Shape > *> Name;\
	public:\
		void addCollider( Collider< Shape >* collider )\
		{\
			if (Name.find(collider) == Name.end())\
			{\
				Name.insert(collider); \
			}\
		}\
		void removeCollider(Collider< Shape >* collider)\
		{\
			auto iter = Name.find(collider); \
			if (iter != Name.end())\
			{\
				Name.erase(iter);\
			}\
		}\
	private:\
	bool checkCollision(WorldObjectPtr, WorldObjectPtr, Shape, Shape, Point2d& collisionVector ); \


		//SHAPE_SUPPORT(Circle, _cirles, _circlesTracked);
		SHAPE_SUPPORT(AABB, _aabbs, _aabbsTracked );

	private:		
		// controlls collision states, sends events to delegates and updates containers
		template< class TLShape, class TRShape >
		void trackCollisionState( Collider<TLShape>* lhs, Collider<TRShape>* rhs, 
			std::set< std::pair<Collider<TLShape>*, Collider<TRShape>*> >& trackingSet,
			bool inCollision );

	private:
		World* _world;
		FuncPtr _updateCallback;
	};
}


#include "ColliderImpl.hpp"


#endif // _COLLIDERS_H_