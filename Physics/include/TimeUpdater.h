#ifndef _TIME_UPDATER_H_
#define _TIME_UPDATER_H_

#include "PhysicsDll.h"
#include <chrono>
#include <functional>
#include <set>
#include "ObjectComponent.h"

namespace Engine
{
	/*
		main engine loop, with update subscription
	*/
	struct PHYS_API TimeUpdater
	{
		void update();
		void registerUpdate(FuncPtr func );
		void unregisterUpdate(FuncPtr func );

	private:
		std::set< FuncPtr > _updatables;

		typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
		TimePoint _lastUpdate;
		bool _started;
	};
}
#endif // _TIME_UPDATER_H_