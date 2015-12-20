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
	

	struct World;
	template < class TShape >
	struct Collider : public IComponent
	{
		Collider(ObjPtr obj, TShape shape );
		
		TShape getShape();
		WorldObjectPtr getObject();

		virtual void onAddedToWorld(World *);
		virtual void onRemovedFromWorld(World*);

	private:
		TShape _shape;
	};


	struct PHYS_API CollisionController
	{
		CollisionController( World* );
		~CollisionController();
		
		void update(float dt);

#define SHAPE_SUPPORT(Shape, Name)\
	private:\
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
	bool checkCollision(WorldObjectPtr, WorldObjectPtr, Shape, Shape );\


		SHAPE_SUPPORT(Circle, _cirles);
		SHAPE_SUPPORT(Square, _squares );

	private:
		bool checkCollision(WorldObjectPtr, WorldObjectPtr, Circle, Square );

	private:
		World* _world;
		FuncPtr _updateCallback;
	};
}

#include "ColliderImpl.hpp"


#endif // _COLLIDERS_H_