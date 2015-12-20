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