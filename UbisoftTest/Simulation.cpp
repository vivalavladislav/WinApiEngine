#include "stdafx.h"
#include "Simulation.h"
#include "ObjectComponent.h"
#include "Rigidbody.h"

#include <chrono>
#include <fstream>

using namespace Engine;
using namespace Render;
using namespace std;

struct Settings
{
	float PixelsInMeter = 20;
	float Mass;
	Point2d StartPoint;
	Point2d ObjectSize = Point2d(2,2);
	Point2d VelocityVector;
};

auto readSettings = []( string fileName )
{
	Settings res;

	ifstream file;
	file.open( fileName );

	string line;
	while (getline(file, line))
	{
		if (line.find("mass:") != -1)
		{
			auto index = strlen("mass:");
			res.Mass = stof( line.substr(index).c_str() );
		}
		if (line.find("pixels_in_meter:") != -1)
		{
			auto index = strlen("pixels_in_meter:");
			res.PixelsInMeter = stof(line.substr(index).c_str());
		}
		if (line.find("start_point") != -1)
		{
			auto len = strlen("start_point:");
			string raw = line.substr(len );

			auto delim = strlen(raw.c_str());
			if (raw.find(','))
			{
				delim = raw.find(',');
			}

			res.StartPoint.x = stof(raw.substr(0, delim));
			if (delim != strlen(raw.c_str()))
			{
				res.StartPoint.y = stof(raw.substr(delim + 1));
			}
		}
		if (line.find("velocity_vector:") != -1)
		{
			auto len = strlen("velocity_vector:");
			string raw = line.substr(len);

			auto delim = strlen(raw.c_str());
			if (raw.find(','))
			{
				delim = raw.find(',');
			}

			res.VelocityVector.x = stof(raw.substr(0, delim));
			if (delim != strlen(raw.c_str()))
			{
				res.VelocityVector.y = stof(raw.substr(delim + 1));
			}
		}
		if (line.find("object_size_in_meters:") != -1)
		{
			auto len = strlen("object_size_in_meters:");
			string raw = line.substr(len);

			auto delim = strlen(raw.c_str());
			if (raw.find(','))
			{
				delim = raw.find(',');
			}

			res.ObjectSize.x = stof(raw.substr(0, delim));
			if (delim != strlen(raw.c_str()))
			{
				res.ObjectSize.y = stof(raw.substr(delim + 1));
			}
		}
	}
	return res;
};

Settings settings = readSettings("./settings.txt");

