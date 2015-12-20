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

		for (auto iter : _updatables)
		{
			iter.get()->operator()(delta.count());
		}
		_lastUpdate = system_clock::now();
	}

	void TimeUpdater::registerUpdate(FuncPtr func )
	{
		if (_updatables.find(func) == _updatables.end())
		{
			_updatables.insert( func );
		}
	}

	void TimeUpdater::unregisterUpdate( FuncPtr func )
	{
		auto iter = _updatables.find( func );
		if (iter != _updatables.end())
		{
			_updatables.erase( iter );
		}
	}
}
