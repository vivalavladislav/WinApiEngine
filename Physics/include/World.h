#ifndef _WORLD_H_
#define _WORLD_H_

#include "PhysicsDll.h"
#include "Object.h"
#include "WorldObject.h"
#include "TimeUpdater.h"
#include "Colliders.h"


namespace Engine
{
	struct PHYS_API PhysicProperties
	{
		float PixelToMeter;
		float MeterToPixel;

		Point2d Gravity = Point2d(0, -9.8 );

		PhysicProperties(float pixelToMeter)
		{
			PixelToMeter = pixelToMeter;
			MeterToPixel = 1 / pixelToMeter;
		}
	};

	struct PHYS_API World : public Object< WorldObject >
	{
		World(PhysicProperties properites);

		TimeUpdater& getUpdater();
		PhysicProperties getPhysicProperties();
		CollisionController& getCollisionController();

	protected:
		virtual void onChildAdded(WorldObjectPtr child);
		virtual void onChildRemoved(WorldObjectPtr child);

	private:
		TimeUpdater _updater;
		PhysicProperties _props;
		CollisionController _collision;
	};
}

#endif // _WORLD_H_