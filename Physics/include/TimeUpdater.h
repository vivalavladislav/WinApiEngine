#ifndef _TIME_UPDATER_H_
#define _TIME_UPDATER_H_

#include "PhysicsDll.h"
#include <chrono>
#include <functional>
#include <set>
#include "ObjectComponent.h"

namespace Engine
{
	struct PHYS_API TimeUpdater
	{
		void update();
		void registerUpdate(UpdatableComponent* component);
		void unregisterUpdate(UpdatableComponent* component );

	private:
		std::set< UpdatableComponent*> _updatables;

		typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
		TimePoint _lastUpdate;
		bool _started;
	};
}
#endif // _TIME_UPDATER_H_