Simulation::Simulation( Point2d windowSize )
// 20 pixels is one meter
: _world(PhysicProperties{ 1/ settings.PixelsInMeter })
{
	auto settings = readSettings("./settings.txt");
	auto props = _world.getPhysicProperties();


	_world.Position = Point2d( 60, windowSize.y - 100 );
	// swap y axis to make a standart look
	_world.setLocalAxises(Point2d(1, -1));
	

	// setting up physic object, it's components and renderable object
	auto physicObject = make_shared< WorldObject >();
	physicObject->Name = "ELLIPSE";
	physicObject->Position = settings.StartPoint * props.MeterToPixel;
	_world.addChild(physicObject);

	auto sizeInPixels = settings.ObjectSize * props.MeterToPixel;
	auto physicRnd = make_shared< Render::Ellipse >(sizeInPixels, RGB(160, 18, 21), physicObject);
	_renderables.push_back(physicRnd);

	// must be switched to physic coordinates
	auto aabbElipse = AABB{ sizeInPixels };

	// physicObject's Position is center of AABB, thus we create an object for follider to be placed in 
	// correct position
	auto colliderObj = make_shared< WorldObject >();
	colliderObj->Position = -(aabbElipse.Size / 2);
	physicObject->addChild(colliderObj);

	auto collider = unique_ptr< Collider<AABB> >(new Collider<AABB>(physicObject, aabbElipse));

	auto rigidbody = unique_ptr< Rigidbody >(new Rigidbody(physicObject, settings.Mass, move(collider)));
	rigidbody->addVelocity(settings.VelocityVector);
	physicObject->addComponent(move(rigidbody));


	// ground collider
	auto groundPlaneObj = make_shared< WorldObject >();
	_world.addChild(groundPlaneObj);

	auto ySize = 1000;
	auto groundBB = AABB{ Point2d(windowSize.x, ySize) };

	auto aabbGround = unique_ptr< Collider<AABB> >(new Collider<AABB>(groundPlaneObj, groundBB));
	groundPlaneObj->addComponent(move(aabbGround));

	groundPlaneObj->Position = Point2d(-60 , -ySize);


	// creating objects to be tracked by engine
	auto yAxisEngine = make_shared< WorldObject >();
	_world.addChild( yAxisEngine );

	auto xAxisEngine = make_shared< WorldObject >();
	_world.addChild( xAxisEngine );

	// creating renderable axises
	auto yLineRender = make_shared< Line >(Point2d(0, windowSize.y - 110), RGB(34, 87, 21), yAxisEngine);
	_renderables.push_back( yLineRender );

	auto xLineRender = make_shared< Line >(Point2d(windowSize.x - 130, 0), RGB(19, 28, 89), xAxisEngine);
	_renderables.push_back(xLineRender);


	// helper struct for rendering grid properties 
	// creates line renderer and a text to display physical meter properties
	struct Notch
	{
		Notch(Simulation* simulation, WorldObjectPtr parent, Point2d lineLength, wstring text )
		{
			NotchObj = make_shared< WorldObject >();
			parent->addChild(NotchObj);

			LineRend = make_shared< Line >(lineLength, RGB(0, 0, 0), NotchObj);
			simulation->_renderables.push_back(LineRend);

			TextObj = make_shared< WorldObject >();
			NotchObj->addChild(TextObj);

			auto textRendr = make_shared< Text >(text.c_str(), RGB(0, 0, 0), TextObj);
			simulation->_renderables.push_back(textRendr);
		}

		WorldObjectPtr NotchObj;
		WorldObjectPtr TextObj;
		std::shared_ptr<Line> LineRend;
	};

	// y notches
	int yNumber = yLineRender->getOffset().y * props.MeterToPixel;
	for (auto i = 1; i < yNumber; i += 20)
	{
		auto notchLength = 20;
		wstring text = to_wstring(i) + L"m";
		auto notch = Notch( this, yAxisEngine, Point2d(notchLength, 0), text );

		notch.NotchObj->Position = Point2d(-notchLength / 2, props.MeterToPixel * i);
		notch.TextObj->Position = Point2d(-notchLength - 10, notch.LineRend->getHeight());
	}

	// x notches
	int xNumber = xLineRender->getOffset().x * props.MeterToPixel;
	for (auto i = 1; i < xNumber ; i += 15)
	{
		auto notchLength = 20;
		wstring text = to_wstring(i) + L"m";
		auto notch = Notch(this, xAxisEngine, Point2d(0, notchLength), text);

		notch.NotchObj->Position = Point2d(props.MeterToPixel * i, -notchLength / 2.f);
		notch.TextObj->Position = Point2d(-notchLength - 10, notch.LineRend->getHeight());
	}
}

void Simulation::run( HWND window, std::shared_ptr<bool> run )
{
	auto lastUpdate = chrono::system_clock::now();
	auto updateTime = 1 / 30.0; // fps

	while ( *run.get() )
	{
		chrono::duration<double> delta = chrono::system_clock::now() - lastUpdate;
		if (delta.count() >= updateTime)
		{
			InvalidateRect(window, nullptr, true);
			lastUpdate = chrono::system_clock::now();
		}
		_world.getUpdater().update();
	}
}
std::vector<RenderablePtr> Simulation::getRender()
{
	return _renderables;
}

World& Simulation::getWorld()
{
	return _world;
}