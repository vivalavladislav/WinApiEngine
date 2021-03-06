#include "../include/Colliders.h"
#include "../include/World.h"


using namespace std;

namespace Engine
{
	CollisionController::CollisionController(World* world)
		: _world( world )
	{
		auto func = bind(&CollisionController::update, this, placeholders::_1);
		_updateCallback = make_shared< UpdateF >( func );
		_world->getUpdater().registerUpdate( _updateCallback );
	}

	CollisionController::~CollisionController()
	{
		_world->getUpdater().unregisterUpdate(_updateCallback);
	}


	void CollisionController::update(float dt)
	{
		Point2d collision;
		// checking for collision states and updating them
		for (auto iter = _aabbs.begin(); iter != _aabbs.end(); ++iter)
		{
			auto copy = iter;
			for (auto sq = ++copy; sq != _aabbs.end(); ++sq)
			{
				auto collided = checkCollision((*iter)->getObject(), (*sq)->getObject(), (*iter)->getShape(), (*sq)->getShape(), collision);
				trackCollisionState( *iter, *sq, _aabbsTracked, collided );
			}
		}
	}


	// MUST provide a collision vector or other collision response support
	bool CollisionController::checkCollision(WorldObjectPtr leftObj, WorldObjectPtr rightObj, AABB leftShape, AABB rightShape, Point2d& collisionVector)
	{
		// computing vector between centers of aabb's 
		// if its length < sum of half of their size's - collision!

		// AABB collision properties are: 
		// WorldObject Position is bottom left point
		// WorldObject Position + Size is top right point of AABB 
		auto leftCenter = leftObj->getWorldPosition(leftShape.Size / 2);
		auto rightCenter = rightObj->getWorldPosition(rightShape.Size / 2);

		auto vectorBetweenTwoCenters = Point2d(abs(rightCenter.x - leftCenter.x), abs(rightCenter.y - leftCenter.y));
		
		auto result = vectorBetweenTwoCenters.x <= (leftShape.Size.x + rightShape.Size.x) /2 ;
		result &= vectorBetweenTwoCenters.y <= (leftShape.Size.y + rightShape.Size.y) / 2;
		
		return result;
	}
}