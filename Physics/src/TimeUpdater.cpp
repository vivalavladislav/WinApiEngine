#include "../include/TimeUpdater.h"
#include <iostream>

using namespace std::chrono;
using namespace std;


namespace Engine
{
	void TimeUpdater::update()
	{
		if (!_started)
		{
			_started = true;
			_lastUpdate = system_clock::now();
			return;
		}
		
		duration<double> delta = system_clock::now() - _lastUpdate;
		
		if (delta.count() < FLT_EPSILON)
		{
			return;
		}

		cout << "update, delta : " << delta.count() << endl;

		for (auto updatable : _updatables)
		{
			updatable->update(delta.count());
		}
		_lastUpdate = system_clock::now();
	}

	void TimeUpdater::registerUpdate(UpdatableComponent* component)
	{
		auto iter = _updatables.find( component );
		if (iter == _updatables.end())
		{
			_updatables.insert( component );
		}
	}

	void TimeUpdater::unregisterUpdate(UpdatableComponent* component)
	{
		auto iter = _updatables.find(component);
		if (iter != _updatables.end())
		{
			_updatables.erase( iter );
		}
	}
}